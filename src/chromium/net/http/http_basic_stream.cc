// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "net/http/http_basic_stream.h"

#include <utility>

#include "net/http/http_raw_request_headers.h"
#include "net/http/http_request_info.h"
#include "net/http/http_response_body_drainer.h"
#include "net/http/http_stream_parser.h"
#include "net/socket/client_socket_handle.h"
#include "net/ssl/ssl_cert_request_info.h"
#include "net/ssl/ssl_info.h"

namespace net {

HttpBasicStream::HttpBasicStream(std::unique_ptr<ClientSocketHandle> connection,
                                 bool using_proxy,
                                 bool http_09_on_non_default_ports_enabled)
    : state_(std::move(connection),
             using_proxy,
             http_09_on_non_default_ports_enabled) {}

HttpBasicStream::~HttpBasicStream() = default;

int HttpBasicStream::InitializeStream(const HttpRequestInfo* request_info,
                                      bool can_send_early,
                                      RequestPriority priority,
                                      const NetLogWithSource& net_log,
                                      CompletionOnceCallback callback) {
  DCHECK(request_info->traffic_annotation.is_valid());
  state_.Initialize(request_info, priority, net_log);
  int ret = OK;
  if (!can_send_early) {
    ret = parser()->ConfirmHandshake(std::move(callback));
  }
  return ret;
}

int HttpBasicStream::SendRequest(const HttpRequestHeaders& headers,
                                 HttpResponseInfo* response,
                                 CompletionOnceCallback callback) {
  DCHECK(parser());
  if (request_headers_callback_) {
    HttpRawRequestHeaders raw_headers;
    raw_headers.set_request_line(state_.GenerateRequestLine());
    for (net::HttpRequestHeaders::Iterator it(headers); it.GetNext();)
      raw_headers.Add(it.name(), it.value());
    request_headers_callback_.Run(std::move(raw_headers));
  }
  return parser()->SendRequest(
      state_.GenerateRequestLine(), headers,
      NetworkTrafficAnnotationTag(state_.traffic_annotation()), response,
      std::move(callback));
}

int HttpBasicStream::ReadResponseHeaders(CompletionOnceCallback callback) {
  return parser()->ReadResponseHeaders(std::move(callback));
}

int HttpBasicStream::ReadResponseBody(IOBuffer* buf,
                                      int buf_len,
                                      CompletionOnceCallback callback) {
  return parser()->ReadResponseBody(buf, buf_len, std::move(callback));
}

void HttpBasicStream::Close(bool not_reusable) {
  // parser() is null if |this| is created by an orphaned HttpStreamFactory::Job
  // in which case InitializeStream() will not have been called. This also
  // protects against null dereference in the case where
  // state_.ReleaseConnection() has been called.
  //
  // TODO(mmenke):  Can these cases be handled a bit more cleanly?
  // WebSocketHandshakeStream will need to be updated as well.
  if (!parser())
    return;
  StreamSocket* socket = state_.connection()->socket();
  if (not_reusable && socket)
    socket->Disconnect();
  state_.connection()->Reset();
}

HttpStream* HttpBasicStream::RenewStreamForAuth() {
  DCHECK(IsResponseBodyComplete());
  DCHECK(!parser()->IsMoreDataBuffered());
  // The HttpStreamParser object still has a pointer to the connection. Just to
  // be extra-sure it doesn't touch the connection again, delete it here rather
  // than leaving it until the destructor is called.
  state_.DeleteParser();
  return new HttpBasicStream(state_.ReleaseConnection(), state_.using_proxy(),
                             state_.http_09_on_non_default_ports_enabled());
}

bool HttpBasicStream::IsResponseBodyComplete() const {
  return parser()->IsResponseBodyComplete();
}

bool HttpBasicStream::IsConnectionReused() const {
  return state_.IsConnectionReused();
}

void HttpBasicStream::SetConnectionReused() {
  state_.connection()->set_reuse_type(ClientSocketHandle::REUSED_IDLE);
}

bool HttpBasicStream::CanReuseConnection() const {
  return state_.connection()->socket() && parser()->CanReuseConnection();
}

int64_t HttpBasicStream::GetTotalReceivedBytes() const {
  if (parser())
    return parser()->received_bytes();
  return 0;
}

int64_t HttpBasicStream::GetTotalSentBytes() const {
  if (parser())
    return parser()->sent_bytes();
  return 0;
}

bool HttpBasicStream::GetLoadTimingInfo(
    LoadTimingInfo* load_timing_info) const {
  if (!state_.connection()->GetLoadTimingInfo(IsConnectionReused(),
                                              load_timing_info) ||
      !parser()) {
    return false;
  }

  load_timing_info->receive_headers_start = parser()->response_start_time();
  return true;
}

bool HttpBasicStream::GetAlternativeService(
    AlternativeService* alternative_service) const {
  return false;
}

#if defined(ENABLE_BORINGSSL)
void HttpBasicStream::GetSSLInfo(SSLInfo* ssl_info) {
  if (!state_.connection()->socket()) {
    ssl_info->Reset();
    return;
  }
  parser()->GetSSLInfo(ssl_info);
}

void HttpBasicStream::GetSSLCertRequestInfo(
    SSLCertRequestInfo* cert_request_info) {
  if (!state_.connection()->socket()) {
    cert_request_info->Reset();
    return;
  }
  parser()->GetSSLCertRequestInfo(cert_request_info);
}
#endif // ENABLE_BORINGSSL

bool HttpBasicStream::GetRemoteEndpoint(IPEndPoint* endpoint) {
  if (!state_.connection() || !state_.connection()->socket())
    return false;

  return state_.connection()->socket()->GetPeerAddress(endpoint) == OK;
}

void HttpBasicStream::Drain(HttpNetworkSession* session) {
  HttpResponseBodyDrainer* drainer = new HttpResponseBodyDrainer(this);
  drainer->Start(session);
  // |drainer| will delete itself.
}

#ifdef ENABLE_QUIC
void HttpBasicStream::PopulateNetErrorDetails(NetErrorDetails* details) {
  // TODO(mmenke):  Consumers don't actually care about HTTP version, but seems
  // like the right version should be reported, if headers were received.
  details->connection_info = HttpResponseInfo::CONNECTION_INFO_HTTP1_1;
  return;
}
#endif

void HttpBasicStream::SetPriority(RequestPriority priority) {
  // TODO(akalin): Plumb this through to |connection_|.
}

void HttpBasicStream::SetRequestHeadersCallback(
    RequestHeadersCallback callback) {
  request_headers_callback_ = std::move(callback);
}

}  // namespace net
