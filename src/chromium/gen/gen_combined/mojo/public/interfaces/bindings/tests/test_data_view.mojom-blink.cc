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

#include "mojo/public/interfaces/bindings/tests/test_data_view.mojom-blink.h"

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

#include "mojo/public/interfaces/bindings/tests/test_data_view.mojom-params-data.h"
#include "mojo/public/interfaces/bindings/tests/test_data_view.mojom-shared-message-ids.h"

#include "mojo/public/interfaces/bindings/tests/test_data_view.mojom-blink-import-headers.h"
#include "mojo/public/cpp/bindings/lib/wtf_serialization.h"


#ifndef MOJO_PUBLIC_INTERFACES_BINDINGS_TESTS_TEST_DATA_VIEW_MOJOM_BLINK_JUMBO_H_
#define MOJO_PUBLIC_INTERFACES_BINDINGS_TESTS_TEST_DATA_VIEW_MOJOM_BLINK_JUMBO_H_
#endif
namespace mojo {
namespace test {
namespace data_view {
namespace blink {
NestedStruct::NestedStruct()
    : f_int32() {}

NestedStruct::NestedStruct(
    int32_t f_int32_in)
    : f_int32(std::move(f_int32_in)) {}

NestedStruct::~NestedStruct() = default;
size_t NestedStruct::Hash(size_t seed) const {
  seed = mojo::internal::WTFHash(seed, this->f_int32);
  return seed;
}

bool NestedStruct::Validate(
    const void* data,
    mojo::internal::ValidationContext* validation_context) {
  return Data_::Validate(data, validation_context);
}
TestStruct::TestStruct()
    : f_string(),
      f_struct(),
      f_native_struct(),
      f_bool_array(),
      f_int32_array(),
      f_enum_array(),
      f_interface_array(),
      f_nested_array(),
      f_struct_array(),
      f_union_array(),
      f_map() {}

TestStruct::TestStruct(
    const WTF::String& f_string_in,
    NestedStructPtr f_struct_in,
    TestNativeStructPtr f_native_struct_in,
    const WTF::Vector<bool>& f_bool_array_in,
    const WTF::Vector<int32_t>& f_int32_array_in,
    const WTF::Vector<TestEnum>& f_enum_array_in,
    WTF::Vector<mojo::PendingRemote<TestInterface>> f_interface_array_in,
    const WTF::Vector<WTF::Vector<int32_t>>& f_nested_array_in,
    WTF::Vector<NestedStructPtr> f_struct_array_in,
    WTF::Vector<TestUnionPtr> f_union_array_in,
    const WTF::HashMap<WTF::String, int32_t>& f_map_in)
    : f_string(std::move(f_string_in)),
      f_struct(std::move(f_struct_in)),
      f_native_struct(std::move(f_native_struct_in)),
      f_bool_array(std::move(f_bool_array_in)),
      f_int32_array(std::move(f_int32_array_in)),
      f_enum_array(std::move(f_enum_array_in)),
      f_interface_array(std::move(f_interface_array_in)),
      f_nested_array(std::move(f_nested_array_in)),
      f_struct_array(std::move(f_struct_array_in)),
      f_union_array(std::move(f_union_array_in)),
      f_map(std::move(f_map_in)) {}

TestStruct::~TestStruct() = default;

bool TestStruct::Validate(
    const void* data,
    mojo::internal::ValidationContext* validation_context) {
  return Data_::Validate(data, validation_context);
}
TestUnion::TestUnion() : tag_(Tag::F_BOOL) {
  data_.f_bool = bool();
}

TestUnion::~TestUnion() {
  DestroyActive();
}


void TestUnion::set_f_bool(
    bool f_bool) {
  if (tag_ != Tag::F_BOOL) {
    DestroyActive();
    tag_ = Tag::F_BOOL;
  }
  data_.f_bool = f_bool;
}
void TestUnion::set_f_int32(
    int32_t f_int32) {
  if (tag_ != Tag::F_INT32) {
    DestroyActive();
    tag_ = Tag::F_INT32;
  }
  data_.f_int32 = f_int32;
}

void TestUnion::DestroyActive() {
  switch (tag_) {

    case Tag::F_BOOL:

      break;
    case Tag::F_INT32:

      break;
  }
}
size_t TestUnion::Hash(size_t seed) const {
  seed = mojo::internal::HashCombine(seed, static_cast<uint32_t>(tag_));
  switch (tag_) {

    case Tag::F_BOOL:
      return mojo::internal::WTFHash(seed, data_.f_bool);
    case Tag::F_INT32:
      return mojo::internal::WTFHash(seed, data_.f_int32);
    default:
      NOTREACHED();
      return seed;
  }
}

bool TestUnion::Validate(
    const void* data,
    mojo::internal::ValidationContext* validation_context) {
  return Data_::Validate(data, validation_context, false);
}
const char TestInterface::Name_[] = "mojo.test.data_view.TestInterface";
bool TestInterface::Echo(int32_t value, int32_t* out_out_value) {
  NOTREACHED();
  return false;
}
class TestInterface_Echo_HandleSyncResponse
    : public mojo::MessageReceiver {
 public:
  TestInterface_Echo_HandleSyncResponse(
      bool* result, int32_t* out_out_value)
      : result_(result), out_out_value_(out_out_value) {
    DCHECK(!*result_);
  }
  bool Accept(mojo::Message* message) override;
 private:
  bool* result_;
  int32_t* out_out_value_;DISALLOW_COPY_AND_ASSIGN(TestInterface_Echo_HandleSyncResponse);
};

class TestInterface_Echo_ForwardToCallback
    : public mojo::MessageReceiver {
 public:
  TestInterface_Echo_ForwardToCallback(
      TestInterface::EchoCallback callback
      ) : callback_(std::move(callback)) {
  }
  bool Accept(mojo::Message* message) override;
 private:
  TestInterface::EchoCallback callback_;
  DISALLOW_COPY_AND_ASSIGN(TestInterface_Echo_ForwardToCallback);
};

TestInterfaceProxy::TestInterfaceProxy(mojo::MessageReceiverWithResponder* receiver)
    : receiver_(receiver) {
}
class TestInterfaceProxy_Echo_Message
    : public mojo::internal::UnserializedMessageContext {
 public:
  static const mojo::internal::UnserializedMessageContext::Tag kMessageTag;

  explicit TestInterfaceProxy_Echo_Message(
      uint32_t message_flags
      , int32_t param_value
  )
      : mojo::internal::UnserializedMessageContext(
          &kMessageTag,
          internal::kTestInterface_Echo_Name,
          message_flags)
      , param_value_(std::move(param_value)){}
  ~TestInterfaceProxy_Echo_Message() override = default;

  static mojo::Message Build(
      bool serialize,
      bool expects_response,
      bool is_sync,
      int32_t param_value) {

    
  const uint32_t kFlags =
      ((expects_response) ? mojo::Message::kFlagExpectsResponse : 0) |
      ((is_sync) ? mojo::Message::kFlagIsSync : 0);

    if (!serialize) {
      return mojo::Message(std::make_unique<TestInterfaceProxy_Echo_Message>(
          kFlags
          , std::move(param_value)
          ));
    }

    DCHECK(serialize);
    
  mojo::Message message(
      internal::kTestInterface_Echo_Name, kFlags, 0, 0, nullptr);
  auto* buffer = message.payload_buffer();
  ::mojo::test::data_view::internal::TestInterface_Echo_Params_Data::BufferWriter
      params;
  mojo::internal::SerializationContext serialization_context;
  params.Allocate(buffer);
  params->value = param_value;
  message.AttachHandlesFromSerializationContext(
      &serialization_context);
    return message;
  }


  void Dispatch(TestInterface* impl, TestInterface::EchoCallback callback) {
    impl->Echo(
        std::move(param_value_), std::move(callback));
  }

 private:
  // mojo::internal::UnserializedMessageContext:
  void Serialize(mojo::internal::SerializationContext* serialization_context,
                 mojo::internal::Buffer* buffer) override {
    ::mojo::test::data_view::internal::TestInterface_Echo_Params_Data::BufferWriter
        params;
    params.Allocate(buffer);
  params->value = param_value_;
  }
  int32_t param_value_;

  DISALLOW_COPY_AND_ASSIGN(TestInterfaceProxy_Echo_Message);
};

const mojo::internal::UnserializedMessageContext::Tag
TestInterfaceProxy_Echo_Message::kMessageTag = {};
bool TestInterfaceProxy::Echo(
    int32_t param_value, int32_t* out_param_out_value) {
#if BUILDFLAG(MOJO_TRACE_ENABLED)
  TRACE_EVENT0("mojom", "<class 'jinja2::utils::Namespace'>::TestInterface::Echo");
#endif
  const bool kExpectsResponse = true;
  const bool kIsSync = true;
  const bool kSerialize = receiver_->PrefersSerializedMessages();
  auto message = TestInterfaceProxy_Echo_Message::Build(
      kSerialize, kExpectsResponse, kIsSync, std::move(param_value));

#if defined(ENABLE_IPC_FUZZER)
  message.set_interface_name(TestInterface::Name_);
  message.set_method_name("Echo");
#endif

  bool result = false;
  std::unique_ptr<mojo::MessageReceiver> responder(
      new TestInterface_Echo_HandleSyncResponse(
          &result, out_param_out_value));
  ignore_result(receiver_->AcceptWithResponder(&message, std::move(responder)));
  return result;
}

void TestInterfaceProxy::Echo(
    int32_t in_value, EchoCallback callback) {
#if BUILDFLAG(MOJO_TRACE_ENABLED)
  TRACE_EVENT0("mojom", "<class 'jinja2::utils::Namespace'>::TestInterface::Echo");
#endif
  const bool kExpectsResponse = true;
  const bool kIsSync = false;
  const bool kSerialize = receiver_->PrefersSerializedMessages();
  auto message = TestInterfaceProxy_Echo_Message::Build(
      kSerialize, kExpectsResponse, kIsSync, std::move(in_value));

#if defined(ENABLE_IPC_FUZZER)
  message.set_interface_name(TestInterface::Name_);
  message.set_method_name("Echo");
#endif
  std::unique_ptr<mojo::MessageReceiver> responder(
      new TestInterface_Echo_ForwardToCallback(
          std::move(callback)));
  ignore_result(receiver_->AcceptWithResponder(&message, std::move(responder)));
}
class TestInterface_Echo_ProxyToResponder {
 public:
  static TestInterface::EchoCallback CreateCallback(
      uint64_t request_id,
      bool is_sync,
      std::unique_ptr<mojo::MessageReceiverWithStatus> responder) {
    std::unique_ptr<TestInterface_Echo_ProxyToResponder> proxy(
        new TestInterface_Echo_ProxyToResponder(
            request_id, is_sync, std::move(responder)));
    return base::BindOnce(&TestInterface_Echo_ProxyToResponder::Run,
                          std::move(proxy));
  }

  ~TestInterface_Echo_ProxyToResponder() {
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
  TestInterface_Echo_ProxyToResponder(
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
        << "TestInterface::EchoCallback was destroyed without "
        << "first either being run or its corresponding binding being closed. "
        << "It is an error to drop response callbacks which still correspond "
        << "to an open interface pipe.";
  }
#endif

  void Run(
      int32_t in_out_value);

  uint64_t request_id_;
  bool is_sync_;
  std::unique_ptr<mojo::MessageReceiverWithStatus> responder_;

  DISALLOW_COPY_AND_ASSIGN(TestInterface_Echo_ProxyToResponder);
};
class TestInterface_Echo_Response_Message
    : public mojo::internal::UnserializedMessageContext {
 public:
  static const mojo::internal::UnserializedMessageContext::Tag kMessageTag;

  explicit TestInterface_Echo_Response_Message(
      uint32_t message_flags
      , int32_t param_out_value
  )
      : mojo::internal::UnserializedMessageContext(
          &kMessageTag,
          internal::kTestInterface_Echo_Name,
          message_flags)
      , param_out_value_(std::move(param_out_value)){}
  ~TestInterface_Echo_Response_Message() override = default;

  static mojo::Message Build(
      bool serialize,
      bool is_sync,
      int32_t param_out_value) {

    
  const uint32_t kFlags = mojo::Message::kFlagIsResponse |
      ((is_sync) ? mojo::Message::kFlagIsSync : 0);

    if (!serialize) {
      return mojo::Message(std::make_unique<TestInterface_Echo_Response_Message>(
          kFlags
          , std::move(param_out_value)
          ));
    }

    DCHECK(serialize);
    
  mojo::Message message(
      internal::kTestInterface_Echo_Name, kFlags, 0, 0, nullptr);
  auto* buffer = message.payload_buffer();
  ::mojo::test::data_view::internal::TestInterface_Echo_ResponseParams_Data::BufferWriter
      params;
  mojo::internal::SerializationContext serialization_context;
  params.Allocate(buffer);
  params->out_value = param_out_value;
  message.AttachHandlesFromSerializationContext(
      &serialization_context);
    return message;
  }


  void Dispatch(TestInterface::EchoCallback* callback) {
    std::move(*callback).Run(
        std::move(param_out_value_));
  }


  void HandleSyncResponse(

      int32_t* out_out_value) {

    *out_out_value = std::move(param_out_value_);

  }

 private:
  // mojo::internal::UnserializedMessageContext:
  void Serialize(mojo::internal::SerializationContext* serialization_context,
                 mojo::internal::Buffer* buffer) override {
    ::mojo::test::data_view::internal::TestInterface_Echo_ResponseParams_Data::BufferWriter
        params;
    params.Allocate(buffer);
  params->out_value = param_out_value_;
  }
  int32_t param_out_value_;

  DISALLOW_COPY_AND_ASSIGN(TestInterface_Echo_Response_Message);
};

const mojo::internal::UnserializedMessageContext::Tag
TestInterface_Echo_Response_Message::kMessageTag = {};

bool TestInterface_Echo_ForwardToCallback::Accept(
    mojo::Message* message) {
#if BUILDFLAG(MOJO_TRACE_ENABLED)
  TRACE_EVENT1("mojom", "<class 'jinja2::utils::Namespace'>::TestInterface::EchoCallback",
               "message", message->name());
#endif
  mojo::internal::MessageDispatchContext dispatch_context(message);
  if (!message->is_serialized()) {
    auto context =
        message->TakeUnserializedContext<TestInterface_Echo_Response_Message>();
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
  internal::TestInterface_Echo_ResponseParams_Data* params =
      reinterpret_cast<
          internal::TestInterface_Echo_ResponseParams_Data*>(
              message->mutable_payload());
  
  mojo::internal::SerializationContext serialization_context;
  serialization_context.TakeHandlesFromMessage(message);
  bool success = true;
  int32_t p_out_value{};
  TestInterface_Echo_ResponseParamsDataView input_data_view(params, &serialization_context);
  
  p_out_value = input_data_view.out_value();
  if (!success) {
    ReportValidationErrorForMessage(
        message,
        mojo::internal::VALIDATION_ERROR_DESERIALIZATION_FAILED,
        TestInterface::Name_, 0, true);
    return false;
  }
  if (!callback_.is_null())
    std::move(callback_).Run(
std::move(p_out_value));
  return true;
}

void TestInterface_Echo_ProxyToResponder::Run(
    int32_t in_out_value) {
  const bool kSerialize = responder_->PrefersSerializedMessages();
  auto message = TestInterface_Echo_Response_Message::Build(kSerialize, is_sync_, std::move(in_out_value));

#if BUILDFLAG(MOJO_TRACE_ENABLED)
  TRACE_EVENT1("mojom", "(Impl)<class 'jinja2::utils::Namespace'>::TestInterface::EchoCallback",
               "message", message.name());
#endif

#if defined(ENABLE_IPC_FUZZER)
  message.set_interface_name(TestInterface::Name_);
  message.set_method_name("Echo");
#endif

  message.set_request_id(request_id_);
  ignore_result(responder_->Accept(&message));
  // TODO(darin): Accept() returning false indicates a malformed message, and
  // that may be good reason to close the connection. However, we don't have a
  // way to do that from here. We should add a way.
  responder_ = nullptr;
}
bool TestInterface_Echo_HandleSyncResponse::Accept(
    mojo::Message* message) {
  if (!message->is_serialized()) {
    auto context =
        message->TakeUnserializedContext<TestInterface_Echo_Response_Message>();
    if (!context) {
      // The Message was not of the expected type. It may be a valid message
      // which was built using a different variant of these bindings. Force
      // serialization before dispatch in this case.
      message->SerializeIfNecessary();
    } else {
      context->HandleSyncResponse(
          out_out_value_);
      *result_ = true;
      mojo::internal::SyncMessageResponseSetup::SetCurrentSyncResponseMessage(
          message);
      return true;
    }
  }

  DCHECK(message->is_serialized());
  internal::TestInterface_Echo_ResponseParams_Data* params =
      reinterpret_cast<internal::TestInterface_Echo_ResponseParams_Data*>(
          message->mutable_payload());
  
  mojo::internal::SerializationContext serialization_context;
  serialization_context.TakeHandlesFromMessage(message);
  bool success = true;
  int32_t p_out_value{};
  TestInterface_Echo_ResponseParamsDataView input_data_view(params, &serialization_context);
  
  p_out_value = input_data_view.out_value();
  if (!success) {
    ReportValidationErrorForMessage(
        message,
        mojo::internal::VALIDATION_ERROR_DESERIALIZATION_FAILED,
        TestInterface::Name_, 0, true);
    return false;
  }
  *out_out_value_ = std::move(p_out_value);
  mojo::internal::SyncMessageResponseSetup::SetCurrentSyncResponseMessage(
      message);
  *result_ = true;
  return true;
}

// static
bool TestInterfaceStubDispatch::Accept(
    TestInterface* impl,
    mojo::Message* message) {
  switch (message->header()->name) {
    case internal::kTestInterface_Echo_Name: {
      break;
    }
  }
  return false;
}

// static
bool TestInterfaceStubDispatch::AcceptWithResponder(
    TestInterface* impl,
    mojo::Message* message,
    std::unique_ptr<mojo::MessageReceiverWithStatus> responder) {
  switch (message->header()->name) {
    case internal::kTestInterface_Echo_Name: {
#if BUILDFLAG(MOJO_TRACE_ENABLED)
      TRACE_EVENT1(
          "mojom",
          "(Impl)<class 'jinja2::utils::Namespace'>::TestInterface::Echo",
          "message", message->name());
#endif
      base::TaskAnnotator::ScopedSetIpcProgramCounter scoped_ipc_pc(
          base::GetProgramCounter());
      mojo::internal::MessageDispatchContext context(message);
      if (!message->is_serialized()) {
        auto context = message->TakeUnserializedContext<
            TestInterfaceProxy_Echo_Message>();
        if (!context) {
          // The Message was not of the expected type. It may be a valid message
          // which was built using a different variant of these bindings. Force
          // serialization before dispatch in this case.
          message->SerializeIfNecessary();
        } else {
          TestInterface::EchoCallback callback =
              TestInterface_Echo_ProxyToResponder::CreateCallback(
                  message->request_id(),
                  message->has_flag(mojo::Message::kFlagIsSync),
                  std::move(responder));
          context->Dispatch(impl, std::move(callback));
          return true;
        }
      }

      internal::TestInterface_Echo_Params_Data* params =
          reinterpret_cast<
              internal::TestInterface_Echo_Params_Data*>(
                  message->mutable_payload());
      
      mojo::internal::SerializationContext serialization_context;
      serialization_context.TakeHandlesFromMessage(message);
      bool success = true;
      int32_t p_value{};
      TestInterface_Echo_ParamsDataView input_data_view(params, &serialization_context);
      
      p_value = input_data_view.value();
      if (!success) {
        ReportValidationErrorForMessage(
            message,
            mojo::internal::VALIDATION_ERROR_DESERIALIZATION_FAILED,
            TestInterface::Name_, 0, false);
        return false;
      }
      TestInterface::EchoCallback callback =
          TestInterface_Echo_ProxyToResponder::CreateCallback(
              message->request_id(),
              message->has_flag(mojo::Message::kFlagIsSync),
              std::move(responder));
      // A null |impl| means no implementation was bound.
      DCHECK(impl);
      impl->Echo(
std::move(p_value), std::move(callback));
      return true;
    }
  }
  return false;
}

bool TestInterfaceRequestValidator::Accept(mojo::Message* message) {
  if (!message->is_serialized() ||
      mojo::internal::ControlMessageHandler::IsControlMessage(message)) {
    return true;
  }

  mojo::internal::ValidationContext validation_context(
    message->payload(), message->payload_num_bytes(),
    message->handles()->size(), message->payload_num_interface_ids(), message,
    "TestInterface RequestValidator");

  switch (message->header()->name) {
    case internal::kTestInterface_Echo_Name: {
      if (!mojo::internal::ValidateMessageIsRequestExpectingResponse(
              message, &validation_context)) {
        return false;
      }
      if (!mojo::internal::ValidateMessagePayload<
               internal::TestInterface_Echo_Params_Data>(
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

bool TestInterfaceResponseValidator::Accept(mojo::Message* message) {
  if (!message->is_serialized() ||
      mojo::internal::ControlMessageHandler::IsControlMessage(message)) {
    return true;
  }

  mojo::internal::ValidationContext validation_context(
    message->payload(), message->payload_num_bytes(),
    message->handles()->size(), message->payload_num_interface_ids(), message,
    "TestInterface ResponseValidator");

  if (!mojo::internal::ValidateMessageIsResponse(message, &validation_context))
    return false;
  switch (message->header()->name) {
    case internal::kTestInterface_Echo_Name: {
      if (!mojo::internal::ValidateMessagePayload<
               internal::TestInterface_Echo_ResponseParams_Data>(
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
}  // namespace data_view
}  // namespace test
}  // namespace mojo

namespace mojo {


// static
bool StructTraits<::mojo::test::data_view::blink::NestedStruct::DataView, ::mojo::test::data_view::blink::NestedStructPtr>::Read(
    ::mojo::test::data_view::blink::NestedStruct::DataView input,
    ::mojo::test::data_view::blink::NestedStructPtr* output) {
  bool success = true;
  ::mojo::test::data_view::blink::NestedStructPtr result(::mojo::test::data_view::blink::NestedStruct::New());
  
      result->f_int32 = input.f_int32();
  *output = std::move(result);
  return success;
}


// static
bool StructTraits<::mojo::test::data_view::blink::TestStruct::DataView, ::mojo::test::data_view::blink::TestStructPtr>::Read(
    ::mojo::test::data_view::blink::TestStruct::DataView input,
    ::mojo::test::data_view::blink::TestStructPtr* output) {
  bool success = true;
  ::mojo::test::data_view::blink::TestStructPtr result(::mojo::test::data_view::blink::TestStruct::New());
  
      if (!input.ReadFString(&result->f_string))
        success = false;
      if (!input.ReadFStruct(&result->f_struct))
        success = false;
      if (!input.ReadFNativeStruct(&result->f_native_struct))
        success = false;
      if (!input.ReadFBoolArray(&result->f_bool_array))
        success = false;
      if (!input.ReadFInt32Array(&result->f_int32_array))
        success = false;
      if (!input.ReadFEnumArray(&result->f_enum_array))
        success = false;
      if (!input.ReadFInterfaceArray(&result->f_interface_array))
        success = false;
      if (!input.ReadFNestedArray(&result->f_nested_array))
        success = false;
      if (!input.ReadFStructArray(&result->f_struct_array))
        success = false;
      if (!input.ReadFUnionArray(&result->f_union_array))
        success = false;
      if (!input.ReadFMap(&result->f_map))
        success = false;
  *output = std::move(result);
  return success;
}

// static
bool UnionTraits<::mojo::test::data_view::blink::TestUnion::DataView, ::mojo::test::data_view::blink::TestUnionPtr>::Read(
    ::mojo::test::data_view::blink::TestUnion::DataView input,
    ::mojo::test::data_view::blink::TestUnionPtr* output) {
  using UnionType = ::mojo::test::data_view::blink::TestUnion;
  using Tag = UnionType::Tag;

  switch (input.tag()) {
    case Tag::F_BOOL: {
      *output = UnionType::NewFBool(input.f_bool());
      break;
    }
    case Tag::F_INT32: {
      *output = UnionType::NewFInt32(input.f_int32());
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