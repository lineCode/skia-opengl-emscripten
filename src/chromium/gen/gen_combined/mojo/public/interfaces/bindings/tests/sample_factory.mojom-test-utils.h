// Copyright 2019 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MOJO_PUBLIC_INTERFACES_BINDINGS_TESTS_SAMPLE_FACTORY_MOJOM_TEST_UTILS_H_
#define MOJO_PUBLIC_INTERFACES_BINDINGS_TESTS_SAMPLE_FACTORY_MOJOM_TEST_UTILS_H_

#include "mojo/public/interfaces/bindings/tests/sample_factory.mojom.h"


namespace sample {


class  NamedObjectInterceptorForTesting : public NamedObject {
  virtual NamedObject* GetForwardingInterface() = 0;
  void SetName(const std::string& name) override;
  void GetName(GetNameCallback callback) override;
};
class  NamedObjectAsyncWaiter {
 public:
  explicit NamedObjectAsyncWaiter(NamedObject* proxy);
  ~NamedObjectAsyncWaiter();
  void GetName(
      std::string* out_name);

 private:
  NamedObject* const proxy_;

  DISALLOW_COPY_AND_ASSIGN(NamedObjectAsyncWaiter);
};


class  FactoryInterceptorForTesting : public Factory {
  virtual Factory* GetForwardingInterface() = 0;
  void DoStuff(RequestPtr request, mojo::ScopedMessagePipeHandle pipe, DoStuffCallback callback) override;
  void DoStuff2(mojo::ScopedDataPipeConsumerHandle pipe, DoStuff2Callback callback) override;
  void CreateNamedObject(mojo::PendingReceiver<NamedObject> obj) override;
  void RequestImportedInterface(mojo::PendingReceiver<::imported::ImportedInterface> obj, RequestImportedInterfaceCallback callback) override;
  void TakeImportedInterface(mojo::PendingRemote<::imported::ImportedInterface> obj, TakeImportedInterfaceCallback callback) override;
};
class  FactoryAsyncWaiter {
 public:
  explicit FactoryAsyncWaiter(Factory* proxy);
  ~FactoryAsyncWaiter();
  void DoStuff(
      RequestPtr request, mojo::ScopedMessagePipeHandle pipe, ResponsePtr* out_response, std::string* out_text);
  void DoStuff2(
      mojo::ScopedDataPipeConsumerHandle pipe, std::string* out_text);
  void RequestImportedInterface(
      mojo::PendingReceiver<::imported::ImportedInterface> obj, mojo::PendingReceiver<::imported::ImportedInterface>* out_obj);
  void TakeImportedInterface(
      mojo::PendingRemote<::imported::ImportedInterface> obj, mojo::PendingRemote<::imported::ImportedInterface>* out_obj);

 private:
  Factory* const proxy_;

  DISALLOW_COPY_AND_ASSIGN(FactoryAsyncWaiter);
};




}  // namespace sample

#endif  // MOJO_PUBLIC_INTERFACES_BINDINGS_TESTS_SAMPLE_FACTORY_MOJOM_TEST_UTILS_H_