// Copyright 2019 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef THIRD_PARTY_BLINK_PUBLIC_MOJOM_PAYMENTS_PAYMENT_HANDLER_HOST_MOJOM_TEST_UTILS_H_
#define THIRD_PARTY_BLINK_PUBLIC_MOJOM_PAYMENTS_PAYMENT_HANDLER_HOST_MOJOM_TEST_UTILS_H_

#include "third_party/blink/public/mojom/payments/payment_handler_host.mojom.h"
#include "third_party/blink/public/common/common_export.h"


namespace payments {
namespace mojom {


class BLINK_COMMON_EXPORT PaymentHandlerHostInterceptorForTesting : public PaymentHandlerHost {
  virtual PaymentHandlerHost* GetForwardingInterface() = 0;
  void ChangePaymentMethod(PaymentHandlerMethodDataPtr method_data, ChangePaymentMethodCallback callback) override;
};
class BLINK_COMMON_EXPORT PaymentHandlerHostAsyncWaiter {
 public:
  explicit PaymentHandlerHostAsyncWaiter(PaymentHandlerHost* proxy);
  ~PaymentHandlerHostAsyncWaiter();
  void ChangePaymentMethod(
      PaymentHandlerMethodDataPtr method_data, PaymentMethodChangeResponsePtr* out_response_data);

 private:
  PaymentHandlerHost* const proxy_;

  DISALLOW_COPY_AND_ASSIGN(PaymentHandlerHostAsyncWaiter);
};




}  // namespace mojom
}  // namespace payments

#endif  // THIRD_PARTY_BLINK_PUBLIC_MOJOM_PAYMENTS_PAYMENT_HANDLER_HOST_MOJOM_TEST_UTILS_H_