// Copyright 2019 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MOJO_PUBLIC_INTERFACES_BINDINGS_TESTS_MATH_CALCULATOR_MOJOM_BLINK_TEST_UTILS_H_
#define MOJO_PUBLIC_INTERFACES_BINDINGS_TESTS_MATH_CALCULATOR_MOJOM_BLINK_TEST_UTILS_H_

#include "mojo/public/interfaces/bindings/tests/math_calculator.mojom-blink.h"


namespace math {
namespace blink {


class  CalculatorInterceptorForTesting : public Calculator {
  virtual Calculator* GetForwardingInterface() = 0;
  void Clear(ClearCallback callback) override;
  void Add(double value, AddCallback callback) override;
  void Multiply(double value, MultiplyCallback callback) override;
};
class  CalculatorAsyncWaiter {
 public:
  explicit CalculatorAsyncWaiter(Calculator* proxy);
  ~CalculatorAsyncWaiter();
  void Clear(
      double* out_value);
  void Add(
      double value, double* out_value);
  void Multiply(
      double value, double* out_value);

 private:
  Calculator* const proxy_;

  DISALLOW_COPY_AND_ASSIGN(CalculatorAsyncWaiter);
};




}  // namespace blink
}  // namespace math

#endif  // MOJO_PUBLIC_INTERFACES_BINDINGS_TESTS_MATH_CALCULATOR_MOJOM_BLINK_TEST_UTILS_H_