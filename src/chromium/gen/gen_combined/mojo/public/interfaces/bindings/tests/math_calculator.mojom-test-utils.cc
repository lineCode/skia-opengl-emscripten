// Copyright 2019 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-private-field"
#elif defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4056)
#pragma warning(disable:4065)
#pragma warning(disable:4756)
#endif


#include "mojo/public/interfaces/bindings/tests/math_calculator.mojom-test-utils.h"

#include <utility>

#include "base/bind.h"
#include "base/run_loop.h"


#ifndef MOJO_PUBLIC_INTERFACES_BINDINGS_TESTS_MATH_CALCULATOR_MOJOM_JUMBO_H_
#define MOJO_PUBLIC_INTERFACES_BINDINGS_TESTS_MATH_CALCULATOR_MOJOM_JUMBO_H_
#endif


namespace math {


void CalculatorInterceptorForTesting::Clear(ClearCallback callback) {
  GetForwardingInterface()->Clear(std::move(callback));
}
void CalculatorInterceptorForTesting::Add(double value, AddCallback callback) {
  GetForwardingInterface()->Add(std::move(value), std::move(callback));
}
void CalculatorInterceptorForTesting::Multiply(double value, MultiplyCallback callback) {
  GetForwardingInterface()->Multiply(std::move(value), std::move(callback));
}
CalculatorAsyncWaiter::CalculatorAsyncWaiter(
    Calculator* proxy) : proxy_(proxy) {}

CalculatorAsyncWaiter::~CalculatorAsyncWaiter() = default;

void CalculatorAsyncWaiter::Clear(
    double* out_value) {
  base::RunLoop loop;
  proxy_->Clear(
      base::BindOnce(
          [](base::RunLoop* loop,
             double* out_value
,
             double value) {*out_value = std::move(value);
            loop->Quit();
          },
          &loop,
          out_value));
  loop.Run();
}
void CalculatorAsyncWaiter::Add(
    double value, double* out_value) {
  base::RunLoop loop;
  proxy_->Add(std::move(value),
      base::BindOnce(
          [](base::RunLoop* loop,
             double* out_value
,
             double value) {*out_value = std::move(value);
            loop->Quit();
          },
          &loop,
          out_value));
  loop.Run();
}
void CalculatorAsyncWaiter::Multiply(
    double value, double* out_value) {
  base::RunLoop loop;
  proxy_->Multiply(std::move(value),
      base::BindOnce(
          [](base::RunLoop* loop,
             double* out_value
,
             double value) {*out_value = std::move(value);
            loop->Quit();
          },
          &loop,
          out_value));
  loop.Run();
}





}  // namespace math

#if defined(__clang__)
#pragma clang diagnostic pop
#elif defined(_MSC_VER)
#pragma warning(pop)
#endif