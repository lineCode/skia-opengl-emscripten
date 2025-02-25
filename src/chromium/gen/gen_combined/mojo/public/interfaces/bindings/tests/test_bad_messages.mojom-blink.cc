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

#include "mojo/public/interfaces/bindings/tests/test_bad_messages.mojom-blink.h"

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

#include "mojo/public/interfaces/bindings/tests/test_bad_messages.mojom-params-data.h"
#include "mojo/public/interfaces/bindings/tests/test_bad_messages.mojom-shared-message-ids.h"

#include "mojo/public/interfaces/bindings/tests/test_bad_messages.mojom-blink-import-headers.h"
#include "mojo/public/cpp/bindings/lib/wtf_serialization.h"


#ifndef MOJO_PUBLIC_INTERFACES_BINDINGS_TESTS_TEST_BAD_MESSAGES_MOJOM_BLINK_JUMBO_H_
#define MOJO_PUBLIC_INTERFACES_BINDINGS_TESTS_TEST_BAD_MESSAGES_MOJOM_BLINK_JUMBO_H_
#endif
namespace mojo {
namespace test {
namespace blink {
const char TestBadMessages::Name_[] = "mojo.test.TestBadMessages";
bool TestBadMessages::RejectSync() {
  NOTREACHED();
  return false;
}
bool TestBadMessages::RequestResponseSync() {
  NOTREACHED();
  return false;
}

class TestBadMessages_RejectEventually_ForwardToCallback
    : public mojo::MessageReceiver {
 public:
  TestBadMessages_RejectEventually_ForwardToCallback(
      TestBadMessages::RejectEventuallyCallback callback
      ) : callback_(std::move(callback)) {
  }
  bool Accept(mojo::Message* message) override;
 private:
  TestBadMessages::RejectEventuallyCallback callback_;
  DISALLOW_COPY_AND_ASSIGN(TestBadMessages_RejectEventually_ForwardToCallback);
};

class TestBadMessages_RequestResponse_ForwardToCallback
    : public mojo::MessageReceiver {
 public:
  TestBadMessages_RequestResponse_ForwardToCallback(
      TestBadMessages::RequestResponseCallback callback
      ) : callback_(std::move(callback)) {
  }
  bool Accept(mojo::Message* message) override;
 private:
  TestBadMessages::RequestResponseCallback callback_;
  DISALLOW_COPY_AND_ASSIGN(TestBadMessages_RequestResponse_ForwardToCallback);
};
class TestBadMessages_RejectSync_HandleSyncResponse
    : public mojo::MessageReceiver {
 public:
  TestBadMessages_RejectSync_HandleSyncResponse(
      bool* result)
      : result_(result) {
    DCHECK(!*result_);
  }
  bool Accept(mojo::Message* message) override;
 private:
  bool* result_;DISALLOW_COPY_AND_ASSIGN(TestBadMessages_RejectSync_HandleSyncResponse);
};

class TestBadMessages_RejectSync_ForwardToCallback
    : public mojo::MessageReceiver {
 public:
  TestBadMessages_RejectSync_ForwardToCallback(
      TestBadMessages::RejectSyncCallback callback
      ) : callback_(std::move(callback)) {
  }
  bool Accept(mojo::Message* message) override;
 private:
  TestBadMessages::RejectSyncCallback callback_;
  DISALLOW_COPY_AND_ASSIGN(TestBadMessages_RejectSync_ForwardToCallback);
};
class TestBadMessages_RequestResponseSync_HandleSyncResponse
    : public mojo::MessageReceiver {
 public:
  TestBadMessages_RequestResponseSync_HandleSyncResponse(
      bool* result)
      : result_(result) {
    DCHECK(!*result_);
  }
  bool Accept(mojo::Message* message) override;
 private:
  bool* result_;DISALLOW_COPY_AND_ASSIGN(TestBadMessages_RequestResponseSync_HandleSyncResponse);
};

class TestBadMessages_RequestResponseSync_ForwardToCallback
    : public mojo::MessageReceiver {
 public:
  TestBadMessages_RequestResponseSync_ForwardToCallback(
      TestBadMessages::RequestResponseSyncCallback callback
      ) : callback_(std::move(callback)) {
  }
  bool Accept(mojo::Message* message) override;
 private:
  TestBadMessages::RequestResponseSyncCallback callback_;
  DISALLOW_COPY_AND_ASSIGN(TestBadMessages_RequestResponseSync_ForwardToCallback);
};

TestBadMessagesProxy::TestBadMessagesProxy(mojo::MessageReceiverWithResponder* receiver)
    : receiver_(receiver) {
}
class TestBadMessagesProxy_RejectEventually_Message
    : public mojo::internal::UnserializedMessageContext {
 public:
  static const mojo::internal::UnserializedMessageContext::Tag kMessageTag;

  explicit TestBadMessagesProxy_RejectEventually_Message(
      uint32_t message_flags
  )
      : mojo::internal::UnserializedMessageContext(
          &kMessageTag,
          internal::kTestBadMessages_RejectEventually_Name,
          message_flags){}
  ~TestBadMessagesProxy_RejectEventually_Message() override = default;

  static mojo::Message Build(
      bool serialize,
      bool expects_response,
      bool is_sync) {

    
  const uint32_t kFlags =
      ((expects_response) ? mojo::Message::kFlagExpectsResponse : 0) |
      ((is_sync) ? mojo::Message::kFlagIsSync : 0);

    if (!serialize) {
      return mojo::Message(std::make_unique<TestBadMessagesProxy_RejectEventually_Message>(
          kFlags
          ));
    }

    DCHECK(serialize);
    
  mojo::Message message(
      internal::kTestBadMessages_RejectEventually_Name, kFlags, 0, 0, nullptr);
  auto* buffer = message.payload_buffer();
  ::mojo::test::internal::TestBadMessages_RejectEventually_Params_Data::BufferWriter
      params;
  mojo::internal::SerializationContext serialization_context;
  params.Allocate(buffer);
  message.AttachHandlesFromSerializationContext(
      &serialization_context);
    return message;
  }


  void Dispatch(TestBadMessages* impl, TestBadMessages::RejectEventuallyCallback callback) {
    impl->RejectEventually(std::move(callback));
  }

 private:
  // mojo::internal::UnserializedMessageContext:
  void Serialize(mojo::internal::SerializationContext* serialization_context,
                 mojo::internal::Buffer* buffer) override {
    ::mojo::test::internal::TestBadMessages_RejectEventually_Params_Data::BufferWriter
        params;
    params.Allocate(buffer);
  }

  DISALLOW_COPY_AND_ASSIGN(TestBadMessagesProxy_RejectEventually_Message);
};

const mojo::internal::UnserializedMessageContext::Tag
TestBadMessagesProxy_RejectEventually_Message::kMessageTag = {};

void TestBadMessagesProxy::RejectEventually(
    RejectEventuallyCallback callback) {
#if BUILDFLAG(MOJO_TRACE_ENABLED)
  TRACE_EVENT0("mojom", "<class 'jinja2::utils::Namespace'>::TestBadMessages::RejectEventually");
#endif
  const bool kExpectsResponse = true;
  const bool kIsSync = false;
  const bool kSerialize = receiver_->PrefersSerializedMessages();
  auto message = TestBadMessagesProxy_RejectEventually_Message::Build(
      kSerialize, kExpectsResponse, kIsSync);

#if defined(ENABLE_IPC_FUZZER)
  message.set_interface_name(TestBadMessages::Name_);
  message.set_method_name("RejectEventually");
#endif
  std::unique_ptr<mojo::MessageReceiver> responder(
      new TestBadMessages_RejectEventually_ForwardToCallback(
          std::move(callback)));
  ignore_result(receiver_->AcceptWithResponder(&message, std::move(responder)));
}
class TestBadMessagesProxy_RequestResponse_Message
    : public mojo::internal::UnserializedMessageContext {
 public:
  static const mojo::internal::UnserializedMessageContext::Tag kMessageTag;

  explicit TestBadMessagesProxy_RequestResponse_Message(
      uint32_t message_flags
  )
      : mojo::internal::UnserializedMessageContext(
          &kMessageTag,
          internal::kTestBadMessages_RequestResponse_Name,
          message_flags){}
  ~TestBadMessagesProxy_RequestResponse_Message() override = default;

  static mojo::Message Build(
      bool serialize,
      bool expects_response,
      bool is_sync) {

    
  const uint32_t kFlags =
      ((expects_response) ? mojo::Message::kFlagExpectsResponse : 0) |
      ((is_sync) ? mojo::Message::kFlagIsSync : 0);

    if (!serialize) {
      return mojo::Message(std::make_unique<TestBadMessagesProxy_RequestResponse_Message>(
          kFlags
          ));
    }

    DCHECK(serialize);
    
  mojo::Message message(
      internal::kTestBadMessages_RequestResponse_Name, kFlags, 0, 0, nullptr);
  auto* buffer = message.payload_buffer();
  ::mojo::test::internal::TestBadMessages_RequestResponse_Params_Data::BufferWriter
      params;
  mojo::internal::SerializationContext serialization_context;
  params.Allocate(buffer);
  message.AttachHandlesFromSerializationContext(
      &serialization_context);
    return message;
  }


  void Dispatch(TestBadMessages* impl, TestBadMessages::RequestResponseCallback callback) {
    impl->RequestResponse(std::move(callback));
  }

 private:
  // mojo::internal::UnserializedMessageContext:
  void Serialize(mojo::internal::SerializationContext* serialization_context,
                 mojo::internal::Buffer* buffer) override {
    ::mojo::test::internal::TestBadMessages_RequestResponse_Params_Data::BufferWriter
        params;
    params.Allocate(buffer);
  }

  DISALLOW_COPY_AND_ASSIGN(TestBadMessagesProxy_RequestResponse_Message);
};

const mojo::internal::UnserializedMessageContext::Tag
TestBadMessagesProxy_RequestResponse_Message::kMessageTag = {};

void TestBadMessagesProxy::RequestResponse(
    RequestResponseCallback callback) {
#if BUILDFLAG(MOJO_TRACE_ENABLED)
  TRACE_EVENT0("mojom", "<class 'jinja2::utils::Namespace'>::TestBadMessages::RequestResponse");
#endif
  const bool kExpectsResponse = true;
  const bool kIsSync = false;
  const bool kSerialize = receiver_->PrefersSerializedMessages();
  auto message = TestBadMessagesProxy_RequestResponse_Message::Build(
      kSerialize, kExpectsResponse, kIsSync);

#if defined(ENABLE_IPC_FUZZER)
  message.set_interface_name(TestBadMessages::Name_);
  message.set_method_name("RequestResponse");
#endif
  std::unique_ptr<mojo::MessageReceiver> responder(
      new TestBadMessages_RequestResponse_ForwardToCallback(
          std::move(callback)));
  ignore_result(receiver_->AcceptWithResponder(&message, std::move(responder)));
}
class TestBadMessagesProxy_RejectSync_Message
    : public mojo::internal::UnserializedMessageContext {
 public:
  static const mojo::internal::UnserializedMessageContext::Tag kMessageTag;

  explicit TestBadMessagesProxy_RejectSync_Message(
      uint32_t message_flags
  )
      : mojo::internal::UnserializedMessageContext(
          &kMessageTag,
          internal::kTestBadMessages_RejectSync_Name,
          message_flags){}
  ~TestBadMessagesProxy_RejectSync_Message() override = default;

  static mojo::Message Build(
      bool serialize,
      bool expects_response,
      bool is_sync) {

    
  const uint32_t kFlags =
      ((expects_response) ? mojo::Message::kFlagExpectsResponse : 0) |
      ((is_sync) ? mojo::Message::kFlagIsSync : 0);

    if (!serialize) {
      return mojo::Message(std::make_unique<TestBadMessagesProxy_RejectSync_Message>(
          kFlags
          ));
    }

    DCHECK(serialize);
    
  mojo::Message message(
      internal::kTestBadMessages_RejectSync_Name, kFlags, 0, 0, nullptr);
  auto* buffer = message.payload_buffer();
  ::mojo::test::internal::TestBadMessages_RejectSync_Params_Data::BufferWriter
      params;
  mojo::internal::SerializationContext serialization_context;
  params.Allocate(buffer);
  message.AttachHandlesFromSerializationContext(
      &serialization_context);
    return message;
  }


  void Dispatch(TestBadMessages* impl, TestBadMessages::RejectSyncCallback callback) {
    impl->RejectSync(std::move(callback));
  }

 private:
  // mojo::internal::UnserializedMessageContext:
  void Serialize(mojo::internal::SerializationContext* serialization_context,
                 mojo::internal::Buffer* buffer) override {
    ::mojo::test::internal::TestBadMessages_RejectSync_Params_Data::BufferWriter
        params;
    params.Allocate(buffer);
  }

  DISALLOW_COPY_AND_ASSIGN(TestBadMessagesProxy_RejectSync_Message);
};

const mojo::internal::UnserializedMessageContext::Tag
TestBadMessagesProxy_RejectSync_Message::kMessageTag = {};
bool TestBadMessagesProxy::RejectSync(
    ) {
#if BUILDFLAG(MOJO_TRACE_ENABLED)
  TRACE_EVENT0("mojom", "<class 'jinja2::utils::Namespace'>::TestBadMessages::RejectSync");
#endif
  const bool kExpectsResponse = true;
  const bool kIsSync = true;
  const bool kSerialize = receiver_->PrefersSerializedMessages();
  auto message = TestBadMessagesProxy_RejectSync_Message::Build(
      kSerialize, kExpectsResponse, kIsSync);

#if defined(ENABLE_IPC_FUZZER)
  message.set_interface_name(TestBadMessages::Name_);
  message.set_method_name("RejectSync");
#endif

  bool result = false;
  std::unique_ptr<mojo::MessageReceiver> responder(
      new TestBadMessages_RejectSync_HandleSyncResponse(
          &result));
  ignore_result(receiver_->AcceptWithResponder(&message, std::move(responder)));
  return result;
}

void TestBadMessagesProxy::RejectSync(
    RejectSyncCallback callback) {
#if BUILDFLAG(MOJO_TRACE_ENABLED)
  TRACE_EVENT0("mojom", "<class 'jinja2::utils::Namespace'>::TestBadMessages::RejectSync");
#endif
  const bool kExpectsResponse = true;
  const bool kIsSync = false;
  const bool kSerialize = receiver_->PrefersSerializedMessages();
  auto message = TestBadMessagesProxy_RejectSync_Message::Build(
      kSerialize, kExpectsResponse, kIsSync);

#if defined(ENABLE_IPC_FUZZER)
  message.set_interface_name(TestBadMessages::Name_);
  message.set_method_name("RejectSync");
#endif
  std::unique_ptr<mojo::MessageReceiver> responder(
      new TestBadMessages_RejectSync_ForwardToCallback(
          std::move(callback)));
  ignore_result(receiver_->AcceptWithResponder(&message, std::move(responder)));
}
class TestBadMessagesProxy_RequestResponseSync_Message
    : public mojo::internal::UnserializedMessageContext {
 public:
  static const mojo::internal::UnserializedMessageContext::Tag kMessageTag;

  explicit TestBadMessagesProxy_RequestResponseSync_Message(
      uint32_t message_flags
  )
      : mojo::internal::UnserializedMessageContext(
          &kMessageTag,
          internal::kTestBadMessages_RequestResponseSync_Name,
          message_flags){}
  ~TestBadMessagesProxy_RequestResponseSync_Message() override = default;

  static mojo::Message Build(
      bool serialize,
      bool expects_response,
      bool is_sync) {

    
  const uint32_t kFlags =
      ((expects_response) ? mojo::Message::kFlagExpectsResponse : 0) |
      ((is_sync) ? mojo::Message::kFlagIsSync : 0);

    if (!serialize) {
      return mojo::Message(std::make_unique<TestBadMessagesProxy_RequestResponseSync_Message>(
          kFlags
          ));
    }

    DCHECK(serialize);
    
  mojo::Message message(
      internal::kTestBadMessages_RequestResponseSync_Name, kFlags, 0, 0, nullptr);
  auto* buffer = message.payload_buffer();
  ::mojo::test::internal::TestBadMessages_RequestResponseSync_Params_Data::BufferWriter
      params;
  mojo::internal::SerializationContext serialization_context;
  params.Allocate(buffer);
  message.AttachHandlesFromSerializationContext(
      &serialization_context);
    return message;
  }


  void Dispatch(TestBadMessages* impl, TestBadMessages::RequestResponseSyncCallback callback) {
    impl->RequestResponseSync(std::move(callback));
  }

 private:
  // mojo::internal::UnserializedMessageContext:
  void Serialize(mojo::internal::SerializationContext* serialization_context,
                 mojo::internal::Buffer* buffer) override {
    ::mojo::test::internal::TestBadMessages_RequestResponseSync_Params_Data::BufferWriter
        params;
    params.Allocate(buffer);
  }

  DISALLOW_COPY_AND_ASSIGN(TestBadMessagesProxy_RequestResponseSync_Message);
};

const mojo::internal::UnserializedMessageContext::Tag
TestBadMessagesProxy_RequestResponseSync_Message::kMessageTag = {};
bool TestBadMessagesProxy::RequestResponseSync(
    ) {
#if BUILDFLAG(MOJO_TRACE_ENABLED)
  TRACE_EVENT0("mojom", "<class 'jinja2::utils::Namespace'>::TestBadMessages::RequestResponseSync");
#endif
  const bool kExpectsResponse = true;
  const bool kIsSync = true;
  const bool kSerialize = receiver_->PrefersSerializedMessages();
  auto message = TestBadMessagesProxy_RequestResponseSync_Message::Build(
      kSerialize, kExpectsResponse, kIsSync);

#if defined(ENABLE_IPC_FUZZER)
  message.set_interface_name(TestBadMessages::Name_);
  message.set_method_name("RequestResponseSync");
#endif

  bool result = false;
  std::unique_ptr<mojo::MessageReceiver> responder(
      new TestBadMessages_RequestResponseSync_HandleSyncResponse(
          &result));
  ignore_result(receiver_->AcceptWithResponder(&message, std::move(responder)));
  return result;
}

void TestBadMessagesProxy::RequestResponseSync(
    RequestResponseSyncCallback callback) {
#if BUILDFLAG(MOJO_TRACE_ENABLED)
  TRACE_EVENT0("mojom", "<class 'jinja2::utils::Namespace'>::TestBadMessages::RequestResponseSync");
#endif
  const bool kExpectsResponse = true;
  const bool kIsSync = false;
  const bool kSerialize = receiver_->PrefersSerializedMessages();
  auto message = TestBadMessagesProxy_RequestResponseSync_Message::Build(
      kSerialize, kExpectsResponse, kIsSync);

#if defined(ENABLE_IPC_FUZZER)
  message.set_interface_name(TestBadMessages::Name_);
  message.set_method_name("RequestResponseSync");
#endif
  std::unique_ptr<mojo::MessageReceiver> responder(
      new TestBadMessages_RequestResponseSync_ForwardToCallback(
          std::move(callback)));
  ignore_result(receiver_->AcceptWithResponder(&message, std::move(responder)));
}
class TestBadMessages_RejectEventually_ProxyToResponder {
 public:
  static TestBadMessages::RejectEventuallyCallback CreateCallback(
      uint64_t request_id,
      bool is_sync,
      std::unique_ptr<mojo::MessageReceiverWithStatus> responder) {
    std::unique_ptr<TestBadMessages_RejectEventually_ProxyToResponder> proxy(
        new TestBadMessages_RejectEventually_ProxyToResponder(
            request_id, is_sync, std::move(responder)));
    return base::BindOnce(&TestBadMessages_RejectEventually_ProxyToResponder::Run,
                          std::move(proxy));
  }

  ~TestBadMessages_RejectEventually_ProxyToResponder() {
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
  TestBadMessages_RejectEventually_ProxyToResponder(
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
        << "TestBadMessages::RejectEventuallyCallback was destroyed without "
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

  DISALLOW_COPY_AND_ASSIGN(TestBadMessages_RejectEventually_ProxyToResponder);
};
class TestBadMessages_RejectEventually_Response_Message
    : public mojo::internal::UnserializedMessageContext {
 public:
  static const mojo::internal::UnserializedMessageContext::Tag kMessageTag;

  explicit TestBadMessages_RejectEventually_Response_Message(
      uint32_t message_flags
  )
      : mojo::internal::UnserializedMessageContext(
          &kMessageTag,
          internal::kTestBadMessages_RejectEventually_Name,
          message_flags){}
  ~TestBadMessages_RejectEventually_Response_Message() override = default;

  static mojo::Message Build(
      bool serialize,
      bool is_sync) {

    
  const uint32_t kFlags = mojo::Message::kFlagIsResponse |
      ((is_sync) ? mojo::Message::kFlagIsSync : 0);

    if (!serialize) {
      return mojo::Message(std::make_unique<TestBadMessages_RejectEventually_Response_Message>(
          kFlags
          ));
    }

    DCHECK(serialize);
    
  mojo::Message message(
      internal::kTestBadMessages_RejectEventually_Name, kFlags, 0, 0, nullptr);
  auto* buffer = message.payload_buffer();
  ::mojo::test::internal::TestBadMessages_RejectEventually_ResponseParams_Data::BufferWriter
      params;
  mojo::internal::SerializationContext serialization_context;
  params.Allocate(buffer);
  message.AttachHandlesFromSerializationContext(
      &serialization_context);
    return message;
  }


  void Dispatch(TestBadMessages::RejectEventuallyCallback* callback) {
    std::move(*callback).Run();
  }



 private:
  // mojo::internal::UnserializedMessageContext:
  void Serialize(mojo::internal::SerializationContext* serialization_context,
                 mojo::internal::Buffer* buffer) override {
    ::mojo::test::internal::TestBadMessages_RejectEventually_ResponseParams_Data::BufferWriter
        params;
    params.Allocate(buffer);
  }

  DISALLOW_COPY_AND_ASSIGN(TestBadMessages_RejectEventually_Response_Message);
};

const mojo::internal::UnserializedMessageContext::Tag
TestBadMessages_RejectEventually_Response_Message::kMessageTag = {};

bool TestBadMessages_RejectEventually_ForwardToCallback::Accept(
    mojo::Message* message) {
#if BUILDFLAG(MOJO_TRACE_ENABLED)
  TRACE_EVENT1("mojom", "<class 'jinja2::utils::Namespace'>::TestBadMessages::RejectEventuallyCallback",
               "message", message->name());
#endif
  mojo::internal::MessageDispatchContext dispatch_context(message);
  if (!message->is_serialized()) {
    auto context =
        message->TakeUnserializedContext<TestBadMessages_RejectEventually_Response_Message>();
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
  internal::TestBadMessages_RejectEventually_ResponseParams_Data* params =
      reinterpret_cast<
          internal::TestBadMessages_RejectEventually_ResponseParams_Data*>(
              message->mutable_payload());
  
  mojo::internal::SerializationContext serialization_context;
  serialization_context.TakeHandlesFromMessage(message);
  bool success = true;
  TestBadMessages_RejectEventually_ResponseParamsDataView input_data_view(params, &serialization_context);
  
  if (!success) {
    ReportValidationErrorForMessage(
        message,
        mojo::internal::VALIDATION_ERROR_DESERIALIZATION_FAILED,
        TestBadMessages::Name_, 0, true);
    return false;
  }
  if (!callback_.is_null())
    std::move(callback_).Run();
  return true;
}

void TestBadMessages_RejectEventually_ProxyToResponder::Run(
    ) {
  const bool kSerialize = responder_->PrefersSerializedMessages();
  auto message = TestBadMessages_RejectEventually_Response_Message::Build(kSerialize, is_sync_);

#if BUILDFLAG(MOJO_TRACE_ENABLED)
  TRACE_EVENT1("mojom", "(Impl)<class 'jinja2::utils::Namespace'>::TestBadMessages::RejectEventuallyCallback",
               "message", message.name());
#endif

#if defined(ENABLE_IPC_FUZZER)
  message.set_interface_name(TestBadMessages::Name_);
  message.set_method_name("RejectEventually");
#endif

  message.set_request_id(request_id_);
  ignore_result(responder_->Accept(&message));
  // TODO(darin): Accept() returning false indicates a malformed message, and
  // that may be good reason to close the connection. However, we don't have a
  // way to do that from here. We should add a way.
  responder_ = nullptr;
}
class TestBadMessages_RequestResponse_ProxyToResponder {
 public:
  static TestBadMessages::RequestResponseCallback CreateCallback(
      uint64_t request_id,
      bool is_sync,
      std::unique_ptr<mojo::MessageReceiverWithStatus> responder) {
    std::unique_ptr<TestBadMessages_RequestResponse_ProxyToResponder> proxy(
        new TestBadMessages_RequestResponse_ProxyToResponder(
            request_id, is_sync, std::move(responder)));
    return base::BindOnce(&TestBadMessages_RequestResponse_ProxyToResponder::Run,
                          std::move(proxy));
  }

  ~TestBadMessages_RequestResponse_ProxyToResponder() {
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
  TestBadMessages_RequestResponse_ProxyToResponder(
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
        << "TestBadMessages::RequestResponseCallback was destroyed without "
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

  DISALLOW_COPY_AND_ASSIGN(TestBadMessages_RequestResponse_ProxyToResponder);
};
class TestBadMessages_RequestResponse_Response_Message
    : public mojo::internal::UnserializedMessageContext {
 public:
  static const mojo::internal::UnserializedMessageContext::Tag kMessageTag;

  explicit TestBadMessages_RequestResponse_Response_Message(
      uint32_t message_flags
  )
      : mojo::internal::UnserializedMessageContext(
          &kMessageTag,
          internal::kTestBadMessages_RequestResponse_Name,
          message_flags){}
  ~TestBadMessages_RequestResponse_Response_Message() override = default;

  static mojo::Message Build(
      bool serialize,
      bool is_sync) {

    
  const uint32_t kFlags = mojo::Message::kFlagIsResponse |
      ((is_sync) ? mojo::Message::kFlagIsSync : 0);

    if (!serialize) {
      return mojo::Message(std::make_unique<TestBadMessages_RequestResponse_Response_Message>(
          kFlags
          ));
    }

    DCHECK(serialize);
    
  mojo::Message message(
      internal::kTestBadMessages_RequestResponse_Name, kFlags, 0, 0, nullptr);
  auto* buffer = message.payload_buffer();
  ::mojo::test::internal::TestBadMessages_RequestResponse_ResponseParams_Data::BufferWriter
      params;
  mojo::internal::SerializationContext serialization_context;
  params.Allocate(buffer);
  message.AttachHandlesFromSerializationContext(
      &serialization_context);
    return message;
  }


  void Dispatch(TestBadMessages::RequestResponseCallback* callback) {
    std::move(*callback).Run();
  }



 private:
  // mojo::internal::UnserializedMessageContext:
  void Serialize(mojo::internal::SerializationContext* serialization_context,
                 mojo::internal::Buffer* buffer) override {
    ::mojo::test::internal::TestBadMessages_RequestResponse_ResponseParams_Data::BufferWriter
        params;
    params.Allocate(buffer);
  }

  DISALLOW_COPY_AND_ASSIGN(TestBadMessages_RequestResponse_Response_Message);
};

const mojo::internal::UnserializedMessageContext::Tag
TestBadMessages_RequestResponse_Response_Message::kMessageTag = {};

bool TestBadMessages_RequestResponse_ForwardToCallback::Accept(
    mojo::Message* message) {
#if BUILDFLAG(MOJO_TRACE_ENABLED)
  TRACE_EVENT1("mojom", "<class 'jinja2::utils::Namespace'>::TestBadMessages::RequestResponseCallback",
               "message", message->name());
#endif
  mojo::internal::MessageDispatchContext dispatch_context(message);
  if (!message->is_serialized()) {
    auto context =
        message->TakeUnserializedContext<TestBadMessages_RequestResponse_Response_Message>();
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
  internal::TestBadMessages_RequestResponse_ResponseParams_Data* params =
      reinterpret_cast<
          internal::TestBadMessages_RequestResponse_ResponseParams_Data*>(
              message->mutable_payload());
  
  mojo::internal::SerializationContext serialization_context;
  serialization_context.TakeHandlesFromMessage(message);
  bool success = true;
  TestBadMessages_RequestResponse_ResponseParamsDataView input_data_view(params, &serialization_context);
  
  if (!success) {
    ReportValidationErrorForMessage(
        message,
        mojo::internal::VALIDATION_ERROR_DESERIALIZATION_FAILED,
        TestBadMessages::Name_, 1, true);
    return false;
  }
  if (!callback_.is_null())
    std::move(callback_).Run();
  return true;
}

void TestBadMessages_RequestResponse_ProxyToResponder::Run(
    ) {
  const bool kSerialize = responder_->PrefersSerializedMessages();
  auto message = TestBadMessages_RequestResponse_Response_Message::Build(kSerialize, is_sync_);

#if BUILDFLAG(MOJO_TRACE_ENABLED)
  TRACE_EVENT1("mojom", "(Impl)<class 'jinja2::utils::Namespace'>::TestBadMessages::RequestResponseCallback",
               "message", message.name());
#endif

#if defined(ENABLE_IPC_FUZZER)
  message.set_interface_name(TestBadMessages::Name_);
  message.set_method_name("RequestResponse");
#endif

  message.set_request_id(request_id_);
  ignore_result(responder_->Accept(&message));
  // TODO(darin): Accept() returning false indicates a malformed message, and
  // that may be good reason to close the connection. However, we don't have a
  // way to do that from here. We should add a way.
  responder_ = nullptr;
}
class TestBadMessages_RejectSync_ProxyToResponder {
 public:
  static TestBadMessages::RejectSyncCallback CreateCallback(
      uint64_t request_id,
      bool is_sync,
      std::unique_ptr<mojo::MessageReceiverWithStatus> responder) {
    std::unique_ptr<TestBadMessages_RejectSync_ProxyToResponder> proxy(
        new TestBadMessages_RejectSync_ProxyToResponder(
            request_id, is_sync, std::move(responder)));
    return base::BindOnce(&TestBadMessages_RejectSync_ProxyToResponder::Run,
                          std::move(proxy));
  }

  ~TestBadMessages_RejectSync_ProxyToResponder() {
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
  TestBadMessages_RejectSync_ProxyToResponder(
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
        << "TestBadMessages::RejectSyncCallback was destroyed without "
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

  DISALLOW_COPY_AND_ASSIGN(TestBadMessages_RejectSync_ProxyToResponder);
};
class TestBadMessages_RejectSync_Response_Message
    : public mojo::internal::UnserializedMessageContext {
 public:
  static const mojo::internal::UnserializedMessageContext::Tag kMessageTag;

  explicit TestBadMessages_RejectSync_Response_Message(
      uint32_t message_flags
  )
      : mojo::internal::UnserializedMessageContext(
          &kMessageTag,
          internal::kTestBadMessages_RejectSync_Name,
          message_flags){}
  ~TestBadMessages_RejectSync_Response_Message() override = default;

  static mojo::Message Build(
      bool serialize,
      bool is_sync) {

    
  const uint32_t kFlags = mojo::Message::kFlagIsResponse |
      ((is_sync) ? mojo::Message::kFlagIsSync : 0);

    if (!serialize) {
      return mojo::Message(std::make_unique<TestBadMessages_RejectSync_Response_Message>(
          kFlags
          ));
    }

    DCHECK(serialize);
    
  mojo::Message message(
      internal::kTestBadMessages_RejectSync_Name, kFlags, 0, 0, nullptr);
  auto* buffer = message.payload_buffer();
  ::mojo::test::internal::TestBadMessages_RejectSync_ResponseParams_Data::BufferWriter
      params;
  mojo::internal::SerializationContext serialization_context;
  params.Allocate(buffer);
  message.AttachHandlesFromSerializationContext(
      &serialization_context);
    return message;
  }


  void Dispatch(TestBadMessages::RejectSyncCallback* callback) {
    std::move(*callback).Run();
  }


  void HandleSyncResponse(
) {

  }

 private:
  // mojo::internal::UnserializedMessageContext:
  void Serialize(mojo::internal::SerializationContext* serialization_context,
                 mojo::internal::Buffer* buffer) override {
    ::mojo::test::internal::TestBadMessages_RejectSync_ResponseParams_Data::BufferWriter
        params;
    params.Allocate(buffer);
  }

  DISALLOW_COPY_AND_ASSIGN(TestBadMessages_RejectSync_Response_Message);
};

const mojo::internal::UnserializedMessageContext::Tag
TestBadMessages_RejectSync_Response_Message::kMessageTag = {};

bool TestBadMessages_RejectSync_ForwardToCallback::Accept(
    mojo::Message* message) {
#if BUILDFLAG(MOJO_TRACE_ENABLED)
  TRACE_EVENT1("mojom", "<class 'jinja2::utils::Namespace'>::TestBadMessages::RejectSyncCallback",
               "message", message->name());
#endif
  mojo::internal::MessageDispatchContext dispatch_context(message);
  if (!message->is_serialized()) {
    auto context =
        message->TakeUnserializedContext<TestBadMessages_RejectSync_Response_Message>();
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
  internal::TestBadMessages_RejectSync_ResponseParams_Data* params =
      reinterpret_cast<
          internal::TestBadMessages_RejectSync_ResponseParams_Data*>(
              message->mutable_payload());
  
  mojo::internal::SerializationContext serialization_context;
  serialization_context.TakeHandlesFromMessage(message);
  bool success = true;
  TestBadMessages_RejectSync_ResponseParamsDataView input_data_view(params, &serialization_context);
  
  if (!success) {
    ReportValidationErrorForMessage(
        message,
        mojo::internal::VALIDATION_ERROR_DESERIALIZATION_FAILED,
        TestBadMessages::Name_, 2, true);
    return false;
  }
  if (!callback_.is_null())
    std::move(callback_).Run();
  return true;
}

void TestBadMessages_RejectSync_ProxyToResponder::Run(
    ) {
  const bool kSerialize = responder_->PrefersSerializedMessages();
  auto message = TestBadMessages_RejectSync_Response_Message::Build(kSerialize, is_sync_);

#if BUILDFLAG(MOJO_TRACE_ENABLED)
  TRACE_EVENT1("mojom", "(Impl)<class 'jinja2::utils::Namespace'>::TestBadMessages::RejectSyncCallback",
               "message", message.name());
#endif

#if defined(ENABLE_IPC_FUZZER)
  message.set_interface_name(TestBadMessages::Name_);
  message.set_method_name("RejectSync");
#endif

  message.set_request_id(request_id_);
  ignore_result(responder_->Accept(&message));
  // TODO(darin): Accept() returning false indicates a malformed message, and
  // that may be good reason to close the connection. However, we don't have a
  // way to do that from here. We should add a way.
  responder_ = nullptr;
}
bool TestBadMessages_RejectSync_HandleSyncResponse::Accept(
    mojo::Message* message) {
  if (!message->is_serialized()) {
    auto context =
        message->TakeUnserializedContext<TestBadMessages_RejectSync_Response_Message>();
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
  internal::TestBadMessages_RejectSync_ResponseParams_Data* params =
      reinterpret_cast<internal::TestBadMessages_RejectSync_ResponseParams_Data*>(
          message->mutable_payload());
  
  mojo::internal::SerializationContext serialization_context;
  serialization_context.TakeHandlesFromMessage(message);
  bool success = true;
  TestBadMessages_RejectSync_ResponseParamsDataView input_data_view(params, &serialization_context);
  
  if (!success) {
    ReportValidationErrorForMessage(
        message,
        mojo::internal::VALIDATION_ERROR_DESERIALIZATION_FAILED,
        TestBadMessages::Name_, 2, true);
    return false;
  }
  mojo::internal::SyncMessageResponseSetup::SetCurrentSyncResponseMessage(
      message);
  *result_ = true;
  return true;
}
class TestBadMessages_RequestResponseSync_ProxyToResponder {
 public:
  static TestBadMessages::RequestResponseSyncCallback CreateCallback(
      uint64_t request_id,
      bool is_sync,
      std::unique_ptr<mojo::MessageReceiverWithStatus> responder) {
    std::unique_ptr<TestBadMessages_RequestResponseSync_ProxyToResponder> proxy(
        new TestBadMessages_RequestResponseSync_ProxyToResponder(
            request_id, is_sync, std::move(responder)));
    return base::BindOnce(&TestBadMessages_RequestResponseSync_ProxyToResponder::Run,
                          std::move(proxy));
  }

  ~TestBadMessages_RequestResponseSync_ProxyToResponder() {
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
  TestBadMessages_RequestResponseSync_ProxyToResponder(
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
        << "TestBadMessages::RequestResponseSyncCallback was destroyed without "
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

  DISALLOW_COPY_AND_ASSIGN(TestBadMessages_RequestResponseSync_ProxyToResponder);
};
class TestBadMessages_RequestResponseSync_Response_Message
    : public mojo::internal::UnserializedMessageContext {
 public:
  static const mojo::internal::UnserializedMessageContext::Tag kMessageTag;

  explicit TestBadMessages_RequestResponseSync_Response_Message(
      uint32_t message_flags
  )
      : mojo::internal::UnserializedMessageContext(
          &kMessageTag,
          internal::kTestBadMessages_RequestResponseSync_Name,
          message_flags){}
  ~TestBadMessages_RequestResponseSync_Response_Message() override = default;

  static mojo::Message Build(
      bool serialize,
      bool is_sync) {

    
  const uint32_t kFlags = mojo::Message::kFlagIsResponse |
      ((is_sync) ? mojo::Message::kFlagIsSync : 0);

    if (!serialize) {
      return mojo::Message(std::make_unique<TestBadMessages_RequestResponseSync_Response_Message>(
          kFlags
          ));
    }

    DCHECK(serialize);
    
  mojo::Message message(
      internal::kTestBadMessages_RequestResponseSync_Name, kFlags, 0, 0, nullptr);
  auto* buffer = message.payload_buffer();
  ::mojo::test::internal::TestBadMessages_RequestResponseSync_ResponseParams_Data::BufferWriter
      params;
  mojo::internal::SerializationContext serialization_context;
  params.Allocate(buffer);
  message.AttachHandlesFromSerializationContext(
      &serialization_context);
    return message;
  }


  void Dispatch(TestBadMessages::RequestResponseSyncCallback* callback) {
    std::move(*callback).Run();
  }


  void HandleSyncResponse(
) {

  }

 private:
  // mojo::internal::UnserializedMessageContext:
  void Serialize(mojo::internal::SerializationContext* serialization_context,
                 mojo::internal::Buffer* buffer) override {
    ::mojo::test::internal::TestBadMessages_RequestResponseSync_ResponseParams_Data::BufferWriter
        params;
    params.Allocate(buffer);
  }

  DISALLOW_COPY_AND_ASSIGN(TestBadMessages_RequestResponseSync_Response_Message);
};

const mojo::internal::UnserializedMessageContext::Tag
TestBadMessages_RequestResponseSync_Response_Message::kMessageTag = {};

bool TestBadMessages_RequestResponseSync_ForwardToCallback::Accept(
    mojo::Message* message) {
#if BUILDFLAG(MOJO_TRACE_ENABLED)
  TRACE_EVENT1("mojom", "<class 'jinja2::utils::Namespace'>::TestBadMessages::RequestResponseSyncCallback",
               "message", message->name());
#endif
  mojo::internal::MessageDispatchContext dispatch_context(message);
  if (!message->is_serialized()) {
    auto context =
        message->TakeUnserializedContext<TestBadMessages_RequestResponseSync_Response_Message>();
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
  internal::TestBadMessages_RequestResponseSync_ResponseParams_Data* params =
      reinterpret_cast<
          internal::TestBadMessages_RequestResponseSync_ResponseParams_Data*>(
              message->mutable_payload());
  
  mojo::internal::SerializationContext serialization_context;
  serialization_context.TakeHandlesFromMessage(message);
  bool success = true;
  TestBadMessages_RequestResponseSync_ResponseParamsDataView input_data_view(params, &serialization_context);
  
  if (!success) {
    ReportValidationErrorForMessage(
        message,
        mojo::internal::VALIDATION_ERROR_DESERIALIZATION_FAILED,
        TestBadMessages::Name_, 3, true);
    return false;
  }
  if (!callback_.is_null())
    std::move(callback_).Run();
  return true;
}

void TestBadMessages_RequestResponseSync_ProxyToResponder::Run(
    ) {
  const bool kSerialize = responder_->PrefersSerializedMessages();
  auto message = TestBadMessages_RequestResponseSync_Response_Message::Build(kSerialize, is_sync_);

#if BUILDFLAG(MOJO_TRACE_ENABLED)
  TRACE_EVENT1("mojom", "(Impl)<class 'jinja2::utils::Namespace'>::TestBadMessages::RequestResponseSyncCallback",
               "message", message.name());
#endif

#if defined(ENABLE_IPC_FUZZER)
  message.set_interface_name(TestBadMessages::Name_);
  message.set_method_name("RequestResponseSync");
#endif

  message.set_request_id(request_id_);
  ignore_result(responder_->Accept(&message));
  // TODO(darin): Accept() returning false indicates a malformed message, and
  // that may be good reason to close the connection. However, we don't have a
  // way to do that from here. We should add a way.
  responder_ = nullptr;
}
bool TestBadMessages_RequestResponseSync_HandleSyncResponse::Accept(
    mojo::Message* message) {
  if (!message->is_serialized()) {
    auto context =
        message->TakeUnserializedContext<TestBadMessages_RequestResponseSync_Response_Message>();
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
  internal::TestBadMessages_RequestResponseSync_ResponseParams_Data* params =
      reinterpret_cast<internal::TestBadMessages_RequestResponseSync_ResponseParams_Data*>(
          message->mutable_payload());
  
  mojo::internal::SerializationContext serialization_context;
  serialization_context.TakeHandlesFromMessage(message);
  bool success = true;
  TestBadMessages_RequestResponseSync_ResponseParamsDataView input_data_view(params, &serialization_context);
  
  if (!success) {
    ReportValidationErrorForMessage(
        message,
        mojo::internal::VALIDATION_ERROR_DESERIALIZATION_FAILED,
        TestBadMessages::Name_, 3, true);
    return false;
  }
  mojo::internal::SyncMessageResponseSetup::SetCurrentSyncResponseMessage(
      message);
  *result_ = true;
  return true;
}

// static
bool TestBadMessagesStubDispatch::Accept(
    TestBadMessages* impl,
    mojo::Message* message) {
  switch (message->header()->name) {
    case internal::kTestBadMessages_RejectEventually_Name: {
      break;
    }
    case internal::kTestBadMessages_RequestResponse_Name: {
      break;
    }
    case internal::kTestBadMessages_RejectSync_Name: {
      break;
    }
    case internal::kTestBadMessages_RequestResponseSync_Name: {
      break;
    }
  }
  return false;
}

// static
bool TestBadMessagesStubDispatch::AcceptWithResponder(
    TestBadMessages* impl,
    mojo::Message* message,
    std::unique_ptr<mojo::MessageReceiverWithStatus> responder) {
  switch (message->header()->name) {
    case internal::kTestBadMessages_RejectEventually_Name: {
#if BUILDFLAG(MOJO_TRACE_ENABLED)
      TRACE_EVENT1(
          "mojom",
          "(Impl)<class 'jinja2::utils::Namespace'>::TestBadMessages::RejectEventually",
          "message", message->name());
#endif
      base::TaskAnnotator::ScopedSetIpcProgramCounter scoped_ipc_pc(
          base::GetProgramCounter());
      mojo::internal::MessageDispatchContext context(message);
      if (!message->is_serialized()) {
        auto context = message->TakeUnserializedContext<
            TestBadMessagesProxy_RejectEventually_Message>();
        if (!context) {
          // The Message was not of the expected type. It may be a valid message
          // which was built using a different variant of these bindings. Force
          // serialization before dispatch in this case.
          message->SerializeIfNecessary();
        } else {
          TestBadMessages::RejectEventuallyCallback callback =
              TestBadMessages_RejectEventually_ProxyToResponder::CreateCallback(
                  message->request_id(),
                  message->has_flag(mojo::Message::kFlagIsSync),
                  std::move(responder));
          context->Dispatch(impl, std::move(callback));
          return true;
        }
      }

      internal::TestBadMessages_RejectEventually_Params_Data* params =
          reinterpret_cast<
              internal::TestBadMessages_RejectEventually_Params_Data*>(
                  message->mutable_payload());
      
      mojo::internal::SerializationContext serialization_context;
      serialization_context.TakeHandlesFromMessage(message);
      bool success = true;
      TestBadMessages_RejectEventually_ParamsDataView input_data_view(params, &serialization_context);
      
      if (!success) {
        ReportValidationErrorForMessage(
            message,
            mojo::internal::VALIDATION_ERROR_DESERIALIZATION_FAILED,
            TestBadMessages::Name_, 0, false);
        return false;
      }
      TestBadMessages::RejectEventuallyCallback callback =
          TestBadMessages_RejectEventually_ProxyToResponder::CreateCallback(
              message->request_id(),
              message->has_flag(mojo::Message::kFlagIsSync),
              std::move(responder));
      // A null |impl| means no implementation was bound.
      DCHECK(impl);
      impl->RejectEventually(std::move(callback));
      return true;
    }
    case internal::kTestBadMessages_RequestResponse_Name: {
#if BUILDFLAG(MOJO_TRACE_ENABLED)
      TRACE_EVENT1(
          "mojom",
          "(Impl)<class 'jinja2::utils::Namespace'>::TestBadMessages::RequestResponse",
          "message", message->name());
#endif
      base::TaskAnnotator::ScopedSetIpcProgramCounter scoped_ipc_pc(
          base::GetProgramCounter());
      mojo::internal::MessageDispatchContext context(message);
      if (!message->is_serialized()) {
        auto context = message->TakeUnserializedContext<
            TestBadMessagesProxy_RequestResponse_Message>();
        if (!context) {
          // The Message was not of the expected type. It may be a valid message
          // which was built using a different variant of these bindings. Force
          // serialization before dispatch in this case.
          message->SerializeIfNecessary();
        } else {
          TestBadMessages::RequestResponseCallback callback =
              TestBadMessages_RequestResponse_ProxyToResponder::CreateCallback(
                  message->request_id(),
                  message->has_flag(mojo::Message::kFlagIsSync),
                  std::move(responder));
          context->Dispatch(impl, std::move(callback));
          return true;
        }
      }

      internal::TestBadMessages_RequestResponse_Params_Data* params =
          reinterpret_cast<
              internal::TestBadMessages_RequestResponse_Params_Data*>(
                  message->mutable_payload());
      
      mojo::internal::SerializationContext serialization_context;
      serialization_context.TakeHandlesFromMessage(message);
      bool success = true;
      TestBadMessages_RequestResponse_ParamsDataView input_data_view(params, &serialization_context);
      
      if (!success) {
        ReportValidationErrorForMessage(
            message,
            mojo::internal::VALIDATION_ERROR_DESERIALIZATION_FAILED,
            TestBadMessages::Name_, 1, false);
        return false;
      }
      TestBadMessages::RequestResponseCallback callback =
          TestBadMessages_RequestResponse_ProxyToResponder::CreateCallback(
              message->request_id(),
              message->has_flag(mojo::Message::kFlagIsSync),
              std::move(responder));
      // A null |impl| means no implementation was bound.
      DCHECK(impl);
      impl->RequestResponse(std::move(callback));
      return true;
    }
    case internal::kTestBadMessages_RejectSync_Name: {
#if BUILDFLAG(MOJO_TRACE_ENABLED)
      TRACE_EVENT1(
          "mojom",
          "(Impl)<class 'jinja2::utils::Namespace'>::TestBadMessages::RejectSync",
          "message", message->name());
#endif
      base::TaskAnnotator::ScopedSetIpcProgramCounter scoped_ipc_pc(
          base::GetProgramCounter());
      mojo::internal::MessageDispatchContext context(message);
      if (!message->is_serialized()) {
        auto context = message->TakeUnserializedContext<
            TestBadMessagesProxy_RejectSync_Message>();
        if (!context) {
          // The Message was not of the expected type. It may be a valid message
          // which was built using a different variant of these bindings. Force
          // serialization before dispatch in this case.
          message->SerializeIfNecessary();
        } else {
          TestBadMessages::RejectSyncCallback callback =
              TestBadMessages_RejectSync_ProxyToResponder::CreateCallback(
                  message->request_id(),
                  message->has_flag(mojo::Message::kFlagIsSync),
                  std::move(responder));
          context->Dispatch(impl, std::move(callback));
          return true;
        }
      }

      internal::TestBadMessages_RejectSync_Params_Data* params =
          reinterpret_cast<
              internal::TestBadMessages_RejectSync_Params_Data*>(
                  message->mutable_payload());
      
      mojo::internal::SerializationContext serialization_context;
      serialization_context.TakeHandlesFromMessage(message);
      bool success = true;
      TestBadMessages_RejectSync_ParamsDataView input_data_view(params, &serialization_context);
      
      if (!success) {
        ReportValidationErrorForMessage(
            message,
            mojo::internal::VALIDATION_ERROR_DESERIALIZATION_FAILED,
            TestBadMessages::Name_, 2, false);
        return false;
      }
      TestBadMessages::RejectSyncCallback callback =
          TestBadMessages_RejectSync_ProxyToResponder::CreateCallback(
              message->request_id(),
              message->has_flag(mojo::Message::kFlagIsSync),
              std::move(responder));
      // A null |impl| means no implementation was bound.
      DCHECK(impl);
      impl->RejectSync(std::move(callback));
      return true;
    }
    case internal::kTestBadMessages_RequestResponseSync_Name: {
#if BUILDFLAG(MOJO_TRACE_ENABLED)
      TRACE_EVENT1(
          "mojom",
          "(Impl)<class 'jinja2::utils::Namespace'>::TestBadMessages::RequestResponseSync",
          "message", message->name());
#endif
      base::TaskAnnotator::ScopedSetIpcProgramCounter scoped_ipc_pc(
          base::GetProgramCounter());
      mojo::internal::MessageDispatchContext context(message);
      if (!message->is_serialized()) {
        auto context = message->TakeUnserializedContext<
            TestBadMessagesProxy_RequestResponseSync_Message>();
        if (!context) {
          // The Message was not of the expected type. It may be a valid message
          // which was built using a different variant of these bindings. Force
          // serialization before dispatch in this case.
          message->SerializeIfNecessary();
        } else {
          TestBadMessages::RequestResponseSyncCallback callback =
              TestBadMessages_RequestResponseSync_ProxyToResponder::CreateCallback(
                  message->request_id(),
                  message->has_flag(mojo::Message::kFlagIsSync),
                  std::move(responder));
          context->Dispatch(impl, std::move(callback));
          return true;
        }
      }

      internal::TestBadMessages_RequestResponseSync_Params_Data* params =
          reinterpret_cast<
              internal::TestBadMessages_RequestResponseSync_Params_Data*>(
                  message->mutable_payload());
      
      mojo::internal::SerializationContext serialization_context;
      serialization_context.TakeHandlesFromMessage(message);
      bool success = true;
      TestBadMessages_RequestResponseSync_ParamsDataView input_data_view(params, &serialization_context);
      
      if (!success) {
        ReportValidationErrorForMessage(
            message,
            mojo::internal::VALIDATION_ERROR_DESERIALIZATION_FAILED,
            TestBadMessages::Name_, 3, false);
        return false;
      }
      TestBadMessages::RequestResponseSyncCallback callback =
          TestBadMessages_RequestResponseSync_ProxyToResponder::CreateCallback(
              message->request_id(),
              message->has_flag(mojo::Message::kFlagIsSync),
              std::move(responder));
      // A null |impl| means no implementation was bound.
      DCHECK(impl);
      impl->RequestResponseSync(std::move(callback));
      return true;
    }
  }
  return false;
}

bool TestBadMessagesRequestValidator::Accept(mojo::Message* message) {
  if (!message->is_serialized() ||
      mojo::internal::ControlMessageHandler::IsControlMessage(message)) {
    return true;
  }

  mojo::internal::ValidationContext validation_context(
    message->payload(), message->payload_num_bytes(),
    message->handles()->size(), message->payload_num_interface_ids(), message,
    "TestBadMessages RequestValidator");

  switch (message->header()->name) {
    case internal::kTestBadMessages_RejectEventually_Name: {
      if (!mojo::internal::ValidateMessageIsRequestExpectingResponse(
              message, &validation_context)) {
        return false;
      }
      if (!mojo::internal::ValidateMessagePayload<
               internal::TestBadMessages_RejectEventually_Params_Data>(
                  message, &validation_context)) {
        return false;
      }
      return true;
    }
    case internal::kTestBadMessages_RequestResponse_Name: {
      if (!mojo::internal::ValidateMessageIsRequestExpectingResponse(
              message, &validation_context)) {
        return false;
      }
      if (!mojo::internal::ValidateMessagePayload<
               internal::TestBadMessages_RequestResponse_Params_Data>(
                  message, &validation_context)) {
        return false;
      }
      return true;
    }
    case internal::kTestBadMessages_RejectSync_Name: {
      if (!mojo::internal::ValidateMessageIsRequestExpectingResponse(
              message, &validation_context)) {
        return false;
      }
      if (!mojo::internal::ValidateMessagePayload<
               internal::TestBadMessages_RejectSync_Params_Data>(
                  message, &validation_context)) {
        return false;
      }
      return true;
    }
    case internal::kTestBadMessages_RequestResponseSync_Name: {
      if (!mojo::internal::ValidateMessageIsRequestExpectingResponse(
              message, &validation_context)) {
        return false;
      }
      if (!mojo::internal::ValidateMessagePayload<
               internal::TestBadMessages_RequestResponseSync_Params_Data>(
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

bool TestBadMessagesResponseValidator::Accept(mojo::Message* message) {
  if (!message->is_serialized() ||
      mojo::internal::ControlMessageHandler::IsControlMessage(message)) {
    return true;
  }

  mojo::internal::ValidationContext validation_context(
    message->payload(), message->payload_num_bytes(),
    message->handles()->size(), message->payload_num_interface_ids(), message,
    "TestBadMessages ResponseValidator");

  if (!mojo::internal::ValidateMessageIsResponse(message, &validation_context))
    return false;
  switch (message->header()->name) {
    case internal::kTestBadMessages_RejectEventually_Name: {
      if (!mojo::internal::ValidateMessagePayload<
               internal::TestBadMessages_RejectEventually_ResponseParams_Data>(
                    message, &validation_context)) {
        return false;
      }
      return true;
    }
    case internal::kTestBadMessages_RequestResponse_Name: {
      if (!mojo::internal::ValidateMessagePayload<
               internal::TestBadMessages_RequestResponse_ResponseParams_Data>(
                    message, &validation_context)) {
        return false;
      }
      return true;
    }
    case internal::kTestBadMessages_RejectSync_Name: {
      if (!mojo::internal::ValidateMessagePayload<
               internal::TestBadMessages_RejectSync_ResponseParams_Data>(
                    message, &validation_context)) {
        return false;
      }
      return true;
    }
    case internal::kTestBadMessages_RequestResponseSync_Name: {
      if (!mojo::internal::ValidateMessagePayload<
               internal::TestBadMessages_RequestResponseSync_ResponseParams_Data>(
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