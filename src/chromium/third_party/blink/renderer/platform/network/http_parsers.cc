/*
 * Copyright (C) 2006 Alexey Proskuryakov (ap@webkit.org)
 * Copyright (C) 2006, 2007, 2008, 2009 Apple Inc. All rights reserved.
 * Copyright (C) 2009 Torch Mobile Inc. http://www.torchmobile.com/
 * Copyright (C) 2009 Google Inc. All rights reserved.
 * Copyright (C) 2011 Apple Inc. All Rights Reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1.  Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 * 2.  Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 * 3.  Neither the name of Apple Computer, Inc. ("Apple") nor the names of
 *     its contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE AND ITS CONTRIBUTORS "AS IS" AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL APPLE OR ITS CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "third_party/blink/renderer/platform/network/http_parsers.h"

#include <memory>
#if defined(ENABLE_GNET)
#include "net/http/http_content_disposition.h"
#include "net/http/http_response_headers.h"
#include "net/http/http_util.h"
#endif // ENABLE_GNET
#include "third_party/blink/public/platform/web_string.h"
#include "third_party/blink/renderer/platform/loader/fetch/resource_response.h"
#include "third_party/blink/renderer/platform/network/header_field_tokenizer.h"
#include "third_party/blink/renderer/platform/network/http_names.h"
#include "third_party/blink/renderer/platform/wtf/date_math.h"
#include "third_party/blink/renderer/platform/wtf/math_extras.h"
#include "third_party/blink/renderer/platform/wtf/text/character_names.h"
#include "third_party/blink/renderer/platform/wtf/text/cstring.h"
#include "third_party/blink/renderer/platform/wtf/text/parsing_utilities.h"
#include "third_party/blink/renderer/platform/wtf/text/string_builder.h"
#include "third_party/blink/renderer/platform/wtf/text/string_utf8_adaptor.h"
#include "third_party/blink/renderer/platform/wtf/text/wtf_string.h"
#include "third_party/blink/renderer/platform/wtf/wtf.h"

namespace blink {

namespace {

const Vector<AtomicString>& ReplaceHeaders() {
  // The list of response headers that we do not copy from the original
  // response when generating a ResourceResponse for a MIME payload.
  // Note: this is called only on the main thread.
  DEFINE_STATIC_LOCAL(Vector<AtomicString>, headers,
                      ({"content-type", "content-length", "content-disposition",
                        "content-range", "range", "set-cookie"}));
  return headers;
}

bool IsWhitespace(UChar chr) {
  return (chr == ' ') || (chr == '\t');
}

// true if there is more to parse, after incrementing pos past whitespace.
// Note: Might return pos == str.length()
// if |matcher| is nullptr, isWhitespace() is used.
inline bool SkipWhiteSpace(const String& str,
                           unsigned& pos,
                           WTF::CharacterMatchFunctionPtr matcher = nullptr) {
  unsigned len = str.length();

  if (matcher) {
    while (pos < len && matcher(str[pos]))
      ++pos;
  } else {
    while (pos < len && IsWhitespace(str[pos]))
      ++pos;
  }

  return pos < len;
}

template <typename CharType>
inline bool IsASCIILowerAlphaOrDigit(CharType c) {
  return IsASCIILower(c) || IsASCIIDigit(c);
}

template <typename CharType>
inline bool IsASCIILowerAlphaOrDigitOrHyphen(CharType c) {
  return IsASCIILowerAlphaOrDigit(c) || c == '-';
}

// Parse a number with ignoring trailing [0-9.].
// Returns NaN if the source contains invalid characters.
double ParseRefreshTime(const String& source) {
  int full_stop_count = 0;
  unsigned number_end = source.length();
  for (unsigned i = 0; i < source.length(); ++i) {
    UChar ch = source[i];
    if (ch == kFullstopCharacter) {
      // TODO(tkent): According to the HTML specification, we should support
      // only integers. However we support fractional numbers.
      if (++full_stop_count == 2)
        number_end = i;
    } else if (!IsASCIIDigit(ch)) {
      return std::numeric_limits<double>::quiet_NaN();
    }
  }
  bool ok;
  double time = source.Left(number_end).ToDouble(&ok);
  return ok ? time : std::numeric_limits<double>::quiet_NaN();
}

}  // namespace

bool IsValidHTTPHeaderValue(const String& name) {
  // FIXME: This should really match name against
  // field-value in section 4.2 of RFC 2616.

  return name.ContainsOnlyLatin1OrEmpty() && !name.Contains('\r') &&
         !name.Contains('\n') && !name.Contains('\0');
}

// See RFC 7230, Section 3.2.6.
bool IsValidHTTPToken(const String& characters) {
  if (characters.IsEmpty())
    return false;
  for (unsigned i = 0; i < characters.length(); ++i) {
    UChar c = characters[i];
    if (c > 0x7F || !net::HttpUtil::IsTokenChar(c))
      return false;
  }
  return true;
}

bool IsContentDispositionAttachment(const String& content_disposition) {
  CString cstring(content_disposition.Utf8());
  std::string string(cstring.data(), cstring.length());
  return net::HttpContentDisposition(string, std::string()).is_attachment();
}

// https://html.spec.whatwg.org/C/#attr-meta-http-equiv-refresh
bool ParseHTTPRefresh(const String& refresh,
                      WTF::CharacterMatchFunctionPtr matcher,
                      double& delay,
                      String& url) {
  unsigned len = refresh.length();
  unsigned pos = 0;
  matcher = matcher ? matcher : IsWhitespace;

  if (!SkipWhiteSpace(refresh, pos, matcher))
    return false;

  while (pos != len && refresh[pos] != ',' && refresh[pos] != ';' &&
         !matcher(refresh[pos]))
    ++pos;

  if (pos == len) {  // no URL
    url = String();
    delay = ParseRefreshTime(refresh.StripWhiteSpace());
    return std::isfinite(delay);
  } else {
    delay = ParseRefreshTime(refresh.Left(pos).StripWhiteSpace());
    if (!std::isfinite(delay))
      return false;

    SkipWhiteSpace(refresh, pos, matcher);
    if (pos < len && (refresh[pos] == ',' || refresh[pos] == ';'))
      ++pos;
    SkipWhiteSpace(refresh, pos, matcher);
    unsigned url_start_pos = pos;
    if (refresh.FindIgnoringASCIICase("url", url_start_pos) == url_start_pos) {
      url_start_pos += 3;
      SkipWhiteSpace(refresh, url_start_pos, matcher);
      if (refresh[url_start_pos] == '=') {
        ++url_start_pos;
        SkipWhiteSpace(refresh, url_start_pos, matcher);
      } else {
        url_start_pos = pos;  // e.g. "Refresh: 0; url.html"
      }
    }

    unsigned url_end_pos = len;

    if (refresh[url_start_pos] == '"' || refresh[url_start_pos] == '\'') {
      UChar quotation_mark = refresh[url_start_pos];
      url_start_pos++;
      while (url_end_pos > url_start_pos) {
        url_end_pos--;
        if (refresh[url_end_pos] == quotation_mark)
          break;
      }

      // https://bugs.webkit.org/show_bug.cgi?id=27868
      // Sometimes there is no closing quote for the end of the URL even though
      // there was an opening quote.  If we looped over the entire alleged URL
      // string back to the opening quote, just go ahead and use everything
      // after the opening quote instead.
      if (url_end_pos == url_start_pos)
        url_end_pos = len;
    }

    url = refresh.Substring(url_start_pos, url_end_pos - url_start_pos)
              .StripWhiteSpace();
    return true;
  }
}

double ParseDate(const String& value) {
  return ParseDateFromNullTerminatedCharacters(value.Utf8().data());
}

AtomicString ExtractMIMETypeFromMediaType(const AtomicString& media_type) {
  unsigned length = media_type.length();

  unsigned pos = 0;

  while (pos < length) {
    UChar c = media_type[pos];
    if (c != '\t' && c != ' ')
      break;
    ++pos;
  }

  if (pos == length)
    return media_type;

  unsigned type_start = pos;

  unsigned type_end = pos;
  while (pos < length) {
    UChar c = media_type[pos];

    // While RFC 2616 does not allow it, other browsers allow multiple values in
    // the HTTP media type header field, Content-Type. In such cases, the media
    // type string passed here may contain the multiple values separated by
    // commas. For now, this code ignores text after the first comma, which
    // prevents it from simply failing to parse such types altogether.  Later
    // for better compatibility we could consider using the first or last valid
    // MIME type instead.
    // See https://bugs.webkit.org/show_bug.cgi?id=25352 for more discussion.
    if (c == ',' || c == ';')
      break;

    if (c != '\t' && c != ' ')
      type_end = pos + 1;

    ++pos;
  }

  return AtomicString(
      media_type.GetString().Substring(type_start, type_end - type_start));
}

ReflectedXSSDisposition ParseXSSProtectionHeader(const String& header,
                                                 String& failure_reason,
                                                 unsigned& failure_position,
                                                 String& report_url) {
  DEFINE_STATIC_LOCAL(String, failure_reason_invalid_toggle,
                      ("expected token to be 0 or 1"));
  DEFINE_STATIC_LOCAL(String, failure_reason_invalid_separator,
                      ("expected semicolon"));
  DEFINE_STATIC_LOCAL(String, failure_reason_invalid_equals,
                      ("expected equals sign"));
  DEFINE_STATIC_LOCAL(String, failure_reason_invalid_mode,
                      ("invalid mode directive"));
  DEFINE_STATIC_LOCAL(String, failure_reason_invalid_report,
                      ("invalid report directive"));
  DEFINE_STATIC_LOCAL(String, failure_reason_duplicate_mode,
                      ("duplicate mode directive"));
  DEFINE_STATIC_LOCAL(String, failure_reason_duplicate_report,
                      ("duplicate report directive"));
  DEFINE_STATIC_LOCAL(String, failure_reason_invalid_directive,
                      ("unrecognized directive"));

  HeaderFieldTokenizer tokenizer(header);

  StringView toggle;
  if (!tokenizer.ConsumeToken(Mode::kNormal, toggle)) {
    if (tokenizer.IsConsumed())
      return kReflectedXSSUnset;
  }

  if (toggle.length() != 1 || (toggle[0] != '0' && toggle[0] != '1')) {
    failure_reason = failure_reason_invalid_toggle;
    return kReflectedXSSInvalid;
  }

  if (toggle[0] == '0')
    return kAllowReflectedXSS;

  ReflectedXSSDisposition result = kFilterReflectedXSS;
  bool mode_directive_seen = false;
  bool report_directive_seen = false;

  while (!tokenizer.IsConsumed()) {
    // At end of previous directive: consume whitespace, semicolon, and
    // whitespace.
    if (!tokenizer.Consume(';')) {
      failure_reason = failure_reason_invalid_separator;
      failure_position = tokenizer.Index();
      return kReflectedXSSInvalid;
    }

    // Give a pass to a trailing semicolon.
    if (tokenizer.IsConsumed())
      return result;

    // At start of next directive.
    StringView token;
    unsigned token_start = tokenizer.Index();
    if (!tokenizer.ConsumeToken(Mode::kNormal, token)) {
      failure_reason = failure_reason_invalid_directive;
      failure_position = token_start;
      return kReflectedXSSInvalid;
    }
    if (EqualIgnoringASCIICase(token, "mode")) {
      if (mode_directive_seen) {
        failure_reason = failure_reason_duplicate_mode;
        failure_position = token_start;
        return kReflectedXSSInvalid;
      }
      mode_directive_seen = true;
      if (!tokenizer.Consume('=')) {
        failure_reason = failure_reason_invalid_equals;
        failure_position = tokenizer.Index();
        return kReflectedXSSInvalid;
      }
      String value;
      unsigned value_start = tokenizer.Index();
      if (!tokenizer.ConsumeTokenOrQuotedString(Mode::kNormal, value) ||
          !EqualIgnoringASCIICase(value, "block")) {
        failure_reason = failure_reason_invalid_mode;
        failure_position = value_start;
        return kReflectedXSSInvalid;
      }
      result = kBlockReflectedXSS;
    } else if (EqualIgnoringASCIICase(token, "report")) {
      if (report_directive_seen) {
        failure_reason = failure_reason_duplicate_report;
        failure_position = token_start;
        return kReflectedXSSInvalid;
      }
      report_directive_seen = true;
      if (!tokenizer.Consume('=')) {
        failure_reason = failure_reason_invalid_equals;
        failure_position = tokenizer.Index();
        return kReflectedXSSInvalid;
      }
      // Set, just in case later semantic check deems unacceptable.
      failure_position = tokenizer.Index();
      String value;
      // Relaxed mode - unquoted URLs contain colons and such.
      if (!tokenizer.ConsumeTokenOrQuotedString(Mode::kRelaxed, value)) {
        failure_reason = failure_reason_invalid_report;
        return kReflectedXSSInvalid;
      }
      report_url = value;
    } else {
      // Unrecognized directive
      failure_reason = failure_reason_invalid_directive;
      failure_position = token_start;
      return kReflectedXSSInvalid;
    }
  }

  return result;
}

ContentTypeOptionsDisposition ParseContentTypeOptionsHeader(
    const String& value) {
  if (value.IsEmpty())
    return kContentTypeOptionsNone;

  Vector<String> results;
  value.Split(",", results);
  if (results.size() && results[0].StripWhiteSpace().LowerASCII() == "nosniff")
    return kContentTypeOptionsNosniff;
  return kContentTypeOptionsNone;
}

static bool IsCacheHeaderSeparator(UChar c) {
  // See RFC 2616, Section 2.2
  switch (c) {
    case '(':
    case ')':
    case '<':
    case '>':
    case '@':
    case ',':
    case ';':
    case ':':
    case '\\':
    case '"':
    case '/':
    case '[':
    case ']':
    case '?':
    case '=':
    case '{':
    case '}':
    case ' ':
    case '\t':
      return true;
    default:
      return false;
  }
}

static bool IsControlCharacter(UChar c) {
  return c < ' ' || c == 127;
}

static inline String TrimToNextSeparator(const String& str) {
  return str.Substring(0, str.Find(IsCacheHeaderSeparator));
}

static void ParseCacheHeader(const String& header,
                             Vector<std::pair<String, String>>& result) {
  const String safe_header = header.RemoveCharacters(IsControlCharacter);
  wtf_size_t max = safe_header.length();
  for (wtf_size_t pos = 0; pos < max; /* pos incremented in loop */) {
    wtf_size_t next_comma_position = safe_header.find(',', pos);
    wtf_size_t next_equal_sign_position = safe_header.find('=', pos);
    if (next_equal_sign_position != kNotFound &&
        (next_equal_sign_position < next_comma_position ||
         next_comma_position == kNotFound)) {
      // Get directive name, parse right hand side of equal sign, then add to
      // map
      String directive = TrimToNextSeparator(
          safe_header.Substring(pos, next_equal_sign_position - pos)
              .StripWhiteSpace());
      pos += next_equal_sign_position - pos + 1;

      String value = safe_header.Substring(pos, max - pos).StripWhiteSpace();
      if (value[0] == '"') {
        // The value is a quoted string
        wtf_size_t next_double_quote_position = value.find('"', 1);
        if (next_double_quote_position != kNotFound) {
          // Store the value as a quoted string without quotes
          result.push_back(std::pair<String, String>(
              directive, value.Substring(1, next_double_quote_position - 1)
                             .StripWhiteSpace()));
          pos += (safe_header.find('"', pos) - pos) +
                 next_double_quote_position + 1;
          // Move past next comma, if there is one
          wtf_size_t next_comma_position2 = safe_header.find(',', pos);
          if (next_comma_position2 != kNotFound)
            pos += next_comma_position2 - pos + 1;
          else
            return;  // Parse error if there is anything left with no comma
        } else {
          // Parse error; just use the rest as the value
          result.push_back(std::pair<String, String>(
              directive,
              TrimToNextSeparator(
                  value.Substring(1, value.length() - 1).StripWhiteSpace())));
          return;
        }
      } else {
        // The value is a token until the next comma
        wtf_size_t next_comma_position2 = value.find(',');
        if (next_comma_position2 != kNotFound) {
          // The value is delimited by the next comma
          result.push_back(std::pair<String, String>(
              directive,
              TrimToNextSeparator(
                  value.Substring(0, next_comma_position2).StripWhiteSpace())));
          pos += (safe_header.find(',', pos) - pos) + 1;
        } else {
          // The rest is the value; no change to value needed
          result.push_back(
              std::pair<String, String>(directive, TrimToNextSeparator(value)));
          return;
        }
      }
    } else if (next_comma_position != kNotFound &&
               (next_comma_position < next_equal_sign_position ||
                next_equal_sign_position == kNotFound)) {
      // Add directive to map with empty string as value
      result.push_back(std::pair<String, String>(
          TrimToNextSeparator(
              safe_header.Substring(pos, next_comma_position - pos)
                  .StripWhiteSpace()),
          ""));
      pos += next_comma_position - pos + 1;
    } else {
      // Add last directive to map with empty string as value
      result.push_back(std::pair<String, String>(
          TrimToNextSeparator(
              safe_header.Substring(pos, max - pos).StripWhiteSpace()),
          ""));
      return;
    }
  }
}

CacheControlHeader ParseCacheControlDirectives(
    const AtomicString& cache_control_value,
    const AtomicString& pragma_value) {
  CacheControlHeader cache_control_header;
  cache_control_header.parsed = true;
  cache_control_header.max_age = std::numeric_limits<double>::quiet_NaN();
  cache_control_header.stale_while_revalidate =
      std::numeric_limits<double>::quiet_NaN();

  static const char kNoCacheDirective[] = "no-cache";
  static const char kNoStoreDirective[] = "no-store";
  static const char kMustRevalidateDirective[] = "must-revalidate";
  static const char kMaxAgeDirective[] = "max-age";
  static const char kStaleWhileRevalidateDirective[] = "stale-while-revalidate";

  if (!cache_control_value.IsEmpty()) {
    Vector<std::pair<String, String>> directives;
    ParseCacheHeader(cache_control_value, directives);

    wtf_size_t directives_size = directives.size();
    for (wtf_size_t i = 0; i < directives_size; ++i) {
      // RFC2616 14.9.1: A no-cache directive with a value is only meaningful
      // for proxy caches.  It should be ignored by a browser level cache.
      if (DeprecatedEqualIgnoringCase(directives[i].first, kNoCacheDirective) &&
          directives[i].second.IsEmpty()) {
        cache_control_header.contains_no_cache = true;
      } else if (DeprecatedEqualIgnoringCase(directives[i].first,
                                             kNoStoreDirective)) {
        cache_control_header.contains_no_store = true;
      } else if (DeprecatedEqualIgnoringCase(directives[i].first,
                                             kMustRevalidateDirective)) {
        cache_control_header.contains_must_revalidate = true;
      } else if (DeprecatedEqualIgnoringCase(directives[i].first,
                                             kMaxAgeDirective)) {
        if (!std::isnan(cache_control_header.max_age)) {
          // First max-age directive wins if there are multiple ones.
          continue;
        }
        bool ok;
        double max_age = directives[i].second.ToDouble(&ok);
        if (ok)
          cache_control_header.max_age = max_age;
      } else if (DeprecatedEqualIgnoringCase(directives[i].first,
                                             kStaleWhileRevalidateDirective)) {
        if (!std::isnan(cache_control_header.stale_while_revalidate)) {
          // First stale-while-revalidate directive wins if there are multiple
          // ones.
          continue;
        }
        bool ok;
        double stale_while_revalidate = directives[i].second.ToDouble(&ok);
        if (ok)
          cache_control_header.stale_while_revalidate = stale_while_revalidate;
      }
    }
  }

  if (!cache_control_header.contains_no_cache) {
    // Handle Pragma: no-cache
    // This is deprecated and equivalent to Cache-control: no-cache
    // Don't bother tokenizing the value, it is not important
    cache_control_header.contains_no_cache =
        pragma_value.DeprecatedLower().Contains(kNoCacheDirective);
  }
  return cache_control_header;
}

void ParseCommaDelimitedHeader(const String& header_value,
                               CommaDelimitedHeaderSet& header_set) {
  Vector<String> results;
  header_value.Split(",", results);
  for (auto& value : results)
    header_set.insert(value.StripWhiteSpace(IsWhitespace));
}

bool ParseMultipartHeadersFromBody(const char* bytes,
                                   wtf_size_t size,
                                   ResourceResponse* response,
                                   wtf_size_t* end) {
  DCHECK(IsMainThread());

  size_t headers_end_pos =
      net::HttpUtil::LocateEndOfAdditionalHeaders(bytes, size, 0);

  if (headers_end_pos == std::string::npos)
    return false;

  *end = static_cast<wtf_size_t>(headers_end_pos);

  // Eat headers and prepend a status line as is required by
  // HttpResponseHeaders.
  std::string headers("HTTP/1.1 200 OK\r\n");
  headers.append(bytes, headers_end_pos);

  auto response_headers = base::MakeRefCounted<net::HttpResponseHeaders>(
      net::HttpUtil::AssembleRawHeaders(headers));

  std::string mime_type, charset;
  response_headers->GetMimeTypeAndCharset(&mime_type, &charset);
  response->SetMimeType(WebString::FromUTF8(mime_type));
  response->SetTextEncodingName(WebString::FromUTF8(charset));

  // Copy headers listed in replaceHeaders to the response.
  for (const AtomicString& header : ReplaceHeaders()) {
    std::string value;
    StringUTF8Adaptor adaptor(header);
    base::StringPiece header_string_piece(adaptor.AsStringPiece());
    size_t iterator = 0;

    response->ClearHttpHeaderField(header);
    while (response_headers->EnumerateHeader(&iterator, header_string_piece,
                                             &value)) {
      response->AddHttpHeaderField(header, WebString::FromLatin1(value));
    }
  }
  return true;
}

bool ParseMultipartFormHeadersFromBody(const char* bytes,
                                       wtf_size_t size,
                                       HTTPHeaderMap* header_fields,
                                       wtf_size_t* end) {
  DCHECK_EQ(0u, header_fields->size());

  size_t headers_end_pos =
      net::HttpUtil::LocateEndOfAdditionalHeaders(bytes, size, 0);

  if (headers_end_pos == std::string::npos)
    return false;

  *end = static_cast<wtf_size_t>(headers_end_pos);

  // Eat headers and prepend a status line as is required by
  // HttpResponseHeaders.
  std::string headers("HTTP/1.1 200 OK\r\n");
  headers.append(bytes, headers_end_pos);

  auto responseHeaders = base::MakeRefCounted<net::HttpResponseHeaders>(
      net::HttpUtil::AssembleRawHeaders(headers));

  // Copy selected header fields.
  const AtomicString* const headerNamePointers[] = {
      &http_names::kContentDisposition, &http_names::kContentType};
  for (const AtomicString* headerNamePointer : headerNamePointers) {
    StringUTF8Adaptor adaptor(*headerNamePointer);
    size_t iterator = 0;
    base::StringPiece headerNameStringPiece = adaptor.AsStringPiece();
    std::string value;
    while (responseHeaders->EnumerateHeader(&iterator, headerNameStringPiece,
                                            &value)) {
      header_fields->Add(*headerNamePointer, WebString::FromUTF8(value));
    }
  }

  return true;
}

bool ParseContentRangeHeaderFor206(const String& content_range,
                                   int64_t* first_byte_position,
                                   int64_t* last_byte_position,
                                   int64_t* instance_length) {
  return net::HttpUtil::ParseContentRangeHeaderFor206(
      StringUTF8Adaptor(content_range).AsStringPiece(), first_byte_position,
      last_byte_position, instance_length);
}

std::unique_ptr<ServerTimingHeaderVector> ParseServerTimingHeader(
    const String& headerValue) {
  std::unique_ptr<ServerTimingHeaderVector> headers =
      std::make_unique<ServerTimingHeaderVector>();

  if (!headerValue.IsNull()) {
    DCHECK(headerValue.Is8Bit());

    HeaderFieldTokenizer tokenizer(headerValue);
    while (!tokenizer.IsConsumed()) {
      StringView name;
      if (!tokenizer.ConsumeToken(Mode::kNormal, name)) {
        break;
      }

      ServerTimingHeader header(name.ToString());

      while (tokenizer.Consume(';')) {
        StringView parameter_name;
        if (!tokenizer.ConsumeToken(Mode::kNormal, parameter_name)) {
          break;
        }

        String value = "";
        if (tokenizer.Consume('=')) {
          tokenizer.ConsumeTokenOrQuotedString(Mode::kNormal, value);
          tokenizer.ConsumeBeforeAnyCharMatch({',', ';'});
        }
        header.SetParameter(parameter_name, value);
      }

      headers->push_back(std::make_unique<ServerTimingHeader>(header));

      if (!tokenizer.Consume(',')) {
        break;
      }
    }
  }
  return headers;
}

}  // namespace blink
