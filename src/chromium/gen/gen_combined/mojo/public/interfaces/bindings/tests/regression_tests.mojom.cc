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

#include "mojo/public/interfaces/bindings/tests/regression_tests.mojom.h"

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

#include "mojo/public/interfaces/bindings/tests/regression_tests.mojom-params-data.h"
#include "mojo/public/interfaces/bindings/tests/regression_tests.mojom-shared-message-ids.h"

#include "mojo/public/interfaces/bindings/tests/regression_tests.mojom-import-headers.h"


#ifndef MOJO_PUBLIC_INTERFACES_BINDINGS_TESTS_REGRESSION_TESTS_MOJOM_JUMBO_H_
#define MOJO_PUBLIC_INTERFACES_BINDINGS_TESTS_REGRESSION_TESTS_MOJOM_JUMBO_H_
#endif
namespace regression_tests {
Edge::Edge()
    : v() {}

Edge::Edge(
    VertexPtr v_in)
    : v(std::move(v_in)) {}

Edge::~Edge() = default;

bool Edge::Validate(
    const void* data,
    mojo::internal::ValidationContext* validation_context) {
  return Data_::Validate(data, validation_context);
}
Vertex::Vertex()
    : e() {}

Vertex::Vertex(
    EmptyStructPtr e_in)
    : e(std::move(e_in)) {}

Vertex::~Vertex() = default;

bool Vertex::Validate(
    const void* data,
    mojo::internal::ValidationContext* validation_context) {
  return Data_::Validate(data, validation_context);
}
EmptyStruct::EmptyStruct() {}

EmptyStruct::~EmptyStruct() = default;
size_t EmptyStruct::Hash(size_t seed) const {
  return seed;
}

bool EmptyStruct::Validate(
    const void* data,
    mojo::internal::ValidationContext* validation_context) {
  return Data_::Validate(data, validation_context);
}
A::A()
    : b() {}

A::A(
    BPtr b_in)
    : b(std::move(b_in)) {}

A::~A() = default;

bool A::Validate(
    const void* data,
    mojo::internal::ValidationContext* validation_context) {
  return Data_::Validate(data, validation_context);
}
B::B()
    : a() {}

B::B(
    APtr a_in)
    : a(std::move(a_in)) {}

B::~B() = default;

bool B::Validate(
    const void* data,
    mojo::internal::ValidationContext* validation_context) {
  return Data_::Validate(data, validation_context);
}
HandlesNameCollisionStruct::HandlesNameCollisionStruct()
    : handles() {}

HandlesNameCollisionStruct::HandlesNameCollisionStruct(
    EmptyStructPtr handles_in)
    : handles(std::move(handles_in)) {}

HandlesNameCollisionStruct::~HandlesNameCollisionStruct() = default;
size_t HandlesNameCollisionStruct::Hash(size_t seed) const {
  seed = mojo::internal::Hash(seed, this->handles);
  return seed;
}

bool HandlesNameCollisionStruct::Validate(
    const void* data,
    mojo::internal::ValidationContext* validation_context) {
  return Data_::Validate(data, validation_context);
}
HandlesHandleNameCollisionStruct::HandlesHandleNameCollisionStruct()
    : handles() {}

HandlesHandleNameCollisionStruct::HandlesHandleNameCollisionStruct(
    mojo::ScopedHandle handles_in)
    : handles(std::move(handles_in)) {}

HandlesHandleNameCollisionStruct::~HandlesHandleNameCollisionStruct() = default;

bool HandlesHandleNameCollisionStruct::Validate(
    const void* data,
    mojo::internal::ValidationContext* validation_context) {
  return Data_::Validate(data, validation_context);
}
HandlesUnionNameCollisionStruct::HandlesUnionNameCollisionStruct()
    : handles() {}

HandlesUnionNameCollisionStruct::HandlesUnionNameCollisionStruct(
    HandlesNameCollisionUnionPtr handles_in)
    : handles(std::move(handles_in)) {}

HandlesUnionNameCollisionStruct::~HandlesUnionNameCollisionStruct() = default;
size_t HandlesUnionNameCollisionStruct::Hash(size_t seed) const {
  seed = mojo::internal::Hash(seed, this->handles);
  return seed;
}

bool HandlesUnionNameCollisionStruct::Validate(
    const void* data,
    mojo::internal::ValidationContext* validation_context) {
  return Data_::Validate(data, validation_context);
}
HandlesNameCollisionUnion::HandlesNameCollisionUnion() : tag_(Tag::HANDLES) {
  data_.handles = int32_t();
}

HandlesNameCollisionUnion::~HandlesNameCollisionUnion() {
  DestroyActive();
}


void HandlesNameCollisionUnion::set_handles(
    int32_t handles) {
  if (tag_ != Tag::HANDLES) {
    DestroyActive();
    tag_ = Tag::HANDLES;
  }
  data_.handles = handles;
}

void HandlesNameCollisionUnion::DestroyActive() {
  switch (tag_) {

    case Tag::HANDLES:

      break;
  }
}
size_t HandlesNameCollisionUnion::Hash(size_t seed) const {
  seed = mojo::internal::HashCombine(seed, static_cast<uint32_t>(tag_));
  switch (tag_) {

    case Tag::HANDLES:
      return mojo::internal::Hash(seed, data_.handles);
    default:
      NOTREACHED();
      return seed;
  }
}

bool HandlesNameCollisionUnion::Validate(
    const void* data,
    mojo::internal::ValidationContext* validation_context) {
  return Data_::Validate(data, validation_context, false);
}
const char CheckMethodWithEmptyResponse::Name_[] = "regression_tests.CheckMethodWithEmptyResponse";

class CheckMethodWithEmptyResponse_WithouParameterAndEmptyResponse_ForwardToCallback
    : public mojo::MessageReceiver {
 public:
  CheckMethodWithEmptyResponse_WithouParameterAndEmptyResponse_ForwardToCallback(
      CheckMethodWithEmptyResponse::WithouParameterAndEmptyResponseCallback callback
      ) : callback_(std::move(callback)) {
  }
  bool Accept(mojo::Message* message) override;
 private:
  CheckMethodWithEmptyResponse::WithouParameterAndEmptyResponseCallback callback_;
  DISALLOW_COPY_AND_ASSIGN(CheckMethodWithEmptyResponse_WithouParameterAndEmptyResponse_ForwardToCallback);
};

class CheckMethodWithEmptyResponse_WithParameterAndEmptyResponse_ForwardToCallback
    : public mojo::MessageReceiver {
 public:
  CheckMethodWithEmptyResponse_WithParameterAndEmptyResponse_ForwardToCallback(
      CheckMethodWithEmptyResponse::WithParameterAndEmptyResponseCallback callback
      ) : callback_(std::move(callback)) {
  }
  bool Accept(mojo::Message* message) override;
 private:
  CheckMethodWithEmptyResponse::WithParameterAndEmptyResponseCallback callback_;
  DISALLOW_COPY_AND_ASSIGN(CheckMethodWithEmptyResponse_WithParameterAndEmptyResponse_ForwardToCallback);
};

CheckMethodWithEmptyResponseProxy::CheckMethodWithEmptyResponseProxy(mojo::MessageReceiverWithResponder* receiver)
    : receiver_(receiver) {
}
class CheckMethodWithEmptyResponseProxy_WithouParameterAndEmptyResponse_Message
    : public mojo::internal::UnserializedMessageContext {
 public:
  static const mojo::internal::UnserializedMessageContext::Tag kMessageTag;

  explicit CheckMethodWithEmptyResponseProxy_WithouParameterAndEmptyResponse_Message(
      uint32_t message_flags
  )
      : mojo::internal::UnserializedMessageContext(
          &kMessageTag,
          internal::kCheckMethodWithEmptyResponse_WithouParameterAndEmptyResponse_Name,
          message_flags){}
  ~CheckMethodWithEmptyResponseProxy_WithouParameterAndEmptyResponse_Message() override = default;

  static mojo::Message Build(
      bool serialize,
      bool expects_response,
      bool is_sync) {

    
  const uint32_t kFlags =
      ((expects_response) ? mojo::Message::kFlagExpectsResponse : 0) |
      ((is_sync) ? mojo::Message::kFlagIsSync : 0);

    if (!serialize) {
      return mojo::Message(std::make_unique<CheckMethodWithEmptyResponseProxy_WithouParameterAndEmptyResponse_Message>(
          kFlags
          ));
    }

    DCHECK(serialize);
    
  mojo::Message message(
      internal::kCheckMethodWithEmptyResponse_WithouParameterAndEmptyResponse_Name, kFlags, 0, 0, nullptr);
  auto* buffer = message.payload_buffer();
  ::regression_tests::internal::CheckMethodWithEmptyResponse_WithouParameterAndEmptyResponse_Params_Data::BufferWriter
      params;
  mojo::internal::SerializationContext serialization_context;
  params.Allocate(buffer);
  message.AttachHandlesFromSerializationContext(
      &serialization_context);
    return message;
  }


  void Dispatch(CheckMethodWithEmptyResponse* impl, CheckMethodWithEmptyResponse::WithouParameterAndEmptyResponseCallback callback) {
    impl->WithouParameterAndEmptyResponse(std::move(callback));
  }

 private:
  // mojo::internal::UnserializedMessageContext:
  void Serialize(mojo::internal::SerializationContext* serialization_context,
                 mojo::internal::Buffer* buffer) override {
    ::regression_tests::internal::CheckMethodWithEmptyResponse_WithouParameterAndEmptyResponse_Params_Data::BufferWriter
        params;
    params.Allocate(buffer);
  }

  DISALLOW_COPY_AND_ASSIGN(CheckMethodWithEmptyResponseProxy_WithouParameterAndEmptyResponse_Message);
};

const mojo::internal::UnserializedMessageContext::Tag
CheckMethodWithEmptyResponseProxy_WithouParameterAndEmptyResponse_Message::kMessageTag = {};

void CheckMethodWithEmptyResponseProxy::WithouParameterAndEmptyResponse(
    WithouParameterAndEmptyResponseCallback callback) {
#if BUILDFLAG(MOJO_TRACE_ENABLED)
  TRACE_EVENT0("mojom", "<class 'jinja2::utils::Namespace'>::CheckMethodWithEmptyResponse::WithouParameterAndEmptyResponse");
#endif
  const bool kExpectsResponse = true;
  const bool kIsSync = false;
  const bool kSerialize = receiver_->PrefersSerializedMessages();
  auto message = CheckMethodWithEmptyResponseProxy_WithouParameterAndEmptyResponse_Message::Build(
      kSerialize, kExpectsResponse, kIsSync);

#if defined(ENABLE_IPC_FUZZER)
  message.set_interface_name(CheckMethodWithEmptyResponse::Name_);
  message.set_method_name("WithouParameterAndEmptyResponse");
#endif
  std::unique_ptr<mojo::MessageReceiver> responder(
      new CheckMethodWithEmptyResponse_WithouParameterAndEmptyResponse_ForwardToCallback(
          std::move(callback)));
  ignore_result(receiver_->AcceptWithResponder(&message, std::move(responder)));
}
class CheckMethodWithEmptyResponseProxy_WithParameterAndEmptyResponse_Message
    : public mojo::internal::UnserializedMessageContext {
 public:
  static const mojo::internal::UnserializedMessageContext::Tag kMessageTag;

  explicit CheckMethodWithEmptyResponseProxy_WithParameterAndEmptyResponse_Message(
      uint32_t message_flags
      , bool param_b
  )
      : mojo::internal::UnserializedMessageContext(
          &kMessageTag,
          internal::kCheckMethodWithEmptyResponse_WithParameterAndEmptyResponse_Name,
          message_flags)
      , param_b_(std::move(param_b)){}
  ~CheckMethodWithEmptyResponseProxy_WithParameterAndEmptyResponse_Message() override = default;

  static mojo::Message Build(
      bool serialize,
      bool expects_response,
      bool is_sync,
      bool param_b) {

    
  const uint32_t kFlags =
      ((expects_response) ? mojo::Message::kFlagExpectsResponse : 0) |
      ((is_sync) ? mojo::Message::kFlagIsSync : 0);

    if (!serialize) {
      return mojo::Message(std::make_unique<CheckMethodWithEmptyResponseProxy_WithParameterAndEmptyResponse_Message>(
          kFlags
          , std::move(param_b)
          ));
    }

    DCHECK(serialize);
    
  mojo::Message message(
      internal::kCheckMethodWithEmptyResponse_WithParameterAndEmptyResponse_Name, kFlags, 0, 0, nullptr);
  auto* buffer = message.payload_buffer();
  ::regression_tests::internal::CheckMethodWithEmptyResponse_WithParameterAndEmptyResponse_Params_Data::BufferWriter
      params;
  mojo::internal::SerializationContext serialization_context;
  params.Allocate(buffer);
  params->b = param_b;
  message.AttachHandlesFromSerializationContext(
      &serialization_context);
    return message;
  }


  void Dispatch(CheckMethodWithEmptyResponse* impl, CheckMethodWithEmptyResponse::WithParameterAndEmptyResponseCallback callback) {
    impl->WithParameterAndEmptyResponse(
        std::move(param_b_), std::move(callback));
  }

 private:
  // mojo::internal::UnserializedMessageContext:
  void Serialize(mojo::internal::SerializationContext* serialization_context,
                 mojo::internal::Buffer* buffer) override {
    ::regression_tests::internal::CheckMethodWithEmptyResponse_WithParameterAndEmptyResponse_Params_Data::BufferWriter
        params;
    params.Allocate(buffer);
  params->b = param_b_;
  }
  bool param_b_;

  DISALLOW_COPY_AND_ASSIGN(CheckMethodWithEmptyResponseProxy_WithParameterAndEmptyResponse_Message);
};

const mojo::internal::UnserializedMessageContext::Tag
CheckMethodWithEmptyResponseProxy_WithParameterAndEmptyResponse_Message::kMessageTag = {};

void CheckMethodWithEmptyResponseProxy::WithParameterAndEmptyResponse(
    bool in_b, WithParameterAndEmptyResponseCallback callback) {
#if BUILDFLAG(MOJO_TRACE_ENABLED)
  TRACE_EVENT0("mojom", "<class 'jinja2::utils::Namespace'>::CheckMethodWithEmptyResponse::WithParameterAndEmptyResponse");
#endif
  const bool kExpectsResponse = true;
  const bool kIsSync = false;
  const bool kSerialize = receiver_->PrefersSerializedMessages();
  auto message = CheckMethodWithEmptyResponseProxy_WithParameterAndEmptyResponse_Message::Build(
      kSerialize, kExpectsResponse, kIsSync, std::move(in_b));

#if defined(ENABLE_IPC_FUZZER)
  message.set_interface_name(CheckMethodWithEmptyResponse::Name_);
  message.set_method_name("WithParameterAndEmptyResponse");
#endif
  std::unique_ptr<mojo::MessageReceiver> responder(
      new CheckMethodWithEmptyResponse_WithParameterAndEmptyResponse_ForwardToCallback(
          std::move(callback)));
  ignore_result(receiver_->AcceptWithResponder(&message, std::move(responder)));
}
class CheckMethodWithEmptyResponse_WithouParameterAndEmptyResponse_ProxyToResponder {
 public:
  static CheckMethodWithEmptyResponse::WithouParameterAndEmptyResponseCallback CreateCallback(
      uint64_t request_id,
      bool is_sync,
      std::unique_ptr<mojo::MessageReceiverWithStatus> responder) {
    std::unique_ptr<CheckMethodWithEmptyResponse_WithouParameterAndEmptyResponse_ProxyToResponder> proxy(
        new CheckMethodWithEmptyResponse_WithouParameterAndEmptyResponse_ProxyToResponder(
            request_id, is_sync, std::move(responder)));
    return base::BindOnce(&CheckMethodWithEmptyResponse_WithouParameterAndEmptyResponse_ProxyToResponder::Run,
                          std::move(proxy));
  }

  ~CheckMethodWithEmptyResponse_WithouParameterAndEmptyResponse_ProxyToResponder() {
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
  CheckMethodWithEmptyResponse_WithouParameterAndEmptyResponse_ProxyToResponder(
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
        << "CheckMethodWithEmptyResponse::WithouParameterAndEmptyResponseCallback was destroyed without "
        << "first either being run or its corresponding binding being closed. "
        << "It is an error to drop response callbacks which still correspond "
        << "to an open interface pipe.";
  }
#endif

  void Run(
      );

  uint64_t request_id_;
  bool is_sync_;
  std::unique_ptr<mojo::MessageReceiverWithStatus> responder_;

  DISALLOW_COPY_AND_ASSIGN(CheckMethodWithEmptyResponse_WithouParameterAndEmptyResponse_ProxyToResponder);
};
class CheckMethodWithEmptyResponse_WithouParameterAndEmptyResponse_Response_Message
    : public mojo::internal::UnserializedMessageContext {
 public:
  static const mojo::internal::UnserializedMessageContext::Tag kMessageTag;

  explicit CheckMethodWithEmptyResponse_WithouParameterAndEmptyResponse_Response_Message(
      uint32_t message_flags
  )
      : mojo::internal::UnserializedMessageContext(
          &kMessageTag,
          internal::kCheckMethodWithEmptyResponse_WithouParameterAndEmptyResponse_Name,
          message_flags){}
  ~CheckMethodWithEmptyResponse_WithouParameterAndEmptyResponse_Response_Message() override = default;

  static mojo::Message Build(
      bool serialize,
      bool is_sync) {

    
  const uint32_t kFlags = mojo::Message::kFlagIsResponse |
      ((is_sync) ? mojo::Message::kFlagIsSync : 0);

    if (!serialize) {
      return mojo::Message(std::make_unique<CheckMethodWithEmptyResponse_WithouParameterAndEmptyResponse_Response_Message>(
          kFlags
          ));
    }

    DCHECK(serialize);
    
  mojo::Message message(
      internal::kCheckMethodWithEmptyResponse_WithouParameterAndEmptyResponse_Name, kFlags, 0, 0, nullptr);
  auto* buffer = message.payload_buffer();
  ::regression_tests::internal::CheckMethodWithEmptyResponse_WithouParameterAndEmptyResponse_ResponseParams_Data::BufferWriter
      params;
  mojo::internal::SerializationContext serialization_context;
  params.Allocate(buffer);
  message.AttachHandlesFromSerializationContext(
      &serialization_context);
    return message;
  }


  void Dispatch(CheckMethodWithEmptyResponse::WithouParameterAndEmptyResponseCallback* callback) {
    std::move(*callback).Run();
  }



 private:
  // mojo::internal::UnserializedMessageContext:
  void Serialize(mojo::internal::SerializationContext* serialization_context,
                 mojo::internal::Buffer* buffer) override {
    ::regression_tests::internal::CheckMethodWithEmptyResponse_WithouParameterAndEmptyResponse_ResponseParams_Data::BufferWriter
        params;
    params.Allocate(buffer);
  }

  DISALLOW_COPY_AND_ASSIGN(CheckMethodWithEmptyResponse_WithouParameterAndEmptyResponse_Response_Message);
};

const mojo::internal::UnserializedMessageContext::Tag
CheckMethodWithEmptyResponse_WithouParameterAndEmptyResponse_Response_Message::kMessageTag = {};

bool CheckMethodWithEmptyResponse_WithouParameterAndEmptyResponse_ForwardToCallback::Accept(
    mojo::Message* message) {
#if BUILDFLAG(MOJO_TRACE_ENABLED)
  TRACE_EVENT1("mojom", "<class 'jinja2::utils::Namespace'>::CheckMethodWithEmptyResponse::WithouParameterAndEmptyResponseCallback",
               "message", message->name());
#endif
  mojo::internal::MessageDispatchContext dispatch_context(message);
  if (!message->is_serialized()) {
    auto context =
        message->TakeUnserializedContext<CheckMethodWithEmptyResponse_WithouParameterAndEmptyResponse_Response_Message>();
    if (!context) {
      // The Message was not of the expected type. It may be a valid message
      // which was build using a different variant of these bindings. Force
      // serialization before dispatch in this case.
      message->SerializeIfNecessary();
    } else {
      if (!callback_.is_null())
        context->Dispatch(&callback_);
      return true;
    }
  }

  DCHECK(message->is_serialized());
  internal::CheckMethodWithEmptyResponse_WithouParameterAndEmptyResponse_ResponseParams_Data* params =
      reinterpret_cast<
          internal::CheckMethodWithEmptyResponse_WithouParameterAndEmptyResponse_ResponseParams_Data*>(
              message->mutable_payload());
  
  mojo::internal::SerializationContext serialization_context;
  serialization_context.TakeHandlesFromMessage(message);
  bool success = true;
  CheckMethodWithEmptyResponse_WithouParameterAndEmptyResponse_ResponseParamsDataView input_data_view(params, &serialization_context);
  
  if (!success) {
    ReportValidationErrorForMessage(
        message,
        mojo::internal::VALIDATION_ERROR_DESERIALIZATION_FAILED,
        CheckMethodWithEmptyResponse::Name_, 0, true);
    return false;
  }
  if (!callback_.is_null())
    std::move(callback_).Run();
  return true;
}

void CheckMethodWithEmptyResponse_WithouParameterAndEmptyResponse_ProxyToResponder::Run(
    ) {
  const bool kSerialize = responder_->PrefersSerializedMessages();
  auto message = CheckMethodWithEmptyResponse_WithouParameterAndEmptyResponse_Response_Message::Build(kSerialize, is_sync_);

#if BUILDFLAG(MOJO_TRACE_ENABLED)
  TRACE_EVENT1("mojom", "(Impl)<class 'jinja2::utils::Namespace'>::CheckMethodWithEmptyResponse::WithouParameterAndEmptyResponseCallback",
               "message", message.name());
#endif

#if defined(ENABLE_IPC_FUZZER)
  message.set_interface_name(CheckMethodWithEmptyResponse::Name_);
  message.set_method_name("WithouParameterAndEmptyResponse");
#endif

  message.set_request_id(request_id_);
  ignore_result(responder_->Accept(&message));
  // TODO(darin): Accept() returning false indicates a malformed message, and
  // that may be good reason to close the connection. However, we don't have a
  // way to do that from here. We should add a way.
  responder_ = nullptr;
}
class CheckMethodWithEmptyResponse_WithParameterAndEmptyResponse_ProxyToResponder {
 public:
  static CheckMethodWithEmptyResponse::WithParameterAndEmptyResponseCallback CreateCallback(
      uint64_t request_id,
      bool is_sync,
      std::unique_ptr<mojo::MessageReceiverWithStatus> responder) {
    std::unique_ptr<CheckMethodWithEmptyResponse_WithParameterAndEmptyResponse_ProxyToResponder> proxy(
        new CheckMethodWithEmptyResponse_WithParameterAndEmptyResponse_ProxyToResponder(
            request_id, is_sync, std::move(responder)));
    return base::BindOnce(&CheckMethodWithEmptyResponse_WithParameterAndEmptyResponse_ProxyToResponder::Run,
                          std::move(proxy));
  }

  ~CheckMethodWithEmptyResponse_WithParameterAndEmptyResponse_ProxyToResponder() {
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
  CheckMethodWithEmptyResponse_WithParameterAndEmptyResponse_ProxyToResponder(
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
        << "CheckMethodWithEmptyResponse::WithParameterAndEmptyResponseCallback was destroyed without "
        << "first either being run or its corresponding binding being closed. "
        << "It is an error to drop response callbacks which still correspond "
        << "to an open interface pipe.";
  }
#endif

  void Run(
      );

  uint64_t request_id_;
  bool is_sync_;
  std::unique_ptr<mojo::MessageReceiverWithStatus> responder_;

  DISALLOW_COPY_AND_ASSIGN(CheckMethodWithEmptyResponse_WithParameterAndEmptyResponse_ProxyToResponder);
};
class CheckMethodWithEmptyResponse_WithParameterAndEmptyResponse_Response_Message
    : public mojo::internal::UnserializedMessageContext {
 public:
  static const mojo::internal::UnserializedMessageContext::Tag kMessageTag;

  explicit CheckMethodWithEmptyResponse_WithParameterAndEmptyResponse_Response_Message(
      uint32_t message_flags
  )
      : mojo::internal::UnserializedMessageContext(
          &kMessageTag,
          internal::kCheckMethodWithEmptyResponse_WithParameterAndEmptyResponse_Name,
          message_flags){}
  ~CheckMethodWithEmptyResponse_WithParameterAndEmptyResponse_Response_Message() override = default;

  static mojo::Message Build(
      bool serialize,
      bool is_sync) {

    
  const uint32_t kFlags = mojo::Message::kFlagIsResponse |
      ((is_sync) ? mojo::Message::kFlagIsSync : 0);

    if (!serialize) {
      return mojo::Message(std::make_unique<CheckMethodWithEmptyResponse_WithParameterAndEmptyResponse_Response_Message>(
          kFlags
          ));
    }

    DCHECK(serialize);
    
  mojo::Message message(
      internal::kCheckMethodWithEmptyResponse_WithParameterAndEmptyResponse_Name, kFlags, 0, 0, nullptr);
  auto* buffer = message.payload_buffer();
  ::regression_tests::internal::CheckMethodWithEmptyResponse_WithParameterAndEmptyResponse_ResponseParams_Data::BufferWriter
      params;
  mojo::internal::SerializationContext serialization_context;
  params.Allocate(buffer);
  message.AttachHandlesFromSerializationContext(
      &serialization_context);
    return message;
  }


  void Dispatch(CheckMethodWithEmptyResponse::WithParameterAndEmptyResponseCallback* callback) {
    std::move(*callback).Run();
  }



 private:
  // mojo::internal::UnserializedMessageContext:
  void Serialize(mojo::internal::SerializationContext* serialization_context,
                 mojo::internal::Buffer* buffer) override {
    ::regression_tests::internal::CheckMethodWithEmptyResponse_WithParameterAndEmptyResponse_ResponseParams_Data::BufferWriter
        params;
    params.Allocate(buffer);
  }

  DISALLOW_COPY_AND_ASSIGN(CheckMethodWithEmptyResponse_WithParameterAndEmptyResponse_Response_Message);
};

const mojo::internal::UnserializedMessageContext::Tag
CheckMethodWithEmptyResponse_WithParameterAndEmptyResponse_Response_Message::kMessageTag = {};

bool CheckMethodWithEmptyResponse_WithParameterAndEmptyResponse_ForwardToCallback::Accept(
    mojo::Message* message) {
#if BUILDFLAG(MOJO_TRACE_ENABLED)
  TRACE_EVENT1("mojom", "<class 'jinja2::utils::Namespace'>::CheckMethodWithEmptyResponse::WithParameterAndEmptyResponseCallback",
               "message", message->name());
#endif
  mojo::internal::MessageDispatchContext dispatch_context(message);
  if (!message->is_serialized()) {
    auto context =
        message->TakeUnserializedContext<CheckMethodWithEmptyResponse_WithParameterAndEmptyResponse_Response_Message>();
    if (!context) {
      // The Message was not of the expected type. It may be a valid message
      // which was build using a different variant of these bindings. Force
      // serialization before dispatch in this case.
      message->SerializeIfNecessary();
    } else {
      if (!callback_.is_null())
        context->Dispatch(&callback_);
      return true;
    }
  }

  DCHECK(message->is_serialized());
  internal::CheckMethodWithEmptyResponse_WithParameterAndEmptyResponse_ResponseParams_Data* params =
      reinterpret_cast<
          internal::CheckMethodWithEmptyResponse_WithParameterAndEmptyResponse_ResponseParams_Data*>(
              message->mutable_payload());
  
  mojo::internal::SerializationContext serialization_context;
  serialization_context.TakeHandlesFromMessage(message);
  bool success = true;
  CheckMethodWithEmptyResponse_WithParameterAndEmptyResponse_ResponseParamsDataView input_data_view(params, &serialization_context);
  
  if (!success) {
    ReportValidationErrorForMessage(
        message,
        mojo::internal::VALIDATION_ERROR_DESERIALIZATION_FAILED,
        CheckMethodWithEmptyResponse::Name_, 1, true);
    return false;
  }
  if (!callback_.is_null())
    std::move(callback_).Run();
  return true;
}

void CheckMethodWithEmptyResponse_WithParameterAndEmptyResponse_ProxyToResponder::Run(
    ) {
  const bool kSerialize = responder_->PrefersSerializedMessages();
  auto message = CheckMethodWithEmptyResponse_WithParameterAndEmptyResponse_Response_Message::Build(kSerialize, is_sync_);

#if BUILDFLAG(MOJO_TRACE_ENABLED)
  TRACE_EVENT1("mojom", "(Impl)<class 'jinja2::utils::Namespace'>::CheckMethodWithEmptyResponse::WithParameterAndEmptyResponseCallback",
               "message", message.name());
#endif

#if defined(ENABLE_IPC_FUZZER)
  message.set_interface_name(CheckMethodWithEmptyResponse::Name_);
  message.set_method_name("WithParameterAndEmptyResponse");
#endif

  message.set_request_id(request_id_);
  ignore_result(responder_->Accept(&message));
  // TODO(darin): Accept() returning false indicates a malformed message, and
  // that may be good reason to close the connection. However, we don't have a
  // way to do that from here. We should add a way.
  responder_ = nullptr;
}

// static
bool CheckMethodWithEmptyResponseStubDispatch::Accept(
    CheckMethodWithEmptyResponse* impl,
    mojo::Message* message) {
  switch (message->header()->name) {
    case internal::kCheckMethodWithEmptyResponse_WithouParameterAndEmptyResponse_Name: {
      break;
    }
    case internal::kCheckMethodWithEmptyResponse_WithParameterAndEmptyResponse_Name: {
      break;
    }
  }
  return false;
}

// static
bool CheckMethodWithEmptyResponseStubDispatch::AcceptWithResponder(
    CheckMethodWithEmptyResponse* impl,
    mojo::Message* message,
    std::unique_ptr<mojo::MessageReceiverWithStatus> responder) {
  switch (message->header()->name) {
    case internal::kCheckMethodWithEmptyResponse_WithouParameterAndEmptyResponse_Name: {
#if BUILDFLAG(MOJO_TRACE_ENABLED)
      TRACE_EVENT1(
          "mojom",
          "(Impl)<class 'jinja2::utils::Namespace'>::CheckMethodWithEmptyResponse::WithouParameterAndEmptyResponse",
          "message", message->name());
#endif
      base::TaskAnnotator::ScopedSetIpcProgramCounter scoped_ipc_pc(
          base::GetProgramCounter());
      mojo::internal::MessageDispatchContext context(message);
      if (!message->is_serialized()) {
        auto context = message->TakeUnserializedContext<
            CheckMethodWithEmptyResponseProxy_WithouParameterAndEmptyResponse_Message>();
        if (!context) {
          // The Message was not of the expected type. It may be a valid message
          // which was built using a different variant of these bindings. Force
          // serialization before dispatch in this case.
          message->SerializeIfNecessary();
        } else {
          CheckMethodWithEmptyResponse::WithouParameterAndEmptyResponseCallback callback =
              CheckMethodWithEmptyResponse_WithouParameterAndEmptyResponse_ProxyToResponder::CreateCallback(
                  message->request_id(),
                  message->has_flag(mojo::Message::kFlagIsSync),
                  std::move(responder));
          context->Dispatch(impl, std::move(callback));
          return true;
        }
      }

      internal::CheckMethodWithEmptyResponse_WithouParameterAndEmptyResponse_Params_Data* params =
          reinterpret_cast<
              internal::CheckMethodWithEmptyResponse_WithouParameterAndEmptyResponse_Params_Data*>(
                  message->mutable_payload());
      
      mojo::internal::SerializationContext serialization_context;
      serialization_context.TakeHandlesFromMessage(message);
      bool success = true;
      CheckMethodWithEmptyResponse_WithouParameterAndEmptyResponse_ParamsDataView input_data_view(params, &serialization_context);
      
      if (!success) {
        ReportValidationErrorForMessage(
            message,
            mojo::internal::VALIDATION_ERROR_DESERIALIZATION_FAILED,
            CheckMethodWithEmptyResponse::Name_, 0, false);
        return false;
      }
      CheckMethodWithEmptyResponse::WithouParameterAndEmptyResponseCallback callback =
          CheckMethodWithEmptyResponse_WithouParameterAndEmptyResponse_ProxyToResponder::CreateCallback(
              message->request_id(),
              message->has_flag(mojo::Message::kFlagIsSync),
              std::move(responder));
      // A null |impl| means no implementation was bound.
      DCHECK(impl);
      impl->WithouParameterAndEmptyResponse(std::move(callback));
      return true;
    }
    case internal::kCheckMethodWithEmptyResponse_WithParameterAndEmptyResponse_Name: {
#if BUILDFLAG(MOJO_TRACE_ENABLED)
      TRACE_EVENT1(
          "mojom",
          "(Impl)<class 'jinja2::utils::Namespace'>::CheckMethodWithEmptyResponse::WithParameterAndEmptyResponse",
          "message", message->name());
#endif
      base::TaskAnnotator::ScopedSetIpcProgramCounter scoped_ipc_pc(
          base::GetProgramCounter());
      mojo::internal::MessageDispatchContext context(message);
      if (!message->is_serialized()) {
        auto context = message->TakeUnserializedContext<
            CheckMethodWithEmptyResponseProxy_WithParameterAndEmptyResponse_Message>();
        if (!context) {
          // The Message was not of the expected type. It may be a valid message
          // which was built using a different variant of these bindings. Force
          // serialization before dispatch in this case.
          message->SerializeIfNecessary();
        } else {
          CheckMethodWithEmptyResponse::WithParameterAndEmptyResponseCallback callback =
              CheckMethodWithEmptyResponse_WithParameterAndEmptyResponse_ProxyToResponder::CreateCallback(
                  message->request_id(),
                  message->has_flag(mojo::Message::kFlagIsSync),
                  std::move(responder));
          context->Dispatch(impl, std::move(callback));
          return true;
        }
      }

      internal::CheckMethodWithEmptyResponse_WithParameterAndEmptyResponse_Params_Data* params =
          reinterpret_cast<
              internal::CheckMethodWithEmptyResponse_WithParameterAndEmptyResponse_Params_Data*>(
                  message->mutable_payload());
      
      mojo::internal::SerializationContext serialization_context;
      serialization_context.TakeHandlesFromMessage(message);
      bool success = true;
      bool p_b{};
      CheckMethodWithEmptyResponse_WithParameterAndEmptyResponse_ParamsDataView input_data_view(params, &serialization_context);
      
      p_b = input_data_view.b();
      if (!success) {
        ReportValidationErrorForMessage(
            message,
            mojo::internal::VALIDATION_ERROR_DESERIALIZATION_FAILED,
            CheckMethodWithEmptyResponse::Name_, 1, false);
        return false;
      }
      CheckMethodWithEmptyResponse::WithParameterAndEmptyResponseCallback callback =
          CheckMethodWithEmptyResponse_WithParameterAndEmptyResponse_ProxyToResponder::CreateCallback(
              message->request_id(),
              message->has_flag(mojo::Message::kFlagIsSync),
              std::move(responder));
      // A null |impl| means no implementation was bound.
      DCHECK(impl);
      impl->WithParameterAndEmptyResponse(
std::move(p_b), std::move(callback));
      return true;
    }
  }
  return false;
}

bool CheckMethodWithEmptyResponseRequestValidator::Accept(mojo::Message* message) {
  if (!message->is_serialized() ||
      mojo::internal::ControlMessageHandler::IsControlMessage(message)) {
    return true;
  }

  mojo::internal::ValidationContext validation_context(
    message->payload(), message->payload_num_bytes(),
    message->handles()->size(), message->payload_num_interface_ids(), message,
    "CheckMethodWithEmptyResponse RequestValidator");

  switch (message->header()->name) {
    case internal::kCheckMethodWithEmptyResponse_WithouParameterAndEmptyResponse_Name: {
      if (!mojo::internal::ValidateMessageIsRequestExpectingResponse(
              message, &validation_context)) {
        return false;
      }
      if (!mojo::internal::ValidateMessagePayload<
               internal::CheckMethodWithEmptyResponse_WithouParameterAndEmptyResponse_Params_Data>(
                  message, &validation_context)) {
        return false;
      }
      return true;
    }
    case internal::kCheckMethodWithEmptyResponse_WithParameterAndEmptyResponse_Name: {
      if (!mojo::internal::ValidateMessageIsRequestExpectingResponse(
              message, &validation_context)) {
        return false;
      }
      if (!mojo::internal::ValidateMessagePayload<
               internal::CheckMethodWithEmptyResponse_WithParameterAndEmptyResponse_Params_Data>(
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

bool CheckMethodWithEmptyResponseResponseValidator::Accept(mojo::Message* message) {
  if (!message->is_serialized() ||
      mojo::internal::ControlMessageHandler::IsControlMessage(message)) {
    return true;
  }

  mojo::internal::ValidationContext validation_context(
    message->payload(), message->payload_num_bytes(),
    message->handles()->size(), message->payload_num_interface_ids(), message,
    "CheckMethodWithEmptyResponse ResponseValidator");

  if (!mojo::internal::ValidateMessageIsResponse(message, &validation_context))
    return false;
  switch (message->header()->name) {
    case internal::kCheckMethodWithEmptyResponse_WithouParameterAndEmptyResponse_Name: {
      if (!mojo::internal::ValidateMessagePayload<
               internal::CheckMethodWithEmptyResponse_WithouParameterAndEmptyResponse_ResponseParams_Data>(
                    message, &validation_context)) {
        return false;
      }
      return true;
    }
    case internal::kCheckMethodWithEmptyResponse_WithParameterAndEmptyResponse_Name: {
      if (!mojo::internal::ValidateMessagePayload<
               internal::CheckMethodWithEmptyResponse_WithParameterAndEmptyResponse_ResponseParams_Data>(
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
const char CheckNameCollision::Name_[] = "regression_tests.CheckNameCollision";

class CheckNameCollision_WithNameCollision_ForwardToCallback
    : public mojo::MessageReceiver {
 public:
  CheckNameCollision_WithNameCollision_ForwardToCallback(
      CheckNameCollision::WithNameCollisionCallback callback
      ) : callback_(std::move(callback)) {
  }
  bool Accept(mojo::Message* message) override;
 private:
  CheckNameCollision::WithNameCollisionCallback callback_;
  DISALLOW_COPY_AND_ASSIGN(CheckNameCollision_WithNameCollision_ForwardToCallback);
};

CheckNameCollisionProxy::CheckNameCollisionProxy(mojo::MessageReceiverWithResponder* receiver)
    : receiver_(receiver) {
}
class CheckNameCollisionProxy_WithNameCollision_Message
    : public mojo::internal::UnserializedMessageContext {
 public:
  static const mojo::internal::UnserializedMessageContext::Tag kMessageTag;

  explicit CheckNameCollisionProxy_WithNameCollision_Message(
      uint32_t message_flags
      , bool param_message
      , bool param_response
  )
      : mojo::internal::UnserializedMessageContext(
          &kMessageTag,
          internal::kCheckNameCollision_WithNameCollision_Name,
          message_flags)
      , param_message_(std::move(param_message))
      , param_response_(std::move(param_response)){}
  ~CheckNameCollisionProxy_WithNameCollision_Message() override = default;

  static mojo::Message Build(
      bool serialize,
      bool expects_response,
      bool is_sync,
      bool param_message, bool param_response) {

    
  const uint32_t kFlags =
      ((expects_response) ? mojo::Message::kFlagExpectsResponse : 0) |
      ((is_sync) ? mojo::Message::kFlagIsSync : 0);

    if (!serialize) {
      return mojo::Message(std::make_unique<CheckNameCollisionProxy_WithNameCollision_Message>(
          kFlags
          , std::move(param_message)
          , std::move(param_response)
          ));
    }

    DCHECK(serialize);
    
  mojo::Message message(
      internal::kCheckNameCollision_WithNameCollision_Name, kFlags, 0, 0, nullptr);
  auto* buffer = message.payload_buffer();
  ::regression_tests::internal::CheckNameCollision_WithNameCollision_Params_Data::BufferWriter
      params;
  mojo::internal::SerializationContext serialization_context;
  params.Allocate(buffer);
  params->message = param_message;
  params->response = param_response;
  message.AttachHandlesFromSerializationContext(
      &serialization_context);
    return message;
  }


  void Dispatch(CheckNameCollision* impl, CheckNameCollision::WithNameCollisionCallback callback) {
    impl->WithNameCollision(
        std::move(param_message_),
        std::move(param_response_), std::move(callback));
  }

 private:
  // mojo::internal::UnserializedMessageContext:
  void Serialize(mojo::internal::SerializationContext* serialization_context,
                 mojo::internal::Buffer* buffer) override {
    ::regression_tests::internal::CheckNameCollision_WithNameCollision_Params_Data::BufferWriter
        params;
    params.Allocate(buffer);
  params->message = param_message_;
  params->response = param_response_;
  }
  bool param_message_;
  bool param_response_;

  DISALLOW_COPY_AND_ASSIGN(CheckNameCollisionProxy_WithNameCollision_Message);
};

const mojo::internal::UnserializedMessageContext::Tag
CheckNameCollisionProxy_WithNameCollision_Message::kMessageTag = {};

void CheckNameCollisionProxy::WithNameCollision(
    bool in_message, bool in_response, WithNameCollisionCallback callback) {
#if BUILDFLAG(MOJO_TRACE_ENABLED)
  TRACE_EVENT0("mojom", "<class 'jinja2::utils::Namespace'>::CheckNameCollision::WithNameCollision");
#endif
  const bool kExpectsResponse = true;
  const bool kIsSync = false;
  const bool kSerialize = receiver_->PrefersSerializedMessages();
  auto message = CheckNameCollisionProxy_WithNameCollision_Message::Build(
      kSerialize, kExpectsResponse, kIsSync, std::move(in_message), std::move(in_response));

#if defined(ENABLE_IPC_FUZZER)
  message.set_interface_name(CheckNameCollision::Name_);
  message.set_method_name("WithNameCollision");
#endif
  std::unique_ptr<mojo::MessageReceiver> responder(
      new CheckNameCollision_WithNameCollision_ForwardToCallback(
          std::move(callback)));
  ignore_result(receiver_->AcceptWithResponder(&message, std::move(responder)));
}
class CheckNameCollision_WithNameCollision_ProxyToResponder {
 public:
  static CheckNameCollision::WithNameCollisionCallback CreateCallback(
      uint64_t request_id,
      bool is_sync,
      std::unique_ptr<mojo::MessageReceiverWithStatus> responder) {
    std::unique_ptr<CheckNameCollision_WithNameCollision_ProxyToResponder> proxy(
        new CheckNameCollision_WithNameCollision_ProxyToResponder(
            request_id, is_sync, std::move(responder)));
    return base::BindOnce(&CheckNameCollision_WithNameCollision_ProxyToResponder::Run,
                          std::move(proxy));
  }

  ~CheckNameCollision_WithNameCollision_ProxyToResponder() {
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
  CheckNameCollision_WithNameCollision_ProxyToResponder(
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
        << "CheckNameCollision::WithNameCollisionCallback was destroyed without "
        << "first either being run or its corresponding binding being closed. "
        << "It is an error to drop response callbacks which still correspond "
        << "to an open interface pipe.";
  }
#endif

  void Run(
      bool in_message, bool in_response);

  uint64_t request_id_;
  bool is_sync_;
  std::unique_ptr<mojo::MessageReceiverWithStatus> responder_;

  DISALLOW_COPY_AND_ASSIGN(CheckNameCollision_WithNameCollision_ProxyToResponder);
};
class CheckNameCollision_WithNameCollision_Response_Message
    : public mojo::internal::UnserializedMessageContext {
 public:
  static const mojo::internal::UnserializedMessageContext::Tag kMessageTag;

  explicit CheckNameCollision_WithNameCollision_Response_Message(
      uint32_t message_flags
      , bool param_message
      , bool param_response
  )
      : mojo::internal::UnserializedMessageContext(
          &kMessageTag,
          internal::kCheckNameCollision_WithNameCollision_Name,
          message_flags)
      , param_message_(std::move(param_message))
      , param_response_(std::move(param_response)){}
  ~CheckNameCollision_WithNameCollision_Response_Message() override = default;

  static mojo::Message Build(
      bool serialize,
      bool is_sync,
      bool param_message, bool param_response) {

    
  const uint32_t kFlags = mojo::Message::kFlagIsResponse |
      ((is_sync) ? mojo::Message::kFlagIsSync : 0);

    if (!serialize) {
      return mojo::Message(std::make_unique<CheckNameCollision_WithNameCollision_Response_Message>(
          kFlags
          , std::move(param_message)
          , std::move(param_response)
          ));
    }

    DCHECK(serialize);
    
  mojo::Message message(
      internal::kCheckNameCollision_WithNameCollision_Name, kFlags, 0, 0, nullptr);
  auto* buffer = message.payload_buffer();
  ::regression_tests::internal::CheckNameCollision_WithNameCollision_ResponseParams_Data::BufferWriter
      params;
  mojo::internal::SerializationContext serialization_context;
  params.Allocate(buffer);
  params->message = param_message;
  params->response = param_response;
  message.AttachHandlesFromSerializationContext(
      &serialization_context);
    return message;
  }


  void Dispatch(CheckNameCollision::WithNameCollisionCallback* callback) {
    std::move(*callback).Run(
        std::move(param_message_), 
        std::move(param_response_));
  }



 private:
  // mojo::internal::UnserializedMessageContext:
  void Serialize(mojo::internal::SerializationContext* serialization_context,
                 mojo::internal::Buffer* buffer) override {
    ::regression_tests::internal::CheckNameCollision_WithNameCollision_ResponseParams_Data::BufferWriter
        params;
    params.Allocate(buffer);
  params->message = param_message_;
  params->response = param_response_;
  }
  bool param_message_;
  bool param_response_;

  DISALLOW_COPY_AND_ASSIGN(CheckNameCollision_WithNameCollision_Response_Message);
};

const mojo::internal::UnserializedMessageContext::Tag
CheckNameCollision_WithNameCollision_Response_Message::kMessageTag = {};

bool CheckNameCollision_WithNameCollision_ForwardToCallback::Accept(
    mojo::Message* message) {
#if BUILDFLAG(MOJO_TRACE_ENABLED)
  TRACE_EVENT1("mojom", "<class 'jinja2::utils::Namespace'>::CheckNameCollision::WithNameCollisionCallback",
               "message", message->name());
#endif
  mojo::internal::MessageDispatchContext dispatch_context(message);
  if (!message->is_serialized()) {
    auto context =
        message->TakeUnserializedContext<CheckNameCollision_WithNameCollision_Response_Message>();
    if (!context) {
      // The Message was not of the expected type. It may be a valid message
      // which was build using a different variant of these bindings. Force
      // serialization before dispatch in this case.
      message->SerializeIfNecessary();
    } else {
      if (!callback_.is_null())
        context->Dispatch(&callback_);
      return true;
    }
  }

  DCHECK(message->is_serialized());
  internal::CheckNameCollision_WithNameCollision_ResponseParams_Data* params =
      reinterpret_cast<
          internal::CheckNameCollision_WithNameCollision_ResponseParams_Data*>(
              message->mutable_payload());
  
  mojo::internal::SerializationContext serialization_context;
  serialization_context.TakeHandlesFromMessage(message);
  bool success = true;
  bool p_message{};
  bool p_response{};
  CheckNameCollision_WithNameCollision_ResponseParamsDataView input_data_view(params, &serialization_context);
  
  p_message = input_data_view.message();
  p_response = input_data_view.response();
  if (!success) {
    ReportValidationErrorForMessage(
        message,
        mojo::internal::VALIDATION_ERROR_DESERIALIZATION_FAILED,
        CheckNameCollision::Name_, 0, true);
    return false;
  }
  if (!callback_.is_null())
    std::move(callback_).Run(
std::move(p_message), 
std::move(p_response));
  return true;
}

void CheckNameCollision_WithNameCollision_ProxyToResponder::Run(
    bool in_message, bool in_response) {
  const bool kSerialize = responder_->PrefersSerializedMessages();
  auto message = CheckNameCollision_WithNameCollision_Response_Message::Build(kSerialize, is_sync_, std::move(in_message), std::move(in_response));

#if BUILDFLAG(MOJO_TRACE_ENABLED)
  TRACE_EVENT1("mojom", "(Impl)<class 'jinja2::utils::Namespace'>::CheckNameCollision::WithNameCollisionCallback",
               "message", message.name());
#endif

#if defined(ENABLE_IPC_FUZZER)
  message.set_interface_name(CheckNameCollision::Name_);
  message.set_method_name("WithNameCollision");
#endif

  message.set_request_id(request_id_);
  ignore_result(responder_->Accept(&message));
  // TODO(darin): Accept() returning false indicates a malformed message, and
  // that may be good reason to close the connection. However, we don't have a
  // way to do that from here. We should add a way.
  responder_ = nullptr;
}

// static
bool CheckNameCollisionStubDispatch::Accept(
    CheckNameCollision* impl,
    mojo::Message* message) {
  switch (message->header()->name) {
    case internal::kCheckNameCollision_WithNameCollision_Name: {
      break;
    }
  }
  return false;
}

// static
bool CheckNameCollisionStubDispatch::AcceptWithResponder(
    CheckNameCollision* impl,
    mojo::Message* message,
    std::unique_ptr<mojo::MessageReceiverWithStatus> responder) {
  switch (message->header()->name) {
    case internal::kCheckNameCollision_WithNameCollision_Name: {
#if BUILDFLAG(MOJO_TRACE_ENABLED)
      TRACE_EVENT1(
          "mojom",
          "(Impl)<class 'jinja2::utils::Namespace'>::CheckNameCollision::WithNameCollision",
          "message", message->name());
#endif
      base::TaskAnnotator::ScopedSetIpcProgramCounter scoped_ipc_pc(
          base::GetProgramCounter());
      mojo::internal::MessageDispatchContext context(message);
      if (!message->is_serialized()) {
        auto context = message->TakeUnserializedContext<
            CheckNameCollisionProxy_WithNameCollision_Message>();
        if (!context) {
          // The Message was not of the expected type. It may be a valid message
          // which was built using a different variant of these bindings. Force
          // serialization before dispatch in this case.
          message->SerializeIfNecessary();
        } else {
          CheckNameCollision::WithNameCollisionCallback callback =
              CheckNameCollision_WithNameCollision_ProxyToResponder::CreateCallback(
                  message->request_id(),
                  message->has_flag(mojo::Message::kFlagIsSync),
                  std::move(responder));
          context->Dispatch(impl, std::move(callback));
          return true;
        }
      }

      internal::CheckNameCollision_WithNameCollision_Params_Data* params =
          reinterpret_cast<
              internal::CheckNameCollision_WithNameCollision_Params_Data*>(
                  message->mutable_payload());
      
      mojo::internal::SerializationContext serialization_context;
      serialization_context.TakeHandlesFromMessage(message);
      bool success = true;
      bool p_message{};
      bool p_response{};
      CheckNameCollision_WithNameCollision_ParamsDataView input_data_view(params, &serialization_context);
      
      p_message = input_data_view.message();
      p_response = input_data_view.response();
      if (!success) {
        ReportValidationErrorForMessage(
            message,
            mojo::internal::VALIDATION_ERROR_DESERIALIZATION_FAILED,
            CheckNameCollision::Name_, 0, false);
        return false;
      }
      CheckNameCollision::WithNameCollisionCallback callback =
          CheckNameCollision_WithNameCollision_ProxyToResponder::CreateCallback(
              message->request_id(),
              message->has_flag(mojo::Message::kFlagIsSync),
              std::move(responder));
      // A null |impl| means no implementation was bound.
      DCHECK(impl);
      impl->WithNameCollision(
std::move(p_message), 
std::move(p_response), std::move(callback));
      return true;
    }
  }
  return false;
}

bool CheckNameCollisionRequestValidator::Accept(mojo::Message* message) {
  if (!message->is_serialized() ||
      mojo::internal::ControlMessageHandler::IsControlMessage(message)) {
    return true;
  }

  mojo::internal::ValidationContext validation_context(
    message->payload(), message->payload_num_bytes(),
    message->handles()->size(), message->payload_num_interface_ids(), message,
    "CheckNameCollision RequestValidator");

  switch (message->header()->name) {
    case internal::kCheckNameCollision_WithNameCollision_Name: {
      if (!mojo::internal::ValidateMessageIsRequestExpectingResponse(
              message, &validation_context)) {
        return false;
      }
      if (!mojo::internal::ValidateMessagePayload<
               internal::CheckNameCollision_WithNameCollision_Params_Data>(
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

bool CheckNameCollisionResponseValidator::Accept(mojo::Message* message) {
  if (!message->is_serialized() ||
      mojo::internal::ControlMessageHandler::IsControlMessage(message)) {
    return true;
  }

  mojo::internal::ValidationContext validation_context(
    message->payload(), message->payload_num_bytes(),
    message->handles()->size(), message->payload_num_interface_ids(), message,
    "CheckNameCollision ResponseValidator");

  if (!mojo::internal::ValidateMessageIsResponse(message, &validation_context))
    return false;
  switch (message->header()->name) {
    case internal::kCheckNameCollision_WithNameCollision_Name: {
      if (!mojo::internal::ValidateMessagePayload<
               internal::CheckNameCollision_WithNameCollision_ResponseParams_Data>(
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
const char HandlesNameCollisionInterface::Name_[] = "regression_tests.HandlesNameCollisionInterface";

class HandlesNameCollisionInterface_Method_ForwardToCallback
    : public mojo::MessageReceiver {
 public:
  HandlesNameCollisionInterface_Method_ForwardToCallback(
      HandlesNameCollisionInterface::MethodCallback callback
      ) : callback_(std::move(callback)) {
  }
  bool Accept(mojo::Message* message) override;
 private:
  HandlesNameCollisionInterface::MethodCallback callback_;
  DISALLOW_COPY_AND_ASSIGN(HandlesNameCollisionInterface_Method_ForwardToCallback);
};

HandlesNameCollisionInterfaceProxy::HandlesNameCollisionInterfaceProxy(mojo::MessageReceiverWithResponder* receiver)
    : receiver_(receiver) {
}
class HandlesNameCollisionInterfaceProxy_Method_Message
    : public mojo::internal::UnserializedMessageContext {
 public:
  static const mojo::internal::UnserializedMessageContext::Tag kMessageTag;

  explicit HandlesNameCollisionInterfaceProxy_Method_Message(
      uint32_t message_flags
      , EmptyStructPtr param_handles
  )
      : mojo::internal::UnserializedMessageContext(
          &kMessageTag,
          internal::kHandlesNameCollisionInterface_Method_Name,
          message_flags)
      , param_handles_(std::move(param_handles)){}
  ~HandlesNameCollisionInterfaceProxy_Method_Message() override = default;

  static mojo::Message Build(
      bool serialize,
      bool expects_response,
      bool is_sync,
      EmptyStructPtr param_handles) {

    
  const uint32_t kFlags =
      ((expects_response) ? mojo::Message::kFlagExpectsResponse : 0) |
      ((is_sync) ? mojo::Message::kFlagIsSync : 0);

    if (!serialize) {
      return mojo::Message(std::make_unique<HandlesNameCollisionInterfaceProxy_Method_Message>(
          kFlags
          , std::move(param_handles)
          ));
    }

    DCHECK(serialize);
    
  mojo::Message message(
      internal::kHandlesNameCollisionInterface_Method_Name, kFlags, 0, 0, nullptr);
  auto* buffer = message.payload_buffer();
  ::regression_tests::internal::HandlesNameCollisionInterface_Method_Params_Data::BufferWriter
      params;
  mojo::internal::SerializationContext serialization_context;
  params.Allocate(buffer);
  typename decltype(params->handles)::BaseType::BufferWriter
      handles_writer;
  mojo::internal::Serialize<::regression_tests::EmptyStructDataView>(
      param_handles, buffer, &handles_writer, &serialization_context);
  params->handles.Set(
      handles_writer.is_null() ? nullptr : handles_writer.data());
  MOJO_INTERNAL_DLOG_SERIALIZATION_WARNING(
      params->handles.is_null(),
      mojo::internal::VALIDATION_ERROR_UNEXPECTED_NULL_POINTER,
      "null handles in HandlesNameCollisionInterface.Method request");
  message.AttachHandlesFromSerializationContext(
      &serialization_context);
    return message;
  }


  void Dispatch(HandlesNameCollisionInterface* impl, HandlesNameCollisionInterface::MethodCallback callback) {
    impl->Method(
        std::move(param_handles_), std::move(callback));
  }

 private:
  // mojo::internal::UnserializedMessageContext:
  void Serialize(mojo::internal::SerializationContext* serialization_context,
                 mojo::internal::Buffer* buffer) override {
    ::regression_tests::internal::HandlesNameCollisionInterface_Method_Params_Data::BufferWriter
        params;
    params.Allocate(buffer);
  typename decltype(params->handles)::BaseType::BufferWriter
      handles_writer;
  mojo::internal::Serialize<::regression_tests::EmptyStructDataView>(
      param_handles_, buffer, &handles_writer, serialization_context);
  params->handles.Set(
      handles_writer.is_null() ? nullptr : handles_writer.data());
  MOJO_INTERNAL_DLOG_SERIALIZATION_WARNING(
      params->handles.is_null(),
      mojo::internal::VALIDATION_ERROR_UNEXPECTED_NULL_POINTER,
      "null handles in HandlesNameCollisionInterface.Method request");
  }
  EmptyStructPtr param_handles_;

  DISALLOW_COPY_AND_ASSIGN(HandlesNameCollisionInterfaceProxy_Method_Message);
};

const mojo::internal::UnserializedMessageContext::Tag
HandlesNameCollisionInterfaceProxy_Method_Message::kMessageTag = {};

void HandlesNameCollisionInterfaceProxy::Method(
    EmptyStructPtr in_handles, MethodCallback callback) {
#if BUILDFLAG(MOJO_TRACE_ENABLED)
  TRACE_EVENT0("mojom", "<class 'jinja2::utils::Namespace'>::HandlesNameCollisionInterface::Method");
#endif
  const bool kExpectsResponse = true;
  const bool kIsSync = false;
  const bool kSerialize = receiver_->PrefersSerializedMessages();
  auto message = HandlesNameCollisionInterfaceProxy_Method_Message::Build(
      kSerialize, kExpectsResponse, kIsSync, std::move(in_handles));

#if defined(ENABLE_IPC_FUZZER)
  message.set_interface_name(HandlesNameCollisionInterface::Name_);
  message.set_method_name("Method");
#endif
  std::unique_ptr<mojo::MessageReceiver> responder(
      new HandlesNameCollisionInterface_Method_ForwardToCallback(
          std::move(callback)));
  ignore_result(receiver_->AcceptWithResponder(&message, std::move(responder)));
}
class HandlesNameCollisionInterface_Method_ProxyToResponder {
 public:
  static HandlesNameCollisionInterface::MethodCallback CreateCallback(
      uint64_t request_id,
      bool is_sync,
      std::unique_ptr<mojo::MessageReceiverWithStatus> responder) {
    std::unique_ptr<HandlesNameCollisionInterface_Method_ProxyToResponder> proxy(
        new HandlesNameCollisionInterface_Method_ProxyToResponder(
            request_id, is_sync, std::move(responder)));
    return base::BindOnce(&HandlesNameCollisionInterface_Method_ProxyToResponder::Run,
                          std::move(proxy));
  }

  ~HandlesNameCollisionInterface_Method_ProxyToResponder() {
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
  HandlesNameCollisionInterface_Method_ProxyToResponder(
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
        << "HandlesNameCollisionInterface::MethodCallback was destroyed without "
        << "first either being run or its corresponding binding being closed. "
        << "It is an error to drop response callbacks which still correspond "
        << "to an open interface pipe.";
  }
#endif

  void Run(
      mojo::ScopedHandle in_handles);

  uint64_t request_id_;
  bool is_sync_;
  std::unique_ptr<mojo::MessageReceiverWithStatus> responder_;

  DISALLOW_COPY_AND_ASSIGN(HandlesNameCollisionInterface_Method_ProxyToResponder);
};
class HandlesNameCollisionInterface_Method_Response_Message
    : public mojo::internal::UnserializedMessageContext {
 public:
  static const mojo::internal::UnserializedMessageContext::Tag kMessageTag;

  explicit HandlesNameCollisionInterface_Method_Response_Message(
      uint32_t message_flags
      , mojo::ScopedHandle param_handles
  )
      : mojo::internal::UnserializedMessageContext(
          &kMessageTag,
          internal::kHandlesNameCollisionInterface_Method_Name,
          message_flags)
      , param_handles_(std::move(param_handles)){}
  ~HandlesNameCollisionInterface_Method_Response_Message() override = default;

  static mojo::Message Build(
      bool serialize,
      bool is_sync,
      mojo::ScopedHandle param_handles) {

    
  const uint32_t kFlags = mojo::Message::kFlagIsResponse |
      ((is_sync) ? mojo::Message::kFlagIsSync : 0);

    if (!serialize) {
      return mojo::Message(std::make_unique<HandlesNameCollisionInterface_Method_Response_Message>(
          kFlags
          , std::move(param_handles)
          ));
    }

    DCHECK(serialize);
    
  mojo::Message message(
      internal::kHandlesNameCollisionInterface_Method_Name, kFlags, 0, 0, nullptr);
  auto* buffer = message.payload_buffer();
  ::regression_tests::internal::HandlesNameCollisionInterface_Method_ResponseParams_Data::BufferWriter
      params;
  mojo::internal::SerializationContext serialization_context;
  params.Allocate(buffer);
  mojo::internal::Serialize<mojo::ScopedHandle>(
      param_handles, &params->handles, &serialization_context);
  MOJO_INTERNAL_DLOG_SERIALIZATION_WARNING(
      !mojo::internal::IsHandleOrInterfaceValid(params->handles),
      mojo::internal::VALIDATION_ERROR_UNEXPECTED_INVALID_HANDLE,
      "invalid handles in HandlesNameCollisionInterface.Method response");
  message.AttachHandlesFromSerializationContext(
      &serialization_context);
    return message;
  }


  void Dispatch(HandlesNameCollisionInterface::MethodCallback* callback) {
    std::move(*callback).Run(
        std::move(param_handles_));
  }



 private:
  // mojo::internal::UnserializedMessageContext:
  void Serialize(mojo::internal::SerializationContext* serialization_context,
                 mojo::internal::Buffer* buffer) override {
    ::regression_tests::internal::HandlesNameCollisionInterface_Method_ResponseParams_Data::BufferWriter
        params;
    params.Allocate(buffer);
  mojo::internal::Serialize<mojo::ScopedHandle>(
      param_handles_, &params->handles, serialization_context);
  MOJO_INTERNAL_DLOG_SERIALIZATION_WARNING(
      !mojo::internal::IsHandleOrInterfaceValid(params->handles),
      mojo::internal::VALIDATION_ERROR_UNEXPECTED_INVALID_HANDLE,
      "invalid handles in HandlesNameCollisionInterface.Method response");
  }
  mojo::ScopedHandle param_handles_;

  DISALLOW_COPY_AND_ASSIGN(HandlesNameCollisionInterface_Method_Response_Message);
};

const mojo::internal::UnserializedMessageContext::Tag
HandlesNameCollisionInterface_Method_Response_Message::kMessageTag = {};

bool HandlesNameCollisionInterface_Method_ForwardToCallback::Accept(
    mojo::Message* message) {
#if BUILDFLAG(MOJO_TRACE_ENABLED)
  TRACE_EVENT1("mojom", "<class 'jinja2::utils::Namespace'>::HandlesNameCollisionInterface::MethodCallback",
               "message", message->name());
#endif
  mojo::internal::MessageDispatchContext dispatch_context(message);
  if (!message->is_serialized()) {
    auto context =
        message->TakeUnserializedContext<HandlesNameCollisionInterface_Method_Response_Message>();
    if (!context) {
      // The Message was not of the expected type. It may be a valid message
      // which was build using a different variant of these bindings. Force
      // serialization before dispatch in this case.
      message->SerializeIfNecessary();
    } else {
      if (!callback_.is_null())
        context->Dispatch(&callback_);
      return true;
    }
  }

  DCHECK(message->is_serialized());
  internal::HandlesNameCollisionInterface_Method_ResponseParams_Data* params =
      reinterpret_cast<
          internal::HandlesNameCollisionInterface_Method_ResponseParams_Data*>(
              message->mutable_payload());
  
  mojo::internal::SerializationContext serialization_context;
  serialization_context.TakeHandlesFromMessage(message);
  bool success = true;
  mojo::ScopedHandle p_handles{};
  HandlesNameCollisionInterface_Method_ResponseParamsDataView input_data_view(params, &serialization_context);
  
  p_handles = input_data_view.TakeHandles();
  if (!success) {
    ReportValidationErrorForMessage(
        message,
        mojo::internal::VALIDATION_ERROR_DESERIALIZATION_FAILED,
        HandlesNameCollisionInterface::Name_, 0, true);
    return false;
  }
  if (!callback_.is_null())
    std::move(callback_).Run(
std::move(p_handles));
  return true;
}

void HandlesNameCollisionInterface_Method_ProxyToResponder::Run(
    mojo::ScopedHandle in_handles) {
  const bool kSerialize = responder_->PrefersSerializedMessages();
  auto message = HandlesNameCollisionInterface_Method_Response_Message::Build(kSerialize, is_sync_, std::move(in_handles));

#if BUILDFLAG(MOJO_TRACE_ENABLED)
  TRACE_EVENT1("mojom", "(Impl)<class 'jinja2::utils::Namespace'>::HandlesNameCollisionInterface::MethodCallback",
               "message", message.name());
#endif

#if defined(ENABLE_IPC_FUZZER)
  message.set_interface_name(HandlesNameCollisionInterface::Name_);
  message.set_method_name("Method");
#endif

  message.set_request_id(request_id_);
  ignore_result(responder_->Accept(&message));
  // TODO(darin): Accept() returning false indicates a malformed message, and
  // that may be good reason to close the connection. However, we don't have a
  // way to do that from here. We should add a way.
  responder_ = nullptr;
}

// static
bool HandlesNameCollisionInterfaceStubDispatch::Accept(
    HandlesNameCollisionInterface* impl,
    mojo::Message* message) {
  switch (message->header()->name) {
    case internal::kHandlesNameCollisionInterface_Method_Name: {
      break;
    }
  }
  return false;
}

// static
bool HandlesNameCollisionInterfaceStubDispatch::AcceptWithResponder(
    HandlesNameCollisionInterface* impl,
    mojo::Message* message,
    std::unique_ptr<mojo::MessageReceiverWithStatus> responder) {
  switch (message->header()->name) {
    case internal::kHandlesNameCollisionInterface_Method_Name: {
#if BUILDFLAG(MOJO_TRACE_ENABLED)
      TRACE_EVENT1(
          "mojom",
          "(Impl)<class 'jinja2::utils::Namespace'>::HandlesNameCollisionInterface::Method",
          "message", message->name());
#endif
      base::TaskAnnotator::ScopedSetIpcProgramCounter scoped_ipc_pc(
          base::GetProgramCounter());
      mojo::internal::MessageDispatchContext context(message);
      if (!message->is_serialized()) {
        auto context = message->TakeUnserializedContext<
            HandlesNameCollisionInterfaceProxy_Method_Message>();
        if (!context) {
          // The Message was not of the expected type. It may be a valid message
          // which was built using a different variant of these bindings. Force
          // serialization before dispatch in this case.
          message->SerializeIfNecessary();
        } else {
          HandlesNameCollisionInterface::MethodCallback callback =
              HandlesNameCollisionInterface_Method_ProxyToResponder::CreateCallback(
                  message->request_id(),
                  message->has_flag(mojo::Message::kFlagIsSync),
                  std::move(responder));
          context->Dispatch(impl, std::move(callback));
          return true;
        }
      }

      internal::HandlesNameCollisionInterface_Method_Params_Data* params =
          reinterpret_cast<
              internal::HandlesNameCollisionInterface_Method_Params_Data*>(
                  message->mutable_payload());
      
      mojo::internal::SerializationContext serialization_context;
      serialization_context.TakeHandlesFromMessage(message);
      bool success = true;
      EmptyStructPtr p_handles{};
      HandlesNameCollisionInterface_Method_ParamsDataView input_data_view(params, &serialization_context);
      
      if (!input_data_view.ReadHandles(&p_handles))
        success = false;
      if (!success) {
        ReportValidationErrorForMessage(
            message,
            mojo::internal::VALIDATION_ERROR_DESERIALIZATION_FAILED,
            HandlesNameCollisionInterface::Name_, 0, false);
        return false;
      }
      HandlesNameCollisionInterface::MethodCallback callback =
          HandlesNameCollisionInterface_Method_ProxyToResponder::CreateCallback(
              message->request_id(),
              message->has_flag(mojo::Message::kFlagIsSync),
              std::move(responder));
      // A null |impl| means no implementation was bound.
      DCHECK(impl);
      impl->Method(
std::move(p_handles), std::move(callback));
      return true;
    }
  }
  return false;
}

bool HandlesNameCollisionInterfaceRequestValidator::Accept(mojo::Message* message) {
  if (!message->is_serialized() ||
      mojo::internal::ControlMessageHandler::IsControlMessage(message)) {
    return true;
  }

  mojo::internal::ValidationContext validation_context(
    message->payload(), message->payload_num_bytes(),
    message->handles()->size(), message->payload_num_interface_ids(), message,
    "HandlesNameCollisionInterface RequestValidator");

  switch (message->header()->name) {
    case internal::kHandlesNameCollisionInterface_Method_Name: {
      if (!mojo::internal::ValidateMessageIsRequestExpectingResponse(
              message, &validation_context)) {
        return false;
      }
      if (!mojo::internal::ValidateMessagePayload<
               internal::HandlesNameCollisionInterface_Method_Params_Data>(
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

bool HandlesNameCollisionInterfaceResponseValidator::Accept(mojo::Message* message) {
  if (!message->is_serialized() ||
      mojo::internal::ControlMessageHandler::IsControlMessage(message)) {
    return true;
  }

  mojo::internal::ValidationContext validation_context(
    message->payload(), message->payload_num_bytes(),
    message->handles()->size(), message->payload_num_interface_ids(), message,
    "HandlesNameCollisionInterface ResponseValidator");

  if (!mojo::internal::ValidateMessageIsResponse(message, &validation_context))
    return false;
  switch (message->header()->name) {
    case internal::kHandlesNameCollisionInterface_Method_Name: {
      if (!mojo::internal::ValidateMessagePayload<
               internal::HandlesNameCollisionInterface_Method_ResponseParams_Data>(
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
}  // namespace regression_tests

namespace mojo {


// static
bool StructTraits<::regression_tests::Edge::DataView, ::regression_tests::EdgePtr>::Read(
    ::regression_tests::Edge::DataView input,
    ::regression_tests::EdgePtr* output) {
  bool success = true;
  ::regression_tests::EdgePtr result(::regression_tests::Edge::New());
  
      if (!input.ReadV(&result->v))
        success = false;
  *output = std::move(result);
  return success;
}


// static
bool StructTraits<::regression_tests::Vertex::DataView, ::regression_tests::VertexPtr>::Read(
    ::regression_tests::Vertex::DataView input,
    ::regression_tests::VertexPtr* output) {
  bool success = true;
  ::regression_tests::VertexPtr result(::regression_tests::Vertex::New());
  
      if (!input.ReadE(&result->e))
        success = false;
  *output = std::move(result);
  return success;
}


// static
bool StructTraits<::regression_tests::EmptyStruct::DataView, ::regression_tests::EmptyStructPtr>::Read(
    ::regression_tests::EmptyStruct::DataView input,
    ::regression_tests::EmptyStructPtr* output) {
  bool success = true;
  ::regression_tests::EmptyStructPtr result(::regression_tests::EmptyStruct::New());
  
  *output = std::move(result);
  return success;
}


// static
bool StructTraits<::regression_tests::A::DataView, ::regression_tests::APtr>::Read(
    ::regression_tests::A::DataView input,
    ::regression_tests::APtr* output) {
  bool success = true;
  ::regression_tests::APtr result(::regression_tests::A::New());
  
      if (!input.ReadB(&result->b))
        success = false;
  *output = std::move(result);
  return success;
}


// static
bool StructTraits<::regression_tests::B::DataView, ::regression_tests::BPtr>::Read(
    ::regression_tests::B::DataView input,
    ::regression_tests::BPtr* output) {
  bool success = true;
  ::regression_tests::BPtr result(::regression_tests::B::New());
  
      if (!input.ReadA(&result->a))
        success = false;
  *output = std::move(result);
  return success;
}


// static
bool StructTraits<::regression_tests::HandlesNameCollisionStruct::DataView, ::regression_tests::HandlesNameCollisionStructPtr>::Read(
    ::regression_tests::HandlesNameCollisionStruct::DataView input,
    ::regression_tests::HandlesNameCollisionStructPtr* output) {
  bool success = true;
  ::regression_tests::HandlesNameCollisionStructPtr result(::regression_tests::HandlesNameCollisionStruct::New());
  
      if (!input.ReadHandles(&result->handles))
        success = false;
  *output = std::move(result);
  return success;
}


// static
bool StructTraits<::regression_tests::HandlesHandleNameCollisionStruct::DataView, ::regression_tests::HandlesHandleNameCollisionStructPtr>::Read(
    ::regression_tests::HandlesHandleNameCollisionStruct::DataView input,
    ::regression_tests::HandlesHandleNameCollisionStructPtr* output) {
  bool success = true;
  ::regression_tests::HandlesHandleNameCollisionStructPtr result(::regression_tests::HandlesHandleNameCollisionStruct::New());
  
      result->handles = input.TakeHandles();
  *output = std::move(result);
  return success;
}


// static
bool StructTraits<::regression_tests::HandlesUnionNameCollisionStruct::DataView, ::regression_tests::HandlesUnionNameCollisionStructPtr>::Read(
    ::regression_tests::HandlesUnionNameCollisionStruct::DataView input,
    ::regression_tests::HandlesUnionNameCollisionStructPtr* output) {
  bool success = true;
  ::regression_tests::HandlesUnionNameCollisionStructPtr result(::regression_tests::HandlesUnionNameCollisionStruct::New());
  
      if (!input.ReadHandles(&result->handles))
        success = false;
  *output = std::move(result);
  return success;
}

// static
bool UnionTraits<::regression_tests::HandlesNameCollisionUnion::DataView, ::regression_tests::HandlesNameCollisionUnionPtr>::Read(
    ::regression_tests::HandlesNameCollisionUnion::DataView input,
    ::regression_tests::HandlesNameCollisionUnionPtr* output) {
  using UnionType = ::regression_tests::HandlesNameCollisionUnion;
  using Tag = UnionType::Tag;

  switch (input.tag()) {
    case Tag::HANDLES: {
      *output = UnionType::NewHandles(input.handles());
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