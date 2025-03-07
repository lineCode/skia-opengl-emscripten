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

#include "mojo/public/interfaces/bindings/tests/ping_service.mojom-blink.h"

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

#include "mojo/public/interfaces/bindings/tests/ping_service.mojom-params-data.h"
#include "mojo/public/interfaces/bindings/tests/ping_service.mojom-shared-message-ids.h"

#include "mojo/public/interfaces/bindings/tests/ping_service.mojom-blink-import-headers.h"
#include "mojo/public/cpp/bindings/lib/wtf_serialization.h"


#ifndef MOJO_PUBLIC_INTERFACES_BINDINGS_TESTS_PING_SERVICE_MOJOM_BLINK_JUMBO_H_
#define MOJO_PUBLIC_INTERFACES_BINDINGS_TESTS_PING_SERVICE_MOJOM_BLINK_JUMBO_H_
#endif
namespace mojo {
namespace test {
namespace blink {
const char PingService::Name_[] = "mojo.test.PingService";
bool PingService::Ping() {
  NOTREACHED();
  return false;
}
class PingService_Ping_HandleSyncResponse
    : public mojo::MessageReceiver {
 public:
  PingService_Ping_HandleSyncResponse(
      bool* result)
      : result_(result) {
    DCHECK(!*result_);
  }
  bool Accept(mojo::Message* message) override;
 private:
  bool* result_;DISALLOW_COPY_AND_ASSIGN(PingService_Ping_HandleSyncResponse);
};

class PingService_Ping_ForwardToCallback
    : public mojo::MessageReceiver {
 public:
  PingService_Ping_ForwardToCallback(
      PingService::PingCallback callback
      ) : callback_(std::move(callback)) {
  }
  bool Accept(mojo::Message* message) override;
 private:
  PingService::PingCallback callback_;
  DISALLOW_COPY_AND_ASSIGN(PingService_Ping_ForwardToCallback);
};

PingServiceProxy::PingServiceProxy(mojo::MessageReceiverWithResponder* receiver)
    : receiver_(receiver) {
}
class PingServiceProxy_Ping_Message
    : public mojo::internal::UnserializedMessageContext {
 public:
  static const mojo::internal::UnserializedMessageContext::Tag kMessageTag;

  explicit PingServiceProxy_Ping_Message(
      uint32_t message_flags
  )
      : mojo::internal::UnserializedMessageContext(
          &kMessageTag,
          internal::kPingService_Ping_Name,
          message_flags){}
  ~PingServiceProxy_Ping_Message() override = default;

  static mojo::Message Build(
      bool serialize,
      bool expects_response,
      bool is_sync) {

    
  const uint32_t kFlags =
      ((expects_response) ? mojo::Message::kFlagExpectsResponse : 0) |
      ((is_sync) ? mojo::Message::kFlagIsSync : 0);

    if (!serialize) {
      return mojo::Message(std::make_unique<PingServiceProxy_Ping_Message>(
          kFlags
          ));
    }

    DCHECK(serialize);
    
  mojo::Message message(
      internal::kPingService_Ping_Name, kFlags, 0, 0, nullptr);
  auto* buffer = message.payload_buffer();
  ::mojo::test::internal::PingService_Ping_Params_Data::BufferWriter
      params;
  mojo::internal::SerializationContext serialization_context;
  params.Allocate(buffer);
  message.AttachHandlesFromSerializationContext(
      &serialization_context);
    return message;
  }


  void Dispatch(PingService* impl, PingService::PingCallback callback) {
    impl->Ping(std::move(callback));
  }

 private:
  // mojo::internal::UnserializedMessageContext:
  void Serialize(mojo::internal::SerializationContext* serialization_context,
                 mojo::internal::Buffer* buffer) override {
    ::mojo::test::internal::PingService_Ping_Params_Data::BufferWriter
        params;
    params.Allocate(buffer);
  }

  DISALLOW_COPY_AND_ASSIGN(PingServiceProxy_Ping_Message);
};

const mojo::internal::UnserializedMessageContext::Tag
PingServiceProxy_Ping_Message::kMessageTag = {};
bool PingServiceProxy::Ping(
    ) {
#if BUILDFLAG(MOJO_TRACE_ENABLED)
  TRACE_EVENT0("mojom", "<class 'jinja2::utils::Namespace'>::PingService::Ping");
#endif
  const bool kExpectsResponse = true;
  const bool kIsSync = true;
  const bool kSerialize = receiver_->PrefersSerializedMessages();
  auto message = PingServiceProxy_Ping_Message::Build(
      kSerialize, kExpectsResponse, kIsSync);

#if defined(ENABLE_IPC_FUZZER)
  message.set_interface_name(PingService::Name_);
  message.set_method_name("Ping");
#endif

  bool result = false;
  std::unique_ptr<mojo::MessageReceiver> responder(
      new PingService_Ping_HandleSyncResponse(
          &result));
  ignore_result(receiver_->AcceptWithResponder(&message, std::move(responder)));
  return result;
}

void PingServiceProxy::Ping(
    PingCallback callback) {
#if BUILDFLAG(MOJO_TRACE_ENABLED)
  TRACE_EVENT0("mojom", "<class 'jinja2::utils::Namespace'>::PingService::Ping");
#endif
  const bool kExpectsResponse = true;
  const bool kIsSync = false;
  const bool kSerialize = receiver_->PrefersSerializedMessages();
  auto message = PingServiceProxy_Ping_Message::Build(
      kSerialize, kExpectsResponse, kIsSync);

#if defined(ENABLE_IPC_FUZZER)
  message.set_interface_name(PingService::Name_);
  message.set_method_name("Ping");
#endif
  std::unique_ptr<mojo::MessageReceiver> responder(
      new PingService_Ping_ForwardToCallback(
          std::move(callback)));
  ignore_result(receiver_->AcceptWithResponder(&message, std::move(responder)));
}
class PingService_Ping_ProxyToResponder {
 public:
  static PingService::PingCallback CreateCallback(
      uint64_t request_id,
      bool is_sync,
      std::unique_ptr<mojo::MessageReceiverWithStatus> responder) {
    std::unique_ptr<PingService_Ping_ProxyToResponder> proxy(
        new PingService_Ping_ProxyToResponder(
            request_id, is_sync, std::move(responder)));
    return base::BindOnce(&PingService_Ping_ProxyToResponder::Run,
                          std::move(proxy));
  }

  ~PingService_Ping_ProxyToResponder() {
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
  PingService_Ping_ProxyToResponder(
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
        << "PingService::PingCallback was destroyed without "
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

  DISALLOW_COPY_AND_ASSIGN(PingService_Ping_ProxyToResponder);
};
class PingService_Ping_Response_Message
    : public mojo::internal::UnserializedMessageContext {
 public:
  static const mojo::internal::UnserializedMessageContext::Tag kMessageTag;

  explicit PingService_Ping_Response_Message(
      uint32_t message_flags
  )
      : mojo::internal::UnserializedMessageContext(
          &kMessageTag,
          internal::kPingService_Ping_Name,
          message_flags){}
  ~PingService_Ping_Response_Message() override = default;

  static mojo::Message Build(
      bool serialize,
      bool is_sync) {

    
  const uint32_t kFlags = mojo::Message::kFlagIsResponse |
      ((is_sync) ? mojo::Message::kFlagIsSync : 0);

    if (!serialize) {
      return mojo::Message(std::make_unique<PingService_Ping_Response_Message>(
          kFlags
          ));
    }

    DCHECK(serialize);
    
  mojo::Message message(
      internal::kPingService_Ping_Name, kFlags, 0, 0, nullptr);
  auto* buffer = message.payload_buffer();
  ::mojo::test::internal::PingService_Ping_ResponseParams_Data::BufferWriter
      params;
  mojo::internal::SerializationContext serialization_context;
  params.Allocate(buffer);
  message.AttachHandlesFromSerializationContext(
      &serialization_context);
    return message;
  }


  void Dispatch(PingService::PingCallback* callback) {
    std::move(*callback).Run();
  }


  void HandleSyncResponse(
) {

  }

 private:
  // mojo::internal::UnserializedMessageContext:
  void Serialize(mojo::internal::SerializationContext* serialization_context,
                 mojo::internal::Buffer* buffer) override {
    ::mojo::test::internal::PingService_Ping_ResponseParams_Data::BufferWriter
        params;
    params.Allocate(buffer);
  }

  DISALLOW_COPY_AND_ASSIGN(PingService_Ping_Response_Message);
};

const mojo::internal::UnserializedMessageContext::Tag
PingService_Ping_Response_Message::kMessageTag = {};

bool PingService_Ping_ForwardToCallback::Accept(
    mojo::Message* message) {
#if BUILDFLAG(MOJO_TRACE_ENABLED)
  TRACE_EVENT1("mojom", "<class 'jinja2::utils::Namespace'>::PingService::PingCallback",
               "message", message->name());
#endif
  mojo::internal::MessageDispatchContext dispatch_context(message);
  if (!message->is_serialized()) {
    auto context =
        message->TakeUnserializedContext<PingService_Ping_Response_Message>();
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
  internal::PingService_Ping_ResponseParams_Data* params =
      reinterpret_cast<
          internal::PingService_Ping_ResponseParams_Data*>(
              message->mutable_payload());
  
  mojo::internal::SerializationContext serialization_context;
  serialization_context.TakeHandlesFromMessage(message);
  bool success = true;
  PingService_Ping_ResponseParamsDataView input_data_view(params, &serialization_context);
  
  if (!success) {
    ReportValidationErrorForMessage(
        message,
        mojo::internal::VALIDATION_ERROR_DESERIALIZATION_FAILED,
        PingService::Name_, 0, true);
    return false;
  }
  if (!callback_.is_null())
    std::move(callback_).Run();
  return true;
}

void PingService_Ping_ProxyToResponder::Run(
    ) {
  const bool kSerialize = responder_->PrefersSerializedMessages();
  auto message = PingService_Ping_Response_Message::Build(kSerialize, is_sync_);

#if BUILDFLAG(MOJO_TRACE_ENABLED)
  TRACE_EVENT1("mojom", "(Impl)<class 'jinja2::utils::Namespace'>::PingService::PingCallback",
               "message", message.name());
#endif

#if defined(ENABLE_IPC_FUZZER)
  message.set_interface_name(PingService::Name_);
  message.set_method_name("Ping");
#endif

  message.set_request_id(request_id_);
  ignore_result(responder_->Accept(&message));
  // TODO(darin): Accept() returning false indicates a malformed message, and
  // that may be good reason to close the connection. However, we don't have a
  // way to do that from here. We should add a way.
  responder_ = nullptr;
}
bool PingService_Ping_HandleSyncResponse::Accept(
    mojo::Message* message) {
  if (!message->is_serialized()) {
    auto context =
        message->TakeUnserializedContext<PingService_Ping_Response_Message>();
    if (!context) {
      // The Message was not of the expected type. It may be a valid message
      // which was built using a different variant of these bindings. Force
      // serialization before dispatch in this case.
      message->SerializeIfNecessary();
    } else {
      context->HandleSyncResponse();
      *result_ = true;
      mojo::internal::SyncMessageResponseSetup::SetCurrentSyncResponseMessage(
          message);
      return true;
    }
  }

  DCHECK(message->is_serialized());
  internal::PingService_Ping_ResponseParams_Data* params =
      reinterpret_cast<internal::PingService_Ping_ResponseParams_Data*>(
          message->mutable_payload());
  
  mojo::internal::SerializationContext serialization_context;
  serialization_context.TakeHandlesFromMessage(message);
  bool success = true;
  PingService_Ping_ResponseParamsDataView input_data_view(params, &serialization_context);
  
  if (!success) {
    ReportValidationErrorForMessage(
        message,
        mojo::internal::VALIDATION_ERROR_DESERIALIZATION_FAILED,
        PingService::Name_, 0, true);
    return false;
  }
  mojo::internal::SyncMessageResponseSetup::SetCurrentSyncResponseMessage(
      message);
  *result_ = true;
  return true;
}

// static
bool PingServiceStubDispatch::Accept(
    PingService* impl,
    mojo::Message* message) {
  switch (message->header()->name) {
    case internal::kPingService_Ping_Name: {
      break;
    }
  }
  return false;
}

// static
bool PingServiceStubDispatch::AcceptWithResponder(
    PingService* impl,
    mojo::Message* message,
    std::unique_ptr<mojo::MessageReceiverWithStatus> responder) {
  switch (message->header()->name) {
    case internal::kPingService_Ping_Name: {
#if BUILDFLAG(MOJO_TRACE_ENABLED)
      TRACE_EVENT1(
          "mojom",
          "(Impl)<class 'jinja2::utils::Namespace'>::PingService::Ping",
          "message", message->name());
#endif
      base::TaskAnnotator::ScopedSetIpcProgramCounter scoped_ipc_pc(
          base::GetProgramCounter());
      mojo::internal::MessageDispatchContext context(message);
      if (!message->is_serialized()) {
        auto context = message->TakeUnserializedContext<
            PingServiceProxy_Ping_Message>();
        if (!context) {
          // The Message was not of the expected type. It may be a valid message
          // which was built using a different variant of these bindings. Force
          // serialization before dispatch in this case.
          message->SerializeIfNecessary();
        } else {
          PingService::PingCallback callback =
              PingService_Ping_ProxyToResponder::CreateCallback(
                  message->request_id(),
                  message->has_flag(mojo::Message::kFlagIsSync),
                  std::move(responder));
          context->Dispatch(impl, std::move(callback));
          return true;
        }
      }

      internal::PingService_Ping_Params_Data* params =
          reinterpret_cast<
              internal::PingService_Ping_Params_Data*>(
                  message->mutable_payload());
      
      mojo::internal::SerializationContext serialization_context;
      serialization_context.TakeHandlesFromMessage(message);
      bool success = true;
      PingService_Ping_ParamsDataView input_data_view(params, &serialization_context);
      
      if (!success) {
        ReportValidationErrorForMessage(
            message,
            mojo::internal::VALIDATION_ERROR_DESERIALIZATION_FAILED,
            PingService::Name_, 0, false);
        return false;
      }
      PingService::PingCallback callback =
          PingService_Ping_ProxyToResponder::CreateCallback(
              message->request_id(),
              message->has_flag(mojo::Message::kFlagIsSync),
              std::move(responder));
      // A null |impl| means no implementation was bound.
      DCHECK(impl);
      impl->Ping(std::move(callback));
      return true;
    }
  }
  return false;
}

bool PingServiceRequestValidator::Accept(mojo::Message* message) {
  if (!message->is_serialized() ||
      mojo::internal::ControlMessageHandler::IsControlMessage(message)) {
    return true;
  }

  mojo::internal::ValidationContext validation_context(
    message->payload(), message->payload_num_bytes(),
    message->handles()->size(), message->payload_num_interface_ids(), message,
    "PingService RequestValidator");

  switch (message->header()->name) {
    case internal::kPingService_Ping_Name: {
      if (!mojo::internal::ValidateMessageIsRequestExpectingResponse(
              message, &validation_context)) {
        return false;
      }
      if (!mojo::internal::ValidateMessagePayload<
               internal::PingService_Ping_Params_Data>(
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

bool PingServiceResponseValidator::Accept(mojo::Message* message) {
  if (!message->is_serialized() ||
      mojo::internal::ControlMessageHandler::IsControlMessage(message)) {
    return true;
  }

  mojo::internal::ValidationContext validation_context(
    message->payload(), message->payload_num_bytes(),
    message->handles()->size(), message->payload_num_interface_ids(), message,
    "PingService ResponseValidator");

  if (!mojo::internal::ValidateMessageIsResponse(message, &validation_context))
    return false;
  switch (message->header()->name) {
    case internal::kPingService_Ping_Name: {
      if (!mojo::internal::ValidateMessagePayload<
               internal::PingService_Ping_ResponseParams_Data>(
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
const char EchoService::Name_[] = "mojo.test.EchoService";

class EchoService_Echo_ForwardToCallback
    : public mojo::MessageReceiver {
 public:
  EchoService_Echo_ForwardToCallback(
      EchoService::EchoCallback callback
      ) : callback_(std::move(callback)) {
  }
  bool Accept(mojo::Message* message) override;
 private:
  EchoService::EchoCallback callback_;
  DISALLOW_COPY_AND_ASSIGN(EchoService_Echo_ForwardToCallback);
};

EchoServiceProxy::EchoServiceProxy(mojo::MessageReceiverWithResponder* receiver)
    : receiver_(receiver) {
}
class EchoServiceProxy_Echo_Message
    : public mojo::internal::UnserializedMessageContext {
 public:
  static const mojo::internal::UnserializedMessageContext::Tag kMessageTag;

  explicit EchoServiceProxy_Echo_Message(
      uint32_t message_flags
      , const WTF::String& param_test_data
  )
      : mojo::internal::UnserializedMessageContext(
          &kMessageTag,
          internal::kEchoService_Echo_Name,
          message_flags)
      , param_test_data_(std::move(param_test_data)){}
  ~EchoServiceProxy_Echo_Message() override = default;

  static mojo::Message Build(
      bool serialize,
      bool expects_response,
      bool is_sync,
      const WTF::String& param_test_data) {

    
  const uint32_t kFlags =
      ((expects_response) ? mojo::Message::kFlagExpectsResponse : 0) |
      ((is_sync) ? mojo::Message::kFlagIsSync : 0);

    if (!serialize) {
      return mojo::Message(std::make_unique<EchoServiceProxy_Echo_Message>(
          kFlags
          , std::move(param_test_data)
          ));
    }

    DCHECK(serialize);
    
  mojo::Message message(
      internal::kEchoService_Echo_Name, kFlags, 0, 0, nullptr);
  auto* buffer = message.payload_buffer();
  ::mojo::test::internal::EchoService_Echo_Params_Data::BufferWriter
      params;
  mojo::internal::SerializationContext serialization_context;
  params.Allocate(buffer);
  typename decltype(params->test_data)::BaseType::BufferWriter
      test_data_writer;
  mojo::internal::Serialize<mojo::StringDataView>(
      param_test_data, buffer, &test_data_writer, &serialization_context);
  params->test_data.Set(
      test_data_writer.is_null() ? nullptr : test_data_writer.data());
  MOJO_INTERNAL_DLOG_SERIALIZATION_WARNING(
      params->test_data.is_null(),
      mojo::internal::VALIDATION_ERROR_UNEXPECTED_NULL_POINTER,
      "null test_data in EchoService.Echo request");
  message.AttachHandlesFromSerializationContext(
      &serialization_context);
    return message;
  }


  void Dispatch(EchoService* impl, EchoService::EchoCallback callback) {
    impl->Echo(
        std::move(param_test_data_), std::move(callback));
  }

 private:
  // mojo::internal::UnserializedMessageContext:
  void Serialize(mojo::internal::SerializationContext* serialization_context,
                 mojo::internal::Buffer* buffer) override {
    ::mojo::test::internal::EchoService_Echo_Params_Data::BufferWriter
        params;
    params.Allocate(buffer);
  typename decltype(params->test_data)::BaseType::BufferWriter
      test_data_writer;
  mojo::internal::Serialize<mojo::StringDataView>(
      param_test_data_, buffer, &test_data_writer, serialization_context);
  params->test_data.Set(
      test_data_writer.is_null() ? nullptr : test_data_writer.data());
  MOJO_INTERNAL_DLOG_SERIALIZATION_WARNING(
      params->test_data.is_null(),
      mojo::internal::VALIDATION_ERROR_UNEXPECTED_NULL_POINTER,
      "null test_data in EchoService.Echo request");
  }
  WTF::String param_test_data_;

  DISALLOW_COPY_AND_ASSIGN(EchoServiceProxy_Echo_Message);
};

const mojo::internal::UnserializedMessageContext::Tag
EchoServiceProxy_Echo_Message::kMessageTag = {};

void EchoServiceProxy::Echo(
    const WTF::String& in_test_data, EchoCallback callback) {
#if BUILDFLAG(MOJO_TRACE_ENABLED)
  TRACE_EVENT0("mojom", "<class 'jinja2::utils::Namespace'>::EchoService::Echo");
#endif
  const bool kExpectsResponse = true;
  const bool kIsSync = false;
  const bool kSerialize = receiver_->PrefersSerializedMessages();
  auto message = EchoServiceProxy_Echo_Message::Build(
      kSerialize, kExpectsResponse, kIsSync, std::move(in_test_data));

#if defined(ENABLE_IPC_FUZZER)
  message.set_interface_name(EchoService::Name_);
  message.set_method_name("Echo");
#endif
  std::unique_ptr<mojo::MessageReceiver> responder(
      new EchoService_Echo_ForwardToCallback(
          std::move(callback)));
  ignore_result(receiver_->AcceptWithResponder(&message, std::move(responder)));
}
class EchoService_Echo_ProxyToResponder {
 public:
  static EchoService::EchoCallback CreateCallback(
      uint64_t request_id,
      bool is_sync,
      std::unique_ptr<mojo::MessageReceiverWithStatus> responder) {
    std::unique_ptr<EchoService_Echo_ProxyToResponder> proxy(
        new EchoService_Echo_ProxyToResponder(
            request_id, is_sync, std::move(responder)));
    return base::BindOnce(&EchoService_Echo_ProxyToResponder::Run,
                          std::move(proxy));
  }

  ~EchoService_Echo_ProxyToResponder() {
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
  EchoService_Echo_ProxyToResponder(
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
        << "EchoService::EchoCallback was destroyed without "
        << "first either being run or its corresponding binding being closed. "
        << "It is an error to drop response callbacks which still correspond "
        << "to an open interface pipe.";
  }
#endif

  void Run(
      const WTF::String& in_echo_data);

  uint64_t request_id_;
  bool is_sync_;
  std::unique_ptr<mojo::MessageReceiverWithStatus> responder_;

  DISALLOW_COPY_AND_ASSIGN(EchoService_Echo_ProxyToResponder);
};
class EchoService_Echo_Response_Message
    : public mojo::internal::UnserializedMessageContext {
 public:
  static const mojo::internal::UnserializedMessageContext::Tag kMessageTag;

  explicit EchoService_Echo_Response_Message(
      uint32_t message_flags
      , const WTF::String& param_echo_data
  )
      : mojo::internal::UnserializedMessageContext(
          &kMessageTag,
          internal::kEchoService_Echo_Name,
          message_flags)
      , param_echo_data_(std::move(param_echo_data)){}
  ~EchoService_Echo_Response_Message() override = default;

  static mojo::Message Build(
      bool serialize,
      bool is_sync,
      const WTF::String& param_echo_data) {

    
  const uint32_t kFlags = mojo::Message::kFlagIsResponse |
      ((is_sync) ? mojo::Message::kFlagIsSync : 0);

    if (!serialize) {
      return mojo::Message(std::make_unique<EchoService_Echo_Response_Message>(
          kFlags
          , std::move(param_echo_data)
          ));
    }

    DCHECK(serialize);
    
  mojo::Message message(
      internal::kEchoService_Echo_Name, kFlags, 0, 0, nullptr);
  auto* buffer = message.payload_buffer();
  ::mojo::test::internal::EchoService_Echo_ResponseParams_Data::BufferWriter
      params;
  mojo::internal::SerializationContext serialization_context;
  params.Allocate(buffer);
  typename decltype(params->echo_data)::BaseType::BufferWriter
      echo_data_writer;
  mojo::internal::Serialize<mojo::StringDataView>(
      param_echo_data, buffer, &echo_data_writer, &serialization_context);
  params->echo_data.Set(
      echo_data_writer.is_null() ? nullptr : echo_data_writer.data());
  MOJO_INTERNAL_DLOG_SERIALIZATION_WARNING(
      params->echo_data.is_null(),
      mojo::internal::VALIDATION_ERROR_UNEXPECTED_NULL_POINTER,
      "null echo_data in EchoService.Echo response");
  message.AttachHandlesFromSerializationContext(
      &serialization_context);
    return message;
  }


  void Dispatch(EchoService::EchoCallback* callback) {
    std::move(*callback).Run(
        std::move(param_echo_data_));
  }



 private:
  // mojo::internal::UnserializedMessageContext:
  void Serialize(mojo::internal::SerializationContext* serialization_context,
                 mojo::internal::Buffer* buffer) override {
    ::mojo::test::internal::EchoService_Echo_ResponseParams_Data::BufferWriter
        params;
    params.Allocate(buffer);
  typename decltype(params->echo_data)::BaseType::BufferWriter
      echo_data_writer;
  mojo::internal::Serialize<mojo::StringDataView>(
      param_echo_data_, buffer, &echo_data_writer, serialization_context);
  params->echo_data.Set(
      echo_data_writer.is_null() ? nullptr : echo_data_writer.data());
  MOJO_INTERNAL_DLOG_SERIALIZATION_WARNING(
      params->echo_data.is_null(),
      mojo::internal::VALIDATION_ERROR_UNEXPECTED_NULL_POINTER,
      "null echo_data in EchoService.Echo response");
  }
  WTF::String param_echo_data_;

  DISALLOW_COPY_AND_ASSIGN(EchoService_Echo_Response_Message);
};

const mojo::internal::UnserializedMessageContext::Tag
EchoService_Echo_Response_Message::kMessageTag = {};

bool EchoService_Echo_ForwardToCallback::Accept(
    mojo::Message* message) {
#if BUILDFLAG(MOJO_TRACE_ENABLED)
  TRACE_EVENT1("mojom", "<class 'jinja2::utils::Namespace'>::EchoService::EchoCallback",
               "message", message->name());
#endif
  mojo::internal::MessageDispatchContext dispatch_context(message);
  if (!message->is_serialized()) {
    auto context =
        message->TakeUnserializedContext<EchoService_Echo_Response_Message>();
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
  internal::EchoService_Echo_ResponseParams_Data* params =
      reinterpret_cast<
          internal::EchoService_Echo_ResponseParams_Data*>(
              message->mutable_payload());
  
  mojo::internal::SerializationContext serialization_context;
  serialization_context.TakeHandlesFromMessage(message);
  bool success = true;
  WTF::String p_echo_data{};
  EchoService_Echo_ResponseParamsDataView input_data_view(params, &serialization_context);
  
  if (!input_data_view.ReadEchoData(&p_echo_data))
    success = false;
  if (!success) {
    ReportValidationErrorForMessage(
        message,
        mojo::internal::VALIDATION_ERROR_DESERIALIZATION_FAILED,
        EchoService::Name_, 0, true);
    return false;
  }
  if (!callback_.is_null())
    std::move(callback_).Run(
std::move(p_echo_data));
  return true;
}

void EchoService_Echo_ProxyToResponder::Run(
    const WTF::String& in_echo_data) {
  const bool kSerialize = responder_->PrefersSerializedMessages();
  auto message = EchoService_Echo_Response_Message::Build(kSerialize, is_sync_, std::move(in_echo_data));

#if BUILDFLAG(MOJO_TRACE_ENABLED)
  TRACE_EVENT1("mojom", "(Impl)<class 'jinja2::utils::Namespace'>::EchoService::EchoCallback",
               "message", message.name());
#endif

#if defined(ENABLE_IPC_FUZZER)
  message.set_interface_name(EchoService::Name_);
  message.set_method_name("Echo");
#endif

  message.set_request_id(request_id_);
  ignore_result(responder_->Accept(&message));
  // TODO(darin): Accept() returning false indicates a malformed message, and
  // that may be good reason to close the connection. However, we don't have a
  // way to do that from here. We should add a way.
  responder_ = nullptr;
}

// static
bool EchoServiceStubDispatch::Accept(
    EchoService* impl,
    mojo::Message* message) {
  switch (message->header()->name) {
    case internal::kEchoService_Echo_Name: {
      break;
    }
  }
  return false;
}

// static
bool EchoServiceStubDispatch::AcceptWithResponder(
    EchoService* impl,
    mojo::Message* message,
    std::unique_ptr<mojo::MessageReceiverWithStatus> responder) {
  switch (message->header()->name) {
    case internal::kEchoService_Echo_Name: {
#if BUILDFLAG(MOJO_TRACE_ENABLED)
      TRACE_EVENT1(
          "mojom",
          "(Impl)<class 'jinja2::utils::Namespace'>::EchoService::Echo",
          "message", message->name());
#endif
      base::TaskAnnotator::ScopedSetIpcProgramCounter scoped_ipc_pc(
          base::GetProgramCounter());
      mojo::internal::MessageDispatchContext context(message);
      if (!message->is_serialized()) {
        auto context = message->TakeUnserializedContext<
            EchoServiceProxy_Echo_Message>();
        if (!context) {
          // The Message was not of the expected type. It may be a valid message
          // which was built using a different variant of these bindings. Force
          // serialization before dispatch in this case.
          message->SerializeIfNecessary();
        } else {
          EchoService::EchoCallback callback =
              EchoService_Echo_ProxyToResponder::CreateCallback(
                  message->request_id(),
                  message->has_flag(mojo::Message::kFlagIsSync),
                  std::move(responder));
          context->Dispatch(impl, std::move(callback));
          return true;
        }
      }

      internal::EchoService_Echo_Params_Data* params =
          reinterpret_cast<
              internal::EchoService_Echo_Params_Data*>(
                  message->mutable_payload());
      
      mojo::internal::SerializationContext serialization_context;
      serialization_context.TakeHandlesFromMessage(message);
      bool success = true;
      WTF::String p_test_data{};
      EchoService_Echo_ParamsDataView input_data_view(params, &serialization_context);
      
      if (!input_data_view.ReadTestData(&p_test_data))
        success = false;
      if (!success) {
        ReportValidationErrorForMessage(
            message,
            mojo::internal::VALIDATION_ERROR_DESERIALIZATION_FAILED,
            EchoService::Name_, 0, false);
        return false;
      }
      EchoService::EchoCallback callback =
          EchoService_Echo_ProxyToResponder::CreateCallback(
              message->request_id(),
              message->has_flag(mojo::Message::kFlagIsSync),
              std::move(responder));
      // A null |impl| means no implementation was bound.
      DCHECK(impl);
      impl->Echo(
std::move(p_test_data), std::move(callback));
      return true;
    }
  }
  return false;
}

bool EchoServiceRequestValidator::Accept(mojo::Message* message) {
  if (!message->is_serialized() ||
      mojo::internal::ControlMessageHandler::IsControlMessage(message)) {
    return true;
  }

  mojo::internal::ValidationContext validation_context(
    message->payload(), message->payload_num_bytes(),
    message->handles()->size(), message->payload_num_interface_ids(), message,
    "EchoService RequestValidator");

  switch (message->header()->name) {
    case internal::kEchoService_Echo_Name: {
      if (!mojo::internal::ValidateMessageIsRequestExpectingResponse(
              message, &validation_context)) {
        return false;
      }
      if (!mojo::internal::ValidateMessagePayload<
               internal::EchoService_Echo_Params_Data>(
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

bool EchoServiceResponseValidator::Accept(mojo::Message* message) {
  if (!message->is_serialized() ||
      mojo::internal::ControlMessageHandler::IsControlMessage(message)) {
    return true;
  }

  mojo::internal::ValidationContext validation_context(
    message->payload(), message->payload_num_bytes(),
    message->handles()->size(), message->payload_num_interface_ids(), message,
    "EchoService ResponseValidator");

  if (!mojo::internal::ValidateMessageIsResponse(message, &validation_context))
    return false;
  switch (message->header()->name) {
    case internal::kEchoService_Echo_Name: {
      if (!mojo::internal::ValidateMessagePayload<
               internal::EchoService_Echo_ResponseParams_Data>(
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
const char HandleTrampoline::Name_[] = "mojo.test.HandleTrampoline";

class HandleTrampoline_BounceOne_ForwardToCallback
    : public mojo::MessageReceiver {
 public:
  HandleTrampoline_BounceOne_ForwardToCallback(
      HandleTrampoline::BounceOneCallback callback
      ) : callback_(std::move(callback)) {
  }
  bool Accept(mojo::Message* message) override;
 private:
  HandleTrampoline::BounceOneCallback callback_;
  DISALLOW_COPY_AND_ASSIGN(HandleTrampoline_BounceOne_ForwardToCallback);
};

class HandleTrampoline_BounceTwo_ForwardToCallback
    : public mojo::MessageReceiver {
 public:
  HandleTrampoline_BounceTwo_ForwardToCallback(
      HandleTrampoline::BounceTwoCallback callback
      ) : callback_(std::move(callback)) {
  }
  bool Accept(mojo::Message* message) override;
 private:
  HandleTrampoline::BounceTwoCallback callback_;
  DISALLOW_COPY_AND_ASSIGN(HandleTrampoline_BounceTwo_ForwardToCallback);
};

HandleTrampolineProxy::HandleTrampolineProxy(mojo::MessageReceiverWithResponder* receiver)
    : receiver_(receiver) {
}
class HandleTrampolineProxy_BounceOne_Message
    : public mojo::internal::UnserializedMessageContext {
 public:
  static const mojo::internal::UnserializedMessageContext::Tag kMessageTag;

  explicit HandleTrampolineProxy_BounceOne_Message(
      uint32_t message_flags
      , mojo::ScopedMessagePipeHandle param_one
  )
      : mojo::internal::UnserializedMessageContext(
          &kMessageTag,
          internal::kHandleTrampoline_BounceOne_Name,
          message_flags)
      , param_one_(std::move(param_one)){}
  ~HandleTrampolineProxy_BounceOne_Message() override = default;

  static mojo::Message Build(
      bool serialize,
      bool expects_response,
      bool is_sync,
      mojo::ScopedMessagePipeHandle param_one) {

    
  const uint32_t kFlags =
      ((expects_response) ? mojo::Message::kFlagExpectsResponse : 0) |
      ((is_sync) ? mojo::Message::kFlagIsSync : 0);

    if (!serialize) {
      return mojo::Message(std::make_unique<HandleTrampolineProxy_BounceOne_Message>(
          kFlags
          , std::move(param_one)
          ));
    }

    DCHECK(serialize);
    
  mojo::Message message(
      internal::kHandleTrampoline_BounceOne_Name, kFlags, 0, 0, nullptr);
  auto* buffer = message.payload_buffer();
  ::mojo::test::internal::HandleTrampoline_BounceOne_Params_Data::BufferWriter
      params;
  mojo::internal::SerializationContext serialization_context;
  params.Allocate(buffer);
  mojo::internal::Serialize<mojo::ScopedMessagePipeHandle>(
      param_one, &params->one, &serialization_context);
  MOJO_INTERNAL_DLOG_SERIALIZATION_WARNING(
      !mojo::internal::IsHandleOrInterfaceValid(params->one),
      mojo::internal::VALIDATION_ERROR_UNEXPECTED_INVALID_HANDLE,
      "invalid one in HandleTrampoline.BounceOne request");
  message.AttachHandlesFromSerializationContext(
      &serialization_context);
    return message;
  }


  void Dispatch(HandleTrampoline* impl, HandleTrampoline::BounceOneCallback callback) {
    impl->BounceOne(
        std::move(param_one_), std::move(callback));
  }

 private:
  // mojo::internal::UnserializedMessageContext:
  void Serialize(mojo::internal::SerializationContext* serialization_context,
                 mojo::internal::Buffer* buffer) override {
    ::mojo::test::internal::HandleTrampoline_BounceOne_Params_Data::BufferWriter
        params;
    params.Allocate(buffer);
  mojo::internal::Serialize<mojo::ScopedMessagePipeHandle>(
      param_one_, &params->one, serialization_context);
  MOJO_INTERNAL_DLOG_SERIALIZATION_WARNING(
      !mojo::internal::IsHandleOrInterfaceValid(params->one),
      mojo::internal::VALIDATION_ERROR_UNEXPECTED_INVALID_HANDLE,
      "invalid one in HandleTrampoline.BounceOne request");
  }
  mojo::ScopedMessagePipeHandle param_one_;

  DISALLOW_COPY_AND_ASSIGN(HandleTrampolineProxy_BounceOne_Message);
};

const mojo::internal::UnserializedMessageContext::Tag
HandleTrampolineProxy_BounceOne_Message::kMessageTag = {};

void HandleTrampolineProxy::BounceOne(
    mojo::ScopedMessagePipeHandle in_one, BounceOneCallback callback) {
#if BUILDFLAG(MOJO_TRACE_ENABLED)
  TRACE_EVENT0("mojom", "<class 'jinja2::utils::Namespace'>::HandleTrampoline::BounceOne");
#endif
  const bool kExpectsResponse = true;
  const bool kIsSync = false;
  const bool kSerialize = receiver_->PrefersSerializedMessages();
  auto message = HandleTrampolineProxy_BounceOne_Message::Build(
      kSerialize, kExpectsResponse, kIsSync, std::move(in_one));

#if defined(ENABLE_IPC_FUZZER)
  message.set_interface_name(HandleTrampoline::Name_);
  message.set_method_name("BounceOne");
#endif
  std::unique_ptr<mojo::MessageReceiver> responder(
      new HandleTrampoline_BounceOne_ForwardToCallback(
          std::move(callback)));
  ignore_result(receiver_->AcceptWithResponder(&message, std::move(responder)));
}
class HandleTrampolineProxy_BounceTwo_Message
    : public mojo::internal::UnserializedMessageContext {
 public:
  static const mojo::internal::UnserializedMessageContext::Tag kMessageTag;

  explicit HandleTrampolineProxy_BounceTwo_Message(
      uint32_t message_flags
      , mojo::ScopedMessagePipeHandle param_one
      , mojo::ScopedMessagePipeHandle param_two
  )
      : mojo::internal::UnserializedMessageContext(
          &kMessageTag,
          internal::kHandleTrampoline_BounceTwo_Name,
          message_flags)
      , param_one_(std::move(param_one))
      , param_two_(std::move(param_two)){}
  ~HandleTrampolineProxy_BounceTwo_Message() override = default;

  static mojo::Message Build(
      bool serialize,
      bool expects_response,
      bool is_sync,
      mojo::ScopedMessagePipeHandle param_one, mojo::ScopedMessagePipeHandle param_two) {

    
  const uint32_t kFlags =
      ((expects_response) ? mojo::Message::kFlagExpectsResponse : 0) |
      ((is_sync) ? mojo::Message::kFlagIsSync : 0);

    if (!serialize) {
      return mojo::Message(std::make_unique<HandleTrampolineProxy_BounceTwo_Message>(
          kFlags
          , std::move(param_one)
          , std::move(param_two)
          ));
    }

    DCHECK(serialize);
    
  mojo::Message message(
      internal::kHandleTrampoline_BounceTwo_Name, kFlags, 0, 0, nullptr);
  auto* buffer = message.payload_buffer();
  ::mojo::test::internal::HandleTrampoline_BounceTwo_Params_Data::BufferWriter
      params;
  mojo::internal::SerializationContext serialization_context;
  params.Allocate(buffer);
  mojo::internal::Serialize<mojo::ScopedMessagePipeHandle>(
      param_one, &params->one, &serialization_context);
  MOJO_INTERNAL_DLOG_SERIALIZATION_WARNING(
      !mojo::internal::IsHandleOrInterfaceValid(params->one),
      mojo::internal::VALIDATION_ERROR_UNEXPECTED_INVALID_HANDLE,
      "invalid one in HandleTrampoline.BounceTwo request");
  mojo::internal::Serialize<mojo::ScopedMessagePipeHandle>(
      param_two, &params->two, &serialization_context);
  MOJO_INTERNAL_DLOG_SERIALIZATION_WARNING(
      !mojo::internal::IsHandleOrInterfaceValid(params->two),
      mojo::internal::VALIDATION_ERROR_UNEXPECTED_INVALID_HANDLE,
      "invalid two in HandleTrampoline.BounceTwo request");
  message.AttachHandlesFromSerializationContext(
      &serialization_context);
    return message;
  }


  void Dispatch(HandleTrampoline* impl, HandleTrampoline::BounceTwoCallback callback) {
    impl->BounceTwo(
        std::move(param_one_),
        std::move(param_two_), std::move(callback));
  }

 private:
  // mojo::internal::UnserializedMessageContext:
  void Serialize(mojo::internal::SerializationContext* serialization_context,
                 mojo::internal::Buffer* buffer) override {
    ::mojo::test::internal::HandleTrampoline_BounceTwo_Params_Data::BufferWriter
        params;
    params.Allocate(buffer);
  mojo::internal::Serialize<mojo::ScopedMessagePipeHandle>(
      param_one_, &params->one, serialization_context);
  MOJO_INTERNAL_DLOG_SERIALIZATION_WARNING(
      !mojo::internal::IsHandleOrInterfaceValid(params->one),
      mojo::internal::VALIDATION_ERROR_UNEXPECTED_INVALID_HANDLE,
      "invalid one in HandleTrampoline.BounceTwo request");
  mojo::internal::Serialize<mojo::ScopedMessagePipeHandle>(
      param_two_, &params->two, serialization_context);
  MOJO_INTERNAL_DLOG_SERIALIZATION_WARNING(
      !mojo::internal::IsHandleOrInterfaceValid(params->two),
      mojo::internal::VALIDATION_ERROR_UNEXPECTED_INVALID_HANDLE,
      "invalid two in HandleTrampoline.BounceTwo request");
  }
  mojo::ScopedMessagePipeHandle param_one_;
  mojo::ScopedMessagePipeHandle param_two_;

  DISALLOW_COPY_AND_ASSIGN(HandleTrampolineProxy_BounceTwo_Message);
};

const mojo::internal::UnserializedMessageContext::Tag
HandleTrampolineProxy_BounceTwo_Message::kMessageTag = {};

void HandleTrampolineProxy::BounceTwo(
    mojo::ScopedMessagePipeHandle in_one, mojo::ScopedMessagePipeHandle in_two, BounceTwoCallback callback) {
#if BUILDFLAG(MOJO_TRACE_ENABLED)
  TRACE_EVENT0("mojom", "<class 'jinja2::utils::Namespace'>::HandleTrampoline::BounceTwo");
#endif
  const bool kExpectsResponse = true;
  const bool kIsSync = false;
  const bool kSerialize = receiver_->PrefersSerializedMessages();
  auto message = HandleTrampolineProxy_BounceTwo_Message::Build(
      kSerialize, kExpectsResponse, kIsSync, std::move(in_one), std::move(in_two));

#if defined(ENABLE_IPC_FUZZER)
  message.set_interface_name(HandleTrampoline::Name_);
  message.set_method_name("BounceTwo");
#endif
  std::unique_ptr<mojo::MessageReceiver> responder(
      new HandleTrampoline_BounceTwo_ForwardToCallback(
          std::move(callback)));
  ignore_result(receiver_->AcceptWithResponder(&message, std::move(responder)));
}
class HandleTrampoline_BounceOne_ProxyToResponder {
 public:
  static HandleTrampoline::BounceOneCallback CreateCallback(
      uint64_t request_id,
      bool is_sync,
      std::unique_ptr<mojo::MessageReceiverWithStatus> responder) {
    std::unique_ptr<HandleTrampoline_BounceOne_ProxyToResponder> proxy(
        new HandleTrampoline_BounceOne_ProxyToResponder(
            request_id, is_sync, std::move(responder)));
    return base::BindOnce(&HandleTrampoline_BounceOne_ProxyToResponder::Run,
                          std::move(proxy));
  }

  ~HandleTrampoline_BounceOne_ProxyToResponder() {
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
  HandleTrampoline_BounceOne_ProxyToResponder(
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
        << "HandleTrampoline::BounceOneCallback was destroyed without "
        << "first either being run or its corresponding binding being closed. "
        << "It is an error to drop response callbacks which still correspond "
        << "to an open interface pipe.";
  }
#endif

  void Run(
      mojo::ScopedMessagePipeHandle in_one);

  uint64_t request_id_;
  bool is_sync_;
  std::unique_ptr<mojo::MessageReceiverWithStatus> responder_;

  DISALLOW_COPY_AND_ASSIGN(HandleTrampoline_BounceOne_ProxyToResponder);
};
class HandleTrampoline_BounceOne_Response_Message
    : public mojo::internal::UnserializedMessageContext {
 public:
  static const mojo::internal::UnserializedMessageContext::Tag kMessageTag;

  explicit HandleTrampoline_BounceOne_Response_Message(
      uint32_t message_flags
      , mojo::ScopedMessagePipeHandle param_one
  )
      : mojo::internal::UnserializedMessageContext(
          &kMessageTag,
          internal::kHandleTrampoline_BounceOne_Name,
          message_flags)
      , param_one_(std::move(param_one)){}
  ~HandleTrampoline_BounceOne_Response_Message() override = default;

  static mojo::Message Build(
      bool serialize,
      bool is_sync,
      mojo::ScopedMessagePipeHandle param_one) {

    
  const uint32_t kFlags = mojo::Message::kFlagIsResponse |
      ((is_sync) ? mojo::Message::kFlagIsSync : 0);

    if (!serialize) {
      return mojo::Message(std::make_unique<HandleTrampoline_BounceOne_Response_Message>(
          kFlags
          , std::move(param_one)
          ));
    }

    DCHECK(serialize);
    
  mojo::Message message(
      internal::kHandleTrampoline_BounceOne_Name, kFlags, 0, 0, nullptr);
  auto* buffer = message.payload_buffer();
  ::mojo::test::internal::HandleTrampoline_BounceOne_ResponseParams_Data::BufferWriter
      params;
  mojo::internal::SerializationContext serialization_context;
  params.Allocate(buffer);
  mojo::internal::Serialize<mojo::ScopedMessagePipeHandle>(
      param_one, &params->one, &serialization_context);
  MOJO_INTERNAL_DLOG_SERIALIZATION_WARNING(
      !mojo::internal::IsHandleOrInterfaceValid(params->one),
      mojo::internal::VALIDATION_ERROR_UNEXPECTED_INVALID_HANDLE,
      "invalid one in HandleTrampoline.BounceOne response");
  message.AttachHandlesFromSerializationContext(
      &serialization_context);
    return message;
  }


  void Dispatch(HandleTrampoline::BounceOneCallback* callback) {
    std::move(*callback).Run(
        std::move(param_one_));
  }



 private:
  // mojo::internal::UnserializedMessageContext:
  void Serialize(mojo::internal::SerializationContext* serialization_context,
                 mojo::internal::Buffer* buffer) override {
    ::mojo::test::internal::HandleTrampoline_BounceOne_ResponseParams_Data::BufferWriter
        params;
    params.Allocate(buffer);
  mojo::internal::Serialize<mojo::ScopedMessagePipeHandle>(
      param_one_, &params->one, serialization_context);
  MOJO_INTERNAL_DLOG_SERIALIZATION_WARNING(
      !mojo::internal::IsHandleOrInterfaceValid(params->one),
      mojo::internal::VALIDATION_ERROR_UNEXPECTED_INVALID_HANDLE,
      "invalid one in HandleTrampoline.BounceOne response");
  }
  mojo::ScopedMessagePipeHandle param_one_;

  DISALLOW_COPY_AND_ASSIGN(HandleTrampoline_BounceOne_Response_Message);
};

const mojo::internal::UnserializedMessageContext::Tag
HandleTrampoline_BounceOne_Response_Message::kMessageTag = {};

bool HandleTrampoline_BounceOne_ForwardToCallback::Accept(
    mojo::Message* message) {
#if BUILDFLAG(MOJO_TRACE_ENABLED)
  TRACE_EVENT1("mojom", "<class 'jinja2::utils::Namespace'>::HandleTrampoline::BounceOneCallback",
               "message", message->name());
#endif
  mojo::internal::MessageDispatchContext dispatch_context(message);
  if (!message->is_serialized()) {
    auto context =
        message->TakeUnserializedContext<HandleTrampoline_BounceOne_Response_Message>();
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
  internal::HandleTrampoline_BounceOne_ResponseParams_Data* params =
      reinterpret_cast<
          internal::HandleTrampoline_BounceOne_ResponseParams_Data*>(
              message->mutable_payload());
  
  mojo::internal::SerializationContext serialization_context;
  serialization_context.TakeHandlesFromMessage(message);
  bool success = true;
  mojo::ScopedMessagePipeHandle p_one{};
  HandleTrampoline_BounceOne_ResponseParamsDataView input_data_view(params, &serialization_context);
  
  p_one = input_data_view.TakeOne();
  if (!success) {
    ReportValidationErrorForMessage(
        message,
        mojo::internal::VALIDATION_ERROR_DESERIALIZATION_FAILED,
        HandleTrampoline::Name_, 0, true);
    return false;
  }
  if (!callback_.is_null())
    std::move(callback_).Run(
std::move(p_one));
  return true;
}

void HandleTrampoline_BounceOne_ProxyToResponder::Run(
    mojo::ScopedMessagePipeHandle in_one) {
  const bool kSerialize = responder_->PrefersSerializedMessages();
  auto message = HandleTrampoline_BounceOne_Response_Message::Build(kSerialize, is_sync_, std::move(in_one));

#if BUILDFLAG(MOJO_TRACE_ENABLED)
  TRACE_EVENT1("mojom", "(Impl)<class 'jinja2::utils::Namespace'>::HandleTrampoline::BounceOneCallback",
               "message", message.name());
#endif

#if defined(ENABLE_IPC_FUZZER)
  message.set_interface_name(HandleTrampoline::Name_);
  message.set_method_name("BounceOne");
#endif

  message.set_request_id(request_id_);
  ignore_result(responder_->Accept(&message));
  // TODO(darin): Accept() returning false indicates a malformed message, and
  // that may be good reason to close the connection. However, we don't have a
  // way to do that from here. We should add a way.
  responder_ = nullptr;
}
class HandleTrampoline_BounceTwo_ProxyToResponder {
 public:
  static HandleTrampoline::BounceTwoCallback CreateCallback(
      uint64_t request_id,
      bool is_sync,
      std::unique_ptr<mojo::MessageReceiverWithStatus> responder) {
    std::unique_ptr<HandleTrampoline_BounceTwo_ProxyToResponder> proxy(
        new HandleTrampoline_BounceTwo_ProxyToResponder(
            request_id, is_sync, std::move(responder)));
    return base::BindOnce(&HandleTrampoline_BounceTwo_ProxyToResponder::Run,
                          std::move(proxy));
  }

  ~HandleTrampoline_BounceTwo_ProxyToResponder() {
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
  HandleTrampoline_BounceTwo_ProxyToResponder(
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
        << "HandleTrampoline::BounceTwoCallback was destroyed without "
        << "first either being run or its corresponding binding being closed. "
        << "It is an error to drop response callbacks which still correspond "
        << "to an open interface pipe.";
  }
#endif

  void Run(
      mojo::ScopedMessagePipeHandle in_one, mojo::ScopedMessagePipeHandle in_two);

  uint64_t request_id_;
  bool is_sync_;
  std::unique_ptr<mojo::MessageReceiverWithStatus> responder_;

  DISALLOW_COPY_AND_ASSIGN(HandleTrampoline_BounceTwo_ProxyToResponder);
};
class HandleTrampoline_BounceTwo_Response_Message
    : public mojo::internal::UnserializedMessageContext {
 public:
  static const mojo::internal::UnserializedMessageContext::Tag kMessageTag;

  explicit HandleTrampoline_BounceTwo_Response_Message(
      uint32_t message_flags
      , mojo::ScopedMessagePipeHandle param_one
      , mojo::ScopedMessagePipeHandle param_two
  )
      : mojo::internal::UnserializedMessageContext(
          &kMessageTag,
          internal::kHandleTrampoline_BounceTwo_Name,
          message_flags)
      , param_one_(std::move(param_one))
      , param_two_(std::move(param_two)){}
  ~HandleTrampoline_BounceTwo_Response_Message() override = default;

  static mojo::Message Build(
      bool serialize,
      bool is_sync,
      mojo::ScopedMessagePipeHandle param_one, mojo::ScopedMessagePipeHandle param_two) {

    
  const uint32_t kFlags = mojo::Message::kFlagIsResponse |
      ((is_sync) ? mojo::Message::kFlagIsSync : 0);

    if (!serialize) {
      return mojo::Message(std::make_unique<HandleTrampoline_BounceTwo_Response_Message>(
          kFlags
          , std::move(param_one)
          , std::move(param_two)
          ));
    }

    DCHECK(serialize);
    
  mojo::Message message(
      internal::kHandleTrampoline_BounceTwo_Name, kFlags, 0, 0, nullptr);
  auto* buffer = message.payload_buffer();
  ::mojo::test::internal::HandleTrampoline_BounceTwo_ResponseParams_Data::BufferWriter
      params;
  mojo::internal::SerializationContext serialization_context;
  params.Allocate(buffer);
  mojo::internal::Serialize<mojo::ScopedMessagePipeHandle>(
      param_one, &params->one, &serialization_context);
  MOJO_INTERNAL_DLOG_SERIALIZATION_WARNING(
      !mojo::internal::IsHandleOrInterfaceValid(params->one),
      mojo::internal::VALIDATION_ERROR_UNEXPECTED_INVALID_HANDLE,
      "invalid one in HandleTrampoline.BounceTwo response");
  mojo::internal::Serialize<mojo::ScopedMessagePipeHandle>(
      param_two, &params->two, &serialization_context);
  MOJO_INTERNAL_DLOG_SERIALIZATION_WARNING(
      !mojo::internal::IsHandleOrInterfaceValid(params->two),
      mojo::internal::VALIDATION_ERROR_UNEXPECTED_INVALID_HANDLE,
      "invalid two in HandleTrampoline.BounceTwo response");
  message.AttachHandlesFromSerializationContext(
      &serialization_context);
    return message;
  }


  void Dispatch(HandleTrampoline::BounceTwoCallback* callback) {
    std::move(*callback).Run(
        std::move(param_one_), 
        std::move(param_two_));
  }



 private:
  // mojo::internal::UnserializedMessageContext:
  void Serialize(mojo::internal::SerializationContext* serialization_context,
                 mojo::internal::Buffer* buffer) override {
    ::mojo::test::internal::HandleTrampoline_BounceTwo_ResponseParams_Data::BufferWriter
        params;
    params.Allocate(buffer);
  mojo::internal::Serialize<mojo::ScopedMessagePipeHandle>(
      param_one_, &params->one, serialization_context);
  MOJO_INTERNAL_DLOG_SERIALIZATION_WARNING(
      !mojo::internal::IsHandleOrInterfaceValid(params->one),
      mojo::internal::VALIDATION_ERROR_UNEXPECTED_INVALID_HANDLE,
      "invalid one in HandleTrampoline.BounceTwo response");
  mojo::internal::Serialize<mojo::ScopedMessagePipeHandle>(
      param_two_, &params->two, serialization_context);
  MOJO_INTERNAL_DLOG_SERIALIZATION_WARNING(
      !mojo::internal::IsHandleOrInterfaceValid(params->two),
      mojo::internal::VALIDATION_ERROR_UNEXPECTED_INVALID_HANDLE,
      "invalid two in HandleTrampoline.BounceTwo response");
  }
  mojo::ScopedMessagePipeHandle param_one_;
  mojo::ScopedMessagePipeHandle param_two_;

  DISALLOW_COPY_AND_ASSIGN(HandleTrampoline_BounceTwo_Response_Message);
};

const mojo::internal::UnserializedMessageContext::Tag
HandleTrampoline_BounceTwo_Response_Message::kMessageTag = {};

bool HandleTrampoline_BounceTwo_ForwardToCallback::Accept(
    mojo::Message* message) {
#if BUILDFLAG(MOJO_TRACE_ENABLED)
  TRACE_EVENT1("mojom", "<class 'jinja2::utils::Namespace'>::HandleTrampoline::BounceTwoCallback",
               "message", message->name());
#endif
  mojo::internal::MessageDispatchContext dispatch_context(message);
  if (!message->is_serialized()) {
    auto context =
        message->TakeUnserializedContext<HandleTrampoline_BounceTwo_Response_Message>();
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
  internal::HandleTrampoline_BounceTwo_ResponseParams_Data* params =
      reinterpret_cast<
          internal::HandleTrampoline_BounceTwo_ResponseParams_Data*>(
              message->mutable_payload());
  
  mojo::internal::SerializationContext serialization_context;
  serialization_context.TakeHandlesFromMessage(message);
  bool success = true;
  mojo::ScopedMessagePipeHandle p_one{};
  mojo::ScopedMessagePipeHandle p_two{};
  HandleTrampoline_BounceTwo_ResponseParamsDataView input_data_view(params, &serialization_context);
  
  p_one = input_data_view.TakeOne();
  p_two = input_data_view.TakeTwo();
  if (!success) {
    ReportValidationErrorForMessage(
        message,
        mojo::internal::VALIDATION_ERROR_DESERIALIZATION_FAILED,
        HandleTrampoline::Name_, 1, true);
    return false;
  }
  if (!callback_.is_null())
    std::move(callback_).Run(
std::move(p_one), 
std::move(p_two));
  return true;
}

void HandleTrampoline_BounceTwo_ProxyToResponder::Run(
    mojo::ScopedMessagePipeHandle in_one, mojo::ScopedMessagePipeHandle in_two) {
  const bool kSerialize = responder_->PrefersSerializedMessages();
  auto message = HandleTrampoline_BounceTwo_Response_Message::Build(kSerialize, is_sync_, std::move(in_one), std::move(in_two));

#if BUILDFLAG(MOJO_TRACE_ENABLED)
  TRACE_EVENT1("mojom", "(Impl)<class 'jinja2::utils::Namespace'>::HandleTrampoline::BounceTwoCallback",
               "message", message.name());
#endif

#if defined(ENABLE_IPC_FUZZER)
  message.set_interface_name(HandleTrampoline::Name_);
  message.set_method_name("BounceTwo");
#endif

  message.set_request_id(request_id_);
  ignore_result(responder_->Accept(&message));
  // TODO(darin): Accept() returning false indicates a malformed message, and
  // that may be good reason to close the connection. However, we don't have a
  // way to do that from here. We should add a way.
  responder_ = nullptr;
}

// static
bool HandleTrampolineStubDispatch::Accept(
    HandleTrampoline* impl,
    mojo::Message* message) {
  switch (message->header()->name) {
    case internal::kHandleTrampoline_BounceOne_Name: {
      break;
    }
    case internal::kHandleTrampoline_BounceTwo_Name: {
      break;
    }
  }
  return false;
}

// static
bool HandleTrampolineStubDispatch::AcceptWithResponder(
    HandleTrampoline* impl,
    mojo::Message* message,
    std::unique_ptr<mojo::MessageReceiverWithStatus> responder) {
  switch (message->header()->name) {
    case internal::kHandleTrampoline_BounceOne_Name: {
#if BUILDFLAG(MOJO_TRACE_ENABLED)
      TRACE_EVENT1(
          "mojom",
          "(Impl)<class 'jinja2::utils::Namespace'>::HandleTrampoline::BounceOne",
          "message", message->name());
#endif
      base::TaskAnnotator::ScopedSetIpcProgramCounter scoped_ipc_pc(
          base::GetProgramCounter());
      mojo::internal::MessageDispatchContext context(message);
      if (!message->is_serialized()) {
        auto context = message->TakeUnserializedContext<
            HandleTrampolineProxy_BounceOne_Message>();
        if (!context) {
          // The Message was not of the expected type. It may be a valid message
          // which was built using a different variant of these bindings. Force
          // serialization before dispatch in this case.
          message->SerializeIfNecessary();
        } else {
          HandleTrampoline::BounceOneCallback callback =
              HandleTrampoline_BounceOne_ProxyToResponder::CreateCallback(
                  message->request_id(),
                  message->has_flag(mojo::Message::kFlagIsSync),
                  std::move(responder));
          context->Dispatch(impl, std::move(callback));
          return true;
        }
      }

      internal::HandleTrampoline_BounceOne_Params_Data* params =
          reinterpret_cast<
              internal::HandleTrampoline_BounceOne_Params_Data*>(
                  message->mutable_payload());
      
      mojo::internal::SerializationContext serialization_context;
      serialization_context.TakeHandlesFromMessage(message);
      bool success = true;
      mojo::ScopedMessagePipeHandle p_one{};
      HandleTrampoline_BounceOne_ParamsDataView input_data_view(params, &serialization_context);
      
      p_one = input_data_view.TakeOne();
      if (!success) {
        ReportValidationErrorForMessage(
            message,
            mojo::internal::VALIDATION_ERROR_DESERIALIZATION_FAILED,
            HandleTrampoline::Name_, 0, false);
        return false;
      }
      HandleTrampoline::BounceOneCallback callback =
          HandleTrampoline_BounceOne_ProxyToResponder::CreateCallback(
              message->request_id(),
              message->has_flag(mojo::Message::kFlagIsSync),
              std::move(responder));
      // A null |impl| means no implementation was bound.
      DCHECK(impl);
      impl->BounceOne(
std::move(p_one), std::move(callback));
      return true;
    }
    case internal::kHandleTrampoline_BounceTwo_Name: {
#if BUILDFLAG(MOJO_TRACE_ENABLED)
      TRACE_EVENT1(
          "mojom",
          "(Impl)<class 'jinja2::utils::Namespace'>::HandleTrampoline::BounceTwo",
          "message", message->name());
#endif
      base::TaskAnnotator::ScopedSetIpcProgramCounter scoped_ipc_pc(
          base::GetProgramCounter());
      mojo::internal::MessageDispatchContext context(message);
      if (!message->is_serialized()) {
        auto context = message->TakeUnserializedContext<
            HandleTrampolineProxy_BounceTwo_Message>();
        if (!context) {
          // The Message was not of the expected type. It may be a valid message
          // which was built using a different variant of these bindings. Force
          // serialization before dispatch in this case.
          message->SerializeIfNecessary();
        } else {
          HandleTrampoline::BounceTwoCallback callback =
              HandleTrampoline_BounceTwo_ProxyToResponder::CreateCallback(
                  message->request_id(),
                  message->has_flag(mojo::Message::kFlagIsSync),
                  std::move(responder));
          context->Dispatch(impl, std::move(callback));
          return true;
        }
      }

      internal::HandleTrampoline_BounceTwo_Params_Data* params =
          reinterpret_cast<
              internal::HandleTrampoline_BounceTwo_Params_Data*>(
                  message->mutable_payload());
      
      mojo::internal::SerializationContext serialization_context;
      serialization_context.TakeHandlesFromMessage(message);
      bool success = true;
      mojo::ScopedMessagePipeHandle p_one{};
      mojo::ScopedMessagePipeHandle p_two{};
      HandleTrampoline_BounceTwo_ParamsDataView input_data_view(params, &serialization_context);
      
      p_one = input_data_view.TakeOne();
      p_two = input_data_view.TakeTwo();
      if (!success) {
        ReportValidationErrorForMessage(
            message,
            mojo::internal::VALIDATION_ERROR_DESERIALIZATION_FAILED,
            HandleTrampoline::Name_, 1, false);
        return false;
      }
      HandleTrampoline::BounceTwoCallback callback =
          HandleTrampoline_BounceTwo_ProxyToResponder::CreateCallback(
              message->request_id(),
              message->has_flag(mojo::Message::kFlagIsSync),
              std::move(responder));
      // A null |impl| means no implementation was bound.
      DCHECK(impl);
      impl->BounceTwo(
std::move(p_one), 
std::move(p_two), std::move(callback));
      return true;
    }
  }
  return false;
}

bool HandleTrampolineRequestValidator::Accept(mojo::Message* message) {
  if (!message->is_serialized() ||
      mojo::internal::ControlMessageHandler::IsControlMessage(message)) {
    return true;
  }

  mojo::internal::ValidationContext validation_context(
    message->payload(), message->payload_num_bytes(),
    message->handles()->size(), message->payload_num_interface_ids(), message,
    "HandleTrampoline RequestValidator");

  switch (message->header()->name) {
    case internal::kHandleTrampoline_BounceOne_Name: {
      if (!mojo::internal::ValidateMessageIsRequestExpectingResponse(
              message, &validation_context)) {
        return false;
      }
      if (!mojo::internal::ValidateMessagePayload<
               internal::HandleTrampoline_BounceOne_Params_Data>(
                  message, &validation_context)) {
        return false;
      }
      return true;
    }
    case internal::kHandleTrampoline_BounceTwo_Name: {
      if (!mojo::internal::ValidateMessageIsRequestExpectingResponse(
              message, &validation_context)) {
        return false;
      }
      if (!mojo::internal::ValidateMessagePayload<
               internal::HandleTrampoline_BounceTwo_Params_Data>(
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

bool HandleTrampolineResponseValidator::Accept(mojo::Message* message) {
  if (!message->is_serialized() ||
      mojo::internal::ControlMessageHandler::IsControlMessage(message)) {
    return true;
  }

  mojo::internal::ValidationContext validation_context(
    message->payload(), message->payload_num_bytes(),
    message->handles()->size(), message->payload_num_interface_ids(), message,
    "HandleTrampoline ResponseValidator");

  if (!mojo::internal::ValidateMessageIsResponse(message, &validation_context))
    return false;
  switch (message->header()->name) {
    case internal::kHandleTrampoline_BounceOne_Name: {
      if (!mojo::internal::ValidateMessagePayload<
               internal::HandleTrampoline_BounceOne_ResponseParams_Data>(
                    message, &validation_context)) {
        return false;
      }
      return true;
    }
    case internal::kHandleTrampoline_BounceTwo_Name: {
      if (!mojo::internal::ValidateMessagePayload<
               internal::HandleTrampoline_BounceTwo_ResponseParams_Data>(
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
}  // namespace blink
}  // namespace test
}  // namespace mojo

namespace mojo {

}  // namespace mojo

#if defined(__clang__)
#pragma clang diagnostic pop
#elif defined(_MSC_VER)
#pragma warning(pop)
#endif