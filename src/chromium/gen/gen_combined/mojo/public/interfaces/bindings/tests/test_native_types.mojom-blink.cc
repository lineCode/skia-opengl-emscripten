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

#include "mojo/public/interfaces/bindings/tests/test_native_types.mojom-blink.h"

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

#include "mojo/public/interfaces/bindings/tests/test_native_types.mojom-params-data.h"
#include "mojo/public/interfaces/bindings/tests/test_native_types.mojom-shared-message-ids.h"

#include "mojo/public/interfaces/bindings/tests/test_native_types.mojom-blink-import-headers.h"
#include "mojo/public/cpp/bindings/lib/wtf_serialization.h"


#ifndef MOJO_PUBLIC_INTERFACES_BINDINGS_TESTS_TEST_NATIVE_TYPES_MOJOM_BLINK_JUMBO_H_
#define MOJO_PUBLIC_INTERFACES_BINDINGS_TESTS_TEST_NATIVE_TYPES_MOJOM_BLINK_JUMBO_H_
#include "mojo/public/cpp/bindings/tests/rect_blink_traits.h"
#include "mojo/public/cpp/bindings/tests/shared_rect_traits.h"
#endif
namespace mojo {
namespace test {
namespace blink {
PickleContainer::PickleContainer()
    : f_struct(),
      f_enum() {}

PickleContainer::PickleContainer(
    mojo::test::PickledStructBlink f_struct_in,
    mojo::test::PickledEnumBlink f_enum_in)
    : f_struct(std::move(f_struct_in)),
      f_enum(std::move(f_enum_in)) {}

PickleContainer::~PickleContainer() = default;

bool PickleContainer::Validate(
    const void* data,
    mojo::internal::ValidationContext* validation_context) {
  return Data_::Validate(data, validation_context);
}
const char PicklePasser::Name_[] = "mojo.test.PicklePasser";

class PicklePasser_PassPickledStruct_ForwardToCallback
    : public mojo::MessageReceiver {
 public:
  PicklePasser_PassPickledStruct_ForwardToCallback(
      PicklePasser::PassPickledStructCallback callback
      ) : callback_(std::move(callback)) {
  }
  bool Accept(mojo::Message* message) override;
 private:
  PicklePasser::PassPickledStructCallback callback_;
  DISALLOW_COPY_AND_ASSIGN(PicklePasser_PassPickledStruct_ForwardToCallback);
};

class PicklePasser_PassPickledEnum_ForwardToCallback
    : public mojo::MessageReceiver {
 public:
  PicklePasser_PassPickledEnum_ForwardToCallback(
      PicklePasser::PassPickledEnumCallback callback
      ) : callback_(std::move(callback)) {
  }
  bool Accept(mojo::Message* message) override;
 private:
  PicklePasser::PassPickledEnumCallback callback_;
  DISALLOW_COPY_AND_ASSIGN(PicklePasser_PassPickledEnum_ForwardToCallback);
};

class PicklePasser_PassPickleContainer_ForwardToCallback
    : public mojo::MessageReceiver {
 public:
  PicklePasser_PassPickleContainer_ForwardToCallback(
      PicklePasser::PassPickleContainerCallback callback
      ) : callback_(std::move(callback)) {
  }
  bool Accept(mojo::Message* message) override;
 private:
  PicklePasser::PassPickleContainerCallback callback_;
  DISALLOW_COPY_AND_ASSIGN(PicklePasser_PassPickleContainer_ForwardToCallback);
};

class PicklePasser_PassPickles_ForwardToCallback
    : public mojo::MessageReceiver {
 public:
  PicklePasser_PassPickles_ForwardToCallback(
      PicklePasser::PassPicklesCallback callback
      ) : callback_(std::move(callback)) {
  }
  bool Accept(mojo::Message* message) override;
 private:
  PicklePasser::PassPicklesCallback callback_;
  DISALLOW_COPY_AND_ASSIGN(PicklePasser_PassPickles_ForwardToCallback);
};

class PicklePasser_PassPickleArrays_ForwardToCallback
    : public mojo::MessageReceiver {
 public:
  PicklePasser_PassPickleArrays_ForwardToCallback(
      PicklePasser::PassPickleArraysCallback callback
      ) : callback_(std::move(callback)) {
  }
  bool Accept(mojo::Message* message) override;
 private:
  PicklePasser::PassPickleArraysCallback callback_;
  DISALLOW_COPY_AND_ASSIGN(PicklePasser_PassPickleArrays_ForwardToCallback);
};

PicklePasserProxy::PicklePasserProxy(mojo::MessageReceiverWithResponder* receiver)
    : receiver_(receiver) {
}
class PicklePasserProxy_PassPickledStruct_Message
    : public mojo::internal::UnserializedMessageContext {
 public:
  static const mojo::internal::UnserializedMessageContext::Tag kMessageTag;

  explicit PicklePasserProxy_PassPickledStruct_Message(
      uint32_t message_flags
      , mojo::test::PickledStructBlink param_pickle
  )
      : mojo::internal::UnserializedMessageContext(
          &kMessageTag,
          internal::kPicklePasser_PassPickledStruct_Name,
          message_flags)
      , param_pickle_(std::move(param_pickle)){}
  ~PicklePasserProxy_PassPickledStruct_Message() override = default;

  static mojo::Message Build(
      bool serialize,
      bool expects_response,
      bool is_sync,
      mojo::test::PickledStructBlink param_pickle) {

    
  const uint32_t kFlags =
      ((expects_response) ? mojo::Message::kFlagExpectsResponse : 0) |
      ((is_sync) ? mojo::Message::kFlagIsSync : 0);

    if (!serialize) {
      return mojo::Message(std::make_unique<PicklePasserProxy_PassPickledStruct_Message>(
          kFlags
          , std::move(param_pickle)
          ));
    }

    DCHECK(serialize);
    
  mojo::Message message(
      internal::kPicklePasser_PassPickledStruct_Name, kFlags, 0, 0, nullptr);
  auto* buffer = message.payload_buffer();
  ::mojo::test::internal::PicklePasser_PassPickledStruct_Params_Data::BufferWriter
      params;
  mojo::internal::SerializationContext serialization_context;
  params.Allocate(buffer);
  typename decltype(params->pickle)::BaseType::BufferWriter
      pickle_writer;
  mojo::internal::Serialize<::mojo::test::PickledStructDataView>(
      param_pickle, buffer, &pickle_writer, &serialization_context);
  params->pickle.Set(
      pickle_writer.is_null() ? nullptr : pickle_writer.data());
  MOJO_INTERNAL_DLOG_SERIALIZATION_WARNING(
      params->pickle.is_null(),
      mojo::internal::VALIDATION_ERROR_UNEXPECTED_NULL_POINTER,
      "null pickle in PicklePasser.PassPickledStruct request");
  message.AttachHandlesFromSerializationContext(
      &serialization_context);
    return message;
  }


  void Dispatch(PicklePasser* impl, PicklePasser::PassPickledStructCallback callback) {
    impl->PassPickledStruct(
        std::move(param_pickle_), std::move(callback));
  }

 private:
  // mojo::internal::UnserializedMessageContext:
  void Serialize(mojo::internal::SerializationContext* serialization_context,
                 mojo::internal::Buffer* buffer) override {
    ::mojo::test::internal::PicklePasser_PassPickledStruct_Params_Data::BufferWriter
        params;
    params.Allocate(buffer);
  typename decltype(params->pickle)::BaseType::BufferWriter
      pickle_writer;
  mojo::internal::Serialize<::mojo::test::PickledStructDataView>(
      param_pickle_, buffer, &pickle_writer, serialization_context);
  params->pickle.Set(
      pickle_writer.is_null() ? nullptr : pickle_writer.data());
  MOJO_INTERNAL_DLOG_SERIALIZATION_WARNING(
      params->pickle.is_null(),
      mojo::internal::VALIDATION_ERROR_UNEXPECTED_NULL_POINTER,
      "null pickle in PicklePasser.PassPickledStruct request");
  }
  mojo::test::PickledStructBlink param_pickle_;

  DISALLOW_COPY_AND_ASSIGN(PicklePasserProxy_PassPickledStruct_Message);
};

const mojo::internal::UnserializedMessageContext::Tag
PicklePasserProxy_PassPickledStruct_Message::kMessageTag = {};

void PicklePasserProxy::PassPickledStruct(
    mojo::test::PickledStructBlink in_pickle, PassPickledStructCallback callback) {
#if BUILDFLAG(MOJO_TRACE_ENABLED)
  TRACE_EVENT0("mojom", "<class 'jinja2::utils::Namespace'>::PicklePasser::PassPickledStruct");
#endif
  const bool kExpectsResponse = true;
  const bool kIsSync = false;
  const bool kSerialize = receiver_->PrefersSerializedMessages();
  auto message = PicklePasserProxy_PassPickledStruct_Message::Build(
      kSerialize, kExpectsResponse, kIsSync, std::move(in_pickle));

#if defined(ENABLE_IPC_FUZZER)
  message.set_interface_name(PicklePasser::Name_);
  message.set_method_name("PassPickledStruct");
#endif
  std::unique_ptr<mojo::MessageReceiver> responder(
      new PicklePasser_PassPickledStruct_ForwardToCallback(
          std::move(callback)));
  ignore_result(receiver_->AcceptWithResponder(&message, std::move(responder)));
}
class PicklePasserProxy_PassPickledEnum_Message
    : public mojo::internal::UnserializedMessageContext {
 public:
  static const mojo::internal::UnserializedMessageContext::Tag kMessageTag;

  explicit PicklePasserProxy_PassPickledEnum_Message(
      uint32_t message_flags
      , mojo::test::PickledEnumBlink param_pickle
  )
      : mojo::internal::UnserializedMessageContext(
          &kMessageTag,
          internal::kPicklePasser_PassPickledEnum_Name,
          message_flags)
      , param_pickle_(std::move(param_pickle)){}
  ~PicklePasserProxy_PassPickledEnum_Message() override = default;

  static mojo::Message Build(
      bool serialize,
      bool expects_response,
      bool is_sync,
      mojo::test::PickledEnumBlink param_pickle) {

    
  const uint32_t kFlags =
      ((expects_response) ? mojo::Message::kFlagExpectsResponse : 0) |
      ((is_sync) ? mojo::Message::kFlagIsSync : 0);

    if (!serialize) {
      return mojo::Message(std::make_unique<PicklePasserProxy_PassPickledEnum_Message>(
          kFlags
          , std::move(param_pickle)
          ));
    }

    DCHECK(serialize);
    
  mojo::Message message(
      internal::kPicklePasser_PassPickledEnum_Name, kFlags, 0, 0, nullptr);
  auto* buffer = message.payload_buffer();
  ::mojo::test::internal::PicklePasser_PassPickledEnum_Params_Data::BufferWriter
      params;
  mojo::internal::SerializationContext serialization_context;
  params.Allocate(buffer);
  mojo::internal::Serialize<::mojo::test::PickledEnum>(
      param_pickle, &params->pickle);
  message.AttachHandlesFromSerializationContext(
      &serialization_context);
    return message;
  }


  void Dispatch(PicklePasser* impl, PicklePasser::PassPickledEnumCallback callback) {
    impl->PassPickledEnum(
        std::move(param_pickle_), std::move(callback));
  }

 private:
  // mojo::internal::UnserializedMessageContext:
  void Serialize(mojo::internal::SerializationContext* serialization_context,
                 mojo::internal::Buffer* buffer) override {
    ::mojo::test::internal::PicklePasser_PassPickledEnum_Params_Data::BufferWriter
        params;
    params.Allocate(buffer);
  mojo::internal::Serialize<::mojo::test::PickledEnum>(
      param_pickle_, &params->pickle);
  }
  mojo::test::PickledEnumBlink param_pickle_;

  DISALLOW_COPY_AND_ASSIGN(PicklePasserProxy_PassPickledEnum_Message);
};

const mojo::internal::UnserializedMessageContext::Tag
PicklePasserProxy_PassPickledEnum_Message::kMessageTag = {};

void PicklePasserProxy::PassPickledEnum(
    mojo::test::PickledEnumBlink in_pickle, PassPickledEnumCallback callback) {
#if BUILDFLAG(MOJO_TRACE_ENABLED)
  TRACE_EVENT0("mojom", "<class 'jinja2::utils::Namespace'>::PicklePasser::PassPickledEnum");
#endif
  const bool kExpectsResponse = true;
  const bool kIsSync = false;
  const bool kSerialize = receiver_->PrefersSerializedMessages();
  auto message = PicklePasserProxy_PassPickledEnum_Message::Build(
      kSerialize, kExpectsResponse, kIsSync, std::move(in_pickle));

#if defined(ENABLE_IPC_FUZZER)
  message.set_interface_name(PicklePasser::Name_);
  message.set_method_name("PassPickledEnum");
#endif
  std::unique_ptr<mojo::MessageReceiver> responder(
      new PicklePasser_PassPickledEnum_ForwardToCallback(
          std::move(callback)));
  ignore_result(receiver_->AcceptWithResponder(&message, std::move(responder)));
}
class PicklePasserProxy_PassPickleContainer_Message
    : public mojo::internal::UnserializedMessageContext {
 public:
  static const mojo::internal::UnserializedMessageContext::Tag kMessageTag;

  explicit PicklePasserProxy_PassPickleContainer_Message(
      uint32_t message_flags
      , PickleContainerPtr param_container
  )
      : mojo::internal::UnserializedMessageContext(
          &kMessageTag,
          internal::kPicklePasser_PassPickleContainer_Name,
          message_flags)
      , param_container_(std::move(param_container)){}
  ~PicklePasserProxy_PassPickleContainer_Message() override = default;

  static mojo::Message Build(
      bool serialize,
      bool expects_response,
      bool is_sync,
      PickleContainerPtr param_container) {

    
  const uint32_t kFlags =
      ((expects_response) ? mojo::Message::kFlagExpectsResponse : 0) |
      ((is_sync) ? mojo::Message::kFlagIsSync : 0);

    if (!serialize) {
      return mojo::Message(std::make_unique<PicklePasserProxy_PassPickleContainer_Message>(
          kFlags
          , std::move(param_container)
          ));
    }

    DCHECK(serialize);
    
  mojo::Message message(
      internal::kPicklePasser_PassPickleContainer_Name, kFlags, 0, 0, nullptr);
  auto* buffer = message.payload_buffer();
  ::mojo::test::internal::PicklePasser_PassPickleContainer_Params_Data::BufferWriter
      params;
  mojo::internal::SerializationContext serialization_context;
  params.Allocate(buffer);
  typename decltype(params->container)::BaseType::BufferWriter
      container_writer;
  mojo::internal::Serialize<::mojo::test::PickleContainerDataView>(
      param_container, buffer, &container_writer, &serialization_context);
  params->container.Set(
      container_writer.is_null() ? nullptr : container_writer.data());
  MOJO_INTERNAL_DLOG_SERIALIZATION_WARNING(
      params->container.is_null(),
      mojo::internal::VALIDATION_ERROR_UNEXPECTED_NULL_POINTER,
      "null container in PicklePasser.PassPickleContainer request");
  message.AttachHandlesFromSerializationContext(
      &serialization_context);
    return message;
  }


  void Dispatch(PicklePasser* impl, PicklePasser::PassPickleContainerCallback callback) {
    impl->PassPickleContainer(
        std::move(param_container_), std::move(callback));
  }

 private:
  // mojo::internal::UnserializedMessageContext:
  void Serialize(mojo::internal::SerializationContext* serialization_context,
                 mojo::internal::Buffer* buffer) override {
    ::mojo::test::internal::PicklePasser_PassPickleContainer_Params_Data::BufferWriter
        params;
    params.Allocate(buffer);
  typename decltype(params->container)::BaseType::BufferWriter
      container_writer;
  mojo::internal::Serialize<::mojo::test::PickleContainerDataView>(
      param_container_, buffer, &container_writer, serialization_context);
  params->container.Set(
      container_writer.is_null() ? nullptr : container_writer.data());
  MOJO_INTERNAL_DLOG_SERIALIZATION_WARNING(
      params->container.is_null(),
      mojo::internal::VALIDATION_ERROR_UNEXPECTED_NULL_POINTER,
      "null container in PicklePasser.PassPickleContainer request");
  }
  PickleContainerPtr param_container_;

  DISALLOW_COPY_AND_ASSIGN(PicklePasserProxy_PassPickleContainer_Message);
};

const mojo::internal::UnserializedMessageContext::Tag
PicklePasserProxy_PassPickleContainer_Message::kMessageTag = {};

void PicklePasserProxy::PassPickleContainer(
    PickleContainerPtr in_container, PassPickleContainerCallback callback) {
#if BUILDFLAG(MOJO_TRACE_ENABLED)
  TRACE_EVENT0("mojom", "<class 'jinja2::utils::Namespace'>::PicklePasser::PassPickleContainer");
#endif
  const bool kExpectsResponse = true;
  const bool kIsSync = false;
  const bool kSerialize = receiver_->PrefersSerializedMessages();
  auto message = PicklePasserProxy_PassPickleContainer_Message::Build(
      kSerialize, kExpectsResponse, kIsSync, std::move(in_container));

#if defined(ENABLE_IPC_FUZZER)
  message.set_interface_name(PicklePasser::Name_);
  message.set_method_name("PassPickleContainer");
#endif
  std::unique_ptr<mojo::MessageReceiver> responder(
      new PicklePasser_PassPickleContainer_ForwardToCallback(
          std::move(callback)));
  ignore_result(receiver_->AcceptWithResponder(&message, std::move(responder)));
}
class PicklePasserProxy_PassPickles_Message
    : public mojo::internal::UnserializedMessageContext {
 public:
  static const mojo::internal::UnserializedMessageContext::Tag kMessageTag;

  explicit PicklePasserProxy_PassPickles_Message(
      uint32_t message_flags
      , WTF::Vector<mojo::test::PickledStructBlink> param_pickles
  )
      : mojo::internal::UnserializedMessageContext(
          &kMessageTag,
          internal::kPicklePasser_PassPickles_Name,
          message_flags)
      , param_pickles_(std::move(param_pickles)){}
  ~PicklePasserProxy_PassPickles_Message() override = default;

  static mojo::Message Build(
      bool serialize,
      bool expects_response,
      bool is_sync,
      WTF::Vector<mojo::test::PickledStructBlink> param_pickles) {

    
  const uint32_t kFlags =
      ((expects_response) ? mojo::Message::kFlagExpectsResponse : 0) |
      ((is_sync) ? mojo::Message::kFlagIsSync : 0);

    if (!serialize) {
      return mojo::Message(std::make_unique<PicklePasserProxy_PassPickles_Message>(
          kFlags
          , std::move(param_pickles)
          ));
    }

    DCHECK(serialize);
    
  mojo::Message message(
      internal::kPicklePasser_PassPickles_Name, kFlags, 0, 0, nullptr);
  auto* buffer = message.payload_buffer();
  ::mojo::test::internal::PicklePasser_PassPickles_Params_Data::BufferWriter
      params;
  mojo::internal::SerializationContext serialization_context;
  params.Allocate(buffer);
  typename decltype(params->pickles)::BaseType::BufferWriter
      pickles_writer;
  const mojo::internal::ContainerValidateParams pickles_validate_params(
      0, false, nullptr);
  mojo::internal::Serialize<mojo::ArrayDataView<::mojo::test::PickledStructDataView>>(
      param_pickles, buffer, &pickles_writer, &pickles_validate_params,
      &serialization_context);
  params->pickles.Set(
      pickles_writer.is_null() ? nullptr : pickles_writer.data());
  MOJO_INTERNAL_DLOG_SERIALIZATION_WARNING(
      params->pickles.is_null(),
      mojo::internal::VALIDATION_ERROR_UNEXPECTED_NULL_POINTER,
      "null pickles in PicklePasser.PassPickles request");
  message.AttachHandlesFromSerializationContext(
      &serialization_context);
    return message;
  }


  void Dispatch(PicklePasser* impl, PicklePasser::PassPicklesCallback callback) {
    impl->PassPickles(
        std::move(param_pickles_), std::move(callback));
  }

 private:
  // mojo::internal::UnserializedMessageContext:
  void Serialize(mojo::internal::SerializationContext* serialization_context,
                 mojo::internal::Buffer* buffer) override {
    ::mojo::test::internal::PicklePasser_PassPickles_Params_Data::BufferWriter
        params;
    params.Allocate(buffer);
  typename decltype(params->pickles)::BaseType::BufferWriter
      pickles_writer;
  const mojo::internal::ContainerValidateParams pickles_validate_params(
      0, false, nullptr);
  mojo::internal::Serialize<mojo::ArrayDataView<::mojo::test::PickledStructDataView>>(
      param_pickles_, buffer, &pickles_writer, &pickles_validate_params,
      serialization_context);
  params->pickles.Set(
      pickles_writer.is_null() ? nullptr : pickles_writer.data());
  MOJO_INTERNAL_DLOG_SERIALIZATION_WARNING(
      params->pickles.is_null(),
      mojo::internal::VALIDATION_ERROR_UNEXPECTED_NULL_POINTER,
      "null pickles in PicklePasser.PassPickles request");
  }
  WTF::Vector<mojo::test::PickledStructBlink> param_pickles_;

  DISALLOW_COPY_AND_ASSIGN(PicklePasserProxy_PassPickles_Message);
};

const mojo::internal::UnserializedMessageContext::Tag
PicklePasserProxy_PassPickles_Message::kMessageTag = {};

void PicklePasserProxy::PassPickles(
    WTF::Vector<mojo::test::PickledStructBlink> in_pickles, PassPicklesCallback callback) {
#if BUILDFLAG(MOJO_TRACE_ENABLED)
  TRACE_EVENT0("mojom", "<class 'jinja2::utils::Namespace'>::PicklePasser::PassPickles");
#endif
  const bool kExpectsResponse = true;
  const bool kIsSync = false;
  const bool kSerialize = receiver_->PrefersSerializedMessages();
  auto message = PicklePasserProxy_PassPickles_Message::Build(
      kSerialize, kExpectsResponse, kIsSync, std::move(in_pickles));

#if defined(ENABLE_IPC_FUZZER)
  message.set_interface_name(PicklePasser::Name_);
  message.set_method_name("PassPickles");
#endif
  std::unique_ptr<mojo::MessageReceiver> responder(
      new PicklePasser_PassPickles_ForwardToCallback(
          std::move(callback)));
  ignore_result(receiver_->AcceptWithResponder(&message, std::move(responder)));
}
class PicklePasserProxy_PassPickleArrays_Message
    : public mojo::internal::UnserializedMessageContext {
 public:
  static const mojo::internal::UnserializedMessageContext::Tag kMessageTag;

  explicit PicklePasserProxy_PassPickleArrays_Message(
      uint32_t message_flags
      , WTF::Vector<WTF::Vector<mojo::test::PickledStructBlink>> param_pickle_arrays
  )
      : mojo::internal::UnserializedMessageContext(
          &kMessageTag,
          internal::kPicklePasser_PassPickleArrays_Name,
          message_flags)
      , param_pickle_arrays_(std::move(param_pickle_arrays)){}
  ~PicklePasserProxy_PassPickleArrays_Message() override = default;

  static mojo::Message Build(
      bool serialize,
      bool expects_response,
      bool is_sync,
      WTF::Vector<WTF::Vector<mojo::test::PickledStructBlink>> param_pickle_arrays) {

    
  const uint32_t kFlags =
      ((expects_response) ? mojo::Message::kFlagExpectsResponse : 0) |
      ((is_sync) ? mojo::Message::kFlagIsSync : 0);

    if (!serialize) {
      return mojo::Message(std::make_unique<PicklePasserProxy_PassPickleArrays_Message>(
          kFlags
          , std::move(param_pickle_arrays)
          ));
    }

    DCHECK(serialize);
    
  mojo::Message message(
      internal::kPicklePasser_PassPickleArrays_Name, kFlags, 0, 0, nullptr);
  auto* buffer = message.payload_buffer();
  ::mojo::test::internal::PicklePasser_PassPickleArrays_Params_Data::BufferWriter
      params;
  mojo::internal::SerializationContext serialization_context;
  params.Allocate(buffer);
  typename decltype(params->pickle_arrays)::BaseType::BufferWriter
      pickle_arrays_writer;
  const mojo::internal::ContainerValidateParams pickle_arrays_validate_params(
      0, false, new mojo::internal::ContainerValidateParams(0, false, nullptr));
  mojo::internal::Serialize<mojo::ArrayDataView<mojo::ArrayDataView<::mojo::test::PickledStructDataView>>>(
      param_pickle_arrays, buffer, &pickle_arrays_writer, &pickle_arrays_validate_params,
      &serialization_context);
  params->pickle_arrays.Set(
      pickle_arrays_writer.is_null() ? nullptr : pickle_arrays_writer.data());
  MOJO_INTERNAL_DLOG_SERIALIZATION_WARNING(
      params->pickle_arrays.is_null(),
      mojo::internal::VALIDATION_ERROR_UNEXPECTED_NULL_POINTER,
      "null pickle_arrays in PicklePasser.PassPickleArrays request");
  message.AttachHandlesFromSerializationContext(
      &serialization_context);
    return message;
  }


  void Dispatch(PicklePasser* impl, PicklePasser::PassPickleArraysCallback callback) {
    impl->PassPickleArrays(
        std::move(param_pickle_arrays_), std::move(callback));
  }

 private:
  // mojo::internal::UnserializedMessageContext:
  void Serialize(mojo::internal::SerializationContext* serialization_context,
                 mojo::internal::Buffer* buffer) override {
    ::mojo::test::internal::PicklePasser_PassPickleArrays_Params_Data::BufferWriter
        params;
    params.Allocate(buffer);
  typename decltype(params->pickle_arrays)::BaseType::BufferWriter
      pickle_arrays_writer;
  const mojo::internal::ContainerValidateParams pickle_arrays_validate_params(
      0, false, new mojo::internal::ContainerValidateParams(0, false, nullptr));
  mojo::internal::Serialize<mojo::ArrayDataView<mojo::ArrayDataView<::mojo::test::PickledStructDataView>>>(
      param_pickle_arrays_, buffer, &pickle_arrays_writer, &pickle_arrays_validate_params,
      serialization_context);
  params->pickle_arrays.Set(
      pickle_arrays_writer.is_null() ? nullptr : pickle_arrays_writer.data());
  MOJO_INTERNAL_DLOG_SERIALIZATION_WARNING(
      params->pickle_arrays.is_null(),
      mojo::internal::VALIDATION_ERROR_UNEXPECTED_NULL_POINTER,
      "null pickle_arrays in PicklePasser.PassPickleArrays request");
  }
  WTF::Vector<WTF::Vector<mojo::test::PickledStructBlink>> param_pickle_arrays_;

  DISALLOW_COPY_AND_ASSIGN(PicklePasserProxy_PassPickleArrays_Message);
};

const mojo::internal::UnserializedMessageContext::Tag
PicklePasserProxy_PassPickleArrays_Message::kMessageTag = {};

void PicklePasserProxy::PassPickleArrays(
    WTF::Vector<WTF::Vector<mojo::test::PickledStructBlink>> in_pickle_arrays, PassPickleArraysCallback callback) {
#if BUILDFLAG(MOJO_TRACE_ENABLED)
  TRACE_EVENT0("mojom", "<class 'jinja2::utils::Namespace'>::PicklePasser::PassPickleArrays");
#endif
  const bool kExpectsResponse = true;
  const bool kIsSync = false;
  const bool kSerialize = receiver_->PrefersSerializedMessages();
  auto message = PicklePasserProxy_PassPickleArrays_Message::Build(
      kSerialize, kExpectsResponse, kIsSync, std::move(in_pickle_arrays));

#if defined(ENABLE_IPC_FUZZER)
  message.set_interface_name(PicklePasser::Name_);
  message.set_method_name("PassPickleArrays");
#endif
  std::unique_ptr<mojo::MessageReceiver> responder(
      new PicklePasser_PassPickleArrays_ForwardToCallback(
          std::move(callback)));
  ignore_result(receiver_->AcceptWithResponder(&message, std::move(responder)));
}
class PicklePasser_PassPickledStruct_ProxyToResponder {
 public:
  static PicklePasser::PassPickledStructCallback CreateCallback(
      uint64_t request_id,
      bool is_sync,
      std::unique_ptr<mojo::MessageReceiverWithStatus> responder) {
    std::unique_ptr<PicklePasser_PassPickledStruct_ProxyToResponder> proxy(
        new PicklePasser_PassPickledStruct_ProxyToResponder(
            request_id, is_sync, std::move(responder)));
    return base::BindOnce(&PicklePasser_PassPickledStruct_ProxyToResponder::Run,
                          std::move(proxy));
  }

  ~PicklePasser_PassPickledStruct_ProxyToResponder() {
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
  PicklePasser_PassPickledStruct_ProxyToResponder(
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
        << "PicklePasser::PassPickledStructCallback was destroyed without "
        << "first either being run or its corresponding binding being closed. "
        << "It is an error to drop response callbacks which still correspond "
        << "to an open interface pipe.";
  }
#endif

  void Run(
      mojo::test::PickledStructBlink in_passed);

  uint64_t request_id_;
  bool is_sync_;
  std::unique_ptr<mojo::MessageReceiverWithStatus> responder_;

  DISALLOW_COPY_AND_ASSIGN(PicklePasser_PassPickledStruct_ProxyToResponder);
};
class PicklePasser_PassPickledStruct_Response_Message
    : public mojo::internal::UnserializedMessageContext {
 public:
  static const mojo::internal::UnserializedMessageContext::Tag kMessageTag;

  explicit PicklePasser_PassPickledStruct_Response_Message(
      uint32_t message_flags
      , mojo::test::PickledStructBlink param_passed
  )
      : mojo::internal::UnserializedMessageContext(
          &kMessageTag,
          internal::kPicklePasser_PassPickledStruct_Name,
          message_flags)
      , param_passed_(std::move(param_passed)){}
  ~PicklePasser_PassPickledStruct_Response_Message() override = default;

  static mojo::Message Build(
      bool serialize,
      bool is_sync,
      mojo::test::PickledStructBlink param_passed) {

    
  const uint32_t kFlags = mojo::Message::kFlagIsResponse |
      ((is_sync) ? mojo::Message::kFlagIsSync : 0);

    if (!serialize) {
      return mojo::Message(std::make_unique<PicklePasser_PassPickledStruct_Response_Message>(
          kFlags
          , std::move(param_passed)
          ));
    }

    DCHECK(serialize);
    
  mojo::Message message(
      internal::kPicklePasser_PassPickledStruct_Name, kFlags, 0, 0, nullptr);
  auto* buffer = message.payload_buffer();
  ::mojo::test::internal::PicklePasser_PassPickledStruct_ResponseParams_Data::BufferWriter
      params;
  mojo::internal::SerializationContext serialization_context;
  params.Allocate(buffer);
  typename decltype(params->passed)::BaseType::BufferWriter
      passed_writer;
  mojo::internal::Serialize<::mojo::test::PickledStructDataView>(
      param_passed, buffer, &passed_writer, &serialization_context);
  params->passed.Set(
      passed_writer.is_null() ? nullptr : passed_writer.data());
  MOJO_INTERNAL_DLOG_SERIALIZATION_WARNING(
      params->passed.is_null(),
      mojo::internal::VALIDATION_ERROR_UNEXPECTED_NULL_POINTER,
      "null passed in PicklePasser.PassPickledStruct response");
  message.AttachHandlesFromSerializationContext(
      &serialization_context);
    return message;
  }


  void Dispatch(PicklePasser::PassPickledStructCallback* callback) {
    std::move(*callback).Run(
        std::move(param_passed_));
  }



 private:
  // mojo::internal::UnserializedMessageContext:
  void Serialize(mojo::internal::SerializationContext* serialization_context,
                 mojo::internal::Buffer* buffer) override {
    ::mojo::test::internal::PicklePasser_PassPickledStruct_ResponseParams_Data::BufferWriter
        params;
    params.Allocate(buffer);
  typename decltype(params->passed)::BaseType::BufferWriter
      passed_writer;
  mojo::internal::Serialize<::mojo::test::PickledStructDataView>(
      param_passed_, buffer, &passed_writer, serialization_context);
  params->passed.Set(
      passed_writer.is_null() ? nullptr : passed_writer.data());
  MOJO_INTERNAL_DLOG_SERIALIZATION_WARNING(
      params->passed.is_null(),
      mojo::internal::VALIDATION_ERROR_UNEXPECTED_NULL_POINTER,
      "null passed in PicklePasser.PassPickledStruct response");
  }
  mojo::test::PickledStructBlink param_passed_;

  DISALLOW_COPY_AND_ASSIGN(PicklePasser_PassPickledStruct_Response_Message);
};

const mojo::internal::UnserializedMessageContext::Tag
PicklePasser_PassPickledStruct_Response_Message::kMessageTag = {};

bool PicklePasser_PassPickledStruct_ForwardToCallback::Accept(
    mojo::Message* message) {
#if BUILDFLAG(MOJO_TRACE_ENABLED)
  TRACE_EVENT1("mojom", "<class 'jinja2::utils::Namespace'>::PicklePasser::PassPickledStructCallback",
               "message", message->name());
#endif
  mojo::internal::MessageDispatchContext dispatch_context(message);
  if (!message->is_serialized()) {
    auto context =
        message->TakeUnserializedContext<PicklePasser_PassPickledStruct_Response_Message>();
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
  internal::PicklePasser_PassPickledStruct_ResponseParams_Data* params =
      reinterpret_cast<
          internal::PicklePasser_PassPickledStruct_ResponseParams_Data*>(
              message->mutable_payload());
  
  mojo::internal::SerializationContext serialization_context;
  serialization_context.TakeHandlesFromMessage(message);
  bool success = true;
  mojo::test::PickledStructBlink p_passed{};
  PicklePasser_PassPickledStruct_ResponseParamsDataView input_data_view(params, &serialization_context);
  
  if (!input_data_view.ReadPassed(&p_passed))
    success = false;
  if (!success) {
    ReportValidationErrorForMessage(
        message,
        mojo::internal::VALIDATION_ERROR_DESERIALIZATION_FAILED,
        PicklePasser::Name_, 0, true);
    return false;
  }
  if (!callback_.is_null())
    std::move(callback_).Run(
std::move(p_passed));
  return true;
}

void PicklePasser_PassPickledStruct_ProxyToResponder::Run(
    mojo::test::PickledStructBlink in_passed) {
  const bool kSerialize = responder_->PrefersSerializedMessages();
  auto message = PicklePasser_PassPickledStruct_Response_Message::Build(kSerialize, is_sync_, std::move(in_passed));

#if BUILDFLAG(MOJO_TRACE_ENABLED)
  TRACE_EVENT1("mojom", "(Impl)<class 'jinja2::utils::Namespace'>::PicklePasser::PassPickledStructCallback",
               "message", message.name());
#endif

#if defined(ENABLE_IPC_FUZZER)
  message.set_interface_name(PicklePasser::Name_);
  message.set_method_name("PassPickledStruct");
#endif

  message.set_request_id(request_id_);
  ignore_result(responder_->Accept(&message));
  // TODO(darin): Accept() returning false indicates a malformed message, and
  // that may be good reason to close the connection. However, we don't have a
  // way to do that from here. We should add a way.
  responder_ = nullptr;
}
class PicklePasser_PassPickledEnum_ProxyToResponder {
 public:
  static PicklePasser::PassPickledEnumCallback CreateCallback(
      uint64_t request_id,
      bool is_sync,
      std::unique_ptr<mojo::MessageReceiverWithStatus> responder) {
    std::unique_ptr<PicklePasser_PassPickledEnum_ProxyToResponder> proxy(
        new PicklePasser_PassPickledEnum_ProxyToResponder(
            request_id, is_sync, std::move(responder)));
    return base::BindOnce(&PicklePasser_PassPickledEnum_ProxyToResponder::Run,
                          std::move(proxy));
  }

  ~PicklePasser_PassPickledEnum_ProxyToResponder() {
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
  PicklePasser_PassPickledEnum_ProxyToResponder(
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
        << "PicklePasser::PassPickledEnumCallback was destroyed without "
        << "first either being run or its corresponding binding being closed. "
        << "It is an error to drop response callbacks which still correspond "
        << "to an open interface pipe.";
  }
#endif

  void Run(
      mojo::test::PickledEnumBlink in_passed);

  uint64_t request_id_;
  bool is_sync_;
  std::unique_ptr<mojo::MessageReceiverWithStatus> responder_;

  DISALLOW_COPY_AND_ASSIGN(PicklePasser_PassPickledEnum_ProxyToResponder);
};
class PicklePasser_PassPickledEnum_Response_Message
    : public mojo::internal::UnserializedMessageContext {
 public:
  static const mojo::internal::UnserializedMessageContext::Tag kMessageTag;

  explicit PicklePasser_PassPickledEnum_Response_Message(
      uint32_t message_flags
      , mojo::test::PickledEnumBlink param_passed
  )
      : mojo::internal::UnserializedMessageContext(
          &kMessageTag,
          internal::kPicklePasser_PassPickledEnum_Name,
          message_flags)
      , param_passed_(std::move(param_passed)){}
  ~PicklePasser_PassPickledEnum_Response_Message() override = default;

  static mojo::Message Build(
      bool serialize,
      bool is_sync,
      mojo::test::PickledEnumBlink param_passed) {

    
  const uint32_t kFlags = mojo::Message::kFlagIsResponse |
      ((is_sync) ? mojo::Message::kFlagIsSync : 0);

    if (!serialize) {
      return mojo::Message(std::make_unique<PicklePasser_PassPickledEnum_Response_Message>(
          kFlags
          , std::move(param_passed)
          ));
    }

    DCHECK(serialize);
    
  mojo::Message message(
      internal::kPicklePasser_PassPickledEnum_Name, kFlags, 0, 0, nullptr);
  auto* buffer = message.payload_buffer();
  ::mojo::test::internal::PicklePasser_PassPickledEnum_ResponseParams_Data::BufferWriter
      params;
  mojo::internal::SerializationContext serialization_context;
  params.Allocate(buffer);
  mojo::internal::Serialize<::mojo::test::PickledEnum>(
      param_passed, &params->passed);
  message.AttachHandlesFromSerializationContext(
      &serialization_context);
    return message;
  }


  void Dispatch(PicklePasser::PassPickledEnumCallback* callback) {
    std::move(*callback).Run(
        std::move(param_passed_));
  }



 private:
  // mojo::internal::UnserializedMessageContext:
  void Serialize(mojo::internal::SerializationContext* serialization_context,
                 mojo::internal::Buffer* buffer) override {
    ::mojo::test::internal::PicklePasser_PassPickledEnum_ResponseParams_Data::BufferWriter
        params;
    params.Allocate(buffer);
  mojo::internal::Serialize<::mojo::test::PickledEnum>(
      param_passed_, &params->passed);
  }
  mojo::test::PickledEnumBlink param_passed_;

  DISALLOW_COPY_AND_ASSIGN(PicklePasser_PassPickledEnum_Response_Message);
};

const mojo::internal::UnserializedMessageContext::Tag
PicklePasser_PassPickledEnum_Response_Message::kMessageTag = {};

bool PicklePasser_PassPickledEnum_ForwardToCallback::Accept(
    mojo::Message* message) {
#if BUILDFLAG(MOJO_TRACE_ENABLED)
  TRACE_EVENT1("mojom", "<class 'jinja2::utils::Namespace'>::PicklePasser::PassPickledEnumCallback",
               "message", message->name());
#endif
  mojo::internal::MessageDispatchContext dispatch_context(message);
  if (!message->is_serialized()) {
    auto context =
        message->TakeUnserializedContext<PicklePasser_PassPickledEnum_Response_Message>();
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
  internal::PicklePasser_PassPickledEnum_ResponseParams_Data* params =
      reinterpret_cast<
          internal::PicklePasser_PassPickledEnum_ResponseParams_Data*>(
              message->mutable_payload());
  
  mojo::internal::SerializationContext serialization_context;
  serialization_context.TakeHandlesFromMessage(message);
  bool success = true;
  mojo::test::PickledEnumBlink p_passed{};
  PicklePasser_PassPickledEnum_ResponseParamsDataView input_data_view(params, &serialization_context);
  
  if (!input_data_view.ReadPassed(&p_passed))
    success = false;
  if (!success) {
    ReportValidationErrorForMessage(
        message,
        mojo::internal::VALIDATION_ERROR_DESERIALIZATION_FAILED,
        PicklePasser::Name_, 1, true);
    return false;
  }
  if (!callback_.is_null())
    std::move(callback_).Run(
std::move(p_passed));
  return true;
}

void PicklePasser_PassPickledEnum_ProxyToResponder::Run(
    mojo::test::PickledEnumBlink in_passed) {
  const bool kSerialize = responder_->PrefersSerializedMessages();
  auto message = PicklePasser_PassPickledEnum_Response_Message::Build(kSerialize, is_sync_, std::move(in_passed));

#if BUILDFLAG(MOJO_TRACE_ENABLED)
  TRACE_EVENT1("mojom", "(Impl)<class 'jinja2::utils::Namespace'>::PicklePasser::PassPickledEnumCallback",
               "message", message.name());
#endif

#if defined(ENABLE_IPC_FUZZER)
  message.set_interface_name(PicklePasser::Name_);
  message.set_method_name("PassPickledEnum");
#endif

  message.set_request_id(request_id_);
  ignore_result(responder_->Accept(&message));
  // TODO(darin): Accept() returning false indicates a malformed message, and
  // that may be good reason to close the connection. However, we don't have a
  // way to do that from here. We should add a way.
  responder_ = nullptr;
}
class PicklePasser_PassPickleContainer_ProxyToResponder {
 public:
  static PicklePasser::PassPickleContainerCallback CreateCallback(
      uint64_t request_id,
      bool is_sync,
      std::unique_ptr<mojo::MessageReceiverWithStatus> responder) {
    std::unique_ptr<PicklePasser_PassPickleContainer_ProxyToResponder> proxy(
        new PicklePasser_PassPickleContainer_ProxyToResponder(
            request_id, is_sync, std::move(responder)));
    return base::BindOnce(&PicklePasser_PassPickleContainer_ProxyToResponder::Run,
                          std::move(proxy));
  }

  ~PicklePasser_PassPickleContainer_ProxyToResponder() {
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
  PicklePasser_PassPickleContainer_ProxyToResponder(
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
        << "PicklePasser::PassPickleContainerCallback was destroyed without "
        << "first either being run or its corresponding binding being closed. "
        << "It is an error to drop response callbacks which still correspond "
        << "to an open interface pipe.";
  }
#endif

  void Run(
      PickleContainerPtr in_passed);

  uint64_t request_id_;
  bool is_sync_;
  std::unique_ptr<mojo::MessageReceiverWithStatus> responder_;

  DISALLOW_COPY_AND_ASSIGN(PicklePasser_PassPickleContainer_ProxyToResponder);
};
class PicklePasser_PassPickleContainer_Response_Message
    : public mojo::internal::UnserializedMessageContext {
 public:
  static const mojo::internal::UnserializedMessageContext::Tag kMessageTag;

  explicit PicklePasser_PassPickleContainer_Response_Message(
      uint32_t message_flags
      , PickleContainerPtr param_passed
  )
      : mojo::internal::UnserializedMessageContext(
          &kMessageTag,
          internal::kPicklePasser_PassPickleContainer_Name,
          message_flags)
      , param_passed_(std::move(param_passed)){}
  ~PicklePasser_PassPickleContainer_Response_Message() override = default;

  static mojo::Message Build(
      bool serialize,
      bool is_sync,
      PickleContainerPtr param_passed) {

    
  const uint32_t kFlags = mojo::Message::kFlagIsResponse |
      ((is_sync) ? mojo::Message::kFlagIsSync : 0);

    if (!serialize) {
      return mojo::Message(std::make_unique<PicklePasser_PassPickleContainer_Response_Message>(
          kFlags
          , std::move(param_passed)
          ));
    }

    DCHECK(serialize);
    
  mojo::Message message(
      internal::kPicklePasser_PassPickleContainer_Name, kFlags, 0, 0, nullptr);
  auto* buffer = message.payload_buffer();
  ::mojo::test::internal::PicklePasser_PassPickleContainer_ResponseParams_Data::BufferWriter
      params;
  mojo::internal::SerializationContext serialization_context;
  params.Allocate(buffer);
  typename decltype(params->passed)::BaseType::BufferWriter
      passed_writer;
  mojo::internal::Serialize<::mojo::test::PickleContainerDataView>(
      param_passed, buffer, &passed_writer, &serialization_context);
  params->passed.Set(
      passed_writer.is_null() ? nullptr : passed_writer.data());
  MOJO_INTERNAL_DLOG_SERIALIZATION_WARNING(
      params->passed.is_null(),
      mojo::internal::VALIDATION_ERROR_UNEXPECTED_NULL_POINTER,
      "null passed in PicklePasser.PassPickleContainer response");
  message.AttachHandlesFromSerializationContext(
      &serialization_context);
    return message;
  }


  void Dispatch(PicklePasser::PassPickleContainerCallback* callback) {
    std::move(*callback).Run(
        std::move(param_passed_));
  }



 private:
  // mojo::internal::UnserializedMessageContext:
  void Serialize(mojo::internal::SerializationContext* serialization_context,
                 mojo::internal::Buffer* buffer) override {
    ::mojo::test::internal::PicklePasser_PassPickleContainer_ResponseParams_Data::BufferWriter
        params;
    params.Allocate(buffer);
  typename decltype(params->passed)::BaseType::BufferWriter
      passed_writer;
  mojo::internal::Serialize<::mojo::test::PickleContainerDataView>(
      param_passed_, buffer, &passed_writer, serialization_context);
  params->passed.Set(
      passed_writer.is_null() ? nullptr : passed_writer.data());
  MOJO_INTERNAL_DLOG_SERIALIZATION_WARNING(
      params->passed.is_null(),
      mojo::internal::VALIDATION_ERROR_UNEXPECTED_NULL_POINTER,
      "null passed in PicklePasser.PassPickleContainer response");
  }
  PickleContainerPtr param_passed_;

  DISALLOW_COPY_AND_ASSIGN(PicklePasser_PassPickleContainer_Response_Message);
};

const mojo::internal::UnserializedMessageContext::Tag
PicklePasser_PassPickleContainer_Response_Message::kMessageTag = {};

bool PicklePasser_PassPickleContainer_ForwardToCallback::Accept(
    mojo::Message* message) {
#if BUILDFLAG(MOJO_TRACE_ENABLED)
  TRACE_EVENT1("mojom", "<class 'jinja2::utils::Namespace'>::PicklePasser::PassPickleContainerCallback",
               "message", message->name());
#endif
  mojo::internal::MessageDispatchContext dispatch_context(message);
  if (!message->is_serialized()) {
    auto context =
        message->TakeUnserializedContext<PicklePasser_PassPickleContainer_Response_Message>();
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
  internal::PicklePasser_PassPickleContainer_ResponseParams_Data* params =
      reinterpret_cast<
          internal::PicklePasser_PassPickleContainer_ResponseParams_Data*>(
              message->mutable_payload());
  
  mojo::internal::SerializationContext serialization_context;
  serialization_context.TakeHandlesFromMessage(message);
  bool success = true;
  PickleContainerPtr p_passed{};
  PicklePasser_PassPickleContainer_ResponseParamsDataView input_data_view(params, &serialization_context);
  
  if (!input_data_view.ReadPassed(&p_passed))
    success = false;
  if (!success) {
    ReportValidationErrorForMessage(
        message,
        mojo::internal::VALIDATION_ERROR_DESERIALIZATION_FAILED,
        PicklePasser::Name_, 2, true);
    return false;
  }
  if (!callback_.is_null())
    std::move(callback_).Run(
std::move(p_passed));
  return true;
}

void PicklePasser_PassPickleContainer_ProxyToResponder::Run(
    PickleContainerPtr in_passed) {
  const bool kSerialize = responder_->PrefersSerializedMessages();
  auto message = PicklePasser_PassPickleContainer_Response_Message::Build(kSerialize, is_sync_, std::move(in_passed));

#if BUILDFLAG(MOJO_TRACE_ENABLED)
  TRACE_EVENT1("mojom", "(Impl)<class 'jinja2::utils::Namespace'>::PicklePasser::PassPickleContainerCallback",
               "message", message.name());
#endif

#if defined(ENABLE_IPC_FUZZER)
  message.set_interface_name(PicklePasser::Name_);
  message.set_method_name("PassPickleContainer");
#endif

  message.set_request_id(request_id_);
  ignore_result(responder_->Accept(&message));
  // TODO(darin): Accept() returning false indicates a malformed message, and
  // that may be good reason to close the connection. However, we don't have a
  // way to do that from here. We should add a way.
  responder_ = nullptr;
}
class PicklePasser_PassPickles_ProxyToResponder {
 public:
  static PicklePasser::PassPicklesCallback CreateCallback(
      uint64_t request_id,
      bool is_sync,
      std::unique_ptr<mojo::MessageReceiverWithStatus> responder) {
    std::unique_ptr<PicklePasser_PassPickles_ProxyToResponder> proxy(
        new PicklePasser_PassPickles_ProxyToResponder(
            request_id, is_sync, std::move(responder)));
    return base::BindOnce(&PicklePasser_PassPickles_ProxyToResponder::Run,
                          std::move(proxy));
  }

  ~PicklePasser_PassPickles_ProxyToResponder() {
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
  PicklePasser_PassPickles_ProxyToResponder(
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
        << "PicklePasser::PassPicklesCallback was destroyed without "
        << "first either being run or its corresponding binding being closed. "
        << "It is an error to drop response callbacks which still correspond "
        << "to an open interface pipe.";
  }
#endif

  void Run(
      WTF::Vector<mojo::test::PickledStructBlink> in_passed);

  uint64_t request_id_;
  bool is_sync_;
  std::unique_ptr<mojo::MessageReceiverWithStatus> responder_;

  DISALLOW_COPY_AND_ASSIGN(PicklePasser_PassPickles_ProxyToResponder);
};
class PicklePasser_PassPickles_Response_Message
    : public mojo::internal::UnserializedMessageContext {
 public:
  static const mojo::internal::UnserializedMessageContext::Tag kMessageTag;

  explicit PicklePasser_PassPickles_Response_Message(
      uint32_t message_flags
      , WTF::Vector<mojo::test::PickledStructBlink> param_passed
  )
      : mojo::internal::UnserializedMessageContext(
          &kMessageTag,
          internal::kPicklePasser_PassPickles_Name,
          message_flags)
      , param_passed_(std::move(param_passed)){}
  ~PicklePasser_PassPickles_Response_Message() override = default;

  static mojo::Message Build(
      bool serialize,
      bool is_sync,
      WTF::Vector<mojo::test::PickledStructBlink> param_passed) {

    
  const uint32_t kFlags = mojo::Message::kFlagIsResponse |
      ((is_sync) ? mojo::Message::kFlagIsSync : 0);

    if (!serialize) {
      return mojo::Message(std::make_unique<PicklePasser_PassPickles_Response_Message>(
          kFlags
          , std::move(param_passed)
          ));
    }

    DCHECK(serialize);
    
  mojo::Message message(
      internal::kPicklePasser_PassPickles_Name, kFlags, 0, 0, nullptr);
  auto* buffer = message.payload_buffer();
  ::mojo::test::internal::PicklePasser_PassPickles_ResponseParams_Data::BufferWriter
      params;
  mojo::internal::SerializationContext serialization_context;
  params.Allocate(buffer);
  typename decltype(params->passed)::BaseType::BufferWriter
      passed_writer;
  const mojo::internal::ContainerValidateParams passed_validate_params(
      0, false, nullptr);
  mojo::internal::Serialize<mojo::ArrayDataView<::mojo::test::PickledStructDataView>>(
      param_passed, buffer, &passed_writer, &passed_validate_params,
      &serialization_context);
  params->passed.Set(
      passed_writer.is_null() ? nullptr : passed_writer.data());
  MOJO_INTERNAL_DLOG_SERIALIZATION_WARNING(
      params->passed.is_null(),
      mojo::internal::VALIDATION_ERROR_UNEXPECTED_NULL_POINTER,
      "null passed in PicklePasser.PassPickles response");
  message.AttachHandlesFromSerializationContext(
      &serialization_context);
    return message;
  }


  void Dispatch(PicklePasser::PassPicklesCallback* callback) {
    std::move(*callback).Run(
        std::move(param_passed_));
  }



 private:
  // mojo::internal::UnserializedMessageContext:
  void Serialize(mojo::internal::SerializationContext* serialization_context,
                 mojo::internal::Buffer* buffer) override {
    ::mojo::test::internal::PicklePasser_PassPickles_ResponseParams_Data::BufferWriter
        params;
    params.Allocate(buffer);
  typename decltype(params->passed)::BaseType::BufferWriter
      passed_writer;
  const mojo::internal::ContainerValidateParams passed_validate_params(
      0, false, nullptr);
  mojo::internal::Serialize<mojo::ArrayDataView<::mojo::test::PickledStructDataView>>(
      param_passed_, buffer, &passed_writer, &passed_validate_params,
      serialization_context);
  params->passed.Set(
      passed_writer.is_null() ? nullptr : passed_writer.data());
  MOJO_INTERNAL_DLOG_SERIALIZATION_WARNING(
      params->passed.is_null(),
      mojo::internal::VALIDATION_ERROR_UNEXPECTED_NULL_POINTER,
      "null passed in PicklePasser.PassPickles response");
  }
  WTF::Vector<mojo::test::PickledStructBlink> param_passed_;

  DISALLOW_COPY_AND_ASSIGN(PicklePasser_PassPickles_Response_Message);
};

const mojo::internal::UnserializedMessageContext::Tag
PicklePasser_PassPickles_Response_Message::kMessageTag = {};

bool PicklePasser_PassPickles_ForwardToCallback::Accept(
    mojo::Message* message) {
#if BUILDFLAG(MOJO_TRACE_ENABLED)
  TRACE_EVENT1("mojom", "<class 'jinja2::utils::Namespace'>::PicklePasser::PassPicklesCallback",
               "message", message->name());
#endif
  mojo::internal::MessageDispatchContext dispatch_context(message);
  if (!message->is_serialized()) {
    auto context =
        message->TakeUnserializedContext<PicklePasser_PassPickles_Response_Message>();
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
  internal::PicklePasser_PassPickles_ResponseParams_Data* params =
      reinterpret_cast<
          internal::PicklePasser_PassPickles_ResponseParams_Data*>(
              message->mutable_payload());
  
  mojo::internal::SerializationContext serialization_context;
  serialization_context.TakeHandlesFromMessage(message);
  bool success = true;
  WTF::Vector<mojo::test::PickledStructBlink> p_passed{};
  PicklePasser_PassPickles_ResponseParamsDataView input_data_view(params, &serialization_context);
  
  if (!input_data_view.ReadPassed(&p_passed))
    success = false;
  if (!success) {
    ReportValidationErrorForMessage(
        message,
        mojo::internal::VALIDATION_ERROR_DESERIALIZATION_FAILED,
        PicklePasser::Name_, 3, true);
    return false;
  }
  if (!callback_.is_null())
    std::move(callback_).Run(
std::move(p_passed));
  return true;
}

void PicklePasser_PassPickles_ProxyToResponder::Run(
    WTF::Vector<mojo::test::PickledStructBlink> in_passed) {
  const bool kSerialize = responder_->PrefersSerializedMessages();
  auto message = PicklePasser_PassPickles_Response_Message::Build(kSerialize, is_sync_, std::move(in_passed));

#if BUILDFLAG(MOJO_TRACE_ENABLED)
  TRACE_EVENT1("mojom", "(Impl)<class 'jinja2::utils::Namespace'>::PicklePasser::PassPicklesCallback",
               "message", message.name());
#endif

#if defined(ENABLE_IPC_FUZZER)
  message.set_interface_name(PicklePasser::Name_);
  message.set_method_name("PassPickles");
#endif

  message.set_request_id(request_id_);
  ignore_result(responder_->Accept(&message));
  // TODO(darin): Accept() returning false indicates a malformed message, and
  // that may be good reason to close the connection. However, we don't have a
  // way to do that from here. We should add a way.
  responder_ = nullptr;
}
class PicklePasser_PassPickleArrays_ProxyToResponder {
 public:
  static PicklePasser::PassPickleArraysCallback CreateCallback(
      uint64_t request_id,
      bool is_sync,
      std::unique_ptr<mojo::MessageReceiverWithStatus> responder) {
    std::unique_ptr<PicklePasser_PassPickleArrays_ProxyToResponder> proxy(
        new PicklePasser_PassPickleArrays_ProxyToResponder(
            request_id, is_sync, std::move(responder)));
    return base::BindOnce(&PicklePasser_PassPickleArrays_ProxyToResponder::Run,
                          std::move(proxy));
  }

  ~PicklePasser_PassPickleArrays_ProxyToResponder() {
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
  PicklePasser_PassPickleArrays_ProxyToResponder(
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
        << "PicklePasser::PassPickleArraysCallback was destroyed without "
        << "first either being run or its corresponding binding being closed. "
        << "It is an error to drop response callbacks which still correspond "
        << "to an open interface pipe.";
  }
#endif

  void Run(
      WTF::Vector<WTF::Vector<mojo::test::PickledStructBlink>> in_passed);

  uint64_t request_id_;
  bool is_sync_;
  std::unique_ptr<mojo::MessageReceiverWithStatus> responder_;

  DISALLOW_COPY_AND_ASSIGN(PicklePasser_PassPickleArrays_ProxyToResponder);
};
class PicklePasser_PassPickleArrays_Response_Message
    : public mojo::internal::UnserializedMessageContext {
 public:
  static const mojo::internal::UnserializedMessageContext::Tag kMessageTag;

  explicit PicklePasser_PassPickleArrays_Response_Message(
      uint32_t message_flags
      , WTF::Vector<WTF::Vector<mojo::test::PickledStructBlink>> param_passed
  )
      : mojo::internal::UnserializedMessageContext(
          &kMessageTag,
          internal::kPicklePasser_PassPickleArrays_Name,
          message_flags)
      , param_passed_(std::move(param_passed)){}
  ~PicklePasser_PassPickleArrays_Response_Message() override = default;

  static mojo::Message Build(
      bool serialize,
      bool is_sync,
      WTF::Vector<WTF::Vector<mojo::test::PickledStructBlink>> param_passed) {

    
  const uint32_t kFlags = mojo::Message::kFlagIsResponse |
      ((is_sync) ? mojo::Message::kFlagIsSync : 0);

    if (!serialize) {
      return mojo::Message(std::make_unique<PicklePasser_PassPickleArrays_Response_Message>(
          kFlags
          , std::move(param_passed)
          ));
    }

    DCHECK(serialize);
    
  mojo::Message message(
      internal::kPicklePasser_PassPickleArrays_Name, kFlags, 0, 0, nullptr);
  auto* buffer = message.payload_buffer();
  ::mojo::test::internal::PicklePasser_PassPickleArrays_ResponseParams_Data::BufferWriter
      params;
  mojo::internal::SerializationContext serialization_context;
  params.Allocate(buffer);
  typename decltype(params->passed)::BaseType::BufferWriter
      passed_writer;
  const mojo::internal::ContainerValidateParams passed_validate_params(
      0, false, new mojo::internal::ContainerValidateParams(0, false, nullptr));
  mojo::internal::Serialize<mojo::ArrayDataView<mojo::ArrayDataView<::mojo::test::PickledStructDataView>>>(
      param_passed, buffer, &passed_writer, &passed_validate_params,
      &serialization_context);
  params->passed.Set(
      passed_writer.is_null() ? nullptr : passed_writer.data());
  MOJO_INTERNAL_DLOG_SERIALIZATION_WARNING(
      params->passed.is_null(),
      mojo::internal::VALIDATION_ERROR_UNEXPECTED_NULL_POINTER,
      "null passed in PicklePasser.PassPickleArrays response");
  message.AttachHandlesFromSerializationContext(
      &serialization_context);
    return message;
  }


  void Dispatch(PicklePasser::PassPickleArraysCallback* callback) {
    std::move(*callback).Run(
        std::move(param_passed_));
  }



 private:
  // mojo::internal::UnserializedMessageContext:
  void Serialize(mojo::internal::SerializationContext* serialization_context,
                 mojo::internal::Buffer* buffer) override {
    ::mojo::test::internal::PicklePasser_PassPickleArrays_ResponseParams_Data::BufferWriter
        params;
    params.Allocate(buffer);
  typename decltype(params->passed)::BaseType::BufferWriter
      passed_writer;
  const mojo::internal::ContainerValidateParams passed_validate_params(
      0, false, new mojo::internal::ContainerValidateParams(0, false, nullptr));
  mojo::internal::Serialize<mojo::ArrayDataView<mojo::ArrayDataView<::mojo::test::PickledStructDataView>>>(
      param_passed_, buffer, &passed_writer, &passed_validate_params,
      serialization_context);
  params->passed.Set(
      passed_writer.is_null() ? nullptr : passed_writer.data());
  MOJO_INTERNAL_DLOG_SERIALIZATION_WARNING(
      params->passed.is_null(),
      mojo::internal::VALIDATION_ERROR_UNEXPECTED_NULL_POINTER,
      "null passed in PicklePasser.PassPickleArrays response");
  }
  WTF::Vector<WTF::Vector<mojo::test::PickledStructBlink>> param_passed_;

  DISALLOW_COPY_AND_ASSIGN(PicklePasser_PassPickleArrays_Response_Message);
};

const mojo::internal::UnserializedMessageContext::Tag
PicklePasser_PassPickleArrays_Response_Message::kMessageTag = {};

bool PicklePasser_PassPickleArrays_ForwardToCallback::Accept(
    mojo::Message* message) {
#if BUILDFLAG(MOJO_TRACE_ENABLED)
  TRACE_EVENT1("mojom", "<class 'jinja2::utils::Namespace'>::PicklePasser::PassPickleArraysCallback",
               "message", message->name());
#endif
  mojo::internal::MessageDispatchContext dispatch_context(message);
  if (!message->is_serialized()) {
    auto context =
        message->TakeUnserializedContext<PicklePasser_PassPickleArrays_Response_Message>();
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
  internal::PicklePasser_PassPickleArrays_ResponseParams_Data* params =
      reinterpret_cast<
          internal::PicklePasser_PassPickleArrays_ResponseParams_Data*>(
              message->mutable_payload());
  
  mojo::internal::SerializationContext serialization_context;
  serialization_context.TakeHandlesFromMessage(message);
  bool success = true;
  WTF::Vector<WTF::Vector<mojo::test::PickledStructBlink>> p_passed{};
  PicklePasser_PassPickleArrays_ResponseParamsDataView input_data_view(params, &serialization_context);
  
  if (!input_data_view.ReadPassed(&p_passed))
    success = false;
  if (!success) {
    ReportValidationErrorForMessage(
        message,
        mojo::internal::VALIDATION_ERROR_DESERIALIZATION_FAILED,
        PicklePasser::Name_, 4, true);
    return false;
  }
  if (!callback_.is_null())
    std::move(callback_).Run(
std::move(p_passed));
  return true;
}

void PicklePasser_PassPickleArrays_ProxyToResponder::Run(
    WTF::Vector<WTF::Vector<mojo::test::PickledStructBlink>> in_passed) {
  const bool kSerialize = responder_->PrefersSerializedMessages();
  auto message = PicklePasser_PassPickleArrays_Response_Message::Build(kSerialize, is_sync_, std::move(in_passed));

#if BUILDFLAG(MOJO_TRACE_ENABLED)
  TRACE_EVENT1("mojom", "(Impl)<class 'jinja2::utils::Namespace'>::PicklePasser::PassPickleArraysCallback",
               "message", message.name());
#endif

#if defined(ENABLE_IPC_FUZZER)
  message.set_interface_name(PicklePasser::Name_);
  message.set_method_name("PassPickleArrays");
#endif

  message.set_request_id(request_id_);
  ignore_result(responder_->Accept(&message));
  // TODO(darin): Accept() returning false indicates a malformed message, and
  // that may be good reason to close the connection. However, we don't have a
  // way to do that from here. We should add a way.
  responder_ = nullptr;
}

// static
bool PicklePasserStubDispatch::Accept(
    PicklePasser* impl,
    mojo::Message* message) {
  switch (message->header()->name) {
    case internal::kPicklePasser_PassPickledStruct_Name: {
      break;
    }
    case internal::kPicklePasser_PassPickledEnum_Name: {
      break;
    }
    case internal::kPicklePasser_PassPickleContainer_Name: {
      break;
    }
    case internal::kPicklePasser_PassPickles_Name: {
      break;
    }
    case internal::kPicklePasser_PassPickleArrays_Name: {
      break;
    }
  }
  return false;
}

// static
bool PicklePasserStubDispatch::AcceptWithResponder(
    PicklePasser* impl,
    mojo::Message* message,
    std::unique_ptr<mojo::MessageReceiverWithStatus> responder) {
  switch (message->header()->name) {
    case internal::kPicklePasser_PassPickledStruct_Name: {
#if BUILDFLAG(MOJO_TRACE_ENABLED)
      TRACE_EVENT1(
          "mojom",
          "(Impl)<class 'jinja2::utils::Namespace'>::PicklePasser::PassPickledStruct",
          "message", message->name());
#endif
      base::TaskAnnotator::ScopedSetIpcProgramCounter scoped_ipc_pc(
          base::GetProgramCounter());
      mojo::internal::MessageDispatchContext context(message);
      if (!message->is_serialized()) {
        auto context = message->TakeUnserializedContext<
            PicklePasserProxy_PassPickledStruct_Message>();
        if (!context) {
          // The Message was not of the expected type. It may be a valid message
          // which was built using a different variant of these bindings. Force
          // serialization before dispatch in this case.
          message->SerializeIfNecessary();
        } else {
          PicklePasser::PassPickledStructCallback callback =
              PicklePasser_PassPickledStruct_ProxyToResponder::CreateCallback(
                  message->request_id(),
                  message->has_flag(mojo::Message::kFlagIsSync),
                  std::move(responder));
          context->Dispatch(impl, std::move(callback));
          return true;
        }
      }

      internal::PicklePasser_PassPickledStruct_Params_Data* params =
          reinterpret_cast<
              internal::PicklePasser_PassPickledStruct_Params_Data*>(
                  message->mutable_payload());
      
      mojo::internal::SerializationContext serialization_context;
      serialization_context.TakeHandlesFromMessage(message);
      bool success = true;
      mojo::test::PickledStructBlink p_pickle{};
      PicklePasser_PassPickledStruct_ParamsDataView input_data_view(params, &serialization_context);
      
      if (!input_data_view.ReadPickle(&p_pickle))
        success = false;
      if (!success) {
        ReportValidationErrorForMessage(
            message,
            mojo::internal::VALIDATION_ERROR_DESERIALIZATION_FAILED,
            PicklePasser::Name_, 0, false);
        return false;
      }
      PicklePasser::PassPickledStructCallback callback =
          PicklePasser_PassPickledStruct_ProxyToResponder::CreateCallback(
              message->request_id(),
              message->has_flag(mojo::Message::kFlagIsSync),
              std::move(responder));
      // A null |impl| means no implementation was bound.
      DCHECK(impl);
      impl->PassPickledStruct(
std::move(p_pickle), std::move(callback));
      return true;
    }
    case internal::kPicklePasser_PassPickledEnum_Name: {
#if BUILDFLAG(MOJO_TRACE_ENABLED)
      TRACE_EVENT1(
          "mojom",
          "(Impl)<class 'jinja2::utils::Namespace'>::PicklePasser::PassPickledEnum",
          "message", message->name());
#endif
      base::TaskAnnotator::ScopedSetIpcProgramCounter scoped_ipc_pc(
          base::GetProgramCounter());
      mojo::internal::MessageDispatchContext context(message);
      if (!message->is_serialized()) {
        auto context = message->TakeUnserializedContext<
            PicklePasserProxy_PassPickledEnum_Message>();
        if (!context) {
          // The Message was not of the expected type. It may be a valid message
          // which was built using a different variant of these bindings. Force
          // serialization before dispatch in this case.
          message->SerializeIfNecessary();
        } else {
          PicklePasser::PassPickledEnumCallback callback =
              PicklePasser_PassPickledEnum_ProxyToResponder::CreateCallback(
                  message->request_id(),
                  message->has_flag(mojo::Message::kFlagIsSync),
                  std::move(responder));
          context->Dispatch(impl, std::move(callback));
          return true;
        }
      }

      internal::PicklePasser_PassPickledEnum_Params_Data* params =
          reinterpret_cast<
              internal::PicklePasser_PassPickledEnum_Params_Data*>(
                  message->mutable_payload());
      
      mojo::internal::SerializationContext serialization_context;
      serialization_context.TakeHandlesFromMessage(message);
      bool success = true;
      mojo::test::PickledEnumBlink p_pickle{};
      PicklePasser_PassPickledEnum_ParamsDataView input_data_view(params, &serialization_context);
      
      if (!input_data_view.ReadPickle(&p_pickle))
        success = false;
      if (!success) {
        ReportValidationErrorForMessage(
            message,
            mojo::internal::VALIDATION_ERROR_DESERIALIZATION_FAILED,
            PicklePasser::Name_, 1, false);
        return false;
      }
      PicklePasser::PassPickledEnumCallback callback =
          PicklePasser_PassPickledEnum_ProxyToResponder::CreateCallback(
              message->request_id(),
              message->has_flag(mojo::Message::kFlagIsSync),
              std::move(responder));
      // A null |impl| means no implementation was bound.
      DCHECK(impl);
      impl->PassPickledEnum(
std::move(p_pickle), std::move(callback));
      return true;
    }
    case internal::kPicklePasser_PassPickleContainer_Name: {
#if BUILDFLAG(MOJO_TRACE_ENABLED)
      TRACE_EVENT1(
          "mojom",
          "(Impl)<class 'jinja2::utils::Namespace'>::PicklePasser::PassPickleContainer",
          "message", message->name());
#endif
      base::TaskAnnotator::ScopedSetIpcProgramCounter scoped_ipc_pc(
          base::GetProgramCounter());
      mojo::internal::MessageDispatchContext context(message);
      if (!message->is_serialized()) {
        auto context = message->TakeUnserializedContext<
            PicklePasserProxy_PassPickleContainer_Message>();
        if (!context) {
          // The Message was not of the expected type. It may be a valid message
          // which was built using a different variant of these bindings. Force
          // serialization before dispatch in this case.
          message->SerializeIfNecessary();
        } else {
          PicklePasser::PassPickleContainerCallback callback =
              PicklePasser_PassPickleContainer_ProxyToResponder::CreateCallback(
                  message->request_id(),
                  message->has_flag(mojo::Message::kFlagIsSync),
                  std::move(responder));
          context->Dispatch(impl, std::move(callback));
          return true;
        }
      }

      internal::PicklePasser_PassPickleContainer_Params_Data* params =
          reinterpret_cast<
              internal::PicklePasser_PassPickleContainer_Params_Data*>(
                  message->mutable_payload());
      
      mojo::internal::SerializationContext serialization_context;
      serialization_context.TakeHandlesFromMessage(message);
      bool success = true;
      PickleContainerPtr p_container{};
      PicklePasser_PassPickleContainer_ParamsDataView input_data_view(params, &serialization_context);
      
      if (!input_data_view.ReadContainer(&p_container))
        success = false;
      if (!success) {
        ReportValidationErrorForMessage(
            message,
            mojo::internal::VALIDATION_ERROR_DESERIALIZATION_FAILED,
            PicklePasser::Name_, 2, false);
        return false;
      }
      PicklePasser::PassPickleContainerCallback callback =
          PicklePasser_PassPickleContainer_ProxyToResponder::CreateCallback(
              message->request_id(),
              message->has_flag(mojo::Message::kFlagIsSync),
              std::move(responder));
      // A null |impl| means no implementation was bound.
      DCHECK(impl);
      impl->PassPickleContainer(
std::move(p_container), std::move(callback));
      return true;
    }
    case internal::kPicklePasser_PassPickles_Name: {
#if BUILDFLAG(MOJO_TRACE_ENABLED)
      TRACE_EVENT1(
          "mojom",
          "(Impl)<class 'jinja2::utils::Namespace'>::PicklePasser::PassPickles",
          "message", message->name());
#endif
      base::TaskAnnotator::ScopedSetIpcProgramCounter scoped_ipc_pc(
          base::GetProgramCounter());
      mojo::internal::MessageDispatchContext context(message);
      if (!message->is_serialized()) {
        auto context = message->TakeUnserializedContext<
            PicklePasserProxy_PassPickles_Message>();
        if (!context) {
          // The Message was not of the expected type. It may be a valid message
          // which was built using a different variant of these bindings. Force
          // serialization before dispatch in this case.
          message->SerializeIfNecessary();
        } else {
          PicklePasser::PassPicklesCallback callback =
              PicklePasser_PassPickles_ProxyToResponder::CreateCallback(
                  message->request_id(),
                  message->has_flag(mojo::Message::kFlagIsSync),
                  std::move(responder));
          context->Dispatch(impl, std::move(callback));
          return true;
        }
      }

      internal::PicklePasser_PassPickles_Params_Data* params =
          reinterpret_cast<
              internal::PicklePasser_PassPickles_Params_Data*>(
                  message->mutable_payload());
      
      mojo::internal::SerializationContext serialization_context;
      serialization_context.TakeHandlesFromMessage(message);
      bool success = true;
      WTF::Vector<mojo::test::PickledStructBlink> p_pickles{};
      PicklePasser_PassPickles_ParamsDataView input_data_view(params, &serialization_context);
      
      if (!input_data_view.ReadPickles(&p_pickles))
        success = false;
      if (!success) {
        ReportValidationErrorForMessage(
            message,
            mojo::internal::VALIDATION_ERROR_DESERIALIZATION_FAILED,
            PicklePasser::Name_, 3, false);
        return false;
      }
      PicklePasser::PassPicklesCallback callback =
          PicklePasser_PassPickles_ProxyToResponder::CreateCallback(
              message->request_id(),
              message->has_flag(mojo::Message::kFlagIsSync),
              std::move(responder));
      // A null |impl| means no implementation was bound.
      DCHECK(impl);
      impl->PassPickles(
std::move(p_pickles), std::move(callback));
      return true;
    }
    case internal::kPicklePasser_PassPickleArrays_Name: {
#if BUILDFLAG(MOJO_TRACE_ENABLED)
      TRACE_EVENT1(
          "mojom",
          "(Impl)<class 'jinja2::utils::Namespace'>::PicklePasser::PassPickleArrays",
          "message", message->name());
#endif
      base::TaskAnnotator::ScopedSetIpcProgramCounter scoped_ipc_pc(
          base::GetProgramCounter());
      mojo::internal::MessageDispatchContext context(message);
      if (!message->is_serialized()) {
        auto context = message->TakeUnserializedContext<
            PicklePasserProxy_PassPickleArrays_Message>();
        if (!context) {
          // The Message was not of the expected type. It may be a valid message
          // which was built using a different variant of these bindings. Force
          // serialization before dispatch in this case.
          message->SerializeIfNecessary();
        } else {
          PicklePasser::PassPickleArraysCallback callback =
              PicklePasser_PassPickleArrays_ProxyToResponder::CreateCallback(
                  message->request_id(),
                  message->has_flag(mojo::Message::kFlagIsSync),
                  std::move(responder));
          context->Dispatch(impl, std::move(callback));
          return true;
        }
      }

      internal::PicklePasser_PassPickleArrays_Params_Data* params =
          reinterpret_cast<
              internal::PicklePasser_PassPickleArrays_Params_Data*>(
                  message->mutable_payload());
      
      mojo::internal::SerializationContext serialization_context;
      serialization_context.TakeHandlesFromMessage(message);
      bool success = true;
      WTF::Vector<WTF::Vector<mojo::test::PickledStructBlink>> p_pickle_arrays{};
      PicklePasser_PassPickleArrays_ParamsDataView input_data_view(params, &serialization_context);
      
      if (!input_data_view.ReadPickleArrays(&p_pickle_arrays))
        success = false;
      if (!success) {
        ReportValidationErrorForMessage(
            message,
            mojo::internal::VALIDATION_ERROR_DESERIALIZATION_FAILED,
            PicklePasser::Name_, 4, false);
        return false;
      }
      PicklePasser::PassPickleArraysCallback callback =
          PicklePasser_PassPickleArrays_ProxyToResponder::CreateCallback(
              message->request_id(),
              message->has_flag(mojo::Message::kFlagIsSync),
              std::move(responder));
      // A null |impl| means no implementation was bound.
      DCHECK(impl);
      impl->PassPickleArrays(
std::move(p_pickle_arrays), std::move(callback));
      return true;
    }
  }
  return false;
}

bool PicklePasserRequestValidator::Accept(mojo::Message* message) {
  if (!message->is_serialized() ||
      mojo::internal::ControlMessageHandler::IsControlMessage(message)) {
    return true;
  }

  mojo::internal::ValidationContext validation_context(
    message->payload(), message->payload_num_bytes(),
    message->handles()->size(), message->payload_num_interface_ids(), message,
    "PicklePasser RequestValidator");

  switch (message->header()->name) {
    case internal::kPicklePasser_PassPickledStruct_Name: {
      if (!mojo::internal::ValidateMessageIsRequestExpectingResponse(
              message, &validation_context)) {
        return false;
      }
      if (!mojo::internal::ValidateMessagePayload<
               internal::PicklePasser_PassPickledStruct_Params_Data>(
                  message, &validation_context)) {
        return false;
      }
      return true;
    }
    case internal::kPicklePasser_PassPickledEnum_Name: {
      if (!mojo::internal::ValidateMessageIsRequestExpectingResponse(
              message, &validation_context)) {
        return false;
      }
      if (!mojo::internal::ValidateMessagePayload<
               internal::PicklePasser_PassPickledEnum_Params_Data>(
                  message, &validation_context)) {
        return false;
      }
      return true;
    }
    case internal::kPicklePasser_PassPickleContainer_Name: {
      if (!mojo::internal::ValidateMessageIsRequestExpectingResponse(
              message, &validation_context)) {
        return false;
      }
      if (!mojo::internal::ValidateMessagePayload<
               internal::PicklePasser_PassPickleContainer_Params_Data>(
                  message, &validation_context)) {
        return false;
      }
      return true;
    }
    case internal::kPicklePasser_PassPickles_Name: {
      if (!mojo::internal::ValidateMessageIsRequestExpectingResponse(
              message, &validation_context)) {
        return false;
      }
      if (!mojo::internal::ValidateMessagePayload<
               internal::PicklePasser_PassPickles_Params_Data>(
                  message, &validation_context)) {
        return false;
      }
      return true;
    }
    case internal::kPicklePasser_PassPickleArrays_Name: {
      if (!mojo::internal::ValidateMessageIsRequestExpectingResponse(
              message, &validation_context)) {
        return false;
      }
      if (!mojo::internal::ValidateMessagePayload<
               internal::PicklePasser_PassPickleArrays_Params_Data>(
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

bool PicklePasserResponseValidator::Accept(mojo::Message* message) {
  if (!message->is_serialized() ||
      mojo::internal::ControlMessageHandler::IsControlMessage(message)) {
    return true;
  }

  mojo::internal::ValidationContext validation_context(
    message->payload(), message->payload_num_bytes(),
    message->handles()->size(), message->payload_num_interface_ids(), message,
    "PicklePasser ResponseValidator");

  if (!mojo::internal::ValidateMessageIsResponse(message, &validation_context))
    return false;
  switch (message->header()->name) {
    case internal::kPicklePasser_PassPickledStruct_Name: {
      if (!mojo::internal::ValidateMessagePayload<
               internal::PicklePasser_PassPickledStruct_ResponseParams_Data>(
                    message, &validation_context)) {
        return false;
      }
      return true;
    }
    case internal::kPicklePasser_PassPickledEnum_Name: {
      if (!mojo::internal::ValidateMessagePayload<
               internal::PicklePasser_PassPickledEnum_ResponseParams_Data>(
                    message, &validation_context)) {
        return false;
      }
      return true;
    }
    case internal::kPicklePasser_PassPickleContainer_Name: {
      if (!mojo::internal::ValidateMessagePayload<
               internal::PicklePasser_PassPickleContainer_ResponseParams_Data>(
                    message, &validation_context)) {
        return false;
      }
      return true;
    }
    case internal::kPicklePasser_PassPickles_Name: {
      if (!mojo::internal::ValidateMessagePayload<
               internal::PicklePasser_PassPickles_ResponseParams_Data>(
                    message, &validation_context)) {
        return false;
      }
      return true;
    }
    case internal::kPicklePasser_PassPickleArrays_Name: {
      if (!mojo::internal::ValidateMessagePayload<
               internal::PicklePasser_PassPickleArrays_ResponseParams_Data>(
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
const char RectService::Name_[] = "mojo.test.RectService";

class RectService_GetLargestRect_ForwardToCallback
    : public mojo::MessageReceiver {
 public:
  RectService_GetLargestRect_ForwardToCallback(
      RectService::GetLargestRectCallback callback
      ) : callback_(std::move(callback)) {
  }
  bool Accept(mojo::Message* message) override;
 private:
  RectService::GetLargestRectCallback callback_;
  DISALLOW_COPY_AND_ASSIGN(RectService_GetLargestRect_ForwardToCallback);
};

class RectService_PassSharedRect_ForwardToCallback
    : public mojo::MessageReceiver {
 public:
  RectService_PassSharedRect_ForwardToCallback(
      RectService::PassSharedRectCallback callback
      ) : callback_(std::move(callback)) {
  }
  bool Accept(mojo::Message* message) override;
 private:
  RectService::PassSharedRectCallback callback_;
  DISALLOW_COPY_AND_ASSIGN(RectService_PassSharedRect_ForwardToCallback);
};

RectServiceProxy::RectServiceProxy(mojo::MessageReceiverWithResponder* receiver)
    : receiver_(receiver) {
}
class RectServiceProxy_AddRect_Message
    : public mojo::internal::UnserializedMessageContext {
 public:
  static const mojo::internal::UnserializedMessageContext::Tag kMessageTag;

  explicit RectServiceProxy_AddRect_Message(
      uint32_t message_flags
      , const mojo::test::RectBlink& param_r
  )
      : mojo::internal::UnserializedMessageContext(
          &kMessageTag,
          internal::kRectService_AddRect_Name,
          message_flags)
      , param_r_(std::move(param_r)){}
  ~RectServiceProxy_AddRect_Message() override = default;

  static mojo::Message Build(
      bool serialize,
      bool expects_response,
      bool is_sync,
      const mojo::test::RectBlink& param_r) {

    
  const uint32_t kFlags =
      ((expects_response) ? mojo::Message::kFlagExpectsResponse : 0) |
      ((is_sync) ? mojo::Message::kFlagIsSync : 0);

    if (!serialize) {
      return mojo::Message(std::make_unique<RectServiceProxy_AddRect_Message>(
          kFlags
          , std::move(param_r)
          ));
    }

    DCHECK(serialize);
    
  mojo::Message message(
      internal::kRectService_AddRect_Name, kFlags, 0, 0, nullptr);
  auto* buffer = message.payload_buffer();
  ::mojo::test::internal::RectService_AddRect_Params_Data::BufferWriter
      params;
  mojo::internal::SerializationContext serialization_context;
  params.Allocate(buffer);
  typename decltype(params->r)::BaseType::BufferWriter
      r_writer;
  mojo::internal::Serialize<::mojo::test::TypemappedRectDataView>(
      param_r, buffer, &r_writer, &serialization_context);
  params->r.Set(
      r_writer.is_null() ? nullptr : r_writer.data());
  MOJO_INTERNAL_DLOG_SERIALIZATION_WARNING(
      params->r.is_null(),
      mojo::internal::VALIDATION_ERROR_UNEXPECTED_NULL_POINTER,
      "null r in RectService.AddRect request");
  message.AttachHandlesFromSerializationContext(
      &serialization_context);
    return message;
  }


  void Dispatch(RectService* impl) {
    impl->AddRect(
        std::move(param_r_));
  }

 private:
  // mojo::internal::UnserializedMessageContext:
  void Serialize(mojo::internal::SerializationContext* serialization_context,
                 mojo::internal::Buffer* buffer) override {
    ::mojo::test::internal::RectService_AddRect_Params_Data::BufferWriter
        params;
    params.Allocate(buffer);
  typename decltype(params->r)::BaseType::BufferWriter
      r_writer;
  mojo::internal::Serialize<::mojo::test::TypemappedRectDataView>(
      param_r_, buffer, &r_writer, serialization_context);
  params->r.Set(
      r_writer.is_null() ? nullptr : r_writer.data());
  MOJO_INTERNAL_DLOG_SERIALIZATION_WARNING(
      params->r.is_null(),
      mojo::internal::VALIDATION_ERROR_UNEXPECTED_NULL_POINTER,
      "null r in RectService.AddRect request");
  }
  mojo::test::RectBlink param_r_;

  DISALLOW_COPY_AND_ASSIGN(RectServiceProxy_AddRect_Message);
};

const mojo::internal::UnserializedMessageContext::Tag
RectServiceProxy_AddRect_Message::kMessageTag = {};

void RectServiceProxy::AddRect(
    const mojo::test::RectBlink& in_r) {
#if BUILDFLAG(MOJO_TRACE_ENABLED)
  TRACE_EVENT0("mojom", "<class 'jinja2::utils::Namespace'>::RectService::AddRect");
#endif
  const bool kExpectsResponse = false;
  const bool kIsSync = false;
  const bool kSerialize = receiver_->PrefersSerializedMessages();
  auto message = RectServiceProxy_AddRect_Message::Build(
      kSerialize, kExpectsResponse, kIsSync, std::move(in_r));

#if defined(ENABLE_IPC_FUZZER)
  message.set_interface_name(RectService::Name_);
  message.set_method_name("AddRect");
#endif
  // This return value may be ignored as false implies the Connector has
  // encountered an error, which will be visible through other means.
  ignore_result(receiver_->Accept(&message));
}
class RectServiceProxy_GetLargestRect_Message
    : public mojo::internal::UnserializedMessageContext {
 public:
  static const mojo::internal::UnserializedMessageContext::Tag kMessageTag;

  explicit RectServiceProxy_GetLargestRect_Message(
      uint32_t message_flags
  )
      : mojo::internal::UnserializedMessageContext(
          &kMessageTag,
          internal::kRectService_GetLargestRect_Name,
          message_flags){}
  ~RectServiceProxy_GetLargestRect_Message() override = default;

  static mojo::Message Build(
      bool serialize,
      bool expects_response,
      bool is_sync) {

    
  const uint32_t kFlags =
      ((expects_response) ? mojo::Message::kFlagExpectsResponse : 0) |
      ((is_sync) ? mojo::Message::kFlagIsSync : 0);

    if (!serialize) {
      return mojo::Message(std::make_unique<RectServiceProxy_GetLargestRect_Message>(
          kFlags
          ));
    }

    DCHECK(serialize);
    
  mojo::Message message(
      internal::kRectService_GetLargestRect_Name, kFlags, 0, 0, nullptr);
  auto* buffer = message.payload_buffer();
  ::mojo::test::internal::RectService_GetLargestRect_Params_Data::BufferWriter
      params;
  mojo::internal::SerializationContext serialization_context;
  params.Allocate(buffer);
  message.AttachHandlesFromSerializationContext(
      &serialization_context);
    return message;
  }


  void Dispatch(RectService* impl, RectService::GetLargestRectCallback callback) {
    impl->GetLargestRect(std::move(callback));
  }

 private:
  // mojo::internal::UnserializedMessageContext:
  void Serialize(mojo::internal::SerializationContext* serialization_context,
                 mojo::internal::Buffer* buffer) override {
    ::mojo::test::internal::RectService_GetLargestRect_Params_Data::BufferWriter
        params;
    params.Allocate(buffer);
  }

  DISALLOW_COPY_AND_ASSIGN(RectServiceProxy_GetLargestRect_Message);
};

const mojo::internal::UnserializedMessageContext::Tag
RectServiceProxy_GetLargestRect_Message::kMessageTag = {};

void RectServiceProxy::GetLargestRect(
    GetLargestRectCallback callback) {
#if BUILDFLAG(MOJO_TRACE_ENABLED)
  TRACE_EVENT0("mojom", "<class 'jinja2::utils::Namespace'>::RectService::GetLargestRect");
#endif
  const bool kExpectsResponse = true;
  const bool kIsSync = false;
  const bool kSerialize = receiver_->PrefersSerializedMessages();
  auto message = RectServiceProxy_GetLargestRect_Message::Build(
      kSerialize, kExpectsResponse, kIsSync);

#if defined(ENABLE_IPC_FUZZER)
  message.set_interface_name(RectService::Name_);
  message.set_method_name("GetLargestRect");
#endif
  std::unique_ptr<mojo::MessageReceiver> responder(
      new RectService_GetLargestRect_ForwardToCallback(
          std::move(callback)));
  ignore_result(receiver_->AcceptWithResponder(&message, std::move(responder)));
}
class RectServiceProxy_PassSharedRect_Message
    : public mojo::internal::UnserializedMessageContext {
 public:
  static const mojo::internal::UnserializedMessageContext::Tag kMessageTag;

  explicit RectServiceProxy_PassSharedRect_Message(
      uint32_t message_flags
      , const mojo::test::SharedRect& param_r
  )
      : mojo::internal::UnserializedMessageContext(
          &kMessageTag,
          internal::kRectService_PassSharedRect_Name,
          message_flags)
      , param_r_(std::move(param_r)){}
  ~RectServiceProxy_PassSharedRect_Message() override = default;

  static mojo::Message Build(
      bool serialize,
      bool expects_response,
      bool is_sync,
      const mojo::test::SharedRect& param_r) {

    
  const uint32_t kFlags =
      ((expects_response) ? mojo::Message::kFlagExpectsResponse : 0) |
      ((is_sync) ? mojo::Message::kFlagIsSync : 0);

    if (!serialize) {
      return mojo::Message(std::make_unique<RectServiceProxy_PassSharedRect_Message>(
          kFlags
          , std::move(param_r)
          ));
    }

    DCHECK(serialize);
    
  mojo::Message message(
      internal::kRectService_PassSharedRect_Name, kFlags, 0, 0, nullptr);
  auto* buffer = message.payload_buffer();
  ::mojo::test::internal::RectService_PassSharedRect_Params_Data::BufferWriter
      params;
  mojo::internal::SerializationContext serialization_context;
  params.Allocate(buffer);
  typename decltype(params->r)::BaseType::BufferWriter
      r_writer;
  mojo::internal::Serialize<::mojo::test::SharedTypemappedRectDataView>(
      param_r, buffer, &r_writer, &serialization_context);
  params->r.Set(
      r_writer.is_null() ? nullptr : r_writer.data());
  MOJO_INTERNAL_DLOG_SERIALIZATION_WARNING(
      params->r.is_null(),
      mojo::internal::VALIDATION_ERROR_UNEXPECTED_NULL_POINTER,
      "null r in RectService.PassSharedRect request");
  message.AttachHandlesFromSerializationContext(
      &serialization_context);
    return message;
  }


  void Dispatch(RectService* impl, RectService::PassSharedRectCallback callback) {
    impl->PassSharedRect(
        std::move(param_r_), std::move(callback));
  }

 private:
  // mojo::internal::UnserializedMessageContext:
  void Serialize(mojo::internal::SerializationContext* serialization_context,
                 mojo::internal::Buffer* buffer) override {
    ::mojo::test::internal::RectService_PassSharedRect_Params_Data::BufferWriter
        params;
    params.Allocate(buffer);
  typename decltype(params->r)::BaseType::BufferWriter
      r_writer;
  mojo::internal::Serialize<::mojo::test::SharedTypemappedRectDataView>(
      param_r_, buffer, &r_writer, serialization_context);
  params->r.Set(
      r_writer.is_null() ? nullptr : r_writer.data());
  MOJO_INTERNAL_DLOG_SERIALIZATION_WARNING(
      params->r.is_null(),
      mojo::internal::VALIDATION_ERROR_UNEXPECTED_NULL_POINTER,
      "null r in RectService.PassSharedRect request");
  }
  mojo::test::SharedRect param_r_;

  DISALLOW_COPY_AND_ASSIGN(RectServiceProxy_PassSharedRect_Message);
};

const mojo::internal::UnserializedMessageContext::Tag
RectServiceProxy_PassSharedRect_Message::kMessageTag = {};

void RectServiceProxy::PassSharedRect(
    const mojo::test::SharedRect& in_r, PassSharedRectCallback callback) {
#if BUILDFLAG(MOJO_TRACE_ENABLED)
  TRACE_EVENT0("mojom", "<class 'jinja2::utils::Namespace'>::RectService::PassSharedRect");
#endif
  const bool kExpectsResponse = true;
  const bool kIsSync = false;
  const bool kSerialize = receiver_->PrefersSerializedMessages();
  auto message = RectServiceProxy_PassSharedRect_Message::Build(
      kSerialize, kExpectsResponse, kIsSync, std::move(in_r));

#if defined(ENABLE_IPC_FUZZER)
  message.set_interface_name(RectService::Name_);
  message.set_method_name("PassSharedRect");
#endif
  std::unique_ptr<mojo::MessageReceiver> responder(
      new RectService_PassSharedRect_ForwardToCallback(
          std::move(callback)));
  ignore_result(receiver_->AcceptWithResponder(&message, std::move(responder)));
}
class RectService_GetLargestRect_ProxyToResponder {
 public:
  static RectService::GetLargestRectCallback CreateCallback(
      uint64_t request_id,
      bool is_sync,
      std::unique_ptr<mojo::MessageReceiverWithStatus> responder) {
    std::unique_ptr<RectService_GetLargestRect_ProxyToResponder> proxy(
        new RectService_GetLargestRect_ProxyToResponder(
            request_id, is_sync, std::move(responder)));
    return base::BindOnce(&RectService_GetLargestRect_ProxyToResponder::Run,
                          std::move(proxy));
  }

  ~RectService_GetLargestRect_ProxyToResponder() {
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
  RectService_GetLargestRect_ProxyToResponder(
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
        << "RectService::GetLargestRectCallback was destroyed without "
        << "first either being run or its corresponding binding being closed. "
        << "It is an error to drop response callbacks which still correspond "
        << "to an open interface pipe.";
  }
#endif

  void Run(
      const mojo::test::RectBlink& in_largest);

  uint64_t request_id_;
  bool is_sync_;
  std::unique_ptr<mojo::MessageReceiverWithStatus> responder_;

  DISALLOW_COPY_AND_ASSIGN(RectService_GetLargestRect_ProxyToResponder);
};
class RectService_GetLargestRect_Response_Message
    : public mojo::internal::UnserializedMessageContext {
 public:
  static const mojo::internal::UnserializedMessageContext::Tag kMessageTag;

  explicit RectService_GetLargestRect_Response_Message(
      uint32_t message_flags
      , const mojo::test::RectBlink& param_largest
  )
      : mojo::internal::UnserializedMessageContext(
          &kMessageTag,
          internal::kRectService_GetLargestRect_Name,
          message_flags)
      , param_largest_(std::move(param_largest)){}
  ~RectService_GetLargestRect_Response_Message() override = default;

  static mojo::Message Build(
      bool serialize,
      bool is_sync,
      const mojo::test::RectBlink& param_largest) {

    
  const uint32_t kFlags = mojo::Message::kFlagIsResponse |
      ((is_sync) ? mojo::Message::kFlagIsSync : 0);

    if (!serialize) {
      return mojo::Message(std::make_unique<RectService_GetLargestRect_Response_Message>(
          kFlags
          , std::move(param_largest)
          ));
    }

    DCHECK(serialize);
    
  mojo::Message message(
      internal::kRectService_GetLargestRect_Name, kFlags, 0, 0, nullptr);
  auto* buffer = message.payload_buffer();
  ::mojo::test::internal::RectService_GetLargestRect_ResponseParams_Data::BufferWriter
      params;
  mojo::internal::SerializationContext serialization_context;
  params.Allocate(buffer);
  typename decltype(params->largest)::BaseType::BufferWriter
      largest_writer;
  mojo::internal::Serialize<::mojo::test::TypemappedRectDataView>(
      param_largest, buffer, &largest_writer, &serialization_context);
  params->largest.Set(
      largest_writer.is_null() ? nullptr : largest_writer.data());
  MOJO_INTERNAL_DLOG_SERIALIZATION_WARNING(
      params->largest.is_null(),
      mojo::internal::VALIDATION_ERROR_UNEXPECTED_NULL_POINTER,
      "null largest in RectService.GetLargestRect response");
  message.AttachHandlesFromSerializationContext(
      &serialization_context);
    return message;
  }


  void Dispatch(RectService::GetLargestRectCallback* callback) {
    std::move(*callback).Run(
        std::move(param_largest_));
  }



 private:
  // mojo::internal::UnserializedMessageContext:
  void Serialize(mojo::internal::SerializationContext* serialization_context,
                 mojo::internal::Buffer* buffer) override {
    ::mojo::test::internal::RectService_GetLargestRect_ResponseParams_Data::BufferWriter
        params;
    params.Allocate(buffer);
  typename decltype(params->largest)::BaseType::BufferWriter
      largest_writer;
  mojo::internal::Serialize<::mojo::test::TypemappedRectDataView>(
      param_largest_, buffer, &largest_writer, serialization_context);
  params->largest.Set(
      largest_writer.is_null() ? nullptr : largest_writer.data());
  MOJO_INTERNAL_DLOG_SERIALIZATION_WARNING(
      params->largest.is_null(),
      mojo::internal::VALIDATION_ERROR_UNEXPECTED_NULL_POINTER,
      "null largest in RectService.GetLargestRect response");
  }
  mojo::test::RectBlink param_largest_;

  DISALLOW_COPY_AND_ASSIGN(RectService_GetLargestRect_Response_Message);
};

const mojo::internal::UnserializedMessageContext::Tag
RectService_GetLargestRect_Response_Message::kMessageTag = {};

bool RectService_GetLargestRect_ForwardToCallback::Accept(
    mojo::Message* message) {
#if BUILDFLAG(MOJO_TRACE_ENABLED)
  TRACE_EVENT1("mojom", "<class 'jinja2::utils::Namespace'>::RectService::GetLargestRectCallback",
               "message", message->name());
#endif
  mojo::internal::MessageDispatchContext dispatch_context(message);
  if (!message->is_serialized()) {
    auto context =
        message->TakeUnserializedContext<RectService_GetLargestRect_Response_Message>();
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
  internal::RectService_GetLargestRect_ResponseParams_Data* params =
      reinterpret_cast<
          internal::RectService_GetLargestRect_ResponseParams_Data*>(
              message->mutable_payload());
  
  mojo::internal::SerializationContext serialization_context;
  serialization_context.TakeHandlesFromMessage(message);
  bool success = true;
  mojo::test::RectBlink p_largest{};
  RectService_GetLargestRect_ResponseParamsDataView input_data_view(params, &serialization_context);
  
  if (!input_data_view.ReadLargest(&p_largest))
    success = false;
  if (!success) {
    ReportValidationErrorForMessage(
        message,
        mojo::internal::VALIDATION_ERROR_DESERIALIZATION_FAILED,
        RectService::Name_, 1, true);
    return false;
  }
  if (!callback_.is_null())
    std::move(callback_).Run(
std::move(p_largest));
  return true;
}

void RectService_GetLargestRect_ProxyToResponder::Run(
    const mojo::test::RectBlink& in_largest) {
  const bool kSerialize = responder_->PrefersSerializedMessages();
  auto message = RectService_GetLargestRect_Response_Message::Build(kSerialize, is_sync_, std::move(in_largest));

#if BUILDFLAG(MOJO_TRACE_ENABLED)
  TRACE_EVENT1("mojom", "(Impl)<class 'jinja2::utils::Namespace'>::RectService::GetLargestRectCallback",
               "message", message.name());
#endif

#if defined(ENABLE_IPC_FUZZER)
  message.set_interface_name(RectService::Name_);
  message.set_method_name("GetLargestRect");
#endif

  message.set_request_id(request_id_);
  ignore_result(responder_->Accept(&message));
  // TODO(darin): Accept() returning false indicates a malformed message, and
  // that may be good reason to close the connection. However, we don't have a
  // way to do that from here. We should add a way.
  responder_ = nullptr;
}
class RectService_PassSharedRect_ProxyToResponder {
 public:
  static RectService::PassSharedRectCallback CreateCallback(
      uint64_t request_id,
      bool is_sync,
      std::unique_ptr<mojo::MessageReceiverWithStatus> responder) {
    std::unique_ptr<RectService_PassSharedRect_ProxyToResponder> proxy(
        new RectService_PassSharedRect_ProxyToResponder(
            request_id, is_sync, std::move(responder)));
    return base::BindOnce(&RectService_PassSharedRect_ProxyToResponder::Run,
                          std::move(proxy));
  }

  ~RectService_PassSharedRect_ProxyToResponder() {
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
  RectService_PassSharedRect_ProxyToResponder(
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
        << "RectService::PassSharedRectCallback was destroyed without "
        << "first either being run or its corresponding binding being closed. "
        << "It is an error to drop response callbacks which still correspond "
        << "to an open interface pipe.";
  }
#endif

  void Run(
      const mojo::test::SharedRect& in_passed);

  uint64_t request_id_;
  bool is_sync_;
  std::unique_ptr<mojo::MessageReceiverWithStatus> responder_;

  DISALLOW_COPY_AND_ASSIGN(RectService_PassSharedRect_ProxyToResponder);
};
class RectService_PassSharedRect_Response_Message
    : public mojo::internal::UnserializedMessageContext {
 public:
  static const mojo::internal::UnserializedMessageContext::Tag kMessageTag;

  explicit RectService_PassSharedRect_Response_Message(
      uint32_t message_flags
      , const mojo::test::SharedRect& param_passed
  )
      : mojo::internal::UnserializedMessageContext(
          &kMessageTag,
          internal::kRectService_PassSharedRect_Name,
          message_flags)
      , param_passed_(std::move(param_passed)){}
  ~RectService_PassSharedRect_Response_Message() override = default;

  static mojo::Message Build(
      bool serialize,
      bool is_sync,
      const mojo::test::SharedRect& param_passed) {

    
  const uint32_t kFlags = mojo::Message::kFlagIsResponse |
      ((is_sync) ? mojo::Message::kFlagIsSync : 0);

    if (!serialize) {
      return mojo::Message(std::make_unique<RectService_PassSharedRect_Response_Message>(
          kFlags
          , std::move(param_passed)
          ));
    }

    DCHECK(serialize);
    
  mojo::Message message(
      internal::kRectService_PassSharedRect_Name, kFlags, 0, 0, nullptr);
  auto* buffer = message.payload_buffer();
  ::mojo::test::internal::RectService_PassSharedRect_ResponseParams_Data::BufferWriter
      params;
  mojo::internal::SerializationContext serialization_context;
  params.Allocate(buffer);
  typename decltype(params->passed)::BaseType::BufferWriter
      passed_writer;
  mojo::internal::Serialize<::mojo::test::SharedTypemappedRectDataView>(
      param_passed, buffer, &passed_writer, &serialization_context);
  params->passed.Set(
      passed_writer.is_null() ? nullptr : passed_writer.data());
  MOJO_INTERNAL_DLOG_SERIALIZATION_WARNING(
      params->passed.is_null(),
      mojo::internal::VALIDATION_ERROR_UNEXPECTED_NULL_POINTER,
      "null passed in RectService.PassSharedRect response");
  message.AttachHandlesFromSerializationContext(
      &serialization_context);
    return message;
  }


  void Dispatch(RectService::PassSharedRectCallback* callback) {
    std::move(*callback).Run(
        std::move(param_passed_));
  }



 private:
  // mojo::internal::UnserializedMessageContext:
  void Serialize(mojo::internal::SerializationContext* serialization_context,
                 mojo::internal::Buffer* buffer) override {
    ::mojo::test::internal::RectService_PassSharedRect_ResponseParams_Data::BufferWriter
        params;
    params.Allocate(buffer);
  typename decltype(params->passed)::BaseType::BufferWriter
      passed_writer;
  mojo::internal::Serialize<::mojo::test::SharedTypemappedRectDataView>(
      param_passed_, buffer, &passed_writer, serialization_context);
  params->passed.Set(
      passed_writer.is_null() ? nullptr : passed_writer.data());
  MOJO_INTERNAL_DLOG_SERIALIZATION_WARNING(
      params->passed.is_null(),
      mojo::internal::VALIDATION_ERROR_UNEXPECTED_NULL_POINTER,
      "null passed in RectService.PassSharedRect response");
  }
  mojo::test::SharedRect param_passed_;

  DISALLOW_COPY_AND_ASSIGN(RectService_PassSharedRect_Response_Message);
};

const mojo::internal::UnserializedMessageContext::Tag
RectService_PassSharedRect_Response_Message::kMessageTag = {};

bool RectService_PassSharedRect_ForwardToCallback::Accept(
    mojo::Message* message) {
#if BUILDFLAG(MOJO_TRACE_ENABLED)
  TRACE_EVENT1("mojom", "<class 'jinja2::utils::Namespace'>::RectService::PassSharedRectCallback",
               "message", message->name());
#endif
  mojo::internal::MessageDispatchContext dispatch_context(message);
  if (!message->is_serialized()) {
    auto context =
        message->TakeUnserializedContext<RectService_PassSharedRect_Response_Message>();
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
  internal::RectService_PassSharedRect_ResponseParams_Data* params =
      reinterpret_cast<
          internal::RectService_PassSharedRect_ResponseParams_Data*>(
              message->mutable_payload());
  
  mojo::internal::SerializationContext serialization_context;
  serialization_context.TakeHandlesFromMessage(message);
  bool success = true;
  mojo::test::SharedRect p_passed{};
  RectService_PassSharedRect_ResponseParamsDataView input_data_view(params, &serialization_context);
  
  if (!input_data_view.ReadPassed(&p_passed))
    success = false;
  if (!success) {
    ReportValidationErrorForMessage(
        message,
        mojo::internal::VALIDATION_ERROR_DESERIALIZATION_FAILED,
        RectService::Name_, 2, true);
    return false;
  }
  if (!callback_.is_null())
    std::move(callback_).Run(
std::move(p_passed));
  return true;
}

void RectService_PassSharedRect_ProxyToResponder::Run(
    const mojo::test::SharedRect& in_passed) {
  const bool kSerialize = responder_->PrefersSerializedMessages();
  auto message = RectService_PassSharedRect_Response_Message::Build(kSerialize, is_sync_, std::move(in_passed));

#if BUILDFLAG(MOJO_TRACE_ENABLED)
  TRACE_EVENT1("mojom", "(Impl)<class 'jinja2::utils::Namespace'>::RectService::PassSharedRectCallback",
               "message", message.name());
#endif

#if defined(ENABLE_IPC_FUZZER)
  message.set_interface_name(RectService::Name_);
  message.set_method_name("PassSharedRect");
#endif

  message.set_request_id(request_id_);
  ignore_result(responder_->Accept(&message));
  // TODO(darin): Accept() returning false indicates a malformed message, and
  // that may be good reason to close the connection. However, we don't have a
  // way to do that from here. We should add a way.
  responder_ = nullptr;
}

// static
bool RectServiceStubDispatch::Accept(
    RectService* impl,
    mojo::Message* message) {
  switch (message->header()->name) {
    case internal::kRectService_AddRect_Name: {
#if BUILDFLAG(MOJO_TRACE_ENABLED)
      TRACE_EVENT1(
          "mojom",
          "(Impl)<class 'jinja2::utils::Namespace'>::RectService::AddRect",
          "message", message->name());
#endif
      base::TaskAnnotator::ScopedSetIpcProgramCounter scoped_ipc_pc(
          base::GetProgramCounter());
      mojo::internal::MessageDispatchContext context(message);
      if (!message->is_serialized()) {
        auto context = message->TakeUnserializedContext<
            RectServiceProxy_AddRect_Message>();
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
      internal::RectService_AddRect_Params_Data* params =
          reinterpret_cast<internal::RectService_AddRect_Params_Data*>(
              message->mutable_payload());
      
      mojo::internal::SerializationContext serialization_context;
      serialization_context.TakeHandlesFromMessage(message);
      bool success = true;
      mojo::test::RectBlink p_r{};
      RectService_AddRect_ParamsDataView input_data_view(params, &serialization_context);
      
      if (!input_data_view.ReadR(&p_r))
        success = false;
      if (!success) {
        ReportValidationErrorForMessage(
            message,
            mojo::internal::VALIDATION_ERROR_DESERIALIZATION_FAILED,
            RectService::Name_, 0, false);
        return false;
      }
      // A null |impl| means no implementation was bound.
      DCHECK(impl);
      impl->AddRect(
std::move(p_r));
      return true;
    }
    case internal::kRectService_GetLargestRect_Name: {
      break;
    }
    case internal::kRectService_PassSharedRect_Name: {
      break;
    }
  }
  return false;
}

// static
bool RectServiceStubDispatch::AcceptWithResponder(
    RectService* impl,
    mojo::Message* message,
    std::unique_ptr<mojo::MessageReceiverWithStatus> responder) {
  switch (message->header()->name) {
    case internal::kRectService_AddRect_Name: {
      break;
    }
    case internal::kRectService_GetLargestRect_Name: {
#if BUILDFLAG(MOJO_TRACE_ENABLED)
      TRACE_EVENT1(
          "mojom",
          "(Impl)<class 'jinja2::utils::Namespace'>::RectService::GetLargestRect",
          "message", message->name());
#endif
      base::TaskAnnotator::ScopedSetIpcProgramCounter scoped_ipc_pc(
          base::GetProgramCounter());
      mojo::internal::MessageDispatchContext context(message);
      if (!message->is_serialized()) {
        auto context = message->TakeUnserializedContext<
            RectServiceProxy_GetLargestRect_Message>();
        if (!context) {
          // The Message was not of the expected type. It may be a valid message
          // which was built using a different variant of these bindings. Force
          // serialization before dispatch in this case.
          message->SerializeIfNecessary();
        } else {
          RectService::GetLargestRectCallback callback =
              RectService_GetLargestRect_ProxyToResponder::CreateCallback(
                  message->request_id(),
                  message->has_flag(mojo::Message::kFlagIsSync),
                  std::move(responder));
          context->Dispatch(impl, std::move(callback));
          return true;
        }
      }

      internal::RectService_GetLargestRect_Params_Data* params =
          reinterpret_cast<
              internal::RectService_GetLargestRect_Params_Data*>(
                  message->mutable_payload());
      
      mojo::internal::SerializationContext serialization_context;
      serialization_context.TakeHandlesFromMessage(message);
      bool success = true;
      RectService_GetLargestRect_ParamsDataView input_data_view(params, &serialization_context);
      
      if (!success) {
        ReportValidationErrorForMessage(
            message,
            mojo::internal::VALIDATION_ERROR_DESERIALIZATION_FAILED,
            RectService::Name_, 1, false);
        return false;
      }
      RectService::GetLargestRectCallback callback =
          RectService_GetLargestRect_ProxyToResponder::CreateCallback(
              message->request_id(),
              message->has_flag(mojo::Message::kFlagIsSync),
              std::move(responder));
      // A null |impl| means no implementation was bound.
      DCHECK(impl);
      impl->GetLargestRect(std::move(callback));
      return true;
    }
    case internal::kRectService_PassSharedRect_Name: {
#if BUILDFLAG(MOJO_TRACE_ENABLED)
      TRACE_EVENT1(
          "mojom",
          "(Impl)<class 'jinja2::utils::Namespace'>::RectService::PassSharedRect",
          "message", message->name());
#endif
      base::TaskAnnotator::ScopedSetIpcProgramCounter scoped_ipc_pc(
          base::GetProgramCounter());
      mojo::internal::MessageDispatchContext context(message);
      if (!message->is_serialized()) {
        auto context = message->TakeUnserializedContext<
            RectServiceProxy_PassSharedRect_Message>();
        if (!context) {
          // The Message was not of the expected type. It may be a valid message
          // which was built using a different variant of these bindings. Force
          // serialization before dispatch in this case.
          message->SerializeIfNecessary();
        } else {
          RectService::PassSharedRectCallback callback =
              RectService_PassSharedRect_ProxyToResponder::CreateCallback(
                  message->request_id(),
                  message->has_flag(mojo::Message::kFlagIsSync),
                  std::move(responder));
          context->Dispatch(impl, std::move(callback));
          return true;
        }
      }

      internal::RectService_PassSharedRect_Params_Data* params =
          reinterpret_cast<
              internal::RectService_PassSharedRect_Params_Data*>(
                  message->mutable_payload());
      
      mojo::internal::SerializationContext serialization_context;
      serialization_context.TakeHandlesFromMessage(message);
      bool success = true;
      mojo::test::SharedRect p_r{};
      RectService_PassSharedRect_ParamsDataView input_data_view(params, &serialization_context);
      
      if (!input_data_view.ReadR(&p_r))
        success = false;
      if (!success) {
        ReportValidationErrorForMessage(
            message,
            mojo::internal::VALIDATION_ERROR_DESERIALIZATION_FAILED,
            RectService::Name_, 2, false);
        return false;
      }
      RectService::PassSharedRectCallback callback =
          RectService_PassSharedRect_ProxyToResponder::CreateCallback(
              message->request_id(),
              message->has_flag(mojo::Message::kFlagIsSync),
              std::move(responder));
      // A null |impl| means no implementation was bound.
      DCHECK(impl);
      impl->PassSharedRect(
std::move(p_r), std::move(callback));
      return true;
    }
  }
  return false;
}

bool RectServiceRequestValidator::Accept(mojo::Message* message) {
  if (!message->is_serialized() ||
      mojo::internal::ControlMessageHandler::IsControlMessage(message)) {
    return true;
  }

  mojo::internal::ValidationContext validation_context(
    message->payload(), message->payload_num_bytes(),
    message->handles()->size(), message->payload_num_interface_ids(), message,
    "RectService RequestValidator");

  switch (message->header()->name) {
    case internal::kRectService_AddRect_Name: {
      if (!mojo::internal::ValidateMessageIsRequestWithoutResponse(
              message, &validation_context)) {
        return false;
      }
      if (!mojo::internal::ValidateMessagePayload<
               internal::RectService_AddRect_Params_Data>(
                  message, &validation_context)) {
        return false;
      }
      return true;
    }
    case internal::kRectService_GetLargestRect_Name: {
      if (!mojo::internal::ValidateMessageIsRequestExpectingResponse(
              message, &validation_context)) {
        return false;
      }
      if (!mojo::internal::ValidateMessagePayload<
               internal::RectService_GetLargestRect_Params_Data>(
                  message, &validation_context)) {
        return false;
      }
      return true;
    }
    case internal::kRectService_PassSharedRect_Name: {
      if (!mojo::internal::ValidateMessageIsRequestExpectingResponse(
              message, &validation_context)) {
        return false;
      }
      if (!mojo::internal::ValidateMessagePayload<
               internal::RectService_PassSharedRect_Params_Data>(
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

bool RectServiceResponseValidator::Accept(mojo::Message* message) {
  if (!message->is_serialized() ||
      mojo::internal::ControlMessageHandler::IsControlMessage(message)) {
    return true;
  }

  mojo::internal::ValidationContext validation_context(
    message->payload(), message->payload_num_bytes(),
    message->handles()->size(), message->payload_num_interface_ids(), message,
    "RectService ResponseValidator");

  if (!mojo::internal::ValidateMessageIsResponse(message, &validation_context))
    return false;
  switch (message->header()->name) {
    case internal::kRectService_GetLargestRect_Name: {
      if (!mojo::internal::ValidateMessagePayload<
               internal::RectService_GetLargestRect_ResponseParams_Data>(
                    message, &validation_context)) {
        return false;
      }
      return true;
    }
    case internal::kRectService_PassSharedRect_Name: {
      if (!mojo::internal::ValidateMessagePayload<
               internal::RectService_PassSharedRect_ResponseParams_Data>(
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
const char NativeTypeTester::Name_[] = "mojo.test.NativeTypeTester";

class NativeTypeTester_PassNativeStruct_ForwardToCallback
    : public mojo::MessageReceiver {
 public:
  NativeTypeTester_PassNativeStruct_ForwardToCallback(
      NativeTypeTester::PassNativeStructCallback callback
      ) : callback_(std::move(callback)) {
  }
  bool Accept(mojo::Message* message) override;
 private:
  NativeTypeTester::PassNativeStructCallback callback_;
  DISALLOW_COPY_AND_ASSIGN(NativeTypeTester_PassNativeStruct_ForwardToCallback);
};

class NativeTypeTester_PassNativeStructWithAttachments_ForwardToCallback
    : public mojo::MessageReceiver {
 public:
  NativeTypeTester_PassNativeStructWithAttachments_ForwardToCallback(
      NativeTypeTester::PassNativeStructWithAttachmentsCallback callback
      ) : callback_(std::move(callback)) {
  }
  bool Accept(mojo::Message* message) override;
 private:
  NativeTypeTester::PassNativeStructWithAttachmentsCallback callback_;
  DISALLOW_COPY_AND_ASSIGN(NativeTypeTester_PassNativeStructWithAttachments_ForwardToCallback);
};

NativeTypeTesterProxy::NativeTypeTesterProxy(mojo::MessageReceiverWithResponder* receiver)
    : receiver_(receiver) {
}
class NativeTypeTesterProxy_PassNativeStruct_Message
    : public mojo::internal::UnserializedMessageContext {
 public:
  static const mojo::internal::UnserializedMessageContext::Tag kMessageTag;

  explicit NativeTypeTesterProxy_PassNativeStruct_Message(
      uint32_t message_flags
      , TestNativeStructMojomPtr param_s
  )
      : mojo::internal::UnserializedMessageContext(
          &kMessageTag,
          internal::kNativeTypeTester_PassNativeStruct_Name,
          message_flags)
      , param_s_(std::move(param_s)){}
  ~NativeTypeTesterProxy_PassNativeStruct_Message() override = default;

  static mojo::Message Build(
      bool serialize,
      bool expects_response,
      bool is_sync,
      TestNativeStructMojomPtr param_s) {

    
  const uint32_t kFlags =
      ((expects_response) ? mojo::Message::kFlagExpectsResponse : 0) |
      ((is_sync) ? mojo::Message::kFlagIsSync : 0);

    if (!serialize) {
      return mojo::Message(std::make_unique<NativeTypeTesterProxy_PassNativeStruct_Message>(
          kFlags
          , std::move(param_s)
          ));
    }

    DCHECK(serialize);
    
  mojo::Message message(
      internal::kNativeTypeTester_PassNativeStruct_Name, kFlags, 0, 0, nullptr);
  auto* buffer = message.payload_buffer();
  ::mojo::test::internal::NativeTypeTester_PassNativeStruct_Params_Data::BufferWriter
      params;
  mojo::internal::SerializationContext serialization_context;
  params.Allocate(buffer);
  typename decltype(params->s)::BaseType::BufferWriter
      s_writer;
  mojo::internal::Serialize<::mojo::test::TestNativeStructMojomDataView>(
      param_s, buffer, &s_writer, &serialization_context);
  params->s.Set(
      s_writer.is_null() ? nullptr : s_writer.data());
  MOJO_INTERNAL_DLOG_SERIALIZATION_WARNING(
      params->s.is_null(),
      mojo::internal::VALIDATION_ERROR_UNEXPECTED_NULL_POINTER,
      "null s in NativeTypeTester.PassNativeStruct request");
  message.AttachHandlesFromSerializationContext(
      &serialization_context);
    return message;
  }


  void Dispatch(NativeTypeTester* impl, NativeTypeTester::PassNativeStructCallback callback) {
    impl->PassNativeStruct(
        std::move(param_s_), std::move(callback));
  }

 private:
  // mojo::internal::UnserializedMessageContext:
  void Serialize(mojo::internal::SerializationContext* serialization_context,
                 mojo::internal::Buffer* buffer) override {
    ::mojo::test::internal::NativeTypeTester_PassNativeStruct_Params_Data::BufferWriter
        params;
    params.Allocate(buffer);
  typename decltype(params->s)::BaseType::BufferWriter
      s_writer;
  mojo::internal::Serialize<::mojo::test::TestNativeStructMojomDataView>(
      param_s_, buffer, &s_writer, serialization_context);
  params->s.Set(
      s_writer.is_null() ? nullptr : s_writer.data());
  MOJO_INTERNAL_DLOG_SERIALIZATION_WARNING(
      params->s.is_null(),
      mojo::internal::VALIDATION_ERROR_UNEXPECTED_NULL_POINTER,
      "null s in NativeTypeTester.PassNativeStruct request");
  }
  TestNativeStructMojomPtr param_s_;

  DISALLOW_COPY_AND_ASSIGN(NativeTypeTesterProxy_PassNativeStruct_Message);
};

const mojo::internal::UnserializedMessageContext::Tag
NativeTypeTesterProxy_PassNativeStruct_Message::kMessageTag = {};

void NativeTypeTesterProxy::PassNativeStruct(
    TestNativeStructMojomPtr in_s, PassNativeStructCallback callback) {
#if BUILDFLAG(MOJO_TRACE_ENABLED)
  TRACE_EVENT0("mojom", "<class 'jinja2::utils::Namespace'>::NativeTypeTester::PassNativeStruct");
#endif
  const bool kExpectsResponse = true;
  const bool kIsSync = false;
  const bool kSerialize = receiver_->PrefersSerializedMessages();
  auto message = NativeTypeTesterProxy_PassNativeStruct_Message::Build(
      kSerialize, kExpectsResponse, kIsSync, std::move(in_s));

#if defined(ENABLE_IPC_FUZZER)
  message.set_interface_name(NativeTypeTester::Name_);
  message.set_method_name("PassNativeStruct");
#endif
  std::unique_ptr<mojo::MessageReceiver> responder(
      new NativeTypeTester_PassNativeStruct_ForwardToCallback(
          std::move(callback)));
  ignore_result(receiver_->AcceptWithResponder(&message, std::move(responder)));
}
class NativeTypeTesterProxy_PassNativeStructWithAttachments_Message
    : public mojo::internal::UnserializedMessageContext {
 public:
  static const mojo::internal::UnserializedMessageContext::Tag kMessageTag;

  explicit NativeTypeTesterProxy_PassNativeStructWithAttachments_Message(
      uint32_t message_flags
      , TestNativeStructWithAttachmentsMojomPtr param_s
  )
      : mojo::internal::UnserializedMessageContext(
          &kMessageTag,
          internal::kNativeTypeTester_PassNativeStructWithAttachments_Name,
          message_flags)
      , param_s_(std::move(param_s)){}
  ~NativeTypeTesterProxy_PassNativeStructWithAttachments_Message() override = default;

  static mojo::Message Build(
      bool serialize,
      bool expects_response,
      bool is_sync,
      TestNativeStructWithAttachmentsMojomPtr param_s) {

    
  const uint32_t kFlags =
      ((expects_response) ? mojo::Message::kFlagExpectsResponse : 0) |
      ((is_sync) ? mojo::Message::kFlagIsSync : 0);

    if (!serialize) {
      return mojo::Message(std::make_unique<NativeTypeTesterProxy_PassNativeStructWithAttachments_Message>(
          kFlags
          , std::move(param_s)
          ));
    }

    DCHECK(serialize);
    
  mojo::Message message(
      internal::kNativeTypeTester_PassNativeStructWithAttachments_Name, kFlags, 0, 0, nullptr);
  auto* buffer = message.payload_buffer();
  ::mojo::test::internal::NativeTypeTester_PassNativeStructWithAttachments_Params_Data::BufferWriter
      params;
  mojo::internal::SerializationContext serialization_context;
  params.Allocate(buffer);
  typename decltype(params->s)::BaseType::BufferWriter
      s_writer;
  mojo::internal::Serialize<::mojo::test::TestNativeStructWithAttachmentsMojomDataView>(
      param_s, buffer, &s_writer, &serialization_context);
  params->s.Set(
      s_writer.is_null() ? nullptr : s_writer.data());
  MOJO_INTERNAL_DLOG_SERIALIZATION_WARNING(
      params->s.is_null(),
      mojo::internal::VALIDATION_ERROR_UNEXPECTED_NULL_POINTER,
      "null s in NativeTypeTester.PassNativeStructWithAttachments request");
  message.AttachHandlesFromSerializationContext(
      &serialization_context);
    return message;
  }


  void Dispatch(NativeTypeTester* impl, NativeTypeTester::PassNativeStructWithAttachmentsCallback callback) {
    impl->PassNativeStructWithAttachments(
        std::move(param_s_), std::move(callback));
  }

 private:
  // mojo::internal::UnserializedMessageContext:
  void Serialize(mojo::internal::SerializationContext* serialization_context,
                 mojo::internal::Buffer* buffer) override {
    ::mojo::test::internal::NativeTypeTester_PassNativeStructWithAttachments_Params_Data::BufferWriter
        params;
    params.Allocate(buffer);
  typename decltype(params->s)::BaseType::BufferWriter
      s_writer;
  mojo::internal::Serialize<::mojo::test::TestNativeStructWithAttachmentsMojomDataView>(
      param_s_, buffer, &s_writer, serialization_context);
  params->s.Set(
      s_writer.is_null() ? nullptr : s_writer.data());
  MOJO_INTERNAL_DLOG_SERIALIZATION_WARNING(
      params->s.is_null(),
      mojo::internal::VALIDATION_ERROR_UNEXPECTED_NULL_POINTER,
      "null s in NativeTypeTester.PassNativeStructWithAttachments request");
  }
  TestNativeStructWithAttachmentsMojomPtr param_s_;

  DISALLOW_COPY_AND_ASSIGN(NativeTypeTesterProxy_PassNativeStructWithAttachments_Message);
};

const mojo::internal::UnserializedMessageContext::Tag
NativeTypeTesterProxy_PassNativeStructWithAttachments_Message::kMessageTag = {};

void NativeTypeTesterProxy::PassNativeStructWithAttachments(
    TestNativeStructWithAttachmentsMojomPtr in_s, PassNativeStructWithAttachmentsCallback callback) {
#if BUILDFLAG(MOJO_TRACE_ENABLED)
  TRACE_EVENT0("mojom", "<class 'jinja2::utils::Namespace'>::NativeTypeTester::PassNativeStructWithAttachments");
#endif
  const bool kExpectsResponse = true;
  const bool kIsSync = false;
  const bool kSerialize = receiver_->PrefersSerializedMessages();
  auto message = NativeTypeTesterProxy_PassNativeStructWithAttachments_Message::Build(
      kSerialize, kExpectsResponse, kIsSync, std::move(in_s));

#if defined(ENABLE_IPC_FUZZER)
  message.set_interface_name(NativeTypeTester::Name_);
  message.set_method_name("PassNativeStructWithAttachments");
#endif
  std::unique_ptr<mojo::MessageReceiver> responder(
      new NativeTypeTester_PassNativeStructWithAttachments_ForwardToCallback(
          std::move(callback)));
  ignore_result(receiver_->AcceptWithResponder(&message, std::move(responder)));
}
class NativeTypeTester_PassNativeStruct_ProxyToResponder {
 public:
  static NativeTypeTester::PassNativeStructCallback CreateCallback(
      uint64_t request_id,
      bool is_sync,
      std::unique_ptr<mojo::MessageReceiverWithStatus> responder) {
    std::unique_ptr<NativeTypeTester_PassNativeStruct_ProxyToResponder> proxy(
        new NativeTypeTester_PassNativeStruct_ProxyToResponder(
            request_id, is_sync, std::move(responder)));
    return base::BindOnce(&NativeTypeTester_PassNativeStruct_ProxyToResponder::Run,
                          std::move(proxy));
  }

  ~NativeTypeTester_PassNativeStruct_ProxyToResponder() {
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
  NativeTypeTester_PassNativeStruct_ProxyToResponder(
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
        << "NativeTypeTester::PassNativeStructCallback was destroyed without "
        << "first either being run or its corresponding binding being closed. "
        << "It is an error to drop response callbacks which still correspond "
        << "to an open interface pipe.";
  }
#endif

  void Run(
      TestNativeStructMojomPtr in_passed);

  uint64_t request_id_;
  bool is_sync_;
  std::unique_ptr<mojo::MessageReceiverWithStatus> responder_;

  DISALLOW_COPY_AND_ASSIGN(NativeTypeTester_PassNativeStruct_ProxyToResponder);
};
class NativeTypeTester_PassNativeStruct_Response_Message
    : public mojo::internal::UnserializedMessageContext {
 public:
  static const mojo::internal::UnserializedMessageContext::Tag kMessageTag;

  explicit NativeTypeTester_PassNativeStruct_Response_Message(
      uint32_t message_flags
      , TestNativeStructMojomPtr param_passed
  )
      : mojo::internal::UnserializedMessageContext(
          &kMessageTag,
          internal::kNativeTypeTester_PassNativeStruct_Name,
          message_flags)
      , param_passed_(std::move(param_passed)){}
  ~NativeTypeTester_PassNativeStruct_Response_Message() override = default;

  static mojo::Message Build(
      bool serialize,
      bool is_sync,
      TestNativeStructMojomPtr param_passed) {

    
  const uint32_t kFlags = mojo::Message::kFlagIsResponse |
      ((is_sync) ? mojo::Message::kFlagIsSync : 0);

    if (!serialize) {
      return mojo::Message(std::make_unique<NativeTypeTester_PassNativeStruct_Response_Message>(
          kFlags
          , std::move(param_passed)
          ));
    }

    DCHECK(serialize);
    
  mojo::Message message(
      internal::kNativeTypeTester_PassNativeStruct_Name, kFlags, 0, 0, nullptr);
  auto* buffer = message.payload_buffer();
  ::mojo::test::internal::NativeTypeTester_PassNativeStruct_ResponseParams_Data::BufferWriter
      params;
  mojo::internal::SerializationContext serialization_context;
  params.Allocate(buffer);
  typename decltype(params->passed)::BaseType::BufferWriter
      passed_writer;
  mojo::internal::Serialize<::mojo::test::TestNativeStructMojomDataView>(
      param_passed, buffer, &passed_writer, &serialization_context);
  params->passed.Set(
      passed_writer.is_null() ? nullptr : passed_writer.data());
  MOJO_INTERNAL_DLOG_SERIALIZATION_WARNING(
      params->passed.is_null(),
      mojo::internal::VALIDATION_ERROR_UNEXPECTED_NULL_POINTER,
      "null passed in NativeTypeTester.PassNativeStruct response");
  message.AttachHandlesFromSerializationContext(
      &serialization_context);
    return message;
  }


  void Dispatch(NativeTypeTester::PassNativeStructCallback* callback) {
    std::move(*callback).Run(
        std::move(param_passed_));
  }



 private:
  // mojo::internal::UnserializedMessageContext:
  void Serialize(mojo::internal::SerializationContext* serialization_context,
                 mojo::internal::Buffer* buffer) override {
    ::mojo::test::internal::NativeTypeTester_PassNativeStruct_ResponseParams_Data::BufferWriter
        params;
    params.Allocate(buffer);
  typename decltype(params->passed)::BaseType::BufferWriter
      passed_writer;
  mojo::internal::Serialize<::mojo::test::TestNativeStructMojomDataView>(
      param_passed_, buffer, &passed_writer, serialization_context);
  params->passed.Set(
      passed_writer.is_null() ? nullptr : passed_writer.data());
  MOJO_INTERNAL_DLOG_SERIALIZATION_WARNING(
      params->passed.is_null(),
      mojo::internal::VALIDATION_ERROR_UNEXPECTED_NULL_POINTER,
      "null passed in NativeTypeTester.PassNativeStruct response");
  }
  TestNativeStructMojomPtr param_passed_;

  DISALLOW_COPY_AND_ASSIGN(NativeTypeTester_PassNativeStruct_Response_Message);
};

const mojo::internal::UnserializedMessageContext::Tag
NativeTypeTester_PassNativeStruct_Response_Message::kMessageTag = {};

bool NativeTypeTester_PassNativeStruct_ForwardToCallback::Accept(
    mojo::Message* message) {
#if BUILDFLAG(MOJO_TRACE_ENABLED)
  TRACE_EVENT1("mojom", "<class 'jinja2::utils::Namespace'>::NativeTypeTester::PassNativeStructCallback",
               "message", message->name());
#endif
  mojo::internal::MessageDispatchContext dispatch_context(message);
  if (!message->is_serialized()) {
    auto context =
        message->TakeUnserializedContext<NativeTypeTester_PassNativeStruct_Response_Message>();
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
  internal::NativeTypeTester_PassNativeStruct_ResponseParams_Data* params =
      reinterpret_cast<
          internal::NativeTypeTester_PassNativeStruct_ResponseParams_Data*>(
              message->mutable_payload());
  
  mojo::internal::SerializationContext serialization_context;
  serialization_context.TakeHandlesFromMessage(message);
  bool success = true;
  TestNativeStructMojomPtr p_passed{};
  NativeTypeTester_PassNativeStruct_ResponseParamsDataView input_data_view(params, &serialization_context);
  
  if (!input_data_view.ReadPassed(&p_passed))
    success = false;
  if (!success) {
    ReportValidationErrorForMessage(
        message,
        mojo::internal::VALIDATION_ERROR_DESERIALIZATION_FAILED,
        NativeTypeTester::Name_, 0, true);
    return false;
  }
  if (!callback_.is_null())
    std::move(callback_).Run(
std::move(p_passed));
  return true;
}

void NativeTypeTester_PassNativeStruct_ProxyToResponder::Run(
    TestNativeStructMojomPtr in_passed) {
  const bool kSerialize = responder_->PrefersSerializedMessages();
  auto message = NativeTypeTester_PassNativeStruct_Response_Message::Build(kSerialize, is_sync_, std::move(in_passed));

#if BUILDFLAG(MOJO_TRACE_ENABLED)
  TRACE_EVENT1("mojom", "(Impl)<class 'jinja2::utils::Namespace'>::NativeTypeTester::PassNativeStructCallback",
               "message", message.name());
#endif

#if defined(ENABLE_IPC_FUZZER)
  message.set_interface_name(NativeTypeTester::Name_);
  message.set_method_name("PassNativeStruct");
#endif

  message.set_request_id(request_id_);
  ignore_result(responder_->Accept(&message));
  // TODO(darin): Accept() returning false indicates a malformed message, and
  // that may be good reason to close the connection. However, we don't have a
  // way to do that from here. We should add a way.
  responder_ = nullptr;
}
class NativeTypeTester_PassNativeStructWithAttachments_ProxyToResponder {
 public:
  static NativeTypeTester::PassNativeStructWithAttachmentsCallback CreateCallback(
      uint64_t request_id,
      bool is_sync,
      std::unique_ptr<mojo::MessageReceiverWithStatus> responder) {
    std::unique_ptr<NativeTypeTester_PassNativeStructWithAttachments_ProxyToResponder> proxy(
        new NativeTypeTester_PassNativeStructWithAttachments_ProxyToResponder(
            request_id, is_sync, std::move(responder)));
    return base::BindOnce(&NativeTypeTester_PassNativeStructWithAttachments_ProxyToResponder::Run,
                          std::move(proxy));
  }

  ~NativeTypeTester_PassNativeStructWithAttachments_ProxyToResponder() {
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
  NativeTypeTester_PassNativeStructWithAttachments_ProxyToResponder(
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
        << "NativeTypeTester::PassNativeStructWithAttachmentsCallback was destroyed without "
        << "first either being run or its corresponding binding being closed. "
        << "It is an error to drop response callbacks which still correspond "
        << "to an open interface pipe.";
  }
#endif

  void Run(
      TestNativeStructWithAttachmentsMojomPtr in_s);

  uint64_t request_id_;
  bool is_sync_;
  std::unique_ptr<mojo::MessageReceiverWithStatus> responder_;

  DISALLOW_COPY_AND_ASSIGN(NativeTypeTester_PassNativeStructWithAttachments_ProxyToResponder);
};
class NativeTypeTester_PassNativeStructWithAttachments_Response_Message
    : public mojo::internal::UnserializedMessageContext {
 public:
  static const mojo::internal::UnserializedMessageContext::Tag kMessageTag;

  explicit NativeTypeTester_PassNativeStructWithAttachments_Response_Message(
      uint32_t message_flags
      , TestNativeStructWithAttachmentsMojomPtr param_s
  )
      : mojo::internal::UnserializedMessageContext(
          &kMessageTag,
          internal::kNativeTypeTester_PassNativeStructWithAttachments_Name,
          message_flags)
      , param_s_(std::move(param_s)){}
  ~NativeTypeTester_PassNativeStructWithAttachments_Response_Message() override = default;

  static mojo::Message Build(
      bool serialize,
      bool is_sync,
      TestNativeStructWithAttachmentsMojomPtr param_s) {

    
  const uint32_t kFlags = mojo::Message::kFlagIsResponse |
      ((is_sync) ? mojo::Message::kFlagIsSync : 0);

    if (!serialize) {
      return mojo::Message(std::make_unique<NativeTypeTester_PassNativeStructWithAttachments_Response_Message>(
          kFlags
          , std::move(param_s)
          ));
    }

    DCHECK(serialize);
    
  mojo::Message message(
      internal::kNativeTypeTester_PassNativeStructWithAttachments_Name, kFlags, 0, 0, nullptr);
  auto* buffer = message.payload_buffer();
  ::mojo::test::internal::NativeTypeTester_PassNativeStructWithAttachments_ResponseParams_Data::BufferWriter
      params;
  mojo::internal::SerializationContext serialization_context;
  params.Allocate(buffer);
  typename decltype(params->s)::BaseType::BufferWriter
      s_writer;
  mojo::internal::Serialize<::mojo::test::TestNativeStructWithAttachmentsMojomDataView>(
      param_s, buffer, &s_writer, &serialization_context);
  params->s.Set(
      s_writer.is_null() ? nullptr : s_writer.data());
  MOJO_INTERNAL_DLOG_SERIALIZATION_WARNING(
      params->s.is_null(),
      mojo::internal::VALIDATION_ERROR_UNEXPECTED_NULL_POINTER,
      "null s in NativeTypeTester.PassNativeStructWithAttachments response");
  message.AttachHandlesFromSerializationContext(
      &serialization_context);
    return message;
  }


  void Dispatch(NativeTypeTester::PassNativeStructWithAttachmentsCallback* callback) {
    std::move(*callback).Run(
        std::move(param_s_));
  }



 private:
  // mojo::internal::UnserializedMessageContext:
  void Serialize(mojo::internal::SerializationContext* serialization_context,
                 mojo::internal::Buffer* buffer) override {
    ::mojo::test::internal::NativeTypeTester_PassNativeStructWithAttachments_ResponseParams_Data::BufferWriter
        params;
    params.Allocate(buffer);
  typename decltype(params->s)::BaseType::BufferWriter
      s_writer;
  mojo::internal::Serialize<::mojo::test::TestNativeStructWithAttachmentsMojomDataView>(
      param_s_, buffer, &s_writer, serialization_context);
  params->s.Set(
      s_writer.is_null() ? nullptr : s_writer.data());
  MOJO_INTERNAL_DLOG_SERIALIZATION_WARNING(
      params->s.is_null(),
      mojo::internal::VALIDATION_ERROR_UNEXPECTED_NULL_POINTER,
      "null s in NativeTypeTester.PassNativeStructWithAttachments response");
  }
  TestNativeStructWithAttachmentsMojomPtr param_s_;

  DISALLOW_COPY_AND_ASSIGN(NativeTypeTester_PassNativeStructWithAttachments_Response_Message);
};

const mojo::internal::UnserializedMessageContext::Tag
NativeTypeTester_PassNativeStructWithAttachments_Response_Message::kMessageTag = {};

bool NativeTypeTester_PassNativeStructWithAttachments_ForwardToCallback::Accept(
    mojo::Message* message) {
#if BUILDFLAG(MOJO_TRACE_ENABLED)
  TRACE_EVENT1("mojom", "<class 'jinja2::utils::Namespace'>::NativeTypeTester::PassNativeStructWithAttachmentsCallback",
               "message", message->name());
#endif
  mojo::internal::MessageDispatchContext dispatch_context(message);
  if (!message->is_serialized()) {
    auto context =
        message->TakeUnserializedContext<NativeTypeTester_PassNativeStructWithAttachments_Response_Message>();
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
  internal::NativeTypeTester_PassNativeStructWithAttachments_ResponseParams_Data* params =
      reinterpret_cast<
          internal::NativeTypeTester_PassNativeStructWithAttachments_ResponseParams_Data*>(
              message->mutable_payload());
  
  mojo::internal::SerializationContext serialization_context;
  serialization_context.TakeHandlesFromMessage(message);
  bool success = true;
  TestNativeStructWithAttachmentsMojomPtr p_s{};
  NativeTypeTester_PassNativeStructWithAttachments_ResponseParamsDataView input_data_view(params, &serialization_context);
  
  if (!input_data_view.ReadS(&p_s))
    success = false;
  if (!success) {
    ReportValidationErrorForMessage(
        message,
        mojo::internal::VALIDATION_ERROR_DESERIALIZATION_FAILED,
        NativeTypeTester::Name_, 1, true);
    return false;
  }
  if (!callback_.is_null())
    std::move(callback_).Run(
std::move(p_s));
  return true;
}

void NativeTypeTester_PassNativeStructWithAttachments_ProxyToResponder::Run(
    TestNativeStructWithAttachmentsMojomPtr in_s) {
  const bool kSerialize = responder_->PrefersSerializedMessages();
  auto message = NativeTypeTester_PassNativeStructWithAttachments_Response_Message::Build(kSerialize, is_sync_, std::move(in_s));

#if BUILDFLAG(MOJO_TRACE_ENABLED)
  TRACE_EVENT1("mojom", "(Impl)<class 'jinja2::utils::Namespace'>::NativeTypeTester::PassNativeStructWithAttachmentsCallback",
               "message", message.name());
#endif

#if defined(ENABLE_IPC_FUZZER)
  message.set_interface_name(NativeTypeTester::Name_);
  message.set_method_name("PassNativeStructWithAttachments");
#endif

  message.set_request_id(request_id_);
  ignore_result(responder_->Accept(&message));
  // TODO(darin): Accept() returning false indicates a malformed message, and
  // that may be good reason to close the connection. However, we don't have a
  // way to do that from here. We should add a way.
  responder_ = nullptr;
}

// static
bool NativeTypeTesterStubDispatch::Accept(
    NativeTypeTester* impl,
    mojo::Message* message) {
  switch (message->header()->name) {
    case internal::kNativeTypeTester_PassNativeStruct_Name: {
      break;
    }
    case internal::kNativeTypeTester_PassNativeStructWithAttachments_Name: {
      break;
    }
  }
  return false;
}

// static
bool NativeTypeTesterStubDispatch::AcceptWithResponder(
    NativeTypeTester* impl,
    mojo::Message* message,
    std::unique_ptr<mojo::MessageReceiverWithStatus> responder) {
  switch (message->header()->name) {
    case internal::kNativeTypeTester_PassNativeStruct_Name: {
#if BUILDFLAG(MOJO_TRACE_ENABLED)
      TRACE_EVENT1(
          "mojom",
          "(Impl)<class 'jinja2::utils::Namespace'>::NativeTypeTester::PassNativeStruct",
          "message", message->name());
#endif
      base::TaskAnnotator::ScopedSetIpcProgramCounter scoped_ipc_pc(
          base::GetProgramCounter());
      mojo::internal::MessageDispatchContext context(message);
      if (!message->is_serialized()) {
        auto context = message->TakeUnserializedContext<
            NativeTypeTesterProxy_PassNativeStruct_Message>();
        if (!context) {
          // The Message was not of the expected type. It may be a valid message
          // which was built using a different variant of these bindings. Force
          // serialization before dispatch in this case.
          message->SerializeIfNecessary();
        } else {
          NativeTypeTester::PassNativeStructCallback callback =
              NativeTypeTester_PassNativeStruct_ProxyToResponder::CreateCallback(
                  message->request_id(),
                  message->has_flag(mojo::Message::kFlagIsSync),
                  std::move(responder));
          context->Dispatch(impl, std::move(callback));
          return true;
        }
      }

      internal::NativeTypeTester_PassNativeStruct_Params_Data* params =
          reinterpret_cast<
              internal::NativeTypeTester_PassNativeStruct_Params_Data*>(
                  message->mutable_payload());
      
      mojo::internal::SerializationContext serialization_context;
      serialization_context.TakeHandlesFromMessage(message);
      bool success = true;
      TestNativeStructMojomPtr p_s{};
      NativeTypeTester_PassNativeStruct_ParamsDataView input_data_view(params, &serialization_context);
      
      if (!input_data_view.ReadS(&p_s))
        success = false;
      if (!success) {
        ReportValidationErrorForMessage(
            message,
            mojo::internal::VALIDATION_ERROR_DESERIALIZATION_FAILED,
            NativeTypeTester::Name_, 0, false);
        return false;
      }
      NativeTypeTester::PassNativeStructCallback callback =
          NativeTypeTester_PassNativeStruct_ProxyToResponder::CreateCallback(
              message->request_id(),
              message->has_flag(mojo::Message::kFlagIsSync),
              std::move(responder));
      // A null |impl| means no implementation was bound.
      DCHECK(impl);
      impl->PassNativeStruct(
std::move(p_s), std::move(callback));
      return true;
    }
    case internal::kNativeTypeTester_PassNativeStructWithAttachments_Name: {
#if BUILDFLAG(MOJO_TRACE_ENABLED)
      TRACE_EVENT1(
          "mojom",
          "(Impl)<class 'jinja2::utils::Namespace'>::NativeTypeTester::PassNativeStructWithAttachments",
          "message", message->name());
#endif
      base::TaskAnnotator::ScopedSetIpcProgramCounter scoped_ipc_pc(
          base::GetProgramCounter());
      mojo::internal::MessageDispatchContext context(message);
      if (!message->is_serialized()) {
        auto context = message->TakeUnserializedContext<
            NativeTypeTesterProxy_PassNativeStructWithAttachments_Message>();
        if (!context) {
          // The Message was not of the expected type. It may be a valid message
          // which was built using a different variant of these bindings. Force
          // serialization before dispatch in this case.
          message->SerializeIfNecessary();
        } else {
          NativeTypeTester::PassNativeStructWithAttachmentsCallback callback =
              NativeTypeTester_PassNativeStructWithAttachments_ProxyToResponder::CreateCallback(
                  message->request_id(),
                  message->has_flag(mojo::Message::kFlagIsSync),
                  std::move(responder));
          context->Dispatch(impl, std::move(callback));
          return true;
        }
      }

      internal::NativeTypeTester_PassNativeStructWithAttachments_Params_Data* params =
          reinterpret_cast<
              internal::NativeTypeTester_PassNativeStructWithAttachments_Params_Data*>(
                  message->mutable_payload());
      
      mojo::internal::SerializationContext serialization_context;
      serialization_context.TakeHandlesFromMessage(message);
      bool success = true;
      TestNativeStructWithAttachmentsMojomPtr p_s{};
      NativeTypeTester_PassNativeStructWithAttachments_ParamsDataView input_data_view(params, &serialization_context);
      
      if (!input_data_view.ReadS(&p_s))
        success = false;
      if (!success) {
        ReportValidationErrorForMessage(
            message,
            mojo::internal::VALIDATION_ERROR_DESERIALIZATION_FAILED,
            NativeTypeTester::Name_, 1, false);
        return false;
      }
      NativeTypeTester::PassNativeStructWithAttachmentsCallback callback =
          NativeTypeTester_PassNativeStructWithAttachments_ProxyToResponder::CreateCallback(
              message->request_id(),
              message->has_flag(mojo::Message::kFlagIsSync),
              std::move(responder));
      // A null |impl| means no implementation was bound.
      DCHECK(impl);
      impl->PassNativeStructWithAttachments(
std::move(p_s), std::move(callback));
      return true;
    }
  }
  return false;
}

bool NativeTypeTesterRequestValidator::Accept(mojo::Message* message) {
  if (!message->is_serialized() ||
      mojo::internal::ControlMessageHandler::IsControlMessage(message)) {
    return true;
  }

  mojo::internal::ValidationContext validation_context(
    message->payload(), message->payload_num_bytes(),
    message->handles()->size(), message->payload_num_interface_ids(), message,
    "NativeTypeTester RequestValidator");

  switch (message->header()->name) {
    case internal::kNativeTypeTester_PassNativeStruct_Name: {
      if (!mojo::internal::ValidateMessageIsRequestExpectingResponse(
              message, &validation_context)) {
        return false;
      }
      if (!mojo::internal::ValidateMessagePayload<
               internal::NativeTypeTester_PassNativeStruct_Params_Data>(
                  message, &validation_context)) {
        return false;
      }
      return true;
    }
    case internal::kNativeTypeTester_PassNativeStructWithAttachments_Name: {
      if (!mojo::internal::ValidateMessageIsRequestExpectingResponse(
              message, &validation_context)) {
        return false;
      }
      if (!mojo::internal::ValidateMessagePayload<
               internal::NativeTypeTester_PassNativeStructWithAttachments_Params_Data>(
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

bool NativeTypeTesterResponseValidator::Accept(mojo::Message* message) {
  if (!message->is_serialized() ||
      mojo::internal::ControlMessageHandler::IsControlMessage(message)) {
    return true;
  }

  mojo::internal::ValidationContext validation_context(
    message->payload(), message->payload_num_bytes(),
    message->handles()->size(), message->payload_num_interface_ids(), message,
    "NativeTypeTester ResponseValidator");

  if (!mojo::internal::ValidateMessageIsResponse(message, &validation_context))
    return false;
  switch (message->header()->name) {
    case internal::kNativeTypeTester_PassNativeStruct_Name: {
      if (!mojo::internal::ValidateMessagePayload<
               internal::NativeTypeTester_PassNativeStruct_ResponseParams_Data>(
                    message, &validation_context)) {
        return false;
      }
      return true;
    }
    case internal::kNativeTypeTester_PassNativeStructWithAttachments_Name: {
      if (!mojo::internal::ValidateMessagePayload<
               internal::NativeTypeTester_PassNativeStructWithAttachments_ResponseParams_Data>(
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


// static
bool StructTraits<::mojo::test::blink::PickleContainer::DataView, ::mojo::test::blink::PickleContainerPtr>::Read(
    ::mojo::test::blink::PickleContainer::DataView input,
    ::mojo::test::blink::PickleContainerPtr* output) {
  bool success = true;
  ::mojo::test::blink::PickleContainerPtr result(::mojo::test::blink::PickleContainer::New());
  
      if (!input.ReadFStruct(&result->f_struct))
        success = false;
      if (!input.ReadFEnum(&result->f_enum))
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