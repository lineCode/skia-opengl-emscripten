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

#include "third_party/blink/public/mojom/idle/idle_manager.mojom.h"

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

#include "third_party/blink/public/mojom/idle/idle_manager.mojom-params-data.h"
#include "third_party/blink/public/mojom/idle/idle_manager.mojom-shared-message-ids.h"

#include "third_party/blink/public/mojom/idle/idle_manager.mojom-import-headers.h"


#ifndef THIRD_PARTY_BLINK_PUBLIC_MOJOM_IDLE_IDLE_MANAGER_MOJOM_JUMBO_H_
#define THIRD_PARTY_BLINK_PUBLIC_MOJOM_IDLE_IDLE_MANAGER_MOJOM_JUMBO_H_
#include "mojo/public/cpp/base/time_mojom_traits.h"
#endif
namespace blink {
namespace mojom {
IdleState::IdleState()
    : user(),
      screen() {}

IdleState::IdleState(
    UserIdleState user_in,
    ScreenIdleState screen_in)
    : user(std::move(user_in)),
      screen(std::move(screen_in)) {}

IdleState::~IdleState() = default;
size_t IdleState::Hash(size_t seed) const {
  seed = mojo::internal::Hash(seed, this->user);
  seed = mojo::internal::Hash(seed, this->screen);
  return seed;
}

bool IdleState::Validate(
    const void* data,
    mojo::internal::ValidationContext* validation_context) {
  return Data_::Validate(data, validation_context);
}
const char IdleMonitor::Name_[] = "blink.mojom.IdleMonitor";

IdleMonitorProxy::IdleMonitorProxy(mojo::MessageReceiverWithResponder* receiver)
    : receiver_(receiver) {
}

void IdleMonitorProxy::Update(
    IdleStatePtr in_state) {
#if BUILDFLAG(MOJO_TRACE_ENABLED)
  TRACE_EVENT0("mojom", "<class 'jinja2::utils::Namespace'>::IdleMonitor::Update");
#endif
  const bool kExpectsResponse = false;
  const bool kIsSync = false;
  
  const uint32_t kFlags =
      ((kExpectsResponse) ? mojo::Message::kFlagExpectsResponse : 0) |
      ((kIsSync) ? mojo::Message::kFlagIsSync : 0);
  
  mojo::Message message(
      internal::kIdleMonitor_Update_Name, kFlags, 0, 0, nullptr);
  auto* buffer = message.payload_buffer();
  ::blink::mojom::internal::IdleMonitor_Update_Params_Data::BufferWriter
      params;
  mojo::internal::SerializationContext serialization_context;
  params.Allocate(buffer);
  typename decltype(params->state)::BaseType::BufferWriter
      state_writer;
  mojo::internal::Serialize<::blink::mojom::IdleStateDataView>(
      in_state, buffer, &state_writer, &serialization_context);
  params->state.Set(
      state_writer.is_null() ? nullptr : state_writer.data());
  MOJO_INTERNAL_DLOG_SERIALIZATION_WARNING(
      params->state.is_null(),
      mojo::internal::VALIDATION_ERROR_UNEXPECTED_NULL_POINTER,
      "null state in IdleMonitor.Update request");
  message.AttachHandlesFromSerializationContext(
      &serialization_context);

#if defined(ENABLE_IPC_FUZZER)
  message.set_interface_name(IdleMonitor::Name_);
  message.set_method_name("Update");
#endif
  // This return value may be ignored as false implies the Connector has
  // encountered an error, which will be visible through other means.
  ignore_result(receiver_->Accept(&message));
}

// static
bool IdleMonitorStubDispatch::Accept(
    IdleMonitor* impl,
    mojo::Message* message) {
  switch (message->header()->name) {
    case internal::kIdleMonitor_Update_Name: {
#if BUILDFLAG(MOJO_TRACE_ENABLED)
      TRACE_EVENT1(
          "mojom",
          "(Impl)<class 'jinja2::utils::Namespace'>::IdleMonitor::Update",
          "message", message->name());
#endif
      base::TaskAnnotator::ScopedSetIpcProgramCounter scoped_ipc_pc(
          base::GetProgramCounter());
      mojo::internal::MessageDispatchContext context(message);

      DCHECK(message->is_serialized());
      internal::IdleMonitor_Update_Params_Data* params =
          reinterpret_cast<internal::IdleMonitor_Update_Params_Data*>(
              message->mutable_payload());
      
      mojo::internal::SerializationContext serialization_context;
      serialization_context.TakeHandlesFromMessage(message);
      bool success = true;
      IdleStatePtr p_state{};
      IdleMonitor_Update_ParamsDataView input_data_view(params, &serialization_context);
      
      if (!input_data_view.ReadState(&p_state))
        success = false;
      if (!success) {
        ReportValidationErrorForMessage(
            message,
            mojo::internal::VALIDATION_ERROR_DESERIALIZATION_FAILED,
            IdleMonitor::Name_, 0, false);
        return false;
      }
      // A null |impl| means no implementation was bound.
      DCHECK(impl);
      impl->Update(
std::move(p_state));
      return true;
    }
  }
  return false;
}

// static
bool IdleMonitorStubDispatch::AcceptWithResponder(
    IdleMonitor* impl,
    mojo::Message* message,
    std::unique_ptr<mojo::MessageReceiverWithStatus> responder) {
  switch (message->header()->name) {
    case internal::kIdleMonitor_Update_Name: {
      break;
    }
  }
  return false;
}

bool IdleMonitorRequestValidator::Accept(mojo::Message* message) {
  if (!message->is_serialized() ||
      mojo::internal::ControlMessageHandler::IsControlMessage(message)) {
    return true;
  }

  mojo::internal::ValidationContext validation_context(
    message->payload(), message->payload_num_bytes(),
    message->handles()->size(), message->payload_num_interface_ids(), message,
    "IdleMonitor RequestValidator");

  switch (message->header()->name) {
    case internal::kIdleMonitor_Update_Name: {
      if (!mojo::internal::ValidateMessageIsRequestWithoutResponse(
              message, &validation_context)) {
        return false;
      }
      if (!mojo::internal::ValidateMessagePayload<
               internal::IdleMonitor_Update_Params_Data>(
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

const char IdleManager::Name_[] = "blink.mojom.IdleManager";

class IdleManager_AddMonitor_ForwardToCallback
    : public mojo::MessageReceiver {
 public:
  IdleManager_AddMonitor_ForwardToCallback(
      IdleManager::AddMonitorCallback callback
      ) : callback_(std::move(callback)) {
  }
  bool Accept(mojo::Message* message) override;
 private:
  IdleManager::AddMonitorCallback callback_;
  DISALLOW_COPY_AND_ASSIGN(IdleManager_AddMonitor_ForwardToCallback);
};

IdleManagerProxy::IdleManagerProxy(mojo::MessageReceiverWithResponder* receiver)
    : receiver_(receiver) {
}

void IdleManagerProxy::AddMonitor(
    base::TimeDelta in_threshold, IdleMonitorPtr in_monitor, AddMonitorCallback callback) {
#if BUILDFLAG(MOJO_TRACE_ENABLED)
  TRACE_EVENT0("mojom", "<class 'jinja2::utils::Namespace'>::IdleManager::AddMonitor");
#endif
  const bool kExpectsResponse = true;
  const bool kIsSync = false;
  
  const uint32_t kFlags =
      ((kExpectsResponse) ? mojo::Message::kFlagExpectsResponse : 0) |
      ((kIsSync) ? mojo::Message::kFlagIsSync : 0);
  
  mojo::Message message(
      internal::kIdleManager_AddMonitor_Name, kFlags, 0, 0, nullptr);
  auto* buffer = message.payload_buffer();
  ::blink::mojom::internal::IdleManager_AddMonitor_Params_Data::BufferWriter
      params;
  mojo::internal::SerializationContext serialization_context;
  params.Allocate(buffer);
  typename decltype(params->threshold)::BaseType::BufferWriter
      threshold_writer;
  mojo::internal::Serialize<::mojo_base::mojom::TimeDeltaDataView>(
      in_threshold, buffer, &threshold_writer, &serialization_context);
  params->threshold.Set(
      threshold_writer.is_null() ? nullptr : threshold_writer.data());
  MOJO_INTERNAL_DLOG_SERIALIZATION_WARNING(
      params->threshold.is_null(),
      mojo::internal::VALIDATION_ERROR_UNEXPECTED_NULL_POINTER,
      "null threshold in IdleManager.AddMonitor request");
  mojo::internal::Serialize<::blink::mojom::IdleMonitorPtrDataView>(
      in_monitor, &params->monitor, &serialization_context);
  MOJO_INTERNAL_DLOG_SERIALIZATION_WARNING(
      !mojo::internal::IsHandleOrInterfaceValid(params->monitor),
      mojo::internal::VALIDATION_ERROR_UNEXPECTED_INVALID_HANDLE,
      "invalid monitor in IdleManager.AddMonitor request");
  message.AttachHandlesFromSerializationContext(
      &serialization_context);

#if defined(ENABLE_IPC_FUZZER)
  message.set_interface_name(IdleManager::Name_);
  message.set_method_name("AddMonitor");
#endif
  std::unique_ptr<mojo::MessageReceiver> responder(
      new IdleManager_AddMonitor_ForwardToCallback(
          std::move(callback)));
  ignore_result(receiver_->AcceptWithResponder(&message, std::move(responder)));
}
class IdleManager_AddMonitor_ProxyToResponder {
 public:
  static IdleManager::AddMonitorCallback CreateCallback(
      uint64_t request_id,
      bool is_sync,
      std::unique_ptr<mojo::MessageReceiverWithStatus> responder) {
    std::unique_ptr<IdleManager_AddMonitor_ProxyToResponder> proxy(
        new IdleManager_AddMonitor_ProxyToResponder(
            request_id, is_sync, std::move(responder)));
    return base::BindOnce(&IdleManager_AddMonitor_ProxyToResponder::Run,
                          std::move(proxy));
  }

  ~IdleManager_AddMonitor_ProxyToResponder() {
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
  IdleManager_AddMonitor_ProxyToResponder(
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
        << "IdleManager::AddMonitorCallback was destroyed without "
        << "first either being run or its corresponding binding being closed. "
        << "It is an error to drop response callbacks which still correspond "
        << "to an open interface pipe.";
  }
#endif

  void Run(
      IdleStatePtr in_state);

  uint64_t request_id_;
  bool is_sync_;
  std::unique_ptr<mojo::MessageReceiverWithStatus> responder_;

  DISALLOW_COPY_AND_ASSIGN(IdleManager_AddMonitor_ProxyToResponder);
};

bool IdleManager_AddMonitor_ForwardToCallback::Accept(
    mojo::Message* message) {
#if BUILDFLAG(MOJO_TRACE_ENABLED)
  TRACE_EVENT1("mojom", "<class 'jinja2::utils::Namespace'>::IdleManager::AddMonitorCallback",
               "message", message->name());
#endif
  mojo::internal::MessageDispatchContext dispatch_context(message);

  DCHECK(message->is_serialized());
  internal::IdleManager_AddMonitor_ResponseParams_Data* params =
      reinterpret_cast<
          internal::IdleManager_AddMonitor_ResponseParams_Data*>(
              message->mutable_payload());
  
  mojo::internal::SerializationContext serialization_context;
  serialization_context.TakeHandlesFromMessage(message);
  bool success = true;
  IdleStatePtr p_state{};
  IdleManager_AddMonitor_ResponseParamsDataView input_data_view(params, &serialization_context);
  
  if (!input_data_view.ReadState(&p_state))
    success = false;
  if (!success) {
    ReportValidationErrorForMessage(
        message,
        mojo::internal::VALIDATION_ERROR_DESERIALIZATION_FAILED,
        IdleManager::Name_, 0, true);
    return false;
  }
  if (!callback_.is_null())
    std::move(callback_).Run(
std::move(p_state));
  return true;
}

void IdleManager_AddMonitor_ProxyToResponder::Run(
    IdleStatePtr in_state) {
  
  const uint32_t kFlags = mojo::Message::kFlagIsResponse |
      ((is_sync_) ? mojo::Message::kFlagIsSync : 0);
  
  mojo::Message message(
      internal::kIdleManager_AddMonitor_Name, kFlags, 0, 0, nullptr);
  auto* buffer = message.payload_buffer();
  ::blink::mojom::internal::IdleManager_AddMonitor_ResponseParams_Data::BufferWriter
      params;
  mojo::internal::SerializationContext serialization_context;
  params.Allocate(buffer);
  typename decltype(params->state)::BaseType::BufferWriter
      state_writer;
  mojo::internal::Serialize<::blink::mojom::IdleStateDataView>(
      in_state, buffer, &state_writer, &serialization_context);
  params->state.Set(
      state_writer.is_null() ? nullptr : state_writer.data());
  MOJO_INTERNAL_DLOG_SERIALIZATION_WARNING(
      params->state.is_null(),
      mojo::internal::VALIDATION_ERROR_UNEXPECTED_NULL_POINTER,
      "null state in ");
  message.AttachHandlesFromSerializationContext(
      &serialization_context);

#if BUILDFLAG(MOJO_TRACE_ENABLED)
  TRACE_EVENT1("mojom", "(Impl)<class 'jinja2::utils::Namespace'>::IdleManager::AddMonitorCallback",
               "message", message.name());
#endif

#if defined(ENABLE_IPC_FUZZER)
  message.set_interface_name(IdleManager::Name_);
  message.set_method_name("AddMonitor");
#endif

  message.set_request_id(request_id_);
  ignore_result(responder_->Accept(&message));
  // TODO(darin): Accept() returning false indicates a malformed message, and
  // that may be good reason to close the connection. However, we don't have a
  // way to do that from here. We should add a way.
  responder_ = nullptr;
}

// static
bool IdleManagerStubDispatch::Accept(
    IdleManager* impl,
    mojo::Message* message) {
  switch (message->header()->name) {
    case internal::kIdleManager_AddMonitor_Name: {
      break;
    }
  }
  return false;
}

// static
bool IdleManagerStubDispatch::AcceptWithResponder(
    IdleManager* impl,
    mojo::Message* message,
    std::unique_ptr<mojo::MessageReceiverWithStatus> responder) {
  switch (message->header()->name) {
    case internal::kIdleManager_AddMonitor_Name: {
#if BUILDFLAG(MOJO_TRACE_ENABLED)
      TRACE_EVENT1(
          "mojom",
          "(Impl)<class 'jinja2::utils::Namespace'>::IdleManager::AddMonitor",
          "message", message->name());
#endif
      base::TaskAnnotator::ScopedSetIpcProgramCounter scoped_ipc_pc(
          base::GetProgramCounter());
      mojo::internal::MessageDispatchContext context(message);

      internal::IdleManager_AddMonitor_Params_Data* params =
          reinterpret_cast<
              internal::IdleManager_AddMonitor_Params_Data*>(
                  message->mutable_payload());
      
      mojo::internal::SerializationContext serialization_context;
      serialization_context.TakeHandlesFromMessage(message);
      bool success = true;
      base::TimeDelta p_threshold{};
      IdleMonitorPtr p_monitor{};
      IdleManager_AddMonitor_ParamsDataView input_data_view(params, &serialization_context);
      
      if (!input_data_view.ReadThreshold(&p_threshold))
        success = false;
      p_monitor =
          input_data_view.TakeMonitor<decltype(p_monitor)>();
      if (!success) {
        ReportValidationErrorForMessage(
            message,
            mojo::internal::VALIDATION_ERROR_DESERIALIZATION_FAILED,
            IdleManager::Name_, 0, false);
        return false;
      }
      IdleManager::AddMonitorCallback callback =
          IdleManager_AddMonitor_ProxyToResponder::CreateCallback(
              message->request_id(),
              message->has_flag(mojo::Message::kFlagIsSync),
              std::move(responder));
      // A null |impl| means no implementation was bound.
      DCHECK(impl);
      impl->AddMonitor(
std::move(p_threshold), 
std::move(p_monitor), std::move(callback));
      return true;
    }
  }
  return false;
}

bool IdleManagerRequestValidator::Accept(mojo::Message* message) {
  if (!message->is_serialized() ||
      mojo::internal::ControlMessageHandler::IsControlMessage(message)) {
    return true;
  }

  mojo::internal::ValidationContext validation_context(
    message->payload(), message->payload_num_bytes(),
    message->handles()->size(), message->payload_num_interface_ids(), message,
    "IdleManager RequestValidator");

  switch (message->header()->name) {
    case internal::kIdleManager_AddMonitor_Name: {
      if (!mojo::internal::ValidateMessageIsRequestExpectingResponse(
              message, &validation_context)) {
        return false;
      }
      if (!mojo::internal::ValidateMessagePayload<
               internal::IdleManager_AddMonitor_Params_Data>(
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

bool IdleManagerResponseValidator::Accept(mojo::Message* message) {
  if (!message->is_serialized() ||
      mojo::internal::ControlMessageHandler::IsControlMessage(message)) {
    return true;
  }

  mojo::internal::ValidationContext validation_context(
    message->payload(), message->payload_num_bytes(),
    message->handles()->size(), message->payload_num_interface_ids(), message,
    "IdleManager ResponseValidator");

  if (!mojo::internal::ValidateMessageIsResponse(message, &validation_context))
    return false;
  switch (message->header()->name) {
    case internal::kIdleManager_AddMonitor_Name: {
      if (!mojo::internal::ValidateMessagePayload<
               internal::IdleManager_AddMonitor_ResponseParams_Data>(
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
}  // namespace mojom
}  // namespace blink

namespace mojo {


// static
bool StructTraits<::blink::mojom::IdleState::DataView, ::blink::mojom::IdleStatePtr>::Read(
    ::blink::mojom::IdleState::DataView input,
    ::blink::mojom::IdleStatePtr* output) {
  bool success = true;
  ::blink::mojom::IdleStatePtr result(::blink::mojom::IdleState::New());
  
      if (!input.ReadUser(&result->user))
        success = false;
      if (!input.ReadScreen(&result->screen))
        success = false;
  *output = std::move(result);
  return success;
}

}  // namespace mojo

#if defined(__clang__)
#pragma clang diagnostic pop
#elif defined(_MSC_VER)
#pragma warning(pop)
#endif