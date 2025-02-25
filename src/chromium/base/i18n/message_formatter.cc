// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "base/i18n/message_formatter.h"

#include "base/i18n/unicodestring.h"
#include "base/logging.h"
#include "base/numerics/safe_conversions.h"
#include "base/time/time.h"
#include "third_party/icu/source/common/unicode/unistr.h"
#include "third_party/icu/source/common/unicode/utypes.h"
#include "third_party/icu/source/i18n/unicode/fmtable.h"
#include "third_party/icu/source/i18n/unicode/msgfmt.h"

using icu::UnicodeString;

namespace base {
namespace i18n {
namespace {
UnicodeString UnicodeStringFromStringPiece(StringPiece str) {
  return UnicodeString::fromUTF8(
      icu::StringPiece(str.data(), base::checked_cast<int32_t>(str.size())));
}
}  // anonymous namespace

namespace internal {
MessageArg::MessageArg()
#if !UCONFIG_NO_FORMATTING
: formattable(nullptr)
#endif // UCONFIG_NO_FORMATTING
   {}

MessageArg::MessageArg(const char* s)
#if !UCONFIG_NO_FORMATTING
    : formattable(new icu::Formattable(UnicodeStringFromStringPiece(s)))
#endif // UCONFIG_NO_FORMATTING
   {}

MessageArg::MessageArg(StringPiece s)
#if !UCONFIG_NO_FORMATTING
    : formattable(new icu::Formattable(UnicodeStringFromStringPiece(s)))
#endif // UCONFIG_NO_FORMATTING
   {}

MessageArg::MessageArg(const std::string& s)
#if !UCONFIG_NO_FORMATTING
    : formattable(new icu::Formattable(UnicodeString::fromUTF8(s)))
#endif // UCONFIG_NO_FORMATTING
   {}

MessageArg::MessageArg(const string16& s)
#if !UCONFIG_NO_FORMATTING
    : formattable(new icu::Formattable(UnicodeString(s.data(), s.size())))
#endif // UCONFIG_NO_FORMATTING
   {}

MessageArg::MessageArg(int i)
#if !UCONFIG_NO_FORMATTING
    : formattable(new icu::Formattable(i))
#endif // UCONFIG_NO_FORMATTING
   {}

MessageArg::MessageArg(int64_t i)
#if !UCONFIG_NO_FORMATTING
    : formattable(new icu::Formattable(i))
#endif // UCONFIG_NO_FORMATTING
   {}

MessageArg::MessageArg(double d)
#if !UCONFIG_NO_FORMATTING
    : formattable(new icu::Formattable(d))
#endif // UCONFIG_NO_FORMATTING
   {}

MessageArg::MessageArg(const Time& t)
#if !UCONFIG_NO_FORMATTING
    : formattable(new icu::Formattable(static_cast<UDate>(t.ToJsTime())))
#endif // UCONFIG_NO_FORMATTING
   {}

MessageArg::~MessageArg() = default;

// Tests if this argument has a value, and if so increments *count.
bool MessageArg::has_value(int *count) const {
#if !UCONFIG_NO_FORMATTING
  if (formattable == nullptr)
    return false;

  ++*count;
#else
  NOTIMPLEMENTED();
#endif // UCONFIG_NO_FORMATTING
  return true;
}

}  // namespace internal

string16 MessageFormatter::FormatWithNumberedArgs(
    StringPiece16 msg,
    const internal::MessageArg& arg0,
    const internal::MessageArg& arg1,
    const internal::MessageArg& arg2,
    const internal::MessageArg& arg3,
    const internal::MessageArg& arg4,
    const internal::MessageArg& arg5,
    const internal::MessageArg& arg6) {
#if !UCONFIG_NO_FORMATTING
  int32_t args_count = 0;
  icu::Formattable args[] = {
      arg0.has_value(&args_count) ? *arg0.formattable : icu::Formattable(),
      arg1.has_value(&args_count) ? *arg1.formattable : icu::Formattable(),
      arg2.has_value(&args_count) ? *arg2.formattable : icu::Formattable(),
      arg3.has_value(&args_count) ? *arg3.formattable : icu::Formattable(),
      arg4.has_value(&args_count) ? *arg4.formattable : icu::Formattable(),
      arg5.has_value(&args_count) ? *arg5.formattable : icu::Formattable(),
      arg6.has_value(&args_count) ? *arg6.formattable : icu::Formattable(),
  };

  UnicodeString msg_string(msg.data(), msg.size());
  UErrorCode error = U_ZERO_ERROR;
  icu::MessageFormat format(msg_string,  error);
  icu::UnicodeString formatted;
  icu::FieldPosition ignore(icu::FieldPosition::DONT_CARE);
  format.format(args, args_count, formatted, ignore, error);
  if (U_FAILURE(error)) {
    LOG(ERROR) << "MessageFormat(" << msg.as_string() << ") failed with "
               << u_errorName(error);
    return string16();
  }
  return i18n::UnicodeStringToString16(formatted);
#else
  NOTIMPLEMENTED();
  return string16();
#endif // UCONFIG_NO_FORMATTING
}

string16 MessageFormatter::FormatWithNamedArgs(
    StringPiece16 msg,
    StringPiece name0, const internal::MessageArg& arg0,
    StringPiece name1, const internal::MessageArg& arg1,
    StringPiece name2, const internal::MessageArg& arg2,
    StringPiece name3, const internal::MessageArg& arg3,
    StringPiece name4, const internal::MessageArg& arg4,
    StringPiece name5, const internal::MessageArg& arg5,
    StringPiece name6, const internal::MessageArg& arg6) {
#if !UCONFIG_NO_FORMATTING
  icu::UnicodeString names[] = {
      UnicodeStringFromStringPiece(name0),
      UnicodeStringFromStringPiece(name1),
      UnicodeStringFromStringPiece(name2),
      UnicodeStringFromStringPiece(name3),
      UnicodeStringFromStringPiece(name4),
      UnicodeStringFromStringPiece(name5),
      UnicodeStringFromStringPiece(name6),
  };
  int32_t args_count = 0;
  icu::Formattable args[] = {
      arg0.has_value(&args_count) ? *arg0.formattable : icu::Formattable(),
      arg1.has_value(&args_count) ? *arg1.formattable : icu::Formattable(),
      arg2.has_value(&args_count) ? *arg2.formattable : icu::Formattable(),
      arg3.has_value(&args_count) ? *arg3.formattable : icu::Formattable(),
      arg4.has_value(&args_count) ? *arg4.formattable : icu::Formattable(),
      arg5.has_value(&args_count) ? *arg5.formattable : icu::Formattable(),
      arg6.has_value(&args_count) ? *arg6.formattable : icu::Formattable(),
  };

  UnicodeString msg_string(msg.data(), msg.size());
  UErrorCode error = U_ZERO_ERROR;
  icu::MessageFormat format(msg_string, error);

  icu::UnicodeString formatted;
  format.format(names, args, args_count, formatted, error);
  if (U_FAILURE(error)) {
    LOG(ERROR) << "MessageFormat(" << msg.as_string() << ") failed with "
               << u_errorName(error);
    return string16();
  }
  return i18n::UnicodeStringToString16(formatted);
#else
  NOTIMPLEMENTED();
  return string16();
#endif // UCONFIG_NO_FORMATTING
}

}  // namespace i18n
}  // namespace base
