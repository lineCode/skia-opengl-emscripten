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

#include "mojo/public/interfaces/bindings/tests/sample_factory.mojom.h"

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

#include "mojo/public/interfaces/bindings/tests/sample_factory.mojom-params-data.h"
#include "mojo/public/interfaces/bindings/tests/sample_factory.mojom-shared-message-ids.h"

#include "mojo/public/interfaces/bindings/tests/sample_factory.mojom-import-headers.h"


#ifndef MOJO_PUBLIC_INTERFACES_BINDINGS_TESTS_SAMPLE_FACTORY_MOJOM_JUMBO_H_
#define MOJO_PUBLIC_INTERFACES_BINDINGS_TESTS_SAMPLE_FACTORY_MOJOM_JUMBO_H_
#endif
namespace sample {
Request::Request()
    : x(),
      pipe(),
      more_pipes(),
      obj() {}

Request::Request(
    int32_t x_in,
    mojo::ScopedMessagePipeHandle pipe_in,
    base::Optional<std::vector<mojo::ScopedMessagePipeHandle>> more_pipes_in,
    mojo::PendingRemote<::imported::ImportedInterface> obj_in)
    : x(std::move(x_in)),
      pipe(std::move(pipe_in)),
      more_pipes(std::move(more_pipes_in)),
      obj(std::move(obj_in)) {}

Request::~Request() = default;

bool Request::Validate(
    const void* data,
    mojo::internal::ValidationContext* validation_context) {
  return Data_::Validate(data, validation_context);
}
Response::Response()
    : x(),
      pipe() {}

Response::Response(
    int32_t x_in,
    mojo::ScopedMessagePipeHandle pipe_in)
    : x(std::move(x_in)),
      pipe(std::move(pipe_in)) {}

Response::~Response() = default;

bool Response::Validate(
    const void* data,
    mojo::internal::ValidationContext* validation_context) {
  return Data_::Validate(data, validation_context);
}
const char NamedObject::Name_[] = "sample.NamedObject";

class NamedObject_GetName_ForwardToCallback
    : public mojo::MessageReceiver {
 public:
  NamedObject_GetName_ForwardToCallback(
      NamedObject::GetNameCallback callback
      ) : callback_(std::move(callback)) {
  }
  bool Accept(mojo::Message* message) override;
 private:
  NamedObject::GetNameCallback callback_;
  DISALLOW_COPY_AND_ASSIGN(NamedObject_GetName_ForwardToCallback);
};

NamedObjectProxy::NamedObjectProxy(mojo::MessageReceiverWithResponder* receiver)
    : receiver_(receiver) {
}
class NamedObjectProxy_SetName_Message
    : public mojo::internal::UnserializedMessageContext {
 public:
  static const mojo::internal::UnserializedMessageContext::Tag kMessageTag;

  explicit NamedObjectProxy_SetName_Message(
      uint32_t message_flags
      , const std::string& param_name
  )
      : mojo::internal::UnserializedMessageContext(
          &kMessageTag,
          internal::kNamedObject_SetName_Name,
          message_flags)
      , param_name_(std::move(param_name)){}
  ~NamedObjectProxy_SetName_Message() override = default;

  static mojo::Message Build(
      bool serialize,
      bool expects_response,
      bool is_sync,
      const std::string& param_name) {

    
  const uint32_t kFlags =
      ((expects_response) ? mojo::Message::kFlagExpectsResponse : 0) |
      ((is_sync) ? mojo::Message::kFlagIsSync : 0);

    if (!serialize) {
      return mojo::Message(std::make_unique<NamedObjectProxy_SetName_Message>(
          kFlags
          , std::move(param_name)
          ));
    }

    DCHECK(serialize);
    
  mojo::Message message(
      internal::kNamedObject_SetName_Name, kFlags, 0, 0, nullptr);
  auto* buffer = message.payload_buffer();
  ::sample::internal::NamedObject_SetName_Params_Data::BufferWriter
      params;
  mojo::internal::SerializationContext serialization_context;
  params.Allocate(buffer);
  typename decltype(params->name)::BaseType::BufferWriter
      name_writer;
  mojo::internal::Serialize<mojo::StringDataView>(
      param_name, buffer, &name_writer, &serialization_context);
  params->name.Set(
      name_writer.is_null() ? nullptr : name_writer.data());
  MOJO_INTERNAL_DLOG_SERIALIZATION_WARNING(
      params->name.is_null(),
      mojo::internal::VALIDATION_ERROR_UNEXPECTED_NULL_POINTER,
      "null name in NamedObject.SetName request");
  message.AttachHandlesFromSerializationContext(
      &serialization_context);
    return message;
  }


  void Dispatch(NamedObject* impl) {
    impl->SetName(
        std::move(param_name_));
  }

 private:
  // mojo::internal::UnserializedMessageContext:
  void Serialize(mojo::internal::SerializationContext* serialization_context,
                 mojo::internal::Buffer* buffer) override {
    ::sample::internal::NamedObject_SetName_Params_Data::BufferWriter
        params;
    params.Allocate(buffer);
  typename decltype(params->name)::BaseType::BufferWriter
      name_writer;
  mojo::internal::Serialize<mojo::StringDataView>(
      param_name_, buffer, &name_writer, serialization_context);
  params->name.Set(
      name_writer.is_null() ? nullptr : name_writer.data());
  MOJO_INTERNAL_DLOG_SERIALIZATION_WARNING(
      params->name.is_null(),
      mojo::internal::VALIDATION_ERROR_UNEXPECTED_NULL_POINTER,
      "null name in NamedObject.SetName request");
  }
  std::string param_name_;

  DISALLOW_COPY_AND_ASSIGN(NamedObjectProxy_SetName_Message);
};

const mojo::internal::UnserializedMessageContext::Tag
NamedObjectProxy_SetName_Message::kMessageTag = {};

void NamedObjectProxy::SetName(
    const std::string& in_name) {
#if BUILDFLAG(MOJO_TRACE_ENABLED)
  TRACE_EVENT0("mojom", "<class 'jinja2::utils::Namespace'>::NamedObject::SetName");
#endif
  const bool kExpectsResponse = false;
  const bool kIsSync = false;
  const bool kSerialize = receiver_->PrefersSerializedMessages();
  auto message = NamedObjectProxy_SetName_Message::Build(
      kSerialize, kExpectsResponse, kIsSync, std::move(in_name));

#if defined(ENABLE_IPC_FUZZER)
  message.set_interface_name(NamedObject::Name_);
  message.set_method_name("SetName");
#endif
  // This return value may be ignored as false implies the Connector has
  // encountered an error, which will be visible through other means.
  ignore_result(receiver_->Accept(&message));
}
class NamedObjectProxy_GetName_Message
    : public mojo::internal::UnserializedMessageContext {
 public:
  static const mojo::internal::UnserializedMessageContext::Tag kMessageTag;

  explicit NamedObjectProxy_GetName_Message(
      uint32_t message_flags
  )
      : mojo::internal::UnserializedMessageContext(
          &kMessageTag,
          internal::kNamedObject_GetName_Name,
          message_flags){}
  ~NamedObjectProxy_GetName_Message() override = default;

  static mojo::Message Build(
      bool serialize,
      bool expects_response,
      bool is_sync) {

    
  const uint32_t kFlags =
      ((expects_response) ? mojo::Message::kFlagExpectsResponse : 0) |
      ((is_sync) ? mojo::Message::kFlagIsSync : 0);

    if (!serialize) {
      return mojo::Message(std::make_unique<NamedObjectProxy_GetName_Message>(
          kFlags
          ));
    }

    DCHECK(serialize);
    
  mojo::Message message(
      internal::kNamedObject_GetName_Name, kFlags, 0, 0, nullptr);
  auto* buffer = message.payload_buffer();
  ::sample::internal::NamedObject_GetName_Params_Data::BufferWriter
      params;
  mojo::internal::SerializationContext serialization_context;
  params.Allocate(buffer);
  message.AttachHandlesFromSerializationContext(
      &serialization_context);
    return message;
  }


  void Dispatch(NamedObject* impl, NamedObject::GetNameCallback callback) {
    impl->GetName(std::move(callback));
  }

 private:
  // mojo::internal::UnserializedMessageContext:
  void Serialize(mojo::internal::SerializationContext* serialization_context,
                 mojo::internal::Buffer* buffer) override {
    ::sample::internal::NamedObject_GetName_Params_Data::BufferWriter
        params;
    params.Allocate(buffer);
  }

  DISALLOW_COPY_AND_ASSIGN(NamedObjectProxy_GetName_Message);
};

const mojo::internal::UnserializedMessageContext::Tag
NamedObjectProxy_GetName_Message::kMessageTag = {};

void NamedObjectProxy::GetName(
    GetNameCallback callback) {
#if BUILDFLAG(MOJO_TRACE_ENABLED)
  TRACE_EVENT0("mojom", "<class 'jinja2::utils::Namespace'>::NamedObject::GetName");
#endif
  const bool kExpectsResponse = true;
  const bool kIsSync = false;
  const bool kSerialize = receiver_->PrefersSerializedMessages();
  auto message = NamedObjectProxy_GetName_Message::Build(
      kSerialize, kExpectsResponse, kIsSync);

#if defined(ENABLE_IPC_FUZZER)
  message.set_interface_name(NamedObject::Name_);
  message.set_method_name("GetName");
#endif
  std::unique_ptr<mojo::MessageReceiver> responder(
      new NamedObject_GetName_ForwardToCallback(
          std::move(callback)));
  ignore_result(receiver_->AcceptWithResponder(&message, std::move(responder)));
}
class NamedObject_GetName_ProxyToResponder {
 public:
  static NamedObject::GetNameCallback CreateCallback(
      uint64_t request_id,
      bool is_sync,
      std::unique_ptr<mojo::MessageReceiverWithStatus> responder) {
    std::unique_ptr<NamedObject_GetName_ProxyToResponder> proxy(
        new NamedObject_GetName_ProxyToResponder(
            request_id, is_sync, std::move(responder)));
    return base::BindOnce(&NamedObject_GetName_ProxyToResponder::Run,
                          std::move(proxy));
  }

  ~NamedObject_GetName_ProxyToResponder() {
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
  NamedObject_GetName_ProxyToResponder(
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
        << "NamedObject::GetNameCallback was destroyed without "
        << "first either being run or its corresponding binding being closed. "
        << "It is an error to drop response callbacks which still correspond "
        << "to an open interface pipe.";
  }
#endif

  void Run(
      const std::string& in_name);

  uint64_t request_id_;
  bool is_sync_;
  std::unique_ptr<mojo::MessageReceiverWithStatus> responder_;

  DISALLOW_COPY_AND_ASSIGN(NamedObject_GetName_ProxyToResponder);
};
class NamedObject_GetName_Response_Message
    : public mojo::internal::UnserializedMessageContext {
 public:
  static const mojo::internal::UnserializedMessageContext::Tag kMessageTag;

  explicit NamedObject_GetName_Response_Message(
      uint32_t message_flags
      , const std::string& param_name
  )
      : mojo::internal::UnserializedMessageContext(
          &kMessageTag,
          internal::kNamedObject_GetName_Name,
          message_flags)
      , param_name_(std::move(param_name)){}
  ~NamedObject_GetName_Response_Message() override = default;

  static mojo::Message Build(
      bool serialize,
      bool is_sync,
      const std::string& param_name) {

    
  const uint32_t kFlags = mojo::Message::kFlagIsResponse |
      ((is_sync) ? mojo::Message::kFlagIsSync : 0);

    if (!serialize) {
      return mojo::Message(std::make_unique<NamedObject_GetName_Response_Message>(
          kFlags
          , std::move(param_name)
          ));
    }

    DCHECK(serialize);
    
  mojo::Message message(
      internal::kNamedObject_GetName_Name, kFlags, 0, 0, nullptr);
  auto* buffer = message.payload_buffer();
  ::sample::internal::NamedObject_GetName_ResponseParams_Data::BufferWriter
      params;
  mojo::internal::SerializationContext serialization_context;
  params.Allocate(buffer);
  typename decltype(params->name)::BaseType::BufferWriter
      name_writer;
  mojo::internal::Serialize<mojo::StringDataView>(
      param_name, buffer, &name_writer, &serialization_context);
  params->name.Set(
      name_writer.is_null() ? nullptr : name_writer.data());
  MOJO_INTERNAL_DLOG_SERIALIZATION_WARNING(
      params->name.is_null(),
      mojo::internal::VALIDATION_ERROR_UNEXPECTED_NULL_POINTER,
      "null name in NamedObject.GetName response");
  message.AttachHandlesFromSerializationContext(
      &serialization_context);
    return message;
  }


  void Dispatch(NamedObject::GetNameCallback* callback) {
    std::move(*callback).Run(
        std::move(param_name_));
  }



 private:
  // mojo::internal::UnserializedMessageContext:
  void Serialize(mojo::internal::SerializationContext* serialization_context,
                 mojo::internal::Buffer* buffer) override {
    ::sample::internal::NamedObject_GetName_ResponseParams_Data::BufferWriter
        params;
    params.Allocate(buffer);
  typename decltype(params->name)::BaseType::BufferWriter
      name_writer;
  mojo::internal::Serialize<mojo::StringDataView>(
      param_name_, buffer, &name_writer, serialization_context);
  params->name.Set(
      name_writer.is_null() ? nullptr : name_writer.data());
  MOJO_INTERNAL_DLOG_SERIALIZATION_WARNING(
      params->name.is_null(),
      mojo::internal::VALIDATION_ERROR_UNEXPECTED_NULL_POINTER,
      "null name in NamedObject.GetName response");
  }
  std::string param_name_;

  DISALLOW_COPY_AND_ASSIGN(NamedObject_GetName_Response_Message);
};

const mojo::internal::UnserializedMessageContext::Tag
NamedObject_GetName_Response_Message::kMessageTag = {};

bool NamedObject_GetName_ForwardToCallback::Accept(
    mojo::Message* message) {
#if BUILDFLAG(MOJO_TRACE_ENABLED)
  TRACE_EVENT1("mojom", "<class 'jinja2::utils::Namespace'>::NamedObject::GetNameCallback",
               "message", message->name());
#endif
  mojo::internal::MessageDispatchContext dispatch_context(message);
  if (!message->is_serialized()) {
    auto context =
        message->TakeUnserializedContext<NamedObject_GetName_Response_Message>();
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
  internal::NamedObject_GetName_ResponseParams_Data* params =
      reinterpret_cast<
          internal::NamedObject_GetName_ResponseParams_Data*>(
              message->mutable_payload());
  
  mojo::internal::SerializationContext serialization_context;
  serialization_context.TakeHandlesFromMessage(message);
  bool success = true;
  std::string p_name{};
  NamedObject_GetName_ResponseParamsDataView input_data_view(params, &serialization_context);
  
  if (!input_data_view.ReadName(&p_name))
    success = false;
  if (!success) {
    ReportValidationErrorForMessage(
        message,
        mojo::internal::VALIDATION_ERROR_DESERIALIZATION_FAILED,
        NamedObject::Name_, 1, true);
    return false;
  }
  if (!callback_.is_null())
    std::move(callback_).Run(
std::move(p_name));
  return true;
}

void NamedObject_GetName_ProxyToResponder::Run(
    const std::string& in_name) {
  const bool kSerialize = responder_->PrefersSerializedMessages();
  auto message = NamedObject_GetName_Response_Message::Build(kSerialize, is_sync_, std::move(in_name));

#if BUILDFLAG(MOJO_TRACE_ENABLED)
  TRACE_EVENT1("mojom", "(Impl)<class 'jinja2::utils::Namespace'>::NamedObject::GetNameCallback",
               "message", message.name());
#endif

#if defined(ENABLE_IPC_FUZZER)
  message.set_interface_name(NamedObject::Name_);
  message.set_method_name("GetName");
#endif

  message.set_request_id(request_id_);
  ignore_result(responder_->Accept(&message));
  // TODO(darin): Accept() returning false indicates a malformed message, and
  // that may be good reason to close the connection. However, we don't have a
  // way to do that from here. We should add a way.
  responder_ = nullptr;
}

// static
bool NamedObjectStubDispatch::Accept(
    NamedObject* impl,
    mojo::Message* message) {
  switch (message->header()->name) {
    case internal::kNamedObject_SetName_Name: {
#if BUILDFLAG(MOJO_TRACE_ENABLED)
      TRACE_EVENT1(
          "mojom",
          "(Impl)<class 'jinja2::utils::Namespace'>::NamedObject::SetName",
          "message", message->name());
#endif
      base::TaskAnnotator::ScopedSetIpcProgramCounter scoped_ipc_pc(
          base::GetProgramCounter());
      mojo::internal::MessageDispatchContext context(message);
      if (!message->is_serialized()) {
        auto context = message->TakeUnserializedContext<
            NamedObjectProxy_SetName_Message>();
        if (!context) {
          // The Message was not of the expected type. It may be a valid message
          // which was serialized using a different variant of these bindings.
          // Force serialization before dispatch in this case.
          message->SerializeIfNecessary();
        } else {
          context->Dispatch(impl);
          return true;
        }
      }

      DCHECK(message->is_serialized());
      internal::NamedObject_SetName_Params_Data* params =
          reinterpret_cast<internal::NamedObject_SetName_Params_Data*>(
              message->mutable_payload());
      
      mojo::internal::SerializationContext serialization_context;
      serialization_context.TakeHandlesFromMessage(message);
      bool success = true;
      std::string p_name{};
      NamedObject_SetName_ParamsDataView input_data_view(params, &serialization_context);
      
      if (!input_data_view.ReadName(&p_name))
        success = false;
      if (!success) {
        ReportValidationErrorForMessage(
            message,
            mojo::internal::VALIDATION_ERROR_DESERIALIZATION_FAILED,
            NamedObject::Name_, 0, false);
        return false;
      }
      // A null |impl| means no implementation was bound.
      DCHECK(impl);
      impl->SetName(
std::move(p_name));
      return true;
    }
    case internal::kNamedObject_GetName_Name: {
      break;
    }
  }
  return false;
}

// static
bool NamedObjectStubDispatch::AcceptWithResponder(
    NamedObject* impl,
    mojo::Message* message,
    std::unique_ptr<mojo::MessageReceiverWithStatus> responder) {
  switch (message->header()->name) {
    case internal::kNamedObject_SetName_Name: {
      break;
    }
    case internal::kNamedObject_GetName_Name: {
#if BUILDFLAG(MOJO_TRACE_ENABLED)
      TRACE_EVENT1(
          "mojom",
          "(Impl)<class 'jinja2::utils::Namespace'>::NamedObject::GetName",
          "message", message->name());
#endif
      base::TaskAnnotator::ScopedSetIpcProgramCounter scoped_ipc_pc(
          base::GetProgramCounter());
      mojo::internal::MessageDispatchContext context(message);
      if (!message->is_serialized()) {
        auto context = message->TakeUnserializedContext<
            NamedObjectProxy_GetName_Message>();
        if (!context) {
          // The Message was not of the expected type. It may be a valid message
          // which was built using a different variant of these bindings. Force
          // serialization before dispatch in this case.
          message->SerializeIfNecessary();
        } else {
          NamedObject::GetNameCallback callback =
              NamedObject_GetName_ProxyToResponder::CreateCallback(
                  message->request_id(),
                  message->has_flag(mojo::Message::kFlagIsSync),
                  std::move(responder));
          context->Dispatch(impl, std::move(callback));
          return true;
        }
      }

      internal::NamedObject_GetName_Params_Data* params =
          reinterpret_cast<
              internal::NamedObject_GetName_Params_Data*>(
                  message->mutable_payload());
      
      mojo::internal::SerializationContext serialization_context;
      serialization_context.TakeHandlesFromMessage(message);
      bool success = true;
      NamedObject_GetName_ParamsDataView input_data_view(params, &serialization_context);
      
      if (!success) {
        ReportValidationErrorForMessage(
            message,
            mojo::internal::VALIDATION_ERROR_DESERIALIZATION_FAILED,
            NamedObject::Name_, 1, false);
        return false;
      }
      NamedObject::GetNameCallback callback =
          NamedObject_GetName_ProxyToResponder::CreateCallback(
              message->request_id(),
              message->has_flag(mojo::Message::kFlagIsSync),
              std::move(responder));
      // A null |impl| means no implementation was bound.
      DCHECK(impl);
      impl->GetName(std::move(callback));
      return true;
    }
  }
  return false;
}

bool NamedObjectRequestValidator::Accept(mojo::Message* message) {
  if (!message->is_serialized() ||
      mojo::internal::ControlMessageHandler::IsControlMessage(message)) {
    return true;
  }

  mojo::internal::ValidationContext validation_context(
    message->payload(), message->payload_num_bytes(),
    message->handles()->size(), message->payload_num_interface_ids(), message,
    "NamedObject RequestValidator");

  switch (message->header()->name) {
    case internal::kNamedObject_SetName_Name: {
      if (!mojo::internal::ValidateMessageIsRequestWithoutResponse(
              message, &validation_context)) {
        return false;
      }
      if (!mojo::internal::ValidateMessagePayload<
               internal::NamedObject_SetName_Params_Data>(
                  message, &validation_context)) {
        return false;
      }
      return true;
    }
    case internal::kNamedObject_GetName_Name: {
      if (!mojo::internal::ValidateMessageIsRequestExpectingResponse(
              message, &validation_context)) {
        return false;
      }
      if (!mojo::internal::ValidateMessagePayload<
               internal::NamedObject_GetName_Params_Data>(
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

bool NamedObjectResponseValidator::Accept(mojo::Message* message) {
  if (!message->is_serialized() ||
      mojo::internal::ControlMessageHandler::IsControlMessage(message)) {
    return true;
  }

  mojo::internal::ValidationContext validation_context(
    message->payload(), message->payload_num_bytes(),
    message->handles()->size(), message->payload_num_interface_ids(), message,
    "NamedObject ResponseValidator");

  if (!mojo::internal::ValidateMessageIsResponse(message, &validation_context))
    return false;
  switch (message->header()->name) {
    case internal::kNamedObject_GetName_Name: {
      if (!mojo::internal::ValidateMessagePayload<
               internal::NamedObject_GetName_ResponseParams_Data>(
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
const char Factory::Name_[] = "sample.Factory";

class Factory_DoStuff_ForwardToCallback
    : public mojo::MessageReceiver {
 public:
  Factory_DoStuff_ForwardToCallback(
      Factory::DoStuffCallback callback
      ) : callback_(std::move(callback)) {
  }
  bool Accept(mojo::Message* message) override;
 private:
  Factory::DoStuffCallback callback_;
  DISALLOW_COPY_AND_ASSIGN(Factory_DoStuff_ForwardToCallback);
};

class Factory_DoStuff2_ForwardToCallback
    : public mojo::MessageReceiver {
 public:
  Factory_DoStuff2_ForwardToCallback(
      Factory::DoStuff2Callback callback
      ) : callback_(std::move(callback)) {
  }
  bool Accept(mojo::Message* message) override;
 private:
  Factory::DoStuff2Callback callback_;
  DISALLOW_COPY_AND_ASSIGN(Factory_DoStuff2_ForwardToCallback);
};

class Factory_RequestImportedInterface_ForwardToCallback
    : public mojo::MessageReceiver {
 public:
  Factory_RequestImportedInterface_ForwardToCallback(
      Factory::RequestImportedInterfaceCallback callback
      ) : callback_(std::move(callback)) {
  }
  bool Accept(mojo::Message* message) override;
 private:
  Factory::RequestImportedInterfaceCallback callback_;
  DISALLOW_COPY_AND_ASSIGN(Factory_RequestImportedInterface_ForwardToCallback);
};

class Factory_TakeImportedInterface_ForwardToCallback
    : public mojo::MessageReceiver {
 public:
  Factory_TakeImportedInterface_ForwardToCallback(
      Factory::TakeImportedInterfaceCallback callback
      ) : callback_(std::move(callback)) {
  }
  bool Accept(mojo::Message* message) override;
 private:
  Factory::TakeImportedInterfaceCallback callback_;
  DISALLOW_COPY_AND_ASSIGN(Factory_TakeImportedInterface_ForwardToCallback);
};

FactoryProxy::FactoryProxy(mojo::MessageReceiverWithResponder* receiver)
    : receiver_(receiver) {
}
class FactoryProxy_DoStuff_Message
    : public mojo::internal::UnserializedMessageContext {
 public:
  static const mojo::internal::UnserializedMessageContext::Tag kMessageTag;

  explicit FactoryProxy_DoStuff_Message(
      uint32_t message_flags
      , RequestPtr param_request
      , mojo::ScopedMessagePipeHandle param_pipe
  )
      : mojo::internal::UnserializedMessageContext(
          &kMessageTag,
          internal::kFactory_DoStuff_Name,
          message_flags)
      , param_request_(std::move(param_request))
      , param_pipe_(std::move(param_pipe)){}
  ~FactoryProxy_DoStuff_Message() override = default;

  static mojo::Message Build(
      bool serialize,
      bool expects_response,
      bool is_sync,
      RequestPtr param_request, mojo::ScopedMessagePipeHandle param_pipe) {

    
  const uint32_t kFlags =
      ((expects_response) ? mojo::Message::kFlagExpectsResponse : 0) |
      ((is_sync) ? mojo::Message::kFlagIsSync : 0);

    if (!serialize) {
      return mojo::Message(std::make_unique<FactoryProxy_DoStuff_Message>(
          kFlags
          , std::move(param_request)
          , std::move(param_pipe)
          ));
    }

    DCHECK(serialize);
    
  mojo::Message message(
      internal::kFactory_DoStuff_Name, kFlags, 0, 0, nullptr);
  auto* buffer = message.payload_buffer();
  ::sample::internal::Factory_DoStuff_Params_Data::BufferWriter
      params;
  mojo::internal::SerializationContext serialization_context;
  params.Allocate(buffer);
  typename decltype(params->request)::BaseType::BufferWriter
      request_writer;
  mojo::internal::Serialize<::sample::RequestDataView>(
      param_request, buffer, &request_writer, &serialization_context);
  params->request.Set(
      request_writer.is_null() ? nullptr : request_writer.data());
  MOJO_INTERNAL_DLOG_SERIALIZATION_WARNING(
      params->request.is_null(),
      mojo::internal::VALIDATION_ERROR_UNEXPECTED_NULL_POINTER,
      "null request in Factory.DoStuff request");
  mojo::internal::Serialize<mojo::ScopedMessagePipeHandle>(
      param_pipe, &params->pipe, &serialization_context);
  message.AttachHandlesFromSerializationContext(
      &serialization_context);
    return message;
  }


  void Dispatch(Factory* impl, Factory::DoStuffCallback callback) {
    impl->DoStuff(
        std::move(param_request_),
        std::move(param_pipe_), std::move(callback));
  }

 private:
  // mojo::internal::UnserializedMessageContext:
  void Serialize(mojo::internal::SerializationContext* serialization_context,
                 mojo::internal::Buffer* buffer) override {
    ::sample::internal::Factory_DoStuff_Params_Data::BufferWriter
        params;
    params.Allocate(buffer);
  typename decltype(params->request)::BaseType::BufferWriter
      request_writer;
  mojo::internal::Serialize<::sample::RequestDataView>(
      param_request_, buffer, &request_writer, serialization_context);
  params->request.Set(
      request_writer.is_null() ? nullptr : request_writer.data());
  MOJO_INTERNAL_DLOG_SERIALIZATION_WARNING(
      params->request.is_null(),
      mojo::internal::VALIDATION_ERROR_UNEXPECTED_NULL_POINTER,
      "null request in Factory.DoStuff request");
  mojo::internal::Serialize<mojo::ScopedMessagePipeHandle>(
      param_pipe_, &params->pipe, serialization_context);
  }
  RequestPtr param_request_;
  mojo::ScopedMessagePipeHandle param_pipe_;

  DISALLOW_COPY_AND_ASSIGN(FactoryProxy_DoStuff_Message);
};

const mojo::internal::UnserializedMessageContext::Tag
FactoryProxy_DoStuff_Message::kMessageTag = {};

void FactoryProxy::DoStuff(
    RequestPtr in_request, mojo::ScopedMessagePipeHandle in_pipe, DoStuffCallback callback) {
#if BUILDFLAG(MOJO_TRACE_ENABLED)
  TRACE_EVENT0("mojom", "<class 'jinja2::utils::Namespace'>::Factory::DoStuff");
#endif
  const bool kExpectsResponse = true;
  const bool kIsSync = false;
  const bool kSerialize = receiver_->PrefersSerializedMessages();
  auto message = FactoryProxy_DoStuff_Message::Build(
      kSerialize, kExpectsResponse, kIsSync, std::move(in_request), std::move(in_pipe));

#if defined(ENABLE_IPC_FUZZER)
  message.set_interface_name(Factory::Name_);
  message.set_method_name("DoStuff");
#endif
  std::unique_ptr<mojo::MessageReceiver> responder(
      new Factory_DoStuff_ForwardToCallback(
          std::move(callback)));
  ignore_result(receiver_->AcceptWithResponder(&message, std::move(responder)));
}
class FactoryProxy_DoStuff2_Message
    : public mojo::internal::UnserializedMessageContext {
 public:
  static const mojo::internal::UnserializedMessageContext::Tag kMessageTag;

  explicit FactoryProxy_DoStuff2_Message(
      uint32_t message_flags
      , mojo::ScopedDataPipeConsumerHandle param_pipe
  )
      : mojo::internal::UnserializedMessageContext(
          &kMessageTag,
          internal::kFactory_DoStuff2_Name,
          message_flags)
      , param_pipe_(std::move(param_pipe)){}
  ~FactoryProxy_DoStuff2_Message() override = default;

  static mojo::Message Build(
      bool serialize,
      bool expects_response,
      bool is_sync,
      mojo::ScopedDataPipeConsumerHandle param_pipe) {

    
  const uint32_t kFlags =
      ((expects_response) ? mojo::Message::kFlagExpectsResponse : 0) |
      ((is_sync) ? mojo::Message::kFlagIsSync : 0);

    if (!serialize) {
      return mojo::Message(std::make_unique<FactoryProxy_DoStuff2_Message>(
          kFlags
          , std::move(param_pipe)
          ));
    }

    DCHECK(serialize);
    
  mojo::Message message(
      internal::kFactory_DoStuff2_Name, kFlags, 0, 0, nullptr);
  auto* buffer = message.payload_buffer();
  ::sample::internal::Factory_DoStuff2_Params_Data::BufferWriter
      params;
  mojo::internal::SerializationContext serialization_context;
  params.Allocate(buffer);
  mojo::internal::Serialize<mojo::ScopedDataPipeConsumerHandle>(
      param_pipe, &params->pipe, &serialization_context);
  MOJO_INTERNAL_DLOG_SERIALIZATION_WARNING(
      !mojo::internal::IsHandleOrInterfaceValid(params->pipe),
      mojo::internal::VALIDATION_ERROR_UNEXPECTED_INVALID_HANDLE,
      "invalid pipe in Factory.DoStuff2 request");
  message.AttachHandlesFromSerializationContext(
      &serialization_context);
    return message;
  }


  void Dispatch(Factory* impl, Factory::DoStuff2Callback callback) {
    impl->DoStuff2(
        std::move(param_pipe_), std::move(callback));
  }

 private:
  // mojo::internal::UnserializedMessageContext:
  void Serialize(mojo::internal::SerializationContext* serialization_context,
                 mojo::internal::Buffer* buffer) override {
    ::sample::internal::Factory_DoStuff2_Params_Data::BufferWriter
        params;
    params.Allocate(buffer);
  mojo::internal::Serialize<mojo::ScopedDataPipeConsumerHandle>(
      param_pipe_, &params->pipe, serialization_context);
  MOJO_INTERNAL_DLOG_SERIALIZATION_WARNING(
      !mojo::internal::IsHandleOrInterfaceValid(params->pipe),
      mojo::internal::VALIDATION_ERROR_UNEXPECTED_INVALID_HANDLE,
      "invalid pipe in Factory.DoStuff2 request");
  }
  mojo::ScopedDataPipeConsumerHandle param_pipe_;

  DISALLOW_COPY_AND_ASSIGN(FactoryProxy_DoStuff2_Message);
};

const mojo::internal::UnserializedMessageContext::Tag
FactoryProxy_DoStuff2_Message::kMessageTag = {};

void FactoryProxy::DoStuff2(
    mojo::ScopedDataPipeConsumerHandle in_pipe, DoStuff2Callback callback) {
#if BUILDFLAG(MOJO_TRACE_ENABLED)
  TRACE_EVENT0("mojom", "<class 'jinja2::utils::Namespace'>::Factory::DoStuff2");
#endif
  const bool kExpectsResponse = true;
  const bool kIsSync = false;
  const bool kSerialize = receiver_->PrefersSerializedMessages();
  auto message = FactoryProxy_DoStuff2_Message::Build(
      kSerialize, kExpectsResponse, kIsSync, std::move(in_pipe));

#if defined(ENABLE_IPC_FUZZER)
  message.set_interface_name(Factory::Name_);
  message.set_method_name("DoStuff2");
#endif
  std::unique_ptr<mojo::MessageReceiver> responder(
      new Factory_DoStuff2_ForwardToCallback(
          std::move(callback)));
  ignore_result(receiver_->AcceptWithResponder(&message, std::move(responder)));
}
class FactoryProxy_CreateNamedObject_Message
    : public mojo::internal::UnserializedMessageContext {
 public:
  static const mojo::internal::UnserializedMessageContext::Tag kMessageTag;

  explicit FactoryProxy_CreateNamedObject_Message(
      uint32_t message_flags
      , mojo::PendingReceiver<NamedObject> param_obj
  )
      : mojo::internal::UnserializedMessageContext(
          &kMessageTag,
          internal::kFactory_CreateNamedObject_Name,
          message_flags)
      , param_obj_(std::move(param_obj)){}
  ~FactoryProxy_CreateNamedObject_Message() override = default;

  static mojo::Message Build(
      bool serialize,
      bool expects_response,
      bool is_sync,
      mojo::PendingReceiver<NamedObject> param_obj) {

    
  const uint32_t kFlags =
      ((expects_response) ? mojo::Message::kFlagExpectsResponse : 0) |
      ((is_sync) ? mojo::Message::kFlagIsSync : 0);

    if (!serialize) {
      return mojo::Message(std::make_unique<FactoryProxy_CreateNamedObject_Message>(
          kFlags
          , std::move(param_obj)
          ));
    }

    DCHECK(serialize);
    
  mojo::Message message(
      internal::kFactory_CreateNamedObject_Name, kFlags, 0, 0, nullptr);
  auto* buffer = message.payload_buffer();
  ::sample::internal::Factory_CreateNamedObject_Params_Data::BufferWriter
      params;
  mojo::internal::SerializationContext serialization_context;
  params.Allocate(buffer);
  mojo::internal::Serialize<mojo::InterfaceRequestDataView<::sample::NamedObjectInterfaceBase>>(
      param_obj, &params->obj, &serialization_context);
  MOJO_INTERNAL_DLOG_SERIALIZATION_WARNING(
      !mojo::internal::IsHandleOrInterfaceValid(params->obj),
      mojo::internal::VALIDATION_ERROR_UNEXPECTED_INVALID_HANDLE,
      "invalid obj in Factory.CreateNamedObject request");
  message.AttachHandlesFromSerializationContext(
      &serialization_context);
    return message;
  }


  void Dispatch(Factory* impl) {
    impl->CreateNamedObject(
        std::move(param_obj_));
  }

 private:
  // mojo::internal::UnserializedMessageContext:
  void Serialize(mojo::internal::SerializationContext* serialization_context,
                 mojo::internal::Buffer* buffer) override {
    ::sample::internal::Factory_CreateNamedObject_Params_Data::BufferWriter
        params;
    params.Allocate(buffer);
  mojo::internal::Serialize<mojo::InterfaceRequestDataView<::sample::NamedObjectInterfaceBase>>(
      param_obj_, &params->obj, serialization_context);
  MOJO_INTERNAL_DLOG_SERIALIZATION_WARNING(
      !mojo::internal::IsHandleOrInterfaceValid(params->obj),
      mojo::internal::VALIDATION_ERROR_UNEXPECTED_INVALID_HANDLE,
      "invalid obj in Factory.CreateNamedObject request");
  }
  mojo::PendingReceiver<NamedObject> param_obj_;

  DISALLOW_COPY_AND_ASSIGN(FactoryProxy_CreateNamedObject_Message);
};

const mojo::internal::UnserializedMessageContext::Tag
FactoryProxy_CreateNamedObject_Message::kMessageTag = {};

void FactoryProxy::CreateNamedObject(
    mojo::PendingReceiver<NamedObject> in_obj) {
#if BUILDFLAG(MOJO_TRACE_ENABLED)
  TRACE_EVENT0("mojom", "<class 'jinja2::utils::Namespace'>::Factory::CreateNamedObject");
#endif
  const bool kExpectsResponse = false;
  const bool kIsSync = false;
  const bool kSerialize = receiver_->PrefersSerializedMessages();
  auto message = FactoryProxy_CreateNamedObject_Message::Build(
      kSerialize, kExpectsResponse, kIsSync, std::move(in_obj));

#if defined(ENABLE_IPC_FUZZER)
  message.set_interface_name(Factory::Name_);
  message.set_method_name("CreateNamedObject");
#endif
  // This return value may be ignored as false implies the Connector has
  // encountered an error, which will be visible through other means.
  ignore_result(receiver_->Accept(&message));
}
class FactoryProxy_RequestImportedInterface_Message
    : public mojo::internal::UnserializedMessageContext {
 public:
  static const mojo::internal::UnserializedMessageContext::Tag kMessageTag;

  explicit FactoryProxy_RequestImportedInterface_Message(
      uint32_t message_flags
      , mojo::PendingReceiver<::imported::ImportedInterface> param_obj
  )
      : mojo::internal::UnserializedMessageContext(
          &kMessageTag,
          internal::kFactory_RequestImportedInterface_Name,
          message_flags)
      , param_obj_(std::move(param_obj)){}
  ~FactoryProxy_RequestImportedInterface_Message() override = default;

  static mojo::Message Build(
      bool serialize,
      bool expects_response,
      bool is_sync,
      mojo::PendingReceiver<::imported::ImportedInterface> param_obj) {

    
  const uint32_t kFlags =
      ((expects_response) ? mojo::Message::kFlagExpectsResponse : 0) |
      ((is_sync) ? mojo::Message::kFlagIsSync : 0);

    if (!serialize) {
      return mojo::Message(std::make_unique<FactoryProxy_RequestImportedInterface_Message>(
          kFlags
          , std::move(param_obj)
          ));
    }

    DCHECK(serialize);
    
  mojo::Message message(
      internal::kFactory_RequestImportedInterface_Name, kFlags, 0, 0, nullptr);
  auto* buffer = message.payload_buffer();
  ::sample::internal::Factory_RequestImportedInterface_Params_Data::BufferWriter
      params;
  mojo::internal::SerializationContext serialization_context;
  params.Allocate(buffer);
  mojo::internal::Serialize<mojo::InterfaceRequestDataView<::imported::ImportedInterfaceInterfaceBase>>(
      param_obj, &params->obj, &serialization_context);
  MOJO_INTERNAL_DLOG_SERIALIZATION_WARNING(
      !mojo::internal::IsHandleOrInterfaceValid(params->obj),
      mojo::internal::VALIDATION_ERROR_UNEXPECTED_INVALID_HANDLE,
      "invalid obj in Factory.RequestImportedInterface request");
  message.AttachHandlesFromSerializationContext(
      &serialization_context);
    return message;
  }


  void Dispatch(Factory* impl, Factory::RequestImportedInterfaceCallback callback) {
    impl->RequestImportedInterface(
        std::move(param_obj_), std::move(callback));
  }

 private:
  // mojo::internal::UnserializedMessageContext:
  void Serialize(mojo::internal::SerializationContext* serialization_context,
                 mojo::internal::Buffer* buffer) override {
    ::sample::internal::Factory_RequestImportedInterface_Params_Data::BufferWriter
        params;
    params.Allocate(buffer);
  mojo::internal::Serialize<mojo::InterfaceRequestDataView<::imported::ImportedInterfaceInterfaceBase>>(
      param_obj_, &params->obj, serialization_context);
  MOJO_INTERNAL_DLOG_SERIALIZATION_WARNING(
      !mojo::internal::IsHandleOrInterfaceValid(params->obj),
      mojo::internal::VALIDATION_ERROR_UNEXPECTED_INVALID_HANDLE,
      "invalid obj in Factory.RequestImportedInterface request");
  }
  mojo::PendingReceiver<::imported::ImportedInterface> param_obj_;

  DISALLOW_COPY_AND_ASSIGN(FactoryProxy_RequestImportedInterface_Message);
};

const mojo::internal::UnserializedMessageContext::Tag
FactoryProxy_RequestImportedInterface_Message::kMessageTag = {};

void FactoryProxy::RequestImportedInterface(
    mojo::PendingReceiver<::imported::ImportedInterface> in_obj, RequestImportedInterfaceCallback callback) {
#if BUILDFLAG(MOJO_TRACE_ENABLED)
  TRACE_EVENT0("mojom", "<class 'jinja2::utils::Namespace'>::Factory::RequestImportedInterface");
#endif
  const bool kExpectsResponse = true;
  const bool kIsSync = false;
  const bool kSerialize = receiver_->PrefersSerializedMessages();
  auto message = FactoryProxy_RequestImportedInterface_Message::Build(
      kSerialize, kExpectsResponse, kIsSync, std::move(in_obj));

#if defined(ENABLE_IPC_FUZZER)
  message.set_interface_name(Factory::Name_);
  message.set_method_name("RequestImportedInterface");
#endif
  std::unique_ptr<mojo::MessageReceiver> responder(
      new Factory_RequestImportedInterface_ForwardToCallback(
          std::move(callback)));
  ignore_result(receiver_->AcceptWithResponder(&message, std::move(responder)));
}
class FactoryProxy_TakeImportedInterface_Message
    : public mojo::internal::UnserializedMessageContext {
 public:
  static const mojo::internal::UnserializedMessageContext::Tag kMessageTag;

  explicit FactoryProxy_TakeImportedInterface_Message(
      uint32_t message_flags
      , mojo::PendingRemote<::imported::ImportedInterface> param_obj
  )
      : mojo::internal::UnserializedMessageContext(
          &kMessageTag,
          internal::kFactory_TakeImportedInterface_Name,
          message_flags)
      , param_obj_(std::move(param_obj)){}
  ~FactoryProxy_TakeImportedInterface_Message() override = default;

  static mojo::Message Build(
      bool serialize,
      bool expects_response,
      bool is_sync,
      mojo::PendingRemote<::imported::ImportedInterface> param_obj) {

    
  const uint32_t kFlags =
      ((expects_response) ? mojo::Message::kFlagExpectsResponse : 0) |
      ((is_sync) ? mojo::Message::kFlagIsSync : 0);

    if (!serialize) {
      return mojo::Message(std::make_unique<FactoryProxy_TakeImportedInterface_Message>(
          kFlags
          , std::move(param_obj)
          ));
    }

    DCHECK(serialize);
    
  mojo::Message message(
      internal::kFactory_TakeImportedInterface_Name, kFlags, 0, 0, nullptr);
  auto* buffer = message.payload_buffer();
  ::sample::internal::Factory_TakeImportedInterface_Params_Data::BufferWriter
      params;
  mojo::internal::SerializationContext serialization_context;
  params.Allocate(buffer);
  mojo::internal::Serialize<mojo::InterfacePtrDataView<::imported::ImportedInterfaceInterfaceBase>>(
      param_obj, &params->obj, &serialization_context);
  MOJO_INTERNAL_DLOG_SERIALIZATION_WARNING(
      !mojo::internal::IsHandleOrInterfaceValid(params->obj),
      mojo::internal::VALIDATION_ERROR_UNEXPECTED_INVALID_HANDLE,
      "invalid obj in Factory.TakeImportedInterface request");
  message.AttachHandlesFromSerializationContext(
      &serialization_context);
    return message;
  }


  void Dispatch(Factory* impl, Factory::TakeImportedInterfaceCallback callback) {
    impl->TakeImportedInterface(
        std::move(param_obj_), std::move(callback));
  }

 private:
  // mojo::internal::UnserializedMessageContext:
  void Serialize(mojo::internal::SerializationContext* serialization_context,
                 mojo::internal::Buffer* buffer) override {
    ::sample::internal::Factory_TakeImportedInterface_Params_Data::BufferWriter
        params;
    params.Allocate(buffer);
  mojo::internal::Serialize<mojo::InterfacePtrDataView<::imported::ImportedInterfaceInterfaceBase>>(
      param_obj_, &params->obj, serialization_context);
  MOJO_INTERNAL_DLOG_SERIALIZATION_WARNING(
      !mojo::internal::IsHandleOrInterfaceValid(params->obj),
      mojo::internal::VALIDATION_ERROR_UNEXPECTED_INVALID_HANDLE,
      "invalid obj in Factory.TakeImportedInterface request");
  }
  mojo::PendingRemote<::imported::ImportedInterface> param_obj_;

  DISALLOW_COPY_AND_ASSIGN(FactoryProxy_TakeImportedInterface_Message);
};

const mojo::internal::UnserializedMessageContext::Tag
FactoryProxy_TakeImportedInterface_Message::kMessageTag = {};

void FactoryProxy::TakeImportedInterface(
    mojo::PendingRemote<::imported::ImportedInterface> in_obj, TakeImportedInterfaceCallback callback) {
#if BUILDFLAG(MOJO_TRACE_ENABLED)
  TRACE_EVENT0("mojom", "<class 'jinja2::utils::Namespace'>::Factory::TakeImportedInterface");
#endif
  const bool kExpectsResponse = true;
  const bool kIsSync = false;
  const bool kSerialize = receiver_->PrefersSerializedMessages();
  auto message = FactoryProxy_TakeImportedInterface_Message::Build(
      kSerialize, kExpectsResponse, kIsSync, std::move(in_obj));

#if defined(ENABLE_IPC_FUZZER)
  message.set_interface_name(Factory::Name_);
  message.set_method_name("TakeImportedInterface");
#endif
  std::unique_ptr<mojo::MessageReceiver> responder(
      new Factory_TakeImportedInterface_ForwardToCallback(
          std::move(callback)));
  ignore_result(receiver_->AcceptWithResponder(&message, std::move(responder)));
}
class Factory_DoStuff_ProxyToResponder {
 public:
  static Factory::DoStuffCallback CreateCallback(
      uint64_t request_id,
      bool is_sync,
      std::unique_ptr<mojo::MessageReceiverWithStatus> responder) {
    std::unique_ptr<Factory_DoStuff_ProxyToResponder> proxy(
        new Factory_DoStuff_ProxyToResponder(
            request_id, is_sync, std::move(responder)));
    return base::BindOnce(&Factory_DoStuff_ProxyToResponder::Run,
                          std::move(proxy));
  }

  ~Factory_DoStuff_ProxyToResponder() {
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
  Factory_DoStuff_ProxyToResponder(
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
        << "Factory::DoStuffCallback was destroyed without "
        << "first either being run or its corresponding binding being closed. "
        << "It is an error to drop response callbacks which still correspond "
        << "to an open interface pipe.";
  }
#endif

  void Run(
      ResponsePtr in_response, const std::string& in_text);

  uint64_t request_id_;
  bool is_sync_;
  std::unique_ptr<mojo::MessageReceiverWithStatus> responder_;

  DISALLOW_COPY_AND_ASSIGN(Factory_DoStuff_ProxyToResponder);
};
class Factory_DoStuff_Response_Message
    : public mojo::internal::UnserializedMessageContext {
 public:
  static const mojo::internal::UnserializedMessageContext::Tag kMessageTag;

  explicit Factory_DoStuff_Response_Message(
      uint32_t message_flags
      , ResponsePtr param_response
      , const std::string& param_text
  )
      : mojo::internal::UnserializedMessageContext(
          &kMessageTag,
          internal::kFactory_DoStuff_Name,
          message_flags)
      , param_response_(std::move(param_response))
      , param_text_(std::move(param_text)){}
  ~Factory_DoStuff_Response_Message() override = default;

  static mojo::Message Build(
      bool serialize,
      bool is_sync,
      ResponsePtr param_response, const std::string& param_text) {

    
  const uint32_t kFlags = mojo::Message::kFlagIsResponse |
      ((is_sync) ? mojo::Message::kFlagIsSync : 0);

    if (!serialize) {
      return mojo::Message(std::make_unique<Factory_DoStuff_Response_Message>(
          kFlags
          , std::move(param_response)
          , std::move(param_text)
          ));
    }

    DCHECK(serialize);
    
  mojo::Message message(
      internal::kFactory_DoStuff_Name, kFlags, 0, 0, nullptr);
  auto* buffer = message.payload_buffer();
  ::sample::internal::Factory_DoStuff_ResponseParams_Data::BufferWriter
      params;
  mojo::internal::SerializationContext serialization_context;
  params.Allocate(buffer);
  typename decltype(params->response)::BaseType::BufferWriter
      response_writer;
  mojo::internal::Serialize<::sample::ResponseDataView>(
      param_response, buffer, &response_writer, &serialization_context);
  params->response.Set(
      response_writer.is_null() ? nullptr : response_writer.data());
  MOJO_INTERNAL_DLOG_SERIALIZATION_WARNING(
      params->response.is_null(),
      mojo::internal::VALIDATION_ERROR_UNEXPECTED_NULL_POINTER,
      "null response in Factory.DoStuff response");
  typename decltype(params->text)::BaseType::BufferWriter
      text_writer;
  mojo::internal::Serialize<mojo::StringDataView>(
      param_text, buffer, &text_writer, &serialization_context);
  params->text.Set(
      text_writer.is_null() ? nullptr : text_writer.data());
  MOJO_INTERNAL_DLOG_SERIALIZATION_WARNING(
      params->text.is_null(),
      mojo::internal::VALIDATION_ERROR_UNEXPECTED_NULL_POINTER,
      "null text in Factory.DoStuff response");
  message.AttachHandlesFromSerializationContext(
      &serialization_context);
    return message;
  }


  void Dispatch(Factory::DoStuffCallback* callback) {
    std::move(*callback).Run(
        std::move(param_response_), 
        std::move(param_text_));
  }



 private:
  // mojo::internal::UnserializedMessageContext:
  void Serialize(mojo::internal::SerializationContext* serialization_context,
                 mojo::internal::Buffer* buffer) override {
    ::sample::internal::Factory_DoStuff_ResponseParams_Data::BufferWriter
        params;
    params.Allocate(buffer);
  typename decltype(params->response)::BaseType::BufferWriter
      response_writer;
  mojo::internal::Serialize<::sample::ResponseDataView>(
      param_response_, buffer, &response_writer, serialization_context);
  params->response.Set(
      response_writer.is_null() ? nullptr : response_writer.data());
  MOJO_INTERNAL_DLOG_SERIALIZATION_WARNING(
      params->response.is_null(),
      mojo::internal::VALIDATION_ERROR_UNEXPECTED_NULL_POINTER,
      "null response in Factory.DoStuff response");
  typename decltype(params->text)::BaseType::BufferWriter
      text_writer;
  mojo::internal::Serialize<mojo::StringDataView>(
      param_text_, buffer, &text_writer, serialization_context);
  params->text.Set(
      text_writer.is_null() ? nullptr : text_writer.data());
  MOJO_INTERNAL_DLOG_SERIALIZATION_WARNING(
      params->text.is_null(),
      mojo::internal::VALIDATION_ERROR_UNEXPECTED_NULL_POINTER,
      "null text in Factory.DoStuff response");
  }
  ResponsePtr param_response_;
  std::string param_text_;

  DISALLOW_COPY_AND_ASSIGN(Factory_DoStuff_Response_Message);
};

const mojo::internal::UnserializedMessageContext::Tag
Factory_DoStuff_Response_Message::kMessageTag = {};

bool Factory_DoStuff_ForwardToCallback::Accept(
    mojo::Message* message) {
#if BUILDFLAG(MOJO_TRACE_ENABLED)
  TRACE_EVENT1("mojom", "<class 'jinja2::utils::Namespace'>::Factory::DoStuffCallback",
               "message", message->name());
#endif
  mojo::internal::MessageDispatchContext dispatch_context(message);
  if (!message->is_serialized()) {
    auto context =
        message->TakeUnserializedContext<Factory_DoStuff_Response_Message>();
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
  internal::Factory_DoStuff_ResponseParams_Data* params =
      reinterpret_cast<
          internal::Factory_DoStuff_ResponseParams_Data*>(
              message->mutable_payload());
  
  mojo::internal::SerializationContext serialization_context;
  serialization_context.TakeHandlesFromMessage(message);
  bool success = true;
  ResponsePtr p_response{};
  std::string p_text{};
  Factory_DoStuff_ResponseParamsDataView input_data_view(params, &serialization_context);
  
  if (!input_data_view.ReadResponse(&p_response))
    success = false;
  if (!input_data_view.ReadText(&p_text))
    success = false;
  if (!success) {
    ReportValidationErrorForMessage(
        message,
        mojo::internal::VALIDATION_ERROR_DESERIALIZATION_FAILED,
        Factory::Name_, 0, true);
    return false;
  }
  if (!callback_.is_null())
    std::move(callback_).Run(
std::move(p_response), 
std::move(p_text));
  return true;
}

void Factory_DoStuff_ProxyToResponder::Run(
    ResponsePtr in_response, const std::string& in_text) {
  const bool kSerialize = responder_->PrefersSerializedMessages();
  auto message = Factory_DoStuff_Response_Message::Build(kSerialize, is_sync_, std::move(in_response), std::move(in_text));

#if BUILDFLAG(MOJO_TRACE_ENABLED)
  TRACE_EVENT1("mojom", "(Impl)<class 'jinja2::utils::Namespace'>::Factory::DoStuffCallback",
               "message", message.name());
#endif

#if defined(ENABLE_IPC_FUZZER)
  message.set_interface_name(Factory::Name_);
  message.set_method_name("DoStuff");
#endif

  message.set_request_id(request_id_);
  ignore_result(responder_->Accept(&message));
  // TODO(darin): Accept() returning false indicates a malformed message, and
  // that may be good reason to close the connection. However, we don't have a
  // way to do that from here. We should add a way.
  responder_ = nullptr;
}
class Factory_DoStuff2_ProxyToResponder {
 public:
  static Factory::DoStuff2Callback CreateCallback(
      uint64_t request_id,
      bool is_sync,
      std::unique_ptr<mojo::MessageReceiverWithStatus> responder) {
    std::unique_ptr<Factory_DoStuff2_ProxyToResponder> proxy(
        new Factory_DoStuff2_ProxyToResponder(
            request_id, is_sync, std::move(responder)));
    return base::BindOnce(&Factory_DoStuff2_ProxyToResponder::Run,
                          std::move(proxy));
  }

  ~Factory_DoStuff2_ProxyToResponder() {
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
  Factory_DoStuff2_ProxyToResponder(
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
        << "Factory::DoStuff2Callback was destroyed without "
        << "first either being run or its corresponding binding being closed. "
        << "It is an error to drop response callbacks which still correspond "
        << "to an open interface pipe.";
  }
#endif

  void Run(
      const std::string& in_text);

  uint64_t request_id_;
  bool is_sync_;
  std::unique_ptr<mojo::MessageReceiverWithStatus> responder_;

  DISALLOW_COPY_AND_ASSIGN(Factory_DoStuff2_ProxyToResponder);
};
class Factory_DoStuff2_Response_Message
    : public mojo::internal::UnserializedMessageContext {
 public:
  static const mojo::internal::UnserializedMessageContext::Tag kMessageTag;

  explicit Factory_DoStuff2_Response_Message(
      uint32_t message_flags
      , const std::string& param_text
  )
      : mojo::internal::UnserializedMessageContext(
          &kMessageTag,
          internal::kFactory_DoStuff2_Name,
          message_flags)
      , param_text_(std::move(param_text)){}
  ~Factory_DoStuff2_Response_Message() override = default;

  static mojo::Message Build(
      bool serialize,
      bool is_sync,
      const std::string& param_text) {

    
  const uint32_t kFlags = mojo::Message::kFlagIsResponse |
      ((is_sync) ? mojo::Message::kFlagIsSync : 0);

    if (!serialize) {
      return mojo::Message(std::make_unique<Factory_DoStuff2_Response_Message>(
          kFlags
          , std::move(param_text)
          ));
    }

    DCHECK(serialize);
    
  mojo::Message message(
      internal::kFactory_DoStuff2_Name, kFlags, 0, 0, nullptr);
  auto* buffer = message.payload_buffer();
  ::sample::internal::Factory_DoStuff2_ResponseParams_Data::BufferWriter
      params;
  mojo::internal::SerializationContext serialization_context;
  params.Allocate(buffer);
  typename decltype(params->text)::BaseType::BufferWriter
      text_writer;
  mojo::internal::Serialize<mojo::StringDataView>(
      param_text, buffer, &text_writer, &serialization_context);
  params->text.Set(
      text_writer.is_null() ? nullptr : text_writer.data());
  MOJO_INTERNAL_DLOG_SERIALIZATION_WARNING(
      params->text.is_null(),
      mojo::internal::VALIDATION_ERROR_UNEXPECTED_NULL_POINTER,
      "null text in Factory.DoStuff2 response");
  message.AttachHandlesFromSerializationContext(
      &serialization_context);
    return message;
  }


  void Dispatch(Factory::DoStuff2Callback* callback) {
    std::move(*callback).Run(
        std::move(param_text_));
  }



 private:
  // mojo::internal::UnserializedMessageContext:
  void Serialize(mojo::internal::SerializationContext* serialization_context,
                 mojo::internal::Buffer* buffer) override {
    ::sample::internal::Factory_DoStuff2_ResponseParams_Data::BufferWriter
        params;
    params.Allocate(buffer);
  typename decltype(params->text)::BaseType::BufferWriter
      text_writer;
  mojo::internal::Serialize<mojo::StringDataView>(
      param_text_, buffer, &text_writer, serialization_context);
  params->text.Set(
      text_writer.is_null() ? nullptr : text_writer.data());
  MOJO_INTERNAL_DLOG_SERIALIZATION_WARNING(
      params->text.is_null(),
      mojo::internal::VALIDATION_ERROR_UNEXPECTED_NULL_POINTER,
      "null text in Factory.DoStuff2 response");
  }
  std::string param_text_;

  DISALLOW_COPY_AND_ASSIGN(Factory_DoStuff2_Response_Message);
};

const mojo::internal::UnserializedMessageContext::Tag
Factory_DoStuff2_Response_Message::kMessageTag = {};

bool Factory_DoStuff2_ForwardToCallback::Accept(
    mojo::Message* message) {
#if BUILDFLAG(MOJO_TRACE_ENABLED)
  TRACE_EVENT1("mojom", "<class 'jinja2::utils::Namespace'>::Factory::DoStuff2Callback",
               "message", message->name());
#endif
  mojo::internal::MessageDispatchContext dispatch_context(message);
  if (!message->is_serialized()) {
    auto context =
        message->TakeUnserializedContext<Factory_DoStuff2_Response_Message>();
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
  internal::Factory_DoStuff2_ResponseParams_Data* params =
      reinterpret_cast<
          internal::Factory_DoStuff2_ResponseParams_Data*>(
              message->mutable_payload());
  
  mojo::internal::SerializationContext serialization_context;
  serialization_context.TakeHandlesFromMessage(message);
  bool success = true;
  std::string p_text{};
  Factory_DoStuff2_ResponseParamsDataView input_data_view(params, &serialization_context);
  
  if (!input_data_view.ReadText(&p_text))
    success = false;
  if (!success) {
    ReportValidationErrorForMessage(
        message,
        mojo::internal::VALIDATION_ERROR_DESERIALIZATION_FAILED,
        Factory::Name_, 1, true);
    return false;
  }
  if (!callback_.is_null())
    std::move(callback_).Run(
std::move(p_text));
  return true;
}

void Factory_DoStuff2_ProxyToResponder::Run(
    const std::string& in_text) {
  const bool kSerialize = responder_->PrefersSerializedMessages();
  auto message = Factory_DoStuff2_Response_Message::Build(kSerialize, is_sync_, std::move(in_text));

#if BUILDFLAG(MOJO_TRACE_ENABLED)
  TRACE_EVENT1("mojom", "(Impl)<class 'jinja2::utils::Namespace'>::Factory::DoStuff2Callback",
               "message", message.name());
#endif

#if defined(ENABLE_IPC_FUZZER)
  message.set_interface_name(Factory::Name_);
  message.set_method_name("DoStuff2");
#endif

  message.set_request_id(request_id_);
  ignore_result(responder_->Accept(&message));
  // TODO(darin): Accept() returning false indicates a malformed message, and
  // that may be good reason to close the connection. However, we don't have a
  // way to do that from here. We should add a way.
  responder_ = nullptr;
}
class Factory_RequestImportedInterface_ProxyToResponder {
 public:
  static Factory::RequestImportedInterfaceCallback CreateCallback(
      uint64_t request_id,
      bool is_sync,
      std::unique_ptr<mojo::MessageReceiverWithStatus> responder) {
    std::unique_ptr<Factory_RequestImportedInterface_ProxyToResponder> proxy(
        new Factory_RequestImportedInterface_ProxyToResponder(
            request_id, is_sync, std::move(responder)));
    return base::BindOnce(&Factory_RequestImportedInterface_ProxyToResponder::Run,
                          std::move(proxy));
  }

  ~Factory_RequestImportedInterface_ProxyToResponder() {
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
  Factory_RequestImportedInterface_ProxyToResponder(
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
        << "Factory::RequestImportedInterfaceCallback was destroyed without "
        << "first either being run or its corresponding binding being closed. "
        << "It is an error to drop response callbacks which still correspond "
        << "to an open interface pipe.";
  }
#endif

  void Run(
      mojo::PendingReceiver<::imported::ImportedInterface> in_obj);

  uint64_t request_id_;
  bool is_sync_;
  std::unique_ptr<mojo::MessageReceiverWithStatus> responder_;

  DISALLOW_COPY_AND_ASSIGN(Factory_RequestImportedInterface_ProxyToResponder);
};
class Factory_RequestImportedInterface_Response_Message
    : public mojo::internal::UnserializedMessageContext {
 public:
  static const mojo::internal::UnserializedMessageContext::Tag kMessageTag;

  explicit Factory_RequestImportedInterface_Response_Message(
      uint32_t message_flags
      , mojo::PendingReceiver<::imported::ImportedInterface> param_obj
  )
      : mojo::internal::UnserializedMessageContext(
          &kMessageTag,
          internal::kFactory_RequestImportedInterface_Name,
          message_flags)
      , param_obj_(std::move(param_obj)){}
  ~Factory_RequestImportedInterface_Response_Message() override = default;

  static mojo::Message Build(
      bool serialize,
      bool is_sync,
      mojo::PendingReceiver<::imported::ImportedInterface> param_obj) {

    
  const uint32_t kFlags = mojo::Message::kFlagIsResponse |
      ((is_sync) ? mojo::Message::kFlagIsSync : 0);

    if (!serialize) {
      return mojo::Message(std::make_unique<Factory_RequestImportedInterface_Response_Message>(
          kFlags
          , std::move(param_obj)
          ));
    }

    DCHECK(serialize);
    
  mojo::Message message(
      internal::kFactory_RequestImportedInterface_Name, kFlags, 0, 0, nullptr);
  auto* buffer = message.payload_buffer();
  ::sample::internal::Factory_RequestImportedInterface_ResponseParams_Data::BufferWriter
      params;
  mojo::internal::SerializationContext serialization_context;
  params.Allocate(buffer);
  mojo::internal::Serialize<mojo::InterfaceRequestDataView<::imported::ImportedInterfaceInterfaceBase>>(
      param_obj, &params->obj, &serialization_context);
  MOJO_INTERNAL_DLOG_SERIALIZATION_WARNING(
      !mojo::internal::IsHandleOrInterfaceValid(params->obj),
      mojo::internal::VALIDATION_ERROR_UNEXPECTED_INVALID_HANDLE,
      "invalid obj in Factory.RequestImportedInterface response");
  message.AttachHandlesFromSerializationContext(
      &serialization_context);
    return message;
  }


  void Dispatch(Factory::RequestImportedInterfaceCallback* callback) {
    std::move(*callback).Run(
        std::move(param_obj_));
  }



 private:
  // mojo::internal::UnserializedMessageContext:
  void Serialize(mojo::internal::SerializationContext* serialization_context,
                 mojo::internal::Buffer* buffer) override {
    ::sample::internal::Factory_RequestImportedInterface_ResponseParams_Data::BufferWriter
        params;
    params.Allocate(buffer);
  mojo::internal::Serialize<mojo::InterfaceRequestDataView<::imported::ImportedInterfaceInterfaceBase>>(
      param_obj_, &params->obj, serialization_context);
  MOJO_INTERNAL_DLOG_SERIALIZATION_WARNING(
      !mojo::internal::IsHandleOrInterfaceValid(params->obj),
      mojo::internal::VALIDATION_ERROR_UNEXPECTED_INVALID_HANDLE,
      "invalid obj in Factory.RequestImportedInterface response");
  }
  mojo::PendingReceiver<::imported::ImportedInterface> param_obj_;

  DISALLOW_COPY_AND_ASSIGN(Factory_RequestImportedInterface_Response_Message);
};

const mojo::internal::UnserializedMessageContext::Tag
Factory_RequestImportedInterface_Response_Message::kMessageTag = {};

bool Factory_RequestImportedInterface_ForwardToCallback::Accept(
    mojo::Message* message) {
#if BUILDFLAG(MOJO_TRACE_ENABLED)
  TRACE_EVENT1("mojom", "<class 'jinja2::utils::Namespace'>::Factory::RequestImportedInterfaceCallback",
               "message", message->name());
#endif
  mojo::internal::MessageDispatchContext dispatch_context(message);
  if (!message->is_serialized()) {
    auto context =
        message->TakeUnserializedContext<Factory_RequestImportedInterface_Response_Message>();
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
  internal::Factory_RequestImportedInterface_ResponseParams_Data* params =
      reinterpret_cast<
          internal::Factory_RequestImportedInterface_ResponseParams_Data*>(
              message->mutable_payload());
  
  mojo::internal::SerializationContext serialization_context;
  serialization_context.TakeHandlesFromMessage(message);
  bool success = true;
  mojo::PendingReceiver<::imported::ImportedInterface> p_obj{};
  Factory_RequestImportedInterface_ResponseParamsDataView input_data_view(params, &serialization_context);
  
  p_obj =
      input_data_view.TakeObj<decltype(p_obj)>();
  if (!success) {
    ReportValidationErrorForMessage(
        message,
        mojo::internal::VALIDATION_ERROR_DESERIALIZATION_FAILED,
        Factory::Name_, 3, true);
    return false;
  }
  if (!callback_.is_null())
    std::move(callback_).Run(
std::move(p_obj));
  return true;
}

void Factory_RequestImportedInterface_ProxyToResponder::Run(
    mojo::PendingReceiver<::imported::ImportedInterface> in_obj) {
  const bool kSerialize = responder_->PrefersSerializedMessages();
  auto message = Factory_RequestImportedInterface_Response_Message::Build(kSerialize, is_sync_, std::move(in_obj));

#if BUILDFLAG(MOJO_TRACE_ENABLED)
  TRACE_EVENT1("mojom", "(Impl)<class 'jinja2::utils::Namespace'>::Factory::RequestImportedInterfaceCallback",
               "message", message.name());
#endif

#if defined(ENABLE_IPC_FUZZER)
  message.set_interface_name(Factory::Name_);
  message.set_method_name("RequestImportedInterface");
#endif

  message.set_request_id(request_id_);
  ignore_result(responder_->Accept(&message));
  // TODO(darin): Accept() returning false indicates a malformed message, and
  // that may be good reason to close the connection. However, we don't have a
  // way to do that from here. We should add a way.
  responder_ = nullptr;
}
class Factory_TakeImportedInterface_ProxyToResponder {
 public:
  static Factory::TakeImportedInterfaceCallback CreateCallback(
      uint64_t request_id,
      bool is_sync,
      std::unique_ptr<mojo::MessageReceiverWithStatus> responder) {
    std::unique_ptr<Factory_TakeImportedInterface_ProxyToResponder> proxy(
        new Factory_TakeImportedInterface_ProxyToResponder(
            request_id, is_sync, std::move(responder)));
    return base::BindOnce(&Factory_TakeImportedInterface_ProxyToResponder::Run,
                          std::move(proxy));
  }

  ~Factory_TakeImportedInterface_ProxyToResponder() {
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
  Factory_TakeImportedInterface_ProxyToResponder(
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
        << "Factory::TakeImportedInterfaceCallback was destroyed without "
        << "first either being run or its corresponding binding being closed. "
        << "It is an error to drop response callbacks which still correspond "
        << "to an open interface pipe.";
  }
#endif

  void Run(
      mojo::PendingRemote<::imported::ImportedInterface> in_obj);

  uint64_t request_id_;
  bool is_sync_;
  std::unique_ptr<mojo::MessageReceiverWithStatus> responder_;

  DISALLOW_COPY_AND_ASSIGN(Factory_TakeImportedInterface_ProxyToResponder);
};
class Factory_TakeImportedInterface_Response_Message
    : public mojo::internal::UnserializedMessageContext {
 public:
  static const mojo::internal::UnserializedMessageContext::Tag kMessageTag;

  explicit Factory_TakeImportedInterface_Response_Message(
      uint32_t message_flags
      , mojo::PendingRemote<::imported::ImportedInterface> param_obj
  )
      : mojo::internal::UnserializedMessageContext(
          &kMessageTag,
          internal::kFactory_TakeImportedInterface_Name,
          message_flags)
      , param_obj_(std::move(param_obj)){}
  ~Factory_TakeImportedInterface_Response_Message() override = default;

  static mojo::Message Build(
      bool serialize,
      bool is_sync,
      mojo::PendingRemote<::imported::ImportedInterface> param_obj) {

    
  const uint32_t kFlags = mojo::Message::kFlagIsResponse |
      ((is_sync) ? mojo::Message::kFlagIsSync : 0);

    if (!serialize) {
      return mojo::Message(std::make_unique<Factory_TakeImportedInterface_Response_Message>(
          kFlags
          , std::move(param_obj)
          ));
    }

    DCHECK(serialize);
    
  mojo::Message message(
      internal::kFactory_TakeImportedInterface_Name, kFlags, 0, 0, nullptr);
  auto* buffer = message.payload_buffer();
  ::sample::internal::Factory_TakeImportedInterface_ResponseParams_Data::BufferWriter
      params;
  mojo::internal::SerializationContext serialization_context;
  params.Allocate(buffer);
  mojo::internal::Serialize<mojo::InterfacePtrDataView<::imported::ImportedInterfaceInterfaceBase>>(
      param_obj, &params->obj, &serialization_context);
  MOJO_INTERNAL_DLOG_SERIALIZATION_WARNING(
      !mojo::internal::IsHandleOrInterfaceValid(params->obj),
      mojo::internal::VALIDATION_ERROR_UNEXPECTED_INVALID_HANDLE,
      "invalid obj in Factory.TakeImportedInterface response");
  message.AttachHandlesFromSerializationContext(
      &serialization_context);
    return message;
  }


  void Dispatch(Factory::TakeImportedInterfaceCallback* callback) {
    std::move(*callback).Run(
        std::move(param_obj_));
  }



 private:
  // mojo::internal::UnserializedMessageContext:
  void Serialize(mojo::internal::SerializationContext* serialization_context,
                 mojo::internal::Buffer* buffer) override {
    ::sample::internal::Factory_TakeImportedInterface_ResponseParams_Data::BufferWriter
        params;
    params.Allocate(buffer);
  mojo::internal::Serialize<mojo::InterfacePtrDataView<::imported::ImportedInterfaceInterfaceBase>>(
      param_obj_, &params->obj, serialization_context);
  MOJO_INTERNAL_DLOG_SERIALIZATION_WARNING(
      !mojo::internal::IsHandleOrInterfaceValid(params->obj),
      mojo::internal::VALIDATION_ERROR_UNEXPECTED_INVALID_HANDLE,
      "invalid obj in Factory.TakeImportedInterface response");
  }
  mojo::PendingRemote<::imported::ImportedInterface> param_obj_;

  DISALLOW_COPY_AND_ASSIGN(Factory_TakeImportedInterface_Response_Message);
};

const mojo::internal::UnserializedMessageContext::Tag
Factory_TakeImportedInterface_Response_Message::kMessageTag = {};

bool Factory_TakeImportedInterface_ForwardToCallback::Accept(
    mojo::Message* message) {
#if BUILDFLAG(MOJO_TRACE_ENABLED)
  TRACE_EVENT1("mojom", "<class 'jinja2::utils::Namespace'>::Factory::TakeImportedInterfaceCallback",
               "message", message->name());
#endif
  mojo::internal::MessageDispatchContext dispatch_context(message);
  if (!message->is_serialized()) {
    auto context =
        message->TakeUnserializedContext<Factory_TakeImportedInterface_Response_Message>();
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
  internal::Factory_TakeImportedInterface_ResponseParams_Data* params =
      reinterpret_cast<
          internal::Factory_TakeImportedInterface_ResponseParams_Data*>(
              message->mutable_payload());
  
  mojo::internal::SerializationContext serialization_context;
  serialization_context.TakeHandlesFromMessage(message);
  bool success = true;
  mojo::PendingRemote<::imported::ImportedInterface> p_obj{};
  Factory_TakeImportedInterface_ResponseParamsDataView input_data_view(params, &serialization_context);
  
  p_obj =
      input_data_view.TakeObj<decltype(p_obj)>();
  if (!success) {
    ReportValidationErrorForMessage(
        message,
        mojo::internal::VALIDATION_ERROR_DESERIALIZATION_FAILED,
        Factory::Name_, 4, true);
    return false;
  }
  if (!callback_.is_null())
    std::move(callback_).Run(
std::move(p_obj));
  return true;
}

void Factory_TakeImportedInterface_ProxyToResponder::Run(
    mojo::PendingRemote<::imported::ImportedInterface> in_obj) {
  const bool kSerialize = responder_->PrefersSerializedMessages();
  auto message = Factory_TakeImportedInterface_Response_Message::Build(kSerialize, is_sync_, std::move(in_obj));

#if BUILDFLAG(MOJO_TRACE_ENABLED)
  TRACE_EVENT1("mojom", "(Impl)<class 'jinja2::utils::Namespace'>::Factory::TakeImportedInterfaceCallback",
               "message", message.name());
#endif

#if defined(ENABLE_IPC_FUZZER)
  message.set_interface_name(Factory::Name_);
  message.set_method_name("TakeImportedInterface");
#endif

  message.set_request_id(request_id_);
  ignore_result(responder_->Accept(&message));
  // TODO(darin): Accept() returning false indicates a malformed message, and
  // that may be good reason to close the connection. However, we don't have a
  // way to do that from here. We should add a way.
  responder_ = nullptr;
}

// static
bool FactoryStubDispatch::Accept(
    Factory* impl,
    mojo::Message* message) {
  switch (message->header()->name) {
    case internal::kFactory_DoStuff_Name: {
      break;
    }
    case internal::kFactory_DoStuff2_Name: {
      break;
    }
    case internal::kFactory_CreateNamedObject_Name: {
#if BUILDFLAG(MOJO_TRACE_ENABLED)
      TRACE_EVENT1(
          "mojom",
          "(Impl)<class 'jinja2::utils::Namespace'>::Factory::CreateNamedObject",
          "message", message->name());
#endif
      base::TaskAnnotator::ScopedSetIpcProgramCounter scoped_ipc_pc(
          base::GetProgramCounter());
      mojo::internal::MessageDispatchContext context(message);
      if (!message->is_serialized()) {
        auto context = message->TakeUnserializedContext<
            FactoryProxy_CreateNamedObject_Message>();
        if (!context) {
          // The Message was not of the expected type. It may be a valid message
          // which was serialized using a different variant of these bindings.
          // Force serialization before dispatch in this case.
          message->SerializeIfNecessary();
        } else {
          context->Dispatch(impl);
          return true;
        }
      }

      DCHECK(message->is_serialized());
      internal::Factory_CreateNamedObject_Params_Data* params =
          reinterpret_cast<internal::Factory_CreateNamedObject_Params_Data*>(
              message->mutable_payload());
      
      mojo::internal::SerializationContext serialization_context;
      serialization_context.TakeHandlesFromMessage(message);
      bool success = true;
      mojo::PendingReceiver<NamedObject> p_obj{};
      Factory_CreateNamedObject_ParamsDataView input_data_view(params, &serialization_context);
      
      p_obj =
          input_data_view.TakeObj<decltype(p_obj)>();
      if (!success) {
        ReportValidationErrorForMessage(
            message,
            mojo::internal::VALIDATION_ERROR_DESERIALIZATION_FAILED,
            Factory::Name_, 2, false);
        return false;
      }
      // A null |impl| means no implementation was bound.
      DCHECK(impl);
      impl->CreateNamedObject(
std::move(p_obj));
      return true;
    }
    case internal::kFactory_RequestImportedInterface_Name: {
      break;
    }
    case internal::kFactory_TakeImportedInterface_Name: {
      break;
    }
  }
  return false;
}

// static
bool FactoryStubDispatch::AcceptWithResponder(
    Factory* impl,
    mojo::Message* message,
    std::unique_ptr<mojo::MessageReceiverWithStatus> responder) {
  switch (message->header()->name) {
    case internal::kFactory_DoStuff_Name: {
#if BUILDFLAG(MOJO_TRACE_ENABLED)
      TRACE_EVENT1(
          "mojom",
          "(Impl)<class 'jinja2::utils::Namespace'>::Factory::DoStuff",
          "message", message->name());
#endif
      base::TaskAnnotator::ScopedSetIpcProgramCounter scoped_ipc_pc(
          base::GetProgramCounter());
      mojo::internal::MessageDispatchContext context(message);
      if (!message->is_serialized()) {
        auto context = message->TakeUnserializedContext<
            FactoryProxy_DoStuff_Message>();
        if (!context) {
          // The Message was not of the expected type. It may be a valid message
          // which was built using a different variant of these bindings. Force
          // serialization before dispatch in this case.
          message->SerializeIfNecessary();
        } else {
          Factory::DoStuffCallback callback =
              Factory_DoStuff_ProxyToResponder::CreateCallback(
                  message->request_id(),
                  message->has_flag(mojo::Message::kFlagIsSync),
                  std::move(responder));
          context->Dispatch(impl, std::move(callback));
          return true;
        }
      }

      internal::Factory_DoStuff_Params_Data* params =
          reinterpret_cast<
              internal::Factory_DoStuff_Params_Data*>(
                  message->mutable_payload());
      
      mojo::internal::SerializationContext serialization_context;
      serialization_context.TakeHandlesFromMessage(message);
      bool success = true;
      RequestPtr p_request{};
      mojo::ScopedMessagePipeHandle p_pipe{};
      Factory_DoStuff_ParamsDataView input_data_view(params, &serialization_context);
      
      if (!input_data_view.ReadRequest(&p_request))
        success = false;
      p_pipe = input_data_view.TakePipe();
      if (!success) {
        ReportValidationErrorForMessage(
            message,
            mojo::internal::VALIDATION_ERROR_DESERIALIZATION_FAILED,
            Factory::Name_, 0, false);
        return false;
      }
      Factory::DoStuffCallback callback =
          Factory_DoStuff_ProxyToResponder::CreateCallback(
              message->request_id(),
              message->has_flag(mojo::Message::kFlagIsSync),
              std::move(responder));
      // A null |impl| means no implementation was bound.
      DCHECK(impl);
      impl->DoStuff(
std::move(p_request), 
std::move(p_pipe), std::move(callback));
      return true;
    }
    case internal::kFactory_DoStuff2_Name: {
#if BUILDFLAG(MOJO_TRACE_ENABLED)
      TRACE_EVENT1(
          "mojom",
          "(Impl)<class 'jinja2::utils::Namespace'>::Factory::DoStuff2",
          "message", message->name());
#endif
      base::TaskAnnotator::ScopedSetIpcProgramCounter scoped_ipc_pc(
          base::GetProgramCounter());
      mojo::internal::MessageDispatchContext context(message);
      if (!message->is_serialized()) {
        auto context = message->TakeUnserializedContext<
            FactoryProxy_DoStuff2_Message>();
        if (!context) {
          // The Message was not of the expected type. It may be a valid message
          // which was built using a different variant of these bindings. Force
          // serialization before dispatch in this case.
          message->SerializeIfNecessary();
        } else {
          Factory::DoStuff2Callback callback =
              Factory_DoStuff2_ProxyToResponder::CreateCallback(
                  message->request_id(),
                  message->has_flag(mojo::Message::kFlagIsSync),
                  std::move(responder));
          context->Dispatch(impl, std::move(callback));
          return true;
        }
      }

      internal::Factory_DoStuff2_Params_Data* params =
          reinterpret_cast<
              internal::Factory_DoStuff2_Params_Data*>(
                  message->mutable_payload());
      
      mojo::internal::SerializationContext serialization_context;
      serialization_context.TakeHandlesFromMessage(message);
      bool success = true;
      mojo::ScopedDataPipeConsumerHandle p_pipe{};
      Factory_DoStuff2_ParamsDataView input_data_view(params, &serialization_context);
      
      p_pipe = input_data_view.TakePipe();
      if (!success) {
        ReportValidationErrorForMessage(
            message,
            mojo::internal::VALIDATION_ERROR_DESERIALIZATION_FAILED,
            Factory::Name_, 1, false);
        return false;
      }
      Factory::DoStuff2Callback callback =
          Factory_DoStuff2_ProxyToResponder::CreateCallback(
              message->request_id(),
              message->has_flag(mojo::Message::kFlagIsSync),
              std::move(responder));
      // A null |impl| means no implementation was bound.
      DCHECK(impl);
      impl->DoStuff2(
std::move(p_pipe), std::move(callback));
      return true;
    }
    case internal::kFactory_CreateNamedObject_Name: {
      break;
    }
    case internal::kFactory_RequestImportedInterface_Name: {
#if BUILDFLAG(MOJO_TRACE_ENABLED)
      TRACE_EVENT1(
          "mojom",
          "(Impl)<class 'jinja2::utils::Namespace'>::Factory::RequestImportedInterface",
          "message", message->name());
#endif
      base::TaskAnnotator::ScopedSetIpcProgramCounter scoped_ipc_pc(
          base::GetProgramCounter());
      mojo::internal::MessageDispatchContext context(message);
      if (!message->is_serialized()) {
        auto context = message->TakeUnserializedContext<
            FactoryProxy_RequestImportedInterface_Message>();
        if (!context) {
          // The Message was not of the expected type. It may be a valid message
          // which was built using a different variant of these bindings. Force
          // serialization before dispatch in this case.
          message->SerializeIfNecessary();
        } else {
          Factory::RequestImportedInterfaceCallback callback =
              Factory_RequestImportedInterface_ProxyToResponder::CreateCallback(
                  message->request_id(),
                  message->has_flag(mojo::Message::kFlagIsSync),
                  std::move(responder));
          context->Dispatch(impl, std::move(callback));
          return true;
        }
      }

      internal::Factory_RequestImportedInterface_Params_Data* params =
          reinterpret_cast<
              internal::Factory_RequestImportedInterface_Params_Data*>(
                  message->mutable_payload());
      
      mojo::internal::SerializationContext serialization_context;
      serialization_context.TakeHandlesFromMessage(message);
      bool success = true;
      mojo::PendingReceiver<::imported::ImportedInterface> p_obj{};
      Factory_RequestImportedInterface_ParamsDataView input_data_view(params, &serialization_context);
      
      p_obj =
          input_data_view.TakeObj<decltype(p_obj)>();
      if (!success) {
        ReportValidationErrorForMessage(
            message,
            mojo::internal::VALIDATION_ERROR_DESERIALIZATION_FAILED,
            Factory::Name_, 3, false);
        return false;
      }
      Factory::RequestImportedInterfaceCallback callback =
          Factory_RequestImportedInterface_ProxyToResponder::CreateCallback(
              message->request_id(),
              message->has_flag(mojo::Message::kFlagIsSync),
              std::move(responder));
      // A null |impl| means no implementation was bound.
      DCHECK(impl);
      impl->RequestImportedInterface(
std::move(p_obj), std::move(callback));
      return true;
    }
    case internal::kFactory_TakeImportedInterface_Name: {
#if BUILDFLAG(MOJO_TRACE_ENABLED)
      TRACE_EVENT1(
          "mojom",
          "(Impl)<class 'jinja2::utils::Namespace'>::Factory::TakeImportedInterface",
          "message", message->name());
#endif
      base::TaskAnnotator::ScopedSetIpcProgramCounter scoped_ipc_pc(
          base::GetProgramCounter());
      mojo::internal::MessageDispatchContext context(message);
      if (!message->is_serialized()) {
        auto context = message->TakeUnserializedContext<
            FactoryProxy_TakeImportedInterface_Message>();
        if (!context) {
          // The Message was not of the expected type. It may be a valid message
          // which was built using a different variant of these bindings. Force
          // serialization before dispatch in this case.
          message->SerializeIfNecessary();
        } else {
          Factory::TakeImportedInterfaceCallback callback =
              Factory_TakeImportedInterface_ProxyToResponder::CreateCallback(
                  message->request_id(),
                  message->has_flag(mojo::Message::kFlagIsSync),
                  std::move(responder));
          context->Dispatch(impl, std::move(callback));
          return true;
        }
      }

      internal::Factory_TakeImportedInterface_Params_Data* params =
          reinterpret_cast<
              internal::Factory_TakeImportedInterface_Params_Data*>(
                  message->mutable_payload());
      
      mojo::internal::SerializationContext serialization_context;
      serialization_context.TakeHandlesFromMessage(message);
      bool success = true;
      mojo::PendingRemote<::imported::ImportedInterface> p_obj{};
      Factory_TakeImportedInterface_ParamsDataView input_data_view(params, &serialization_context);
      
      p_obj =
          input_data_view.TakeObj<decltype(p_obj)>();
      if (!success) {
        ReportValidationErrorForMessage(
            message,
            mojo::internal::VALIDATION_ERROR_DESERIALIZATION_FAILED,
            Factory::Name_, 4, false);
        return false;
      }
      Factory::TakeImportedInterfaceCallback callback =
          Factory_TakeImportedInterface_ProxyToResponder::CreateCallback(
              message->request_id(),
              message->has_flag(mojo::Message::kFlagIsSync),
              std::move(responder));
      // A null |impl| means no implementation was bound.
      DCHECK(impl);
      impl->TakeImportedInterface(
std::move(p_obj), std::move(callback));
      return true;
    }
  }
  return false;
}

bool FactoryRequestValidator::Accept(mojo::Message* message) {
  if (!message->is_serialized() ||
      mojo::internal::ControlMessageHandler::IsControlMessage(message)) {
    return true;
  }

  mojo::internal::ValidationContext validation_context(
    message->payload(), message->payload_num_bytes(),
    message->handles()->size(), message->payload_num_interface_ids(), message,
    "Factory RequestValidator");

  switch (message->header()->name) {
    case internal::kFactory_DoStuff_Name: {
      if (!mojo::internal::ValidateMessageIsRequestExpectingResponse(
              message, &validation_context)) {
        return false;
      }
      if (!mojo::internal::ValidateMessagePayload<
               internal::Factory_DoStuff_Params_Data>(
                  message, &validation_context)) {
        return false;
      }
      return true;
    }
    case internal::kFactory_DoStuff2_Name: {
      if (!mojo::internal::ValidateMessageIsRequestExpectingResponse(
              message, &validation_context)) {
        return false;
      }
      if (!mojo::internal::ValidateMessagePayload<
               internal::Factory_DoStuff2_Params_Data>(
                  message, &validation_context)) {
        return false;
      }
      return true;
    }
    case internal::kFactory_CreateNamedObject_Name: {
      if (!mojo::internal::ValidateMessageIsRequestWithoutResponse(
              message, &validation_context)) {
        return false;
      }
      if (!mojo::internal::ValidateMessagePayload<
               internal::Factory_CreateNamedObject_Params_Data>(
                  message, &validation_context)) {
        return false;
      }
      return true;
    }
    case internal::kFactory_RequestImportedInterface_Name: {
      if (!mojo::internal::ValidateMessageIsRequestExpectingResponse(
              message, &validation_context)) {
        return false;
      }
      if (!mojo::internal::ValidateMessagePayload<
               internal::Factory_RequestImportedInterface_Params_Data>(
                  message, &validation_context)) {
        return false;
      }
      return true;
    }
    case internal::kFactory_TakeImportedInterface_Name: {
      if (!mojo::internal::ValidateMessageIsRequestExpectingResponse(
              message, &validation_context)) {
        return false;
      }
      if (!mojo::internal::ValidateMessagePayload<
               internal::Factory_TakeImportedInterface_Params_Data>(
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

bool FactoryResponseValidator::Accept(mojo::Message* message) {
  if (!message->is_serialized() ||
      mojo::internal::ControlMessageHandler::IsControlMessage(message)) {
    return true;
  }

  mojo::internal::ValidationContext validation_context(
    message->payload(), message->payload_num_bytes(),
    message->handles()->size(), message->payload_num_interface_ids(), message,
    "Factory ResponseValidator");

  if (!mojo::internal::ValidateMessageIsResponse(message, &validation_context))
    return false;
  switch (message->header()->name) {
    case internal::kFactory_DoStuff_Name: {
      if (!mojo::internal::ValidateMessagePayload<
               internal::Factory_DoStuff_ResponseParams_Data>(
                    message, &validation_context)) {
        return false;
      }
      return true;
    }
    case internal::kFactory_DoStuff2_Name: {
      if (!mojo::internal::ValidateMessagePayload<
               internal::Factory_DoStuff2_ResponseParams_Data>(
                    message, &validation_context)) {
        return false;
      }
      return true;
    }
    case internal::kFactory_RequestImportedInterface_Name: {
      if (!mojo::internal::ValidateMessagePayload<
               internal::Factory_RequestImportedInterface_ResponseParams_Data>(
                    message, &validation_context)) {
        return false;
      }
      return true;
    }
    case internal::kFactory_TakeImportedInterface_Name: {
      if (!mojo::internal::ValidateMessagePayload<
               internal::Factory_TakeImportedInterface_ResponseParams_Data>(
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
}  // namespace sample

namespace mojo {


// static
bool StructTraits<::sample::Request::DataView, ::sample::RequestPtr>::Read(
    ::sample::Request::DataView input,
    ::sample::RequestPtr* output) {
  bool success = true;
  ::sample::RequestPtr result(::sample::Request::New());
  
      result->x = input.x();
      result->pipe = input.TakePipe();
      if (!input.ReadMorePipes(&result->more_pipes))
        success = false;
      result->obj =
          input.TakeObj<decltype(result->obj)>();
  *output = std::move(result);
  return success;
}


// static
bool StructTraits<::sample::Response::DataView, ::sample::ResponsePtr>::Read(
    ::sample::Response::DataView input,
    ::sample::ResponsePtr* output) {
  bool success = true;
  ::sample::ResponsePtr result(::sample::Response::New());
  
      result->x = input.x();
      result->pipe = input.TakePipe();
  *output = std::move(result);
  return success;
}

}  // namespace mojo

#if defined(__clang__)
#pragma clang diagnostic pop
#elif defined(_MSC_VER)
#pragma warning(pop)
#endif