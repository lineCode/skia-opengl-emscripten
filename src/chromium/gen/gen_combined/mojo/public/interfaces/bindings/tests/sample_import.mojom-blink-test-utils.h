// Copyright 2019 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MOJO_PUBLIC_INTERFACES_BINDINGS_TESTS_SAMPLE_IMPORT_MOJOM_BLINK_TEST_UTILS_H_
#define MOJO_PUBLIC_INTERFACES_BINDINGS_TESTS_SAMPLE_IMPORT_MOJOM_BLINK_TEST_UTILS_H_

#include "mojo/public/interfaces/bindings/tests/sample_import.mojom-blink.h"


namespace imported {
namespace blink {


class  ImportedInterfaceInterceptorForTesting : public ImportedInterface {
  virtual ImportedInterface* GetForwardingInterface() = 0;
  void DoSomething() override;
};
class  ImportedInterfaceAsyncWaiter {
 public:
  explicit ImportedInterfaceAsyncWaiter(ImportedInterface* proxy);
  ~ImportedInterfaceAsyncWaiter();

 private:
  ImportedInterface* const proxy_;

  DISALLOW_COPY_AND_ASSIGN(ImportedInterfaceAsyncWaiter);
};




}  // namespace blink
}  // namespace imported

#endif  // MOJO_PUBLIC_INTERFACES_BINDINGS_TESTS_SAMPLE_IMPORT_MOJOM_BLINK_TEST_UTILS_H_