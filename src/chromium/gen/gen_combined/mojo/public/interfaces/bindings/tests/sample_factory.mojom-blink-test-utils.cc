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


#include "mojo/public/interfaces/bindings/tests/sample_factory.mojom-blink-test-utils.h"

#include <utility>

#include "base/bind.h"
#include "base/run_loop.h"
#include "mojo/public/cpp/bindings/lib/wtf_serialization.h"
#include "mojo/public/interfaces/bindings/tests/sample_import.mojom-blink.h"


#ifndef MOJO_PUBLIC_INTERFACES_BINDINGS_TESTS_SAMPLE_FACTORY_MOJOM_BLINK_JUMBO_H_
#define MOJO_PUBLIC_INTERFACES_BINDINGS_TESTS_SAMPLE_FACTORY_MOJOM_BLINK_JUMBO_H_
#endif


namespace sample {
namespace blink {


void NamedObjectInterceptorForTesting::SetName(const WTF::String& name) {
  GetForwardingInterface()->SetName(std::move(name));
}
void NamedObjectInterceptorForTesting::GetName(GetNameCallback callback) {
  GetForwardingInterface()->GetName(std::move(callback));
}
NamedObjectAsyncWaiter::NamedObjectAsyncWaiter(
    NamedObject* proxy) : proxy_(proxy) {}

NamedObjectAsyncWaiter::~NamedObjectAsyncWaiter() = default;

void NamedObjectAsyncWaiter::GetName(
    WTF::String* out_name) {
  base::RunLoop loop;
  proxy_->GetName(
      base::BindOnce(
          [](base::RunLoop* loop,
             WTF::String* out_name
,
             const WTF::String& name) {*out_name = std::move(name);
            loop->Quit();
          },
          &loop,
          out_name));
  loop.Run();
}



void FactoryInterceptorForTesting::DoStuff(RequestPtr request, mojo::ScopedMessagePipeHandle pipe, DoStuffCallback callback) {
  GetForwardingInterface()->DoStuff(std::move(request), std::move(pipe), std::move(callback));
}
void FactoryInterceptorForTesting::DoStuff2(mojo::ScopedDataPipeConsumerHandle pipe, DoStuff2Callback callback) {
  GetForwardingInterface()->DoStuff2(std::move(pipe), std::move(callback));
}
void FactoryInterceptorForTesting::CreateNamedObject(mojo::PendingReceiver<NamedObject> obj) {
  GetForwardingInterface()->CreateNamedObject(std::move(obj));
}
void FactoryInterceptorForTesting::RequestImportedInterface(mojo::PendingReceiver<::imported::blink::ImportedInterface> obj, RequestImportedInterfaceCallback callback) {
  GetForwardingInterface()->RequestImportedInterface(std::move(obj), std::move(callback));
}
void FactoryInterceptorForTesting::TakeImportedInterface(mojo::PendingRemote<::imported::blink::ImportedInterface> obj, TakeImportedInterfaceCallback callback) {
  GetForwardingInterface()->TakeImportedInterface(std::move(obj), std::move(callback));
}
FactoryAsyncWaiter::FactoryAsyncWaiter(
    Factory* proxy) : proxy_(proxy) {}

FactoryAsyncWaiter::~FactoryAsyncWaiter() = default;

void FactoryAsyncWaiter::DoStuff(
    RequestPtr request, mojo::ScopedMessagePipeHandle pipe, ResponsePtr* out_response, WTF::String* out_text) {
  base::RunLoop loop;
  proxy_->DoStuff(std::move(request),std::move(pipe),
      base::BindOnce(
          [](base::RunLoop* loop,
             ResponsePtr* out_response
,
             WTF::String* out_text
,
             ResponsePtr response,
             const WTF::String& text) {*out_response = std::move(response);*out_text = std::move(text);
            loop->Quit();
          },
          &loop,
          out_response,
          out_text));
  loop.Run();
}
void FactoryAsyncWaiter::DoStuff2(
    mojo::ScopedDataPipeConsumerHandle pipe, WTF::String* out_text) {
  base::RunLoop loop;
  proxy_->DoStuff2(std::move(pipe),
      base::BindOnce(
          [](base::RunLoop* loop,
             WTF::String* out_text
,
             const WTF::String& text) {*out_text = std::move(text);
            loop->Quit();
          },
          &loop,
          out_text));
  loop.Run();
}
void FactoryAsyncWaiter::RequestImportedInterface(
    mojo::PendingReceiver<::imported::blink::ImportedInterface> obj, mojo::PendingReceiver<::imported::blink::ImportedInterface>* out_obj) {
  base::RunLoop loop;
  proxy_->RequestImportedInterface(std::move(obj),
      base::BindOnce(
          [](base::RunLoop* loop,
             mojo::PendingReceiver<::imported::blink::ImportedInterface>* out_obj
,
             mojo::PendingReceiver<::imported::blink::ImportedInterface> obj) {*out_obj = std::move(obj);
            loop->Quit();
          },
          &loop,
          out_obj));
  loop.Run();
}
void FactoryAsyncWaiter::TakeImportedInterface(
    mojo::PendingRemote<::imported::blink::ImportedInterface> obj, mojo::PendingRemote<::imported::blink::ImportedInterface>* out_obj) {
  base::RunLoop loop;
  proxy_->TakeImportedInterface(std::move(obj),
      base::BindOnce(
          [](base::RunLoop* loop,
             mojo::PendingRemote<::imported::blink::ImportedInterface>* out_obj
,
             mojo::PendingRemote<::imported::blink::ImportedInterface> obj) {*out_obj = std::move(obj);
            loop->Quit();
          },
          &loop,
          out_obj));
  loop.Run();
}





}  // namespace blink
}  // namespace sample

#if defined(__clang__)
#pragma clang diagnostic pop
#elif defined(_MSC_VER)
#pragma warning(pop)
#endif