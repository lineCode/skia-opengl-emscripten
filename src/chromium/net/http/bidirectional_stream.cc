// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "net/http/bidirectional_stream.h"

#include <string>
#include <utility>

#include "base/bind.h"
#include "base/location.h"
#include "base/logging.h"
#include "base/metrics/histogram_macros.h"
#include "base/threading/thread_task_runner_handle.h"
#include "base/timer/timer.h"
#include "base/values.h"
#include "net/base/load_flags.h"
#include "net/base/net_errors.h"
#include "net/http/bidirectional_stream_request_info.h"
#include "net/http/http_network_session.h"
#include "net/http/http_response_headers.h"
#include "net/http/http_stream.h"
#include "net/log/net_log.h"
#include "net/log/net_log_capture_mode.h"
#include "net/log/net_log_event_type.h"
#include "net/log/net_log_source_type.h"

#if defined(ENABLE_SPDY)
#include "net/spdy/spdy_http_utils.h"
#include "net/spdy/spdy_log_util.h"
#endif

#include "net/ssl/ssl_cert_request_info.h"
#include "net/ssl/ssl_config.h"

#if defined(ENABLE_QUIC)
#include "net/third_party/quiche/src/spdy/core/spdy_header_block.h"
#endif

#include "net/base/io_buffer.h"

#include "net/traffic_annotation/network_traffic_annotation.h"
#include "url/gurl.h"

namespace net {

namespace {

base::Value NetLogHeadersCallback(const spdy::SpdyHeaderBlock* headers,
                                  NetLogCaptureMode capture_mode) {
  base::DictionaryValue dict;

#if defined(ENABLE_QUIC)
  dict.SetKey("headers", ElideSpdyHeaderBlockForNetLog(*headers, capture_mode));
#endif
  return std::move(dict);
}

base::Value NetLogCallback(const GURL* url,
                           const std::string* method,
                           const HttpRequestHeaders* headers,
                           NetLogCaptureMode capture_mode) {
  base::DictionaryValue dict;
  dict.SetString("url", url->possibly_invalid_spec());
  dict.SetString("method", *method);
  std::string empty;
  base::Value headers_param(headers->NetLogCallback(&empty, capture_mode));
  dict.SetKey("headers", std::move(headers_param));
  return std::move(dict);
}

}  // namespace

BidirectionalStream::Delegate::Delegate() = default;

BidirectionalStream::Delegate::~Delegate() = default;

BidirectionalStream::BidirectionalStream(
    std::unique_ptr<BidirectionalStreamRequestInfo> request_info,
    HttpNetworkSession* session,
    bool send_request_headers_automatically,
    Delegate* delegate)
    : BidirectionalStream(std::move(request_info),
                          session,
                          send_request_headers_automatically,
                          delegate,
                          std::make_unique<base::OneShotTimer>()) {}

BidirectionalStream::BidirectionalStream(
    std::unique_ptr<BidirectionalStreamRequestInfo> request_info,
    HttpNetworkSession* session,
    bool send_request_headers_automatically,
    Delegate* delegate,
    std::unique_ptr<base::OneShotTimer> timer)
    : request_info_(std::move(request_info)),
      net_log_(NetLogWithSource::Make(session->net_log(),
                                      NetLogSourceType::BIDIRECTIONAL_STREAM)),
      session_(session),
      send_request_headers_automatically_(send_request_headers_automatically),
      request_headers_sent_(false),
      delegate_(delegate),
      timer_(std::move(timer)),
      weak_factory_(this) {
  DCHECK(delegate_);
  DCHECK(request_info_);

  // Start time should be measured before connect.
  load_timing_info_.request_start_time = base::Time::Now();
  load_timing_info_.request_start = base::TimeTicks::Now();

  if (net_log_.IsCapturing()) {
    net_log_.BeginEvent(
        NetLogEventType::BIDIRECTIONAL_STREAM_ALIVE,
        base::Bind(&NetLogCallback, &request_info_->url, &request_info_->method,
                   base::Unretained(&request_info_->extra_headers)));
  }

  if (!request_info_->url.SchemeIs(url::kHttpsScheme)) {
    base::ThreadTaskRunnerHandle::Get()->PostTask(
        FROM_HERE,
        base::BindOnce(&BidirectionalStream::NotifyFailed,
                       weak_factory_.GetWeakPtr(), ERR_DISALLOWED_URL_SCHEME));
    return;
  }

#if defined(ENABLE_BORINGSSL)
  SSLConfig ssl_config;
#ifdef ENABLE_QUIC
  session->ssl_config_service()->GetSSLConfig(&ssl_config);
  session->GetAlpnProtos(&ssl_config.alpn_protos);
#endif
  StartRequest(ssl_config);
#endif // ENABLE_BORINGSSL
}

BidirectionalStream::~BidirectionalStream() {
  UpdateHistograms();
  if (net_log_.IsCapturing()) {
    net_log_.EndEvent(NetLogEventType::BIDIRECTIONAL_STREAM_ALIVE);
  }
}

void BidirectionalStream::SendRequestHeaders() {
  DCHECK(stream_impl_);
  DCHECK(!request_headers_sent_);
  DCHECK(!send_request_headers_automatically_);

  stream_impl_->SendRequestHeaders();
}

int BidirectionalStream::ReadData(IOBuffer* buf, int buf_len) {
  DCHECK(stream_impl_);

  int rv = stream_impl_->ReadData(buf, buf_len);
  if (rv > 0) {
    read_end_time_ = base::TimeTicks::Now();
#ifdef ENABLE_QUIC
    net_log_.AddByteTransferEvent(
        NetLogEventType::BIDIRECTIONAL_STREAM_BYTES_RECEIVED, rv, buf->data());
#endif
  } else if (rv == ERR_IO_PENDING) {
    read_buffer_ = buf;
    // Bytes will be logged in OnDataRead().
  }
  if (net_log_.IsCapturing()) {
    net_log_.AddEvent(NetLogEventType::BIDIRECTIONAL_STREAM_READ_DATA,
                      NetLog::IntCallback("rv", rv));
  }
  return rv;
}

void BidirectionalStream::SendvData(
    const std::vector<scoped_refptr<IOBuffer>>& buffers,
    const std::vector<int>& lengths,
    bool end_stream) {
  DCHECK(stream_impl_);
  DCHECK_EQ(buffers.size(), lengths.size());
  DCHECK(write_buffer_list_.empty());
  DCHECK(write_buffer_len_list_.empty());

  if (net_log_.IsCapturing()) {
    net_log_.AddEvent(NetLogEventType::BIDIRECTIONAL_STREAM_SENDV_DATA,
                      NetLog::IntCallback("num_buffers", buffers.size()));
  }
  stream_impl_->SendvData(buffers, lengths, end_stream);
  for (size_t i = 0; i < buffers.size(); ++i) {
    write_buffer_list_.push_back(buffers[i]);
    write_buffer_len_list_.push_back(lengths[i]);
  }
}

NextProto BidirectionalStream::GetProtocol() const {
  if (!stream_impl_)
    return kProtoUnknown;

  return stream_impl_->GetProtocol();
}

int64_t BidirectionalStream::GetTotalReceivedBytes() const {
  if (!stream_impl_)
    return 0;

  return stream_impl_->GetTotalReceivedBytes();
}

int64_t BidirectionalStream::GetTotalSentBytes() const {
  if (!stream_impl_)
    return 0;

  return stream_impl_->GetTotalSentBytes();
}

void BidirectionalStream::GetLoadTimingInfo(
    LoadTimingInfo* load_timing_info) const {
  *load_timing_info = load_timing_info_;
}

#ifdef ENABLE_QUIC
void BidirectionalStream::PopulateNetErrorDetails(NetErrorDetails* details) {
  DCHECK(details);
  if (stream_impl_)
    stream_impl_->PopulateNetErrorDetails(details);
}
#endif

#if defined(ENABLE_BORINGSSL)
void BidirectionalStream::StartRequest(const SSLConfig& ssl_config) {
  DCHECK(!stream_request_);
  HttpRequestInfo http_request_info;
  http_request_info.url = request_info_->url;
  http_request_info.method = request_info_->method;
  http_request_info.extra_headers = request_info_->extra_headers;
  http_request_info.socket_tag = request_info_->socket_tag;
  stream_request_ =
      session_->http_stream_factory()->RequestBidirectionalStreamImpl(
          http_request_info, request_info_->priority, ssl_config, ssl_config,
          this,
          /* enable_ip_based_pooling = */ true,
          /* enable_alternative_services = */ true, net_log_);
  // Check that this call does not fail.
  DCHECK(stream_request_);
  // Check that HttpStreamFactory does not invoke OnBidirectionalStreamImplReady
  // synchronously.
  DCHECK(!stream_impl_);
}
#endif // ENABLE_BORINGSSL

void BidirectionalStream::OnStreamReady(bool request_headers_sent) {
  request_headers_sent_ = request_headers_sent;
  if (net_log_.IsCapturing()) {
    net_log_.AddEvent(
        NetLogEventType::BIDIRECTIONAL_STREAM_READY,
        NetLog::BoolCallback("request_headers_sent", request_headers_sent));
  }
  load_timing_info_.send_start = base::TimeTicks::Now();
  load_timing_info_.send_end = load_timing_info_.send_start;
  delegate_->OnStreamReady(request_headers_sent);
}

void BidirectionalStream::OnHeadersReceived(
    const spdy::SpdyHeaderBlock& response_headers) {
  HttpResponseInfo response_info;
#ifdef ENABLE_QUIC
  if (!SpdyHeadersToHttpResponse(response_headers, &response_info)) {
    DLOG(WARNING) << "Invalid headers";
    NotifyFailed(ERR_FAILED);
    return;
  }
#endif
  if (net_log_.IsCapturing()) {
    net_log_.AddEvent(NetLogEventType::BIDIRECTIONAL_STREAM_RECV_HEADERS,
                      base::Bind(&NetLogHeadersCallback, &response_headers));
  }
  // Impl should only provide |connect_timing| and |socket_reused| info,
  // so use a copy to get these information only.
  LoadTimingInfo impl_load_timing_info;
  bool has_load_timing =
      stream_impl_->GetLoadTimingInfo(&impl_load_timing_info);
  if (has_load_timing) {
    load_timing_info_.connect_timing = impl_load_timing_info.connect_timing;
    load_timing_info_.socket_reused = impl_load_timing_info.socket_reused;
  }
  load_timing_info_.receive_headers_end = base::TimeTicks::Now();
  read_end_time_ = load_timing_info_.receive_headers_end;
  session_->http_stream_factory()->ProcessAlternativeServices(
      session_, response_info.headers.get(),
      url::SchemeHostPort(request_info_->url));
  delegate_->OnHeadersReceived(response_headers);
}

void BidirectionalStream::OnDataRead(int bytes_read) {
  DCHECK(read_buffer_);

#ifdef ENABLE_QUIC
  if (net_log_.IsCapturing()) {
    net_log_.AddByteTransferEvent(
        NetLogEventType::BIDIRECTIONAL_STREAM_BYTES_RECEIVED, bytes_read,
        read_buffer_->data());
  }
#endif
  read_end_time_ = base::TimeTicks::Now();
  read_buffer_ = nullptr;
  delegate_->OnDataRead(bytes_read);
}

void BidirectionalStream::OnDataSent() {
  DCHECK(!write_buffer_list_.empty());
  DCHECK_EQ(write_buffer_list_.size(), write_buffer_len_list_.size());

  if (net_log_.IsCapturing()) {
    if (write_buffer_list_.size() > 1) {
      net_log_.BeginEvent(
          NetLogEventType::BIDIRECTIONAL_STREAM_BYTES_SENT_COALESCED,
          NetLog::IntCallback("num_buffers_coalesced",
                              write_buffer_list_.size()));
    }

#ifdef ENABLE_QUIC
    for (size_t i = 0; i < write_buffer_list_.size(); ++i) {
      net_log_.AddByteTransferEvent(
          NetLogEventType::BIDIRECTIONAL_STREAM_BYTES_SENT,
          write_buffer_len_list_[i], write_buffer_list_[i]->data());
    }
#endif

    if (write_buffer_list_.size() > 1) {
      net_log_.EndEvent(
          NetLogEventType::BIDIRECTIONAL_STREAM_BYTES_SENT_COALESCED);
    }
  }
  load_timing_info_.send_end = base::TimeTicks::Now();
  write_buffer_list_.clear();
  write_buffer_len_list_.clear();
  delegate_->OnDataSent();
}

void BidirectionalStream::OnTrailersReceived(
    const spdy::SpdyHeaderBlock& trailers) {
  if (net_log_.IsCapturing()) {
    net_log_.AddEvent(NetLogEventType::BIDIRECTIONAL_STREAM_RECV_TRAILERS,
                      base::Bind(&NetLogHeadersCallback, &trailers));
  }
  read_end_time_ = base::TimeTicks::Now();
  delegate_->OnTrailersReceived(trailers);
}

void BidirectionalStream::OnFailed(int status) {
  if (net_log_.IsCapturing()) {
    net_log_.AddEvent(NetLogEventType::BIDIRECTIONAL_STREAM_FAILED,
                      NetLog::IntCallback("net_error", status));
  }
  NotifyFailed(status);
}

#if defined(ENABLE_BORINGSSL)
void BidirectionalStream::OnStreamReady(const SSLConfig& used_ssl_config,

#ifdef ENABLE_PROXY
                                        const ProxyInfo& used_proxy_info,
#endif

                                        std::unique_ptr<HttpStream> stream) {
  NOTREACHED();
}
#endif // ENABLE_BORINGSSL

#if defined(ENABLE_BORINGSSL)
void BidirectionalStream::OnBidirectionalStreamImplReady(
    const SSLConfig& used_ssl_config,
#ifdef ENABLE_PROXY
    const ProxyInfo& used_proxy_info,
#endif
    std::unique_ptr<BidirectionalStreamImpl> stream) {
  DCHECK(!stream_impl_);

  net::NetworkTrafficAnnotationTag traffic_annotation =
      net::DefineNetworkTrafficAnnotation("bidirectional_stream", R"(
        semantics {
          sender: "Bidirectional Stream"
          description:
            "Bidirectional stream is used to exchange data with a server on "
            "behalf of an RPC API."
          trigger:
            "When an application makes an RPC to the server."
          data:
            "Any arbitrary data."
          destination: OTHER
          destination_other:
            "Any destination that the application chooses."
        }
        policy {
          cookies_allowed: NO
          setting: "This feature is not used in Chrome."
          policy_exception_justification:
            "This feature is not used in Chrome."
        }
    )");

  stream_request_.reset();
  stream_impl_ = std::move(stream);
  stream_impl_->Start(request_info_.get(), net_log_,
                      send_request_headers_automatically_, this,
                      std::move(timer_), traffic_annotation);
}

void BidirectionalStream::OnWebSocketHandshakeStreamReady(
    const SSLConfig& used_ssl_config
#ifdef ENABLE_PROXY
    , const ProxyInfo& used_proxy_info
    , std::unique_ptr<WebSocketHandshakeStreamBase> stream
#endif
    ) {
  NOTREACHED();
}

#ifdef ENABLE_QUIC
void BidirectionalStream::OnStreamFailed(
    int result,
    const NetErrorDetails& net_error_details,
    const SSLConfig& used_ssl_config
#ifdef ENABLE_PROXY
    ,
    const ProxyInfo& used_proxy_info
#endif
    ) {
  DCHECK_LT(result, 0);
  DCHECK_NE(result, ERR_IO_PENDING);
  DCHECK(stream_request_);

  NotifyFailed(result);
}
#endif

void BidirectionalStream::OnCertificateError(int result,
                                             const SSLConfig& used_ssl_config,
                                             const SSLInfo& ssl_info) {
  DCHECK_LT(result, 0);
  DCHECK_NE(result, ERR_IO_PENDING);
  DCHECK(stream_request_);

  NotifyFailed(result);
}

void BidirectionalStream::OnNeedsProxyAuth(
    const HttpResponseInfo& proxy_response,
    const SSLConfig& used_ssl_config,

#ifdef ENABLE_QUIC
    const ProxyInfo& used_proxy_info,
#endif
    HttpAuthController* auth_controller) {
  DCHECK(stream_request_);

  NotifyFailed(ERR_PROXY_AUTH_REQUESTED);
}

void BidirectionalStream::OnNeedsClientAuth(const SSLConfig& used_ssl_config,
                                            SSLCertRequestInfo* cert_info) {
  DCHECK(stream_request_);

  // BidirectionalStream doesn't support client auth. It ignores client auth
  // requests with null client cert and key.
  SSLConfig ssl_config = used_ssl_config;
  ssl_config.send_client_cert = true;
  ssl_config.client_cert = nullptr;
  ssl_config.client_private_key = nullptr;
  session_->ssl_client_auth_cache()->Add(cert_info->host_and_port, nullptr,
                                         nullptr);
  stream_request_ = nullptr;
  StartRequest(ssl_config);
}
#endif // ENABLE_BORINGSSL

void BidirectionalStream::OnQuicBroken() {}

void BidirectionalStream::NotifyFailed(int error) {
  delegate_->OnFailed(error);
}

void BidirectionalStream::UpdateHistograms() {
  // If the request failed before response is started, treat the metrics as
  // bogus and skip logging.
  if (load_timing_info_.request_start.is_null() ||
      load_timing_info_.receive_headers_end.is_null() ||
      read_end_time_.is_null() || load_timing_info_.send_start.is_null() ||
      load_timing_info_.send_end.is_null()) {
    return;
  }
  if (GetProtocol() == kProtoHTTP2) {
    UMA_HISTOGRAM_TIMES("Net.BidirectionalStream.TimeToReadStart.HTTP2",
                        load_timing_info_.receive_headers_end -
                            load_timing_info_.request_start);
    UMA_HISTOGRAM_TIMES("Net.BidirectionalStream.TimeToReadEnd.HTTP2",
                        read_end_time_ - load_timing_info_.request_start);
    UMA_HISTOGRAM_TIMES(
        "Net.BidirectionalStream.TimeToSendStart.HTTP2",
        load_timing_info_.send_start - load_timing_info_.request_start);
    UMA_HISTOGRAM_TIMES(
        "Net.BidirectionalStream.TimeToSendEnd.HTTP2",
        load_timing_info_.send_end - load_timing_info_.request_start);
    UMA_HISTOGRAM_COUNTS_1M("Net.BidirectionalStream.ReceivedBytes.HTTP2",
                            stream_impl_->GetTotalReceivedBytes());
    UMA_HISTOGRAM_COUNTS_1M("Net.BidirectionalStream.SentBytes.HTTP2",
                            stream_impl_->GetTotalSentBytes());
  } else if (GetProtocol() == kProtoQUIC) {
    UMA_HISTOGRAM_TIMES("Net.BidirectionalStream.TimeToReadStart.QUIC",
                        load_timing_info_.receive_headers_end -
                            load_timing_info_.request_start);
    UMA_HISTOGRAM_TIMES("Net.BidirectionalStream.TimeToReadEnd.QUIC",
                        read_end_time_ - load_timing_info_.request_start);
    UMA_HISTOGRAM_TIMES(
        "Net.BidirectionalStream.TimeToSendStart.QUIC",
        load_timing_info_.send_start - load_timing_info_.request_start);
    UMA_HISTOGRAM_TIMES(
        "Net.BidirectionalStream.TimeToSendEnd.QUIC",
        load_timing_info_.send_end - load_timing_info_.request_start);
    UMA_HISTOGRAM_COUNTS_1M("Net.BidirectionalStream.ReceivedBytes.QUIC",
                            stream_impl_->GetTotalReceivedBytes());
    UMA_HISTOGRAM_COUNTS_1M("Net.BidirectionalStream.SentBytes.QUIC",
                            stream_impl_->GetTotalSentBytes());
  }
}

}  // namespace net
