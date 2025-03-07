// Copyright 2013 The Chromium Authors. All rights reserved.
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

#include "mojo/public/cpp/test/module.test-mojom.h"

#include <math.h>
#include <stdint.h>
#include <utility>

#include "base/location.h"
#include "base/logging.h"
#include "base/run_loop.h"
#include "base/task/common/task_annotator.h"
#include "mojo/public/cpp/bindings/lib/message_internal.h"
#include "mojo/public/cpp/bindings/lib/serialization_util.h"
#include "mojo/public/cpp/bindings/lib/unserialized_message_context.h"
#include "mojo/public/cpp/bindings/lib/validate_params.h"
#include "mojo/public/cpp/bindings/lib/validation_context.h"
#include "mojo/public/cpp/bindings/lib/validation_errors.h"
#include "mojo/public/interfaces/bindings/interface_control_messages.mojom.h"

#include "mojo/public/cpp/test/module.test-mojom-params-data.h"
#include "mojo/public/cpp/test/module.test-mojom-shared-message-ids.h"

#include "mojo/public/cpp/test/module.test-mojom-import-headers.h"


#ifndef MOJO_PUBLIC_CPP_TEST_MODULE_TEST_MOJOM_JUMBO_H_
#define MOJO_PUBLIC_CPP_TEST_MODULE_TEST_MOJOM_JUMBO_H_
#endif
namespace test_module {
const char GlobalConst[] = "testString";
const char Struct::StructConst[] = "structConst";
Struct::Struct()
    : c() {}

Struct::Struct(
    bool c_in)
    : c(std::move(c_in)) {}

Struct::~Struct() = default;
size_t Struct::Hash(size_t seed) const {
  seed = mojo::internal::Hash(seed, this->c);
  return seed;
}

bool Struct::Validate(
    const void* data,
    mojo::internal::ValidationContext* validation_context) {
  return Data_::Validate(data, validation_context);
}
Union::Union() : tag_(Tag::A) {
  data_.a = bool();
}

Union::~Union() {
  DestroyActive();
}


void Union::set_a(
    bool a) {
  if (tag_ != Tag::A) {
    DestroyActive();
    tag_ = Tag::A;
  }
  data_.a = a;
}
void Union::set_b(
    int64_t b) {
  if (tag_ != Tag::B) {
    DestroyActive();
    tag_ = Tag::B;
  }
  data_.b = b;
}

void Union::DestroyActive() {
  switch (tag_) {

    case Tag::A:

      break;
    case Tag::B:

      break;
  }
}
size_t Union::Hash(size_t seed) const {
  seed = mojo::internal::HashCombine(seed, static_cast<uint32_t>(tag_));
  switch (tag_) {

    case Tag::A:
      return mojo::internal::Hash(seed, data_.a);
    case Tag::B:
      return mojo::internal::Hash(seed, data_.b);
    default:
      NOTREACHED();
      return seed;
  }
}

bool Union::Validate(
    const void* data,
    mojo::internal::ValidationContext* validation_context) {
  return Data_::Validate(data, validation_context, false);
}
const char Interface::Name_[] = "test_module.Interface";
constexpr bool Interface::InterfaceConst;

class Interface_DoSomethingElse_ForwardToCallback
    : public mojo::MessageReceiver {
 public:
  Interface_DoSomethingElse_ForwardToCallback(
      Interface::DoSomethingElseCallback callback
      ) : callback_(std::move(callback)) {
  }
  bool Accept(mojo::Message* message) override;
 private:
  Interface::DoSomethingElseCallback callback_;
  DISALLOW_COPY_AND_ASSIGN(Interface_DoSomethingElse_ForwardToCallback);
};

InterfaceProxy::InterfaceProxy(mojo::MessageReceiverWithResponder* receiver)
    : receiver_(receiver) {
}

void InterfaceProxy::DoSomething(
    ) {
#if BUILDFLAG(MOJO_TRACE_ENABLED)
  TRACE_EVENT0("mojom", "<class 'jinja2::utils::Namespace'>::Interface::DoSomething");
#endif
  const bool kExpectsResponse = false;
  const bool kIsSync = false;
  
  const uint32_t kFlags =
      ((kExpectsResponse) ? mojo::Message::kFlagExpectsResponse : 0) |
      ((kIsSync) ? mojo::Message::kFlagIsSync : 0);
  
  mojo::Message message(
      internal::kInterface_DoSomething_Name, kFlags, 0, 0, nullptr);
  auto* buffer = message.payload_buffer();
  ::test_module::internal::Interface_DoSomething_Params_Data::BufferWriter
      params;
  mojo::internal::SerializationContext serialization_context;
  params.Allocate(buffer);
  message.AttachHandlesFromSerializationContext(
      &serialization_context);

#if defined(ENABLE_IPC_FUZZER)
  message.set_interface_name(Interface::Name_);
  message.set_method_name("DoSomething");
#endif
  // This return value may be ignored as false implies the Connector has
  // encountered an error, which will be visible through other means.
  ignore_result(receiver_->Accept(&message));
}

void InterfaceProxy::DoSomethingElse(
    StructPtr in_s, DoSomethingElseCallback callback) {
#if BUILDFLAG(MOJO_TRACE_ENABLED)
  TRACE_EVENT0("mojom", "<class 'jinja2::utils::Namespace'>::Interface::DoSomethingElse");
#endif
  const bool kExpectsResponse = true;
  const bool kIsSync = false;
  
  const uint32_t kFlags =
      ((kExpectsResponse) ? mojo::Message::kFlagExpectsResponse : 0) |
      ((kIsSync) ? mojo::Message::kFlagIsSync : 0);
  
  mojo::Message message(
      internal::kInterface_DoSomethingElse_Name, kFlags, 0, 0, nullptr);
  auto* buffer = message.payload_buffer();
  ::test_module::internal::Interface_DoSomethingElse_Params_Data::BufferWriter
      params;
  mojo::internal::SerializationContext serialization_context;
  params.Allocate(buffer);
  typename decltype(params->s)::BaseType::BufferWriter
      s_writer;
  mojo::internal::Serialize<::test_module::StructDataView>(
      in_s, buffer, &s_writer, &serialization_context);
  params->s.Set(
      s_writer.is_null() ? nullptr : s_writer.data());
  MOJO_INTERNAL_DLOG_SERIALIZATION_WARNING(
      params->s.is_null(),
      mojo::internal::VALIDATION_ERROR_UNEXPECTED_NULL_POINTER,
      "null s in Interface.DoSomethingElse request");
  message.AttachHandlesFromSerializationContext(
      &serialization_context);

#if defined(ENABLE_IPC_FUZZER)
  message.set_interface_name(Interface::Name_);
  message.set_method_name("DoSomethingElse");
#endif
  std::unique_ptr<mojo::MessageReceiver> responder(
      new Interface_DoSomethingElse_ForwardToCallback(
          std::move(callback)));
  ignore_result(receiver_->AcceptWithResponder(&message, std::move(responder)));
}
class Interface_DoSomethingElse_ProxyToResponder {
 public:
  static Interface::DoSomethingElseCallback CreateCallback(
      uint64_t request_id,
      bool is_sync,
      std::unique_ptr<mojo::MessageReceiverWithStatus> responder) {
    std::unique_ptr<Interface_DoSomethingElse_ProxyToResponder> proxy(
        new Interface_DoSomethingElse_ProxyToResponder(
            request_id, is_sync, std::move(responder)));
    return base::BindOnce(&Interface_DoSomethingElse_ProxyToResponder::Run,
                          std::move(proxy));
  }

  ~Interface_DoSomethingElse_ProxyToResponder() {
#if DCHECK_IS_ON()
    if (responder_) {
      // If we're being destroyed without being run, we want to ensure the
      // binding endpoint has been closed. This checks for that asynchronously.
      // We pass a bound generated callback to handle the response so that any
      // resulting DCHECK stack will have useful interface type information.
      responder_->IsConnectedAsync(base::BindOnce(&OnIsConnectedComplete));
    }
#endif
    // If the Callback was dropped then deleting the responder will close
    // the pipe so the calling application knows to stop waiting for a reply.
    responder_ = nullptr;
  }

 private:
  Interface_DoSomethingElse_ProxyToResponder(
      uint64_t request_id,
      bool is_sync,
      std::unique_ptr<mojo::MessageReceiverWithStatus> responder)
      : request_id_(request_id),
        is_sync_(is_sync),
        responder_(std::move(responder)) {
  }

#if DCHECK_IS_ON()
  static void OnIsConnectedComplete(bool connected) {
    DCHECK(!connected)
        << "Interface::DoSomethingElseCallback was destroyed without "
        << "first either being run or its corresponding binding being closed. "
        << "It is an error to drop response callbacks which still correspond "
        << "to an open interface pipe.";
  }
#endif

  void Run(
      GlobalEnum in_e);

  uint64_t request_id_;
  bool is_sync_;
  std::unique_ptr<mojo::MessageReceiverWithStatus> responder_;

  DISALLOW_COPY_AND_ASSIGN(Interface_DoSomethingElse_ProxyToResponder);
};

bool Interface_DoSomethingElse_ForwardToCallback::Accept(
    mojo::Message* message) {
#if BUILDFLAG(MOJO_TRACE_ENABLED)
  TRACE_EVENT1("mojom", "<class 'jinja2::utils::Namespace'>::Interface::DoSomethingElseCallback",
               "message", message->name());
#endif
  mojo::internal::MessageDispatchContext dispatch_context(message);

  DCHECK(message->is_serialized());
  internal::Interface_DoSomethingElse_ResponseParams_Data* params =
      reinterpret_cast<
          internal::Interface_DoSomethingElse_ResponseParams_Data*>(
              message->mutable_payload());
  
  mojo::internal::SerializationContext serialization_context;
  serialization_context.TakeHandlesFromMessage(message);
  bool success = true;
  GlobalEnum p_e{};
  Interface_DoSomethingElse_ResponseParamsDataView input_data_view(params, &serialization_context);
  
  if (!input_data_view.ReadE(&p_e))
    success = false;
  if (!success) {
    ReportValidationErrorForMessage(
        message,
        mojo::internal::VALIDATION_ERROR_DESERIALIZATION_FAILED,
        Interface::Name_, 1, true);
    return false;
  }
  if (!callback_.is_null())
    std::move(callback_).Run(
std::move(p_e));
  return true;
}

void Interface_DoSomethingElse_ProxyToResponder::Run(
    GlobalEnum in_e) {
  
  const uint32_t kFlags = mojo::Message::kFlagIsResponse |
      ((is_sync_) ? mojo::Message::kFlagIsSync : 0);
  
  mojo::Message message(
      internal::kInterface_DoSomethingElse_Name, kFlags, 0, 0, nullptr);
  auto* buffer = message.payload_buffer();
  ::test_module::internal::Interface_DoSomethingElse_ResponseParams_Data::BufferWriter
      params;
  mojo::internal::SerializationContext serialization_context;
  params.Allocate(buffer);
  mojo::internal::Serialize<::test_module::GlobalEnum>(
      in_e, &params->e);
  message.AttachHandlesFromSerializationContext(
      &serialization_context);

#if BUILDFLAG(MOJO_TRACE_ENABLED)
  TRACE_EVENT1("mojom", "(Impl)<class 'jinja2::utils::Namespace'>::Interface::DoSomethingElseCallback",
               "message", message.name());
#endif

#if defined(ENABLE_IPC_FUZZER)
  message.set_interface_name(Interface::Name_);
  message.set_method_name("DoSomethingElse");
#endif

  message.set_request_id(request_id_);
  ignore_result(responder_->Accept(&message));
  // TODO(darin): Accept() returning false indicates a malformed message, and
  // that may be good reason to close the connection. However, we don't have a
  // way to do that from here. We should add a way.
  responder_ = nullptr;
}

// static
bool InterfaceStubDispatch::Accept(
    Interface* impl,
    mojo::Message* message) {
  switch (message->header()->name) {
    case internal::kInterface_DoSomething_Name: {
#if BUILDFLAG(MOJO_TRACE_ENABLED)
      TRACE_EVENT1(
          "mojom",
          "(Impl)<class 'jinja2::utils::Namespace'>::Interface::DoSomething",
          "message", message->name());
#endif
      base::TaskAnnotator::ScopedSetIpcProgramCounter scoped_ipc_pc(
          base::GetProgramCounter());
      mojo::internal::MessageDispatchContext context(message);

      DCHECK(message->is_serialized());
      internal::Interface_DoSomething_Params_Data* params =
          reinterpret_cast<internal::Interface_DoSomething_Params_Data*>(
              message->mutable_payload());
      
      mojo::internal::SerializationContext serialization_context;
      serialization_context.TakeHandlesFromMessage(message);
      bool success = true;
      Interface_DoSomething_ParamsDataView input_data_view(params, &serialization_context);
      
      if (!success) {
        ReportValidationErrorForMessage(
            message,
            mojo::internal::VALIDATION_ERROR_DESERIALIZATION_FAILED,
            Interface::Name_, 0, false);
        return false;
      }
      // A null |impl| means no implementation was bound.
      DCHECK(impl);
      impl->DoSomething();
      return true;
    }
    case internal::kInterface_DoSomethingElse_Name: {
      break;
    }
  }
  return false;
}

// static
bool InterfaceStubDispatch::AcceptWithResponder(
    Interface* impl,
    mojo::Message* message,
    std::unique_ptr<mojo::MessageReceiverWithStatus> responder) {
  switch (message->header()->name) {
    case internal::kInterface_DoSomething_Name: {
      break;
    }
    case internal::kInterface_DoSomethingElse_Name: {
#if BUILDFLAG(MOJO_TRACE_ENABLED)
      TRACE_EVENT1(
          "mojom",
          "(Impl)<class 'jinja2::utils::Namespace'>::Interface::DoSomethingElse",
          "message", message->name());
#endif
      base::TaskAnnotator::ScopedSetIpcProgramCounter scoped_ipc_pc(
          base::GetProgramCounter());
      mojo::internal::MessageDispatchContext context(message);

      internal::Interface_DoSomethingElse_Params_Data* params =
          reinterpret_cast<
              internal::Interface_DoSomethingElse_Params_Data*>(
                  message->mutable_payload());
      
      mojo::internal::SerializationContext serialization_context;
      serialization_context.TakeHandlesFromMessage(message);
      bool success = true;
      StructPtr p_s{};
      Interface_DoSomethingElse_ParamsDataView input_data_view(params, &serialization_context);
      
      if (!input_data_view.ReadS(&p_s))
        success = false;
      if (!success) {
        ReportValidationErrorForMessage(
            message,
            mojo::internal::VALIDATION_ERROR_DESERIALIZATION_FAILED,
            Interface::Name_, 1, false);
        return false;
      }
      Interface::DoSomethingElseCallback callback =
          Interface_DoSomethingElse_ProxyToResponder::CreateCallback(
              message->request_id(),
              message->has_flag(mojo::Message::kFlagIsSync),
              std::move(responder));
      // A null |impl| means no implementation was bound.
      DCHECK(impl);
      impl->DoSomethingElse(
std::move(p_s), std::move(callback));
      return true;
    }
  }
  return false;
}

bool InterfaceRequestValidator::Accept(mojo::Message* message) {
  if (!message->is_serialized() ||
      mojo::internal::ControlMessageHandler::IsControlMessage(message)) {
    return true;
  }

  mojo::internal::ValidationContext validation_context(
    message->payload(), message->payload_num_bytes(),
    message->handles()->size(), message->payload_num_interface_ids(), message,
    "Interface RequestValidator");

  switch (message->header()->name) {
    case internal::kInterface_DoSomething_Name: {
      if (!mojo::internal::ValidateMessageIsRequestWithoutResponse(
              message, &validation_context)) {
        return false;
      }
      if (!mojo::internal::ValidateMessagePayload<
               internal::Interface_DoSomething_Params_Data>(
                  message, &validation_context)) {
        return false;
      }
      return true;
    }
    case internal::kInterface_DoSomethingElse_Name: {
      if (!mojo::internal::ValidateMessageIsRequestExpectingResponse(
              message, &validation_context)) {
        return false;
      }
      if (!mojo::internal::ValidateMessagePayload<
               internal::Interface_DoSomethingElse_Params_Data>(
                  message, &validation_context)) {
        return false;
      }
      return true;
    }
    default:
      break;
  }

  // Unrecognized message.
  ReportValidationError(
      &validation_context,
      mojo::internal::VALIDATION_ERROR_MESSAGE_HEADER_UNKNOWN_METHOD);
  return false;
}

bool InterfaceResponseValidator::Accept(mojo::Message* message) {
  if (!message->is_serialized() ||
      mojo::internal::ControlMessageHandler::IsControlMessage(message)) {
    return true;
  }

  mojo::internal::ValidationContext validation_context(
    message->payload(), message->payload_num_bytes(),
    message->handles()->size(), message->payload_num_interface_ids(), message,
    "Interface ResponseValidator");

  if (!mojo::internal::ValidateMessageIsResponse(message, &validation_context))
    return false;
  switch (message->header()->name) {
    case internal::kInterface_DoSomethingElse_Name: {
      if (!mojo::internal::ValidateMessagePayload<
               internal::Interface_DoSomethingElse_ResponseParams_Data>(
                    message, &validation_context)) {
        return false;
      }
      return true;
    }
    default:
      break;
  }

  // Unrecognized message.
  ReportValidationError(
      &validation_context,
      mojo::internal::VALIDATION_ERROR_MESSAGE_HEADER_UNKNOWN_METHOD);
  return false;
}
}  // namespace test_module

namespace mojo {


// static
bool StructTraits<::test_module::Struct::DataView, ::test_module::StructPtr>::Read(
    ::test_module::Struct::DataView input,
    ::test_module::StructPtr* output) {
  bool success = true;
  ::test_module::StructPtr result(::test_module::Struct::New());
  
      result->c = input.c();
  *output = std::move(result);
  return success;
}

// static
bool UnionTraits<::test_module::Union::DataView, ::test_module::UnionPtr>::Read(
    ::test_module::Union::DataView input,
    ::test_module::UnionPtr* output) {
  using UnionType = ::test_module::Union;
  using Tag = UnionType::Tag;

  switch (input.tag()) {
    case Tag::A: {
      *output = UnionType::NewA(input.a());
      break;
    }
    case Tag::B: {
      *output = UnionType::NewB(input.b());
      break;
    }
    default:
      return false;
  }
  return true;
}

}  // namespace mojo

#if defined(__clang__)
#pragma clang diagnostic pop
#elif defined(_MSC_VER)
#pragma warning(pop)
#endif