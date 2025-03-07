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

#include "third_party/blink/public/mojom/renderer_preference_watcher.mojom.h"

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

#include "third_party/blink/public/mojom/renderer_preference_watcher.mojom-params-data.h"
#include "third_party/blink/public/mojom/renderer_preference_watcher.mojom-shared-message-ids.h"

#include "third_party/blink/public/mojom/renderer_preference_watcher.mojom-import-headers.h"


#ifndef THIRD_PARTY_BLINK_PUBLIC_MOJOM_RENDERER_PREFERENCE_WATCHER_MOJOM_JUMBO_H_
#define THIRD_PARTY_BLINK_PUBLIC_MOJOM_RENDERER_PREFERENCE_WATCHER_MOJOM_JUMBO_H_
#include "mojo/public/cpp/base/time_mojom_traits.h"
#include "ui/gfx/mojo/font_render_params_struct_traits.h"
#endif
namespace blink {
namespace mojom {
const char RendererPreferenceWatcher::Name_[] = "blink.mojom.RendererPreferenceWatcher";

RendererPreferenceWatcherProxy::RendererPreferenceWatcherProxy(mojo::MessageReceiverWithResponder* receiver)
    : receiver_(receiver) {
}

void RendererPreferenceWatcherProxy::NotifyUpdate(
    ::blink::mojom::RendererPreferencesPtr in_new_prefs) {
#if BUILDFLAG(MOJO_TRACE_ENABLED)
  TRACE_EVENT0("mojom", "<class 'jinja2::utils::Namespace'>::RendererPreferenceWatcher::NotifyUpdate");
#endif
  const bool kExpectsResponse = false;
  const bool kIsSync = false;
  
  const uint32_t kFlags =
      ((kExpectsResponse) ? mojo::Message::kFlagExpectsResponse : 0) |
      ((kIsSync) ? mojo::Message::kFlagIsSync : 0);
  
  mojo::Message message(
      internal::kRendererPreferenceWatcher_NotifyUpdate_Name, kFlags, 0, 0, nullptr);
  auto* buffer = message.payload_buffer();
  ::blink::mojom::internal::RendererPreferenceWatcher_NotifyUpdate_Params_Data::BufferWriter
      params;
  mojo::internal::SerializationContext serialization_context;
  params.Allocate(buffer);
  typename decltype(params->new_prefs)::BaseType::BufferWriter
      new_prefs_writer;
  mojo::internal::Serialize<::blink::mojom::RendererPreferencesDataView>(
      in_new_prefs, buffer, &new_prefs_writer, &serialization_context);
  params->new_prefs.Set(
      new_prefs_writer.is_null() ? nullptr : new_prefs_writer.data());
  MOJO_INTERNAL_DLOG_SERIALIZATION_WARNING(
      params->new_prefs.is_null(),
      mojo::internal::VALIDATION_ERROR_UNEXPECTED_NULL_POINTER,
      "null new_prefs in RendererPreferenceWatcher.NotifyUpdate request");
  message.AttachHandlesFromSerializationContext(
      &serialization_context);

#if defined(ENABLE_IPC_FUZZER)
  message.set_interface_name(RendererPreferenceWatcher::Name_);
  message.set_method_name("NotifyUpdate");
#endif
  // This return value may be ignored as false implies the Connector has
  // encountered an error, which will be visible through other means.
  ignore_result(receiver_->Accept(&message));
}

// static
bool RendererPreferenceWatcherStubDispatch::Accept(
    RendererPreferenceWatcher* impl,
    mojo::Message* message) {
  switch (message->header()->name) {
    case internal::kRendererPreferenceWatcher_NotifyUpdate_Name: {
#if BUILDFLAG(MOJO_TRACE_ENABLED)
      TRACE_EVENT1(
          "mojom",
          "(Impl)<class 'jinja2::utils::Namespace'>::RendererPreferenceWatcher::NotifyUpdate",
          "message", message->name());
#endif
      base::TaskAnnotator::ScopedSetIpcProgramCounter scoped_ipc_pc(
          base::GetProgramCounter());
      mojo::internal::MessageDispatchContext context(message);

      DCHECK(message->is_serialized());
      internal::RendererPreferenceWatcher_NotifyUpdate_Params_Data* params =
          reinterpret_cast<internal::RendererPreferenceWatcher_NotifyUpdate_Params_Data*>(
              message->mutable_payload());
      
      mojo::internal::SerializationContext serialization_context;
      serialization_context.TakeHandlesFromMessage(message);
      bool success = true;
      ::blink::mojom::RendererPreferencesPtr p_new_prefs{};
      RendererPreferenceWatcher_NotifyUpdate_ParamsDataView input_data_view(params, &serialization_context);
      
      if (!input_data_view.ReadNewPrefs(&p_new_prefs))
        success = false;
      if (!success) {
        ReportValidationErrorForMessage(
            message,
            mojo::internal::VALIDATION_ERROR_DESERIALIZATION_FAILED,
            RendererPreferenceWatcher::Name_, 0, false);
        return false;
      }
      // A null |impl| means no implementation was bound.
      DCHECK(impl);
      impl->NotifyUpdate(
std::move(p_new_prefs));
      return true;
    }
  }
  return false;
}

// static
bool RendererPreferenceWatcherStubDispatch::AcceptWithResponder(
    RendererPreferenceWatcher* impl,
    mojo::Message* message,
    std::unique_ptr<mojo::MessageReceiverWithStatus> responder) {
  switch (message->header()->name) {
    case internal::kRendererPreferenceWatcher_NotifyUpdate_Name: {
      break;
    }
  }
  return false;
}

bool RendererPreferenceWatcherRequestValidator::Accept(mojo::Message* message) {
  if (!message->is_serialized() ||
      mojo::internal::ControlMessageHandler::IsControlMessage(message)) {
    return true;
  }

  mojo::internal::ValidationContext validation_context(
    message->payload(), message->payload_num_bytes(),
    message->handles()->size(), message->payload_num_interface_ids(), message,
    "RendererPreferenceWatcher RequestValidator");

  switch (message->header()->name) {
    case internal::kRendererPreferenceWatcher_NotifyUpdate_Name: {
      if (!mojo::internal::ValidateMessageIsRequestWithoutResponse(
              message, &validation_context)) {
        return false;
      }
      if (!mojo::internal::ValidateMessagePayload<
               internal::RendererPreferenceWatcher_NotifyUpdate_Params_Data>(
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

}  // namespace mojo

#if defined(__clang__)
#pragma clang diagnostic pop
#elif defined(_MSC_VER)
#pragma warning(pop)
#endif