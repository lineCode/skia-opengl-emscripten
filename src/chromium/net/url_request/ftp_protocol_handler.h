// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_URL_REQUEST_FTP_PROTOCOL_HANDLER_H_
#define NET_URL_REQUEST_FTP_PROTOCOL_HANDLER_H_

#include <memory>

#include "base/compiler_specific.h"
#include "base/macros.h"
#include "net/base/net_export.h"
#include "net/url_request/url_request_job_factory.h"

namespace net {

class FtpAuthCache;
class FtpTransactionFactory;
#if defined(ENABLE_DNS)
class HostResolver;
#endif
class NetworkDelegate;
class URLRequestJob;

// Implements a ProtocolHandler for FTP.
class NET_EXPORT FtpProtocolHandler :
    public URLRequestJobFactory::ProtocolHandler {
 public:
  ~FtpProtocolHandler() override;

  // Creates an FtpProtocolHandler using an FtpTransactionFactoryImpl and the
  // specified HostResolver.
  static std::unique_ptr<FtpProtocolHandler> Create(
#if defined(ENABLE_DNS)
      HostResolver* host_resolver
#endif
      );

  // Creates an FtpProtocolHandler using the specified FtpTransactionFactory, to
  // allow a mock to be used for testing.
  static std::unique_ptr<FtpProtocolHandler> CreateForTesting(
      std::unique_ptr<FtpTransactionFactory> ftp_transaction_factory);

  URLRequestJob* MaybeCreateJob(
      URLRequest* request,
      NetworkDelegate* network_delegate) const override;

 private:
  friend class FtpTestURLRequestContext;

  explicit FtpProtocolHandler(
      std::unique_ptr<FtpTransactionFactory> ftp_transaction_factory);

  std::unique_ptr<FtpTransactionFactory> ftp_transaction_factory_;
  std::unique_ptr<FtpAuthCache> ftp_auth_cache_;

  DISALLOW_COPY_AND_ASSIGN(FtpProtocolHandler);
};

}  // namespace net

#endif  // NET_URL_REQUEST_FTP_PROTOCOL_HANDLER_H_
