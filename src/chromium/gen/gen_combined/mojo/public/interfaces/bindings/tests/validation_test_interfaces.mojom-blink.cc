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

#include "mojo/public/interfaces/bindings/tests/validation_test_interfaces.mojom-blink.h"

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

#include "mojo/public/interfaces/bindings/tests/validation_test_interfaces.mojom-params-data.h"
#include "mojo/public/interfaces/bindings/tests/validation_test_interfaces.mojom-shared-message-ids.h"

#include "mojo/public/interfaces/bindings/tests/validation_test_interfaces.mojom-blink-import-headers.h"
#include "mojo/public/cpp/bindings/lib/wtf_serialization.h"


#ifndef MOJO_PUBLIC_INTERFACES_BINDINGS_TESTS_VALIDATION_TEST_INTERFACES_MOJOM_BLINK_JUMBO_H_
#define MOJO_PUBLIC_INTERFACES_BINDINGS_TESTS_VALIDATION_TEST_INTERFACES_MOJOM_BLINK_JUMBO_H_
#endif
namespace mojo {
namespace test {
namespace blink {
StructA::StructA()
    : i() {}

StructA::StructA(
    uint64_t i_in)
    : i(std::move(i_in)) {}

StructA::~StructA() = default;
size_t StructA::Hash(size_t seed) const {
  seed = mojo::internal::WTFHash(seed, this->i);
  return seed;
}

bool StructA::Validate(
    const void* data,
    mojo::internal::ValidationContext* validation_context) {
  return Data_::Validate(data, validation_context);
}
StructB::StructB()
    : struct_a() {}

StructB::StructB(
    StructAPtr struct_a_in)
    : struct_a(std::move(struct_a_in)) {}

StructB::~StructB() = default;
size_t StructB::Hash(size_t seed) const {
  seed = mojo::internal::WTFHash(seed, this->struct_a);
  return seed;
}

bool StructB::Validate(
    const void* data,
    mojo::internal::ValidationContext* validation_context) {
  return Data_::Validate(data, validation_context);
}
StructC::StructC()
    : data() {}

StructC::StructC(
    const WTF::Vector<uint8_t>& data_in)
    : data(std::move(data_in)) {}

StructC::~StructC() = default;

bool StructC::Validate(
    const void* data,
    mojo::internal::ValidationContext* validation_context) {
  return Data_::Validate(data, validation_context);
}
StructD::StructD()
    : message_pipes() {}

StructD::StructD(
    WTF::Vector<mojo::ScopedMessagePipeHandle> message_pipes_in)
    : message_pipes(std::move(message_pipes_in)) {}

StructD::~StructD() = default;

bool StructD::Validate(
    const void* data,
    mojo::internal::ValidationContext* validation_context) {
  return Data_::Validate(data, validation_context);
}
StructE::StructE()
    : struct_d(),
      data_pipe_consumer() {}

StructE::StructE(
    StructDPtr struct_d_in,
    mojo::ScopedDataPipeConsumerHandle data_pipe_consumer_in)
    : struct_d(std::move(struct_d_in)),
      data_pipe_consumer(std::move(data_pipe_consumer_in)) {}

StructE::~StructE() = default;

bool StructE::Validate(
    const void* data,
    mojo::internal::ValidationContext* validation_context) {
  return Data_::Validate(data, validation_context);
}
StructF::StructF()
    : fixed_size_array() {}

StructF::StructF(
    const WTF::Vector<uint8_t>& fixed_size_array_in)
    : fixed_size_array(std::move(fixed_size_array_in)) {}

StructF::~StructF() = default;

bool StructF::Validate(
    const void* data,
    mojo::internal::ValidationContext* validation_context) {
  return Data_::Validate(data, validation_context);
}
StructG::StructG()
    : i(),
      struct_a(),
      str(),
      b() {}

StructG::StructG(
    int32_t i_in)
    : i(std::move(i_in)),
      struct_a(),
      str(),
      b() {}

StructG::StructG(
    int32_t i_in,
    StructAPtr struct_a_in)
    : i(std::move(i_in)),
      struct_a(std::move(struct_a_in)),
      str(),
      b() {}

StructG::StructG(
    int32_t i_in,
    StructAPtr struct_a_in,
    const WTF::String& str_in,
    bool b_in)
    : i(std::move(i_in)),
      struct_a(std::move(struct_a_in)),
      str(std::move(str_in)),
      b(std::move(b_in)) {}

StructG::~StructG() = default;

bool StructG::Validate(
    const void* data,
    mojo::internal::ValidationContext* validation_context) {
  return Data_::Validate(data, validation_context);
}
BasicStruct::BasicStruct()
    : a() {}

BasicStruct::BasicStruct(
    int32_t a_in)
    : a(std::move(a_in)) {}

BasicStruct::~BasicStruct() = default;
size_t BasicStruct::Hash(size_t seed) const {
  seed = mojo::internal::WTFHash(seed, this->a);
  return seed;
}

bool BasicStruct::Validate(
    const void* data,
    mojo::internal::ValidationContext* validation_context) {
  return Data_::Validate(data, validation_context);
}
StructWithEnum::StructWithEnum() {}

StructWithEnum::~StructWithEnum() = default;
size_t StructWithEnum::Hash(size_t seed) const {
  return seed;
}

bool StructWithEnum::Validate(
    const void* data,
    mojo::internal::ValidationContext* validation_context) {
  return Data_::Validate(data, validation_context);
}
Recursive::Recursive()
    : recursive() {}

Recursive::Recursive(
    RecursivePtr recursive_in)
    : recursive(std::move(recursive_in)) {}

Recursive::~Recursive() = default;

bool Recursive::Validate(
    const void* data,
    mojo::internal::ValidationContext* validation_context) {
  return Data_::Validate(data, validation_context);
}
UnionA::UnionA() : tag_(Tag::STRUCT_A) {
  data_.struct_a = new StructAPtr;
}

UnionA::~UnionA() {
  DestroyActive();
}


void UnionA::set_struct_a(
    StructAPtr struct_a) {
  if (tag_ == Tag::STRUCT_A) {
    *(data_.struct_a) = std::move(struct_a);
  } else {
    DestroyActive();
    tag_ = Tag::STRUCT_A;
    data_.struct_a = new StructAPtr(
        std::move(struct_a));
  }
}
void UnionA::set_b(
    bool b) {
  if (tag_ != Tag::B) {
    DestroyActive();
    tag_ = Tag::B;
  }
  data_.b = b;
}

void UnionA::DestroyActive() {
  switch (tag_) {

    case Tag::STRUCT_A:

      delete data_.struct_a;
      break;
    case Tag::B:

      break;
  }
}
size_t UnionA::Hash(size_t seed) const {
  seed = mojo::internal::HashCombine(seed, static_cast<uint32_t>(tag_));
  switch (tag_) {

    case Tag::STRUCT_A:
      return mojo::internal::WTFHash(seed, data_.struct_a);
    case Tag::B:
      return mojo::internal::WTFHash(seed, data_.b);
    default:
      NOTREACHED();
      return seed;
  }
}

bool UnionA::Validate(
    const void* data,
    mojo::internal::ValidationContext* validation_context) {
  return Data_::Validate(data, validation_context, false);
}
const char InterfaceA::Name_[] = "mojo.test.InterfaceA";

InterfaceAProxy::InterfaceAProxy(mojo::MessageReceiverWithResponder* receiver)
    : receiver_(receiver) {
}

// static
bool InterfaceAStubDispatch::Accept(
    InterfaceA* impl,
    mojo::Message* message) {
  return false;
}

// static
bool InterfaceAStubDispatch::AcceptWithResponder(
    InterfaceA* impl,
    mojo::Message* message,
    std::unique_ptr<mojo::MessageReceiverWithStatus> responder) {
  return false;
}

bool InterfaceARequestValidator::Accept(mojo::Message* message) {
  if (!message->is_serialized() ||
      mojo::internal::ControlMessageHandler::IsControlMessage(message)) {
    return true;
  }

  mojo::internal::ValidationContext validation_context(
    message->payload(), message->payload_num_bytes(),
    message->handles()->size(), message->payload_num_interface_ids(), message,
    "InterfaceA RequestValidator");

  switch (message->header()->name) {
    default:
      break;
  }

  // Unrecognized message.
  ReportValidationError(
      &validation_context,
      mojo::internal::VALIDATION_ERROR_MESSAGE_HEADER_UNKNOWN_METHOD);
  return false;
}

const char BoundsCheckTestInterface::Name_[] = "mojo.test.BoundsCheckTestInterface";

class BoundsCheckTestInterface_Method0_ForwardToCallback
    : public mojo::MessageReceiver {
 public:
  BoundsCheckTestInterface_Method0_ForwardToCallback(
      BoundsCheckTestInterface::Method0Callback callback
      ) : callback_(std::move(callback)) {
  }
  bool Accept(mojo::Message* message) override;
 private:
  BoundsCheckTestInterface::Method0Callback callback_;
  DISALLOW_COPY_AND_ASSIGN(BoundsCheckTestInterface_Method0_ForwardToCallback);
};

BoundsCheckTestInterfaceProxy::BoundsCheckTestInterfaceProxy(mojo::MessageReceiverWithResponder* receiver)
    : receiver_(receiver) {
}
class BoundsCheckTestInterfaceProxy_Method0_Message
    : public mojo::internal::UnserializedMessageContext {
 public:
  static const mojo::internal::UnserializedMessageContext::Tag kMessageTag;

  explicit BoundsCheckTestInterfaceProxy_Method0_Message(
      uint32_t message_flags
      , uint8_t param_param0
  )
      : mojo::internal::UnserializedMessageContext(
          &kMessageTag,
          internal::kBoundsCheckTestInterface_Method0_Name,
          message_flags)
      , param_param0_(std::move(param_param0)){}
  ~BoundsCheckTestInterfaceProxy_Method0_Message() override = default;

  static mojo::Message Build(
      bool serialize,
      bool expects_response,
      bool is_sync,
      uint8_t param_param0) {

    
  const uint32_t kFlags =
      ((expects_response) ? mojo::Message::kFlagExpectsResponse : 0) |
      ((is_sync) ? mojo::Message::kFlagIsSync : 0);

    if (!serialize) {
      return mojo::Message(std::make_unique<BoundsCheckTestInterfaceProxy_Method0_Message>(
          kFlags
          , std::move(param_param0)
          ));
    }

    DCHECK(serialize);
    
  mojo::Message message(
      internal::kBoundsCheckTestInterface_Method0_Name, kFlags, 0, 0, nullptr);
  auto* buffer = message.payload_buffer();
  ::mojo::test::internal::BoundsCheckTestInterface_Method0_Params_Data::BufferWriter
      params;
  mojo::internal::SerializationContext serialization_context;
  params.Allocate(buffer);
  params->param0 = param_param0;
  message.AttachHandlesFromSerializationContext(
      &serialization_context);
    return message;
  }


  void Dispatch(BoundsCheckTestInterface* impl, BoundsCheckTestInterface::Method0Callback callback) {
    impl->Method0(
        std::move(param_param0_), std::move(callback));
  }

 private:
  // mojo::internal::UnserializedMessageContext:
  void Serialize(mojo::internal::SerializationContext* serialization_context,
                 mojo::internal::Buffer* buffer) override {
    ::mojo::test::internal::BoundsCheckTestInterface_Method0_Params_Data::BufferWriter
        params;
    params.Allocate(buffer);
  params->param0 = param_param0_;
  }
  uint8_t param_param0_;

  DISALLOW_COPY_AND_ASSIGN(BoundsCheckTestInterfaceProxy_Method0_Message);
};

const mojo::internal::UnserializedMessageContext::Tag
BoundsCheckTestInterfaceProxy_Method0_Message::kMessageTag = {};

void BoundsCheckTestInterfaceProxy::Method0(
    uint8_t in_param0, Method0Callback callback) {
#if BUILDFLAG(MOJO_TRACE_ENABLED)
  TRACE_EVENT0("mojom", "<class 'jinja2::utils::Namespace'>::BoundsCheckTestInterface::Method0");
#endif
  const bool kExpectsResponse = true;
  const bool kIsSync = false;
  const bool kSerialize = receiver_->PrefersSerializedMessages();
  auto message = BoundsCheckTestInterfaceProxy_Method0_Message::Build(
      kSerialize, kExpectsResponse, kIsSync, std::move(in_param0));

#if defined(ENABLE_IPC_FUZZER)
  message.set_interface_name(BoundsCheckTestInterface::Name_);
  message.set_method_name("Method0");
#endif
  std::unique_ptr<mojo::MessageReceiver> responder(
      new BoundsCheckTestInterface_Method0_ForwardToCallback(
          std::move(callback)));
  ignore_result(receiver_->AcceptWithResponder(&message, std::move(responder)));
}
class BoundsCheckTestInterfaceProxy_Method1_Message
    : public mojo::internal::UnserializedMessageContext {
 public:
  static const mojo::internal::UnserializedMessageContext::Tag kMessageTag;

  explicit BoundsCheckTestInterfaceProxy_Method1_Message(
      uint32_t message_flags
      , uint8_t param_param0
  )
      : mojo::internal::UnserializedMessageContext(
          &kMessageTag,
          internal::kBoundsCheckTestInterface_Method1_Name,
          message_flags)
      , param_param0_(std::move(param_param0)){}
  ~BoundsCheckTestInterfaceProxy_Method1_Message() override = default;

  static mojo::Message Build(
      bool serialize,
      bool expects_response,
      bool is_sync,
      uint8_t param_param0) {

    
  const uint32_t kFlags =
      ((expects_response) ? mojo::Message::kFlagExpectsResponse : 0) |
      ((is_sync) ? mojo::Message::kFlagIsSync : 0);

    if (!serialize) {
      return mojo::Message(std::make_unique<BoundsCheckTestInterfaceProxy_Method1_Message>(
          kFlags
          , std::move(param_param0)
          ));
    }

    DCHECK(serialize);
    
  mojo::Message message(
      internal::kBoundsCheckTestInterface_Method1_Name, kFlags, 0, 0, nullptr);
  auto* buffer = message.payload_buffer();
  ::mojo::test::internal::BoundsCheckTestInterface_Method1_Params_Data::BufferWriter
      params;
  mojo::internal::SerializationContext serialization_context;
  params.Allocate(buffer);
  params->param0 = param_param0;
  message.AttachHandlesFromSerializationContext(
      &serialization_context);
    return message;
  }


  void Dispatch(BoundsCheckTestInterface* impl) {
    impl->Method1(
        std::move(param_param0_));
  }

 private:
  // mojo::internal::UnserializedMessageContext:
  void Serialize(mojo::internal::SerializationContext* serialization_context,
                 mojo::internal::Buffer* buffer) override {
    ::mojo::test::internal::BoundsCheckTestInterface_Method1_Params_Data::BufferWriter
        params;
    params.Allocate(buffer);
  params->param0 = param_param0_;
  }
  uint8_t param_param0_;

  DISALLOW_COPY_AND_ASSIGN(BoundsCheckTestInterfaceProxy_Method1_Message);
};

const mojo::internal::UnserializedMessageContext::Tag
BoundsCheckTestInterfaceProxy_Method1_Message::kMessageTag = {};

void BoundsCheckTestInterfaceProxy::Method1(
    uint8_t in_param0) {
#if BUILDFLAG(MOJO_TRACE_ENABLED)
  TRACE_EVENT0("mojom", "<class 'jinja2::utils::Namespace'>::BoundsCheckTestInterface::Method1");
#endif
  const bool kExpectsResponse = false;
  const bool kIsSync = false;
  const bool kSerialize = receiver_->PrefersSerializedMessages();
  auto message = BoundsCheckTestInterfaceProxy_Method1_Message::Build(
      kSerialize, kExpectsResponse, kIsSync, std::move(in_param0));

#if defined(ENABLE_IPC_FUZZER)
  message.set_interface_name(BoundsCheckTestInterface::Name_);
  message.set_method_name("Method1");
#endif
  // This return value may be ignored as false implies the Connector has
  // encountered an error, which will be visible through other means.
  ignore_result(receiver_->Accept(&message));
}
class BoundsCheckTestInterface_Method0_ProxyToResponder {
 public:
  static BoundsCheckTestInterface::Method0Callback CreateCallback(
      uint64_t request_id,
      bool is_sync,
      std::unique_ptr<mojo::MessageReceiverWithStatus> responder) {
    std::unique_ptr<BoundsCheckTestInterface_Method0_ProxyToResponder> proxy(
        new BoundsCheckTestInterface_Method0_ProxyToResponder(
            request_id, is_sync, std::move(responder)));
    return base::BindOnce(&BoundsCheckTestInterface_Method0_ProxyToResponder::Run,
                          std::move(proxy));
  }

  ~BoundsCheckTestInterface_Method0_ProxyToResponder() {
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
  BoundsCheckTestInterface_Method0_ProxyToResponder(
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
        << "BoundsCheckTestInterface::Method0Callback was destroyed without "
        << "first either being run or its corresponding binding being closed. "
        << "It is an error to drop response callbacks which still correspond "
        << "to an open interface pipe.";
  }
#endif

  void Run(
      uint8_t in_param0);

  uint64_t request_id_;
  bool is_sync_;
  std::unique_ptr<mojo::MessageReceiverWithStatus> responder_;

  DISALLOW_COPY_AND_ASSIGN(BoundsCheckTestInterface_Method0_ProxyToResponder);
};
class BoundsCheckTestInterface_Method0_Response_Message
    : public mojo::internal::UnserializedMessageContext {
 public:
  static const mojo::internal::UnserializedMessageContext::Tag kMessageTag;

  explicit BoundsCheckTestInterface_Method0_Response_Message(
      uint32_t message_flags
      , uint8_t param_param0
  )
      : mojo::internal::UnserializedMessageContext(
          &kMessageTag,
          internal::kBoundsCheckTestInterface_Method0_Name,
          message_flags)
      , param_param0_(std::move(param_param0)){}
  ~BoundsCheckTestInterface_Method0_Response_Message() override = default;

  static mojo::Message Build(
      bool serialize,
      bool is_sync,
      uint8_t param_param0) {

    
  const uint32_t kFlags = mojo::Message::kFlagIsResponse |
      ((is_sync) ? mojo::Message::kFlagIsSync : 0);

    if (!serialize) {
      return mojo::Message(std::make_unique<BoundsCheckTestInterface_Method0_Response_Message>(
          kFlags
          , std::move(param_param0)
          ));
    }

    DCHECK(serialize);
    
  mojo::Message message(
      internal::kBoundsCheckTestInterface_Method0_Name, kFlags, 0, 0, nullptr);
  auto* buffer = message.payload_buffer();
  ::mojo::test::internal::BoundsCheckTestInterface_Method0_ResponseParams_Data::BufferWriter
      params;
  mojo::internal::SerializationContext serialization_context;
  params.Allocate(buffer);
  params->param0 = param_param0;
  message.AttachHandlesFromSerializationContext(
      &serialization_context);
    return message;
  }


  void Dispatch(BoundsCheckTestInterface::Method0Callback* callback) {
    std::move(*callback).Run(
        std::move(param_param0_));
  }



 private:
  // mojo::internal::UnserializedMessageContext:
  void Serialize(mojo::internal::SerializationContext* serialization_context,
                 mojo::internal::Buffer* buffer) override {
    ::mojo::test::internal::BoundsCheckTestInterface_Method0_ResponseParams_Data::BufferWriter
        params;
    params.Allocate(buffer);
  params->param0 = param_param0_;
  }
  uint8_t param_param0_;

  DISALLOW_COPY_AND_ASSIGN(BoundsCheckTestInterface_Method0_Response_Message);
};

const mojo::internal::UnserializedMessageContext::Tag
BoundsCheckTestInterface_Method0_Response_Message::kMessageTag = {};

bool BoundsCheckTestInterface_Method0_ForwardToCallback::Accept(
    mojo::Message* message) {
#if BUILDFLAG(MOJO_TRACE_ENABLED)
  TRACE_EVENT1("mojom", "<class 'jinja2::utils::Namespace'>::BoundsCheckTestInterface::Method0Callback",
               "message", message->name());
#endif
  mojo::internal::MessageDispatchContext dispatch_context(message);
  if (!message->is_serialized()) {
    auto context =
        message->TakeUnserializedContext<BoundsCheckTestInterface_Method0_Response_Message>();
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
  internal::BoundsCheckTestInterface_Method0_ResponseParams_Data* params =
      reinterpret_cast<
          internal::BoundsCheckTestInterface_Method0_ResponseParams_Data*>(
              message->mutable_payload());
  
  mojo::internal::SerializationContext serialization_context;
  serialization_context.TakeHandlesFromMessage(message);
  bool success = true;
  uint8_t p_param0{};
  BoundsCheckTestInterface_Method0_ResponseParamsDataView input_data_view(params, &serialization_context);
  
  p_param0 = input_data_view.param0();
  if (!success) {
    ReportValidationErrorForMessage(
        message,
        mojo::internal::VALIDATION_ERROR_DESERIALIZATION_FAILED,
        BoundsCheckTestInterface::Name_, 0, true);
    return false;
  }
  if (!callback_.is_null())
    std::move(callback_).Run(
std::move(p_param0));
  return true;
}

void BoundsCheckTestInterface_Method0_ProxyToResponder::Run(
    uint8_t in_param0) {
  const bool kSerialize = responder_->PrefersSerializedMessages();
  auto message = BoundsCheckTestInterface_Method0_Response_Message::Build(kSerialize, is_sync_, std::move(in_param0));

#if BUILDFLAG(MOJO_TRACE_ENABLED)
  TRACE_EVENT1("mojom", "(Impl)<class 'jinja2::utils::Namespace'>::BoundsCheckTestInterface::Method0Callback",
               "message", message.name());
#endif

#if defined(ENABLE_IPC_FUZZER)
  message.set_interface_name(BoundsCheckTestInterface::Name_);
  message.set_method_name("Method0");
#endif

  message.set_request_id(request_id_);
  ignore_result(responder_->Accept(&message));
  // TODO(darin): Accept() returning false indicates a malformed message, and
  // that may be good reason to close the connection. However, we don't have a
  // way to do that from here. We should add a way.
  responder_ = nullptr;
}

// static
bool BoundsCheckTestInterfaceStubDispatch::Accept(
    BoundsCheckTestInterface* impl,
    mojo::Message* message) {
  switch (message->header()->name) {
    case internal::kBoundsCheckTestInterface_Method0_Name: {
      break;
    }
    case internal::kBoundsCheckTestInterface_Method1_Name: {
#if BUILDFLAG(MOJO_TRACE_ENABLED)
      TRACE_EVENT1(
          "mojom",
          "(Impl)<class 'jinja2::utils::Namespace'>::BoundsCheckTestInterface::Method1",
          "message", message->name());
#endif
      base::TaskAnnotator::ScopedSetIpcProgramCounter scoped_ipc_pc(
          base::GetProgramCounter());
      mojo::internal::MessageDispatchContext context(message);
      if (!message->is_serialized()) {
        auto context = message->TakeUnserializedContext<
            BoundsCheckTestInterfaceProxy_Method1_Message>();
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
      internal::BoundsCheckTestInterface_Method1_Params_Data* params =
          reinterpret_cast<internal::BoundsCheckTestInterface_Method1_Params_Data*>(
              message->mutable_payload());
      
      mojo::internal::SerializationContext serialization_context;
      serialization_context.TakeHandlesFromMessage(message);
      bool success = true;
      uint8_t p_param0{};
      BoundsCheckTestInterface_Method1_ParamsDataView input_data_view(params, &serialization_context);
      
      p_param0 = input_data_view.param0();
      if (!success) {
        ReportValidationErrorForMessage(
            message,
            mojo::internal::VALIDATION_ERROR_DESERIALIZATION_FAILED,
            BoundsCheckTestInterface::Name_, 1, false);
        return false;
      }
      // A null |impl| means no implementation was bound.
      DCHECK(impl);
      impl->Method1(
std::move(p_param0));
      return true;
    }
  }
  return false;
}

// static
bool BoundsCheckTestInterfaceStubDispatch::AcceptWithResponder(
    BoundsCheckTestInterface* impl,
    mojo::Message* message,
    std::unique_ptr<mojo::MessageReceiverWithStatus> responder) {
  switch (message->header()->name) {
    case internal::kBoundsCheckTestInterface_Method0_Name: {
#if BUILDFLAG(MOJO_TRACE_ENABLED)
      TRACE_EVENT1(
          "mojom",
          "(Impl)<class 'jinja2::utils::Namespace'>::BoundsCheckTestInterface::Method0",
          "message", message->name());
#endif
      base::TaskAnnotator::ScopedSetIpcProgramCounter scoped_ipc_pc(
          base::GetProgramCounter());
      mojo::internal::MessageDispatchContext context(message);
      if (!message->is_serialized()) {
        auto context = message->TakeUnserializedContext<
            BoundsCheckTestInterfaceProxy_Method0_Message>();
        if (!context) {
          // The Message was not of the expected type. It may be a valid message
          // which was built using a different variant of these bindings. Force
          // serialization before dispatch in this case.
          message->SerializeIfNecessary();
        } else {
          BoundsCheckTestInterface::Method0Callback callback =
              BoundsCheckTestInterface_Method0_ProxyToResponder::CreateCallback(
                  message->request_id(),
                  message->has_flag(mojo::Message::kFlagIsSync),
                  std::move(responder));
          context->Dispatch(impl, std::move(callback));
          return true;
        }
      }

      internal::BoundsCheckTestInterface_Method0_Params_Data* params =
          reinterpret_cast<
              internal::BoundsCheckTestInterface_Method0_Params_Data*>(
                  message->mutable_payload());
      
      mojo::internal::SerializationContext serialization_context;
      serialization_context.TakeHandlesFromMessage(message);
      bool success = true;
      uint8_t p_param0{};
      BoundsCheckTestInterface_Method0_ParamsDataView input_data_view(params, &serialization_context);
      
      p_param0 = input_data_view.param0();
      if (!success) {
        ReportValidationErrorForMessage(
            message,
            mojo::internal::VALIDATION_ERROR_DESERIALIZATION_FAILED,
            BoundsCheckTestInterface::Name_, 0, false);
        return false;
      }
      BoundsCheckTestInterface::Method0Callback callback =
          BoundsCheckTestInterface_Method0_ProxyToResponder::CreateCallback(
              message->request_id(),
              message->has_flag(mojo::Message::kFlagIsSync),
              std::move(responder));
      // A null |impl| means no implementation was bound.
      DCHECK(impl);
      impl->Method0(
std::move(p_param0), std::move(callback));
      return true;
    }
    case internal::kBoundsCheckTestInterface_Method1_Name: {
      break;
    }
  }
  return false;
}

bool BoundsCheckTestInterfaceRequestValidator::Accept(mojo::Message* message) {
  if (!message->is_serialized() ||
      mojo::internal::ControlMessageHandler::IsControlMessage(message)) {
    return true;
  }

  mojo::internal::ValidationContext validation_context(
    message->payload(), message->payload_num_bytes(),
    message->handles()->size(), message->payload_num_interface_ids(), message,
    "BoundsCheckTestInterface RequestValidator");

  switch (message->header()->name) {
    case internal::kBoundsCheckTestInterface_Method0_Name: {
      if (!mojo::internal::ValidateMessageIsRequestExpectingResponse(
              message, &validation_context)) {
        return false;
      }
      if (!mojo::internal::ValidateMessagePayload<
               internal::BoundsCheckTestInterface_Method0_Params_Data>(
                  message, &validation_context)) {
        return false;
      }
      return true;
    }
    case internal::kBoundsCheckTestInterface_Method1_Name: {
      if (!mojo::internal::ValidateMessageIsRequestWithoutResponse(
              message, &validation_context)) {
        return false;
      }
      if (!mojo::internal::ValidateMessagePayload<
               internal::BoundsCheckTestInterface_Method1_Params_Data>(
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

bool BoundsCheckTestInterfaceResponseValidator::Accept(mojo::Message* message) {
  if (!message->is_serialized() ||
      mojo::internal::ControlMessageHandler::IsControlMessage(message)) {
    return true;
  }

  mojo::internal::ValidationContext validation_context(
    message->payload(), message->payload_num_bytes(),
    message->handles()->size(), message->payload_num_interface_ids(), message,
    "BoundsCheckTestInterface ResponseValidator");

  if (!mojo::internal::ValidateMessageIsResponse(message, &validation_context))
    return false;
  switch (message->header()->name) {
    case internal::kBoundsCheckTestInterface_Method0_Name: {
      if (!mojo::internal::ValidateMessagePayload<
               internal::BoundsCheckTestInterface_Method0_ResponseParams_Data>(
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
const char ConformanceTestInterface::Name_[] = "mojo.test.ConformanceTestInterface";

class ConformanceTestInterface_Method12_ForwardToCallback
    : public mojo::MessageReceiver {
 public:
  ConformanceTestInterface_Method12_ForwardToCallback(
      ConformanceTestInterface::Method12Callback callback
      ) : callback_(std::move(callback)) {
  }
  bool Accept(mojo::Message* message) override;
 private:
  ConformanceTestInterface::Method12Callback callback_;
  DISALLOW_COPY_AND_ASSIGN(ConformanceTestInterface_Method12_ForwardToCallback);
};

ConformanceTestInterfaceProxy::ConformanceTestInterfaceProxy(mojo::MessageReceiverWithResponder* receiver)
    : receiver_(receiver) {
}
class ConformanceTestInterfaceProxy_Method0_Message
    : public mojo::internal::UnserializedMessageContext {
 public:
  static const mojo::internal::UnserializedMessageContext::Tag kMessageTag;

  explicit ConformanceTestInterfaceProxy_Method0_Message(
      uint32_t message_flags
      , float param_param0
  )
      : mojo::internal::UnserializedMessageContext(
          &kMessageTag,
          internal::kConformanceTestInterface_Method0_Name,
          message_flags)
      , param_param0_(std::move(param_param0)){}
  ~ConformanceTestInterfaceProxy_Method0_Message() override = default;

  static mojo::Message Build(
      bool serialize,
      bool expects_response,
      bool is_sync,
      float param_param0) {

    
  const uint32_t kFlags =
      ((expects_response) ? mojo::Message::kFlagExpectsResponse : 0) |
      ((is_sync) ? mojo::Message::kFlagIsSync : 0);

    if (!serialize) {
      return mojo::Message(std::make_unique<ConformanceTestInterfaceProxy_Method0_Message>(
          kFlags
          , std::move(param_param0)
          ));
    }

    DCHECK(serialize);
    
  mojo::Message message(
      internal::kConformanceTestInterface_Method0_Name, kFlags, 0, 0, nullptr);
  auto* buffer = message.payload_buffer();
  ::mojo::test::internal::ConformanceTestInterface_Method0_Params_Data::BufferWriter
      params;
  mojo::internal::SerializationContext serialization_context;
  params.Allocate(buffer);
  params->param0 = param_param0;
  message.AttachHandlesFromSerializationContext(
      &serialization_context);
    return message;
  }


  void Dispatch(ConformanceTestInterface* impl) {
    impl->Method0(
        std::move(param_param0_));
  }

 private:
  // mojo::internal::UnserializedMessageContext:
  void Serialize(mojo::internal::SerializationContext* serialization_context,
                 mojo::internal::Buffer* buffer) override {
    ::mojo::test::internal::ConformanceTestInterface_Method0_Params_Data::BufferWriter
        params;
    params.Allocate(buffer);
  params->param0 = param_param0_;
  }
  float param_param0_;

  DISALLOW_COPY_AND_ASSIGN(ConformanceTestInterfaceProxy_Method0_Message);
};

const mojo::internal::UnserializedMessageContext::Tag
ConformanceTestInterfaceProxy_Method0_Message::kMessageTag = {};

void ConformanceTestInterfaceProxy::Method0(
    float in_param0) {
#if BUILDFLAG(MOJO_TRACE_ENABLED)
  TRACE_EVENT0("mojom", "<class 'jinja2::utils::Namespace'>::ConformanceTestInterface::Method0");
#endif
  const bool kExpectsResponse = false;
  const bool kIsSync = false;
  const bool kSerialize = receiver_->PrefersSerializedMessages();
  auto message = ConformanceTestInterfaceProxy_Method0_Message::Build(
      kSerialize, kExpectsResponse, kIsSync, std::move(in_param0));

#if defined(ENABLE_IPC_FUZZER)
  message.set_interface_name(ConformanceTestInterface::Name_);
  message.set_method_name("Method0");
#endif
  // This return value may be ignored as false implies the Connector has
  // encountered an error, which will be visible through other means.
  ignore_result(receiver_->Accept(&message));
}
class ConformanceTestInterfaceProxy_Method1_Message
    : public mojo::internal::UnserializedMessageContext {
 public:
  static const mojo::internal::UnserializedMessageContext::Tag kMessageTag;

  explicit ConformanceTestInterfaceProxy_Method1_Message(
      uint32_t message_flags
      , StructAPtr param_param0
  )
      : mojo::internal::UnserializedMessageContext(
          &kMessageTag,
          internal::kConformanceTestInterface_Method1_Name,
          message_flags)
      , param_param0_(std::move(param_param0)){}
  ~ConformanceTestInterfaceProxy_Method1_Message() override = default;

  static mojo::Message Build(
      bool serialize,
      bool expects_response,
      bool is_sync,
      StructAPtr param_param0) {

    
  const uint32_t kFlags =
      ((expects_response) ? mojo::Message::kFlagExpectsResponse : 0) |
      ((is_sync) ? mojo::Message::kFlagIsSync : 0);

    if (!serialize) {
      return mojo::Message(std::make_unique<ConformanceTestInterfaceProxy_Method1_Message>(
          kFlags
          , std::move(param_param0)
          ));
    }

    DCHECK(serialize);
    
  mojo::Message message(
      internal::kConformanceTestInterface_Method1_Name, kFlags, 0, 0, nullptr);
  auto* buffer = message.payload_buffer();
  ::mojo::test::internal::ConformanceTestInterface_Method1_Params_Data::BufferWriter
      params;
  mojo::internal::SerializationContext serialization_context;
  params.Allocate(buffer);
  typename decltype(params->param0)::BaseType::BufferWriter
      param0_writer;
  mojo::internal::Serialize<::mojo::test::StructADataView>(
      param_param0, buffer, &param0_writer, &serialization_context);
  params->param0.Set(
      param0_writer.is_null() ? nullptr : param0_writer.data());
  MOJO_INTERNAL_DLOG_SERIALIZATION_WARNING(
      params->param0.is_null(),
      mojo::internal::VALIDATION_ERROR_UNEXPECTED_NULL_POINTER,
      "null param0 in ConformanceTestInterface.Method1 request");
  message.AttachHandlesFromSerializationContext(
      &serialization_context);
    return message;
  }


  void Dispatch(ConformanceTestInterface* impl) {
    impl->Method1(
        std::move(param_param0_));
  }

 private:
  // mojo::internal::UnserializedMessageContext:
  void Serialize(mojo::internal::SerializationContext* serialization_context,
                 mojo::internal::Buffer* buffer) override {
    ::mojo::test::internal::ConformanceTestInterface_Method1_Params_Data::BufferWriter
        params;
    params.Allocate(buffer);
  typename decltype(params->param0)::BaseType::BufferWriter
      param0_writer;
  mojo::internal::Serialize<::mojo::test::StructADataView>(
      param_param0_, buffer, &param0_writer, serialization_context);
  params->param0.Set(
      param0_writer.is_null() ? nullptr : param0_writer.data());
  MOJO_INTERNAL_DLOG_SERIALIZATION_WARNING(
      params->param0.is_null(),
      mojo::internal::VALIDATION_ERROR_UNEXPECTED_NULL_POINTER,
      "null param0 in ConformanceTestInterface.Method1 request");
  }
  StructAPtr param_param0_;

  DISALLOW_COPY_AND_ASSIGN(ConformanceTestInterfaceProxy_Method1_Message);
};

const mojo::internal::UnserializedMessageContext::Tag
ConformanceTestInterfaceProxy_Method1_Message::kMessageTag = {};

void ConformanceTestInterfaceProxy::Method1(
    StructAPtr in_param0) {
#if BUILDFLAG(MOJO_TRACE_ENABLED)
  TRACE_EVENT0("mojom", "<class 'jinja2::utils::Namespace'>::ConformanceTestInterface::Method1");
#endif
  const bool kExpectsResponse = false;
  const bool kIsSync = false;
  const bool kSerialize = receiver_->PrefersSerializedMessages();
  auto message = ConformanceTestInterfaceProxy_Method1_Message::Build(
      kSerialize, kExpectsResponse, kIsSync, std::move(in_param0));

#if defined(ENABLE_IPC_FUZZER)
  message.set_interface_name(ConformanceTestInterface::Name_);
  message.set_method_name("Method1");
#endif
  // This return value may be ignored as false implies the Connector has
  // encountered an error, which will be visible through other means.
  ignore_result(receiver_->Accept(&message));
}
class ConformanceTestInterfaceProxy_Method2_Message
    : public mojo::internal::UnserializedMessageContext {
 public:
  static const mojo::internal::UnserializedMessageContext::Tag kMessageTag;

  explicit ConformanceTestInterfaceProxy_Method2_Message(
      uint32_t message_flags
      , StructBPtr param_param0
      , StructAPtr param_param1
  )
      : mojo::internal::UnserializedMessageContext(
          &kMessageTag,
          internal::kConformanceTestInterface_Method2_Name,
          message_flags)
      , param_param0_(std::move(param_param0))
      , param_param1_(std::move(param_param1)){}
  ~ConformanceTestInterfaceProxy_Method2_Message() override = default;

  static mojo::Message Build(
      bool serialize,
      bool expects_response,
      bool is_sync,
      StructBPtr param_param0, StructAPtr param_param1) {

    
  const uint32_t kFlags =
      ((expects_response) ? mojo::Message::kFlagExpectsResponse : 0) |
      ((is_sync) ? mojo::Message::kFlagIsSync : 0);

    if (!serialize) {
      return mojo::Message(std::make_unique<ConformanceTestInterfaceProxy_Method2_Message>(
          kFlags
          , std::move(param_param0)
          , std::move(param_param1)
          ));
    }

    DCHECK(serialize);
    
  mojo::Message message(
      internal::kConformanceTestInterface_Method2_Name, kFlags, 0, 0, nullptr);
  auto* buffer = message.payload_buffer();
  ::mojo::test::internal::ConformanceTestInterface_Method2_Params_Data::BufferWriter
      params;
  mojo::internal::SerializationContext serialization_context;
  params.Allocate(buffer);
  typename decltype(params->param0)::BaseType::BufferWriter
      param0_writer;
  mojo::internal::Serialize<::mojo::test::StructBDataView>(
      param_param0, buffer, &param0_writer, &serialization_context);
  params->param0.Set(
      param0_writer.is_null() ? nullptr : param0_writer.data());
  MOJO_INTERNAL_DLOG_SERIALIZATION_WARNING(
      params->param0.is_null(),
      mojo::internal::VALIDATION_ERROR_UNEXPECTED_NULL_POINTER,
      "null param0 in ConformanceTestInterface.Method2 request");
  typename decltype(params->param1)::BaseType::BufferWriter
      param1_writer;
  mojo::internal::Serialize<::mojo::test::StructADataView>(
      param_param1, buffer, &param1_writer, &serialization_context);
  params->param1.Set(
      param1_writer.is_null() ? nullptr : param1_writer.data());
  MOJO_INTERNAL_DLOG_SERIALIZATION_WARNING(
      params->param1.is_null(),
      mojo::internal::VALIDATION_ERROR_UNEXPECTED_NULL_POINTER,
      "null param1 in ConformanceTestInterface.Method2 request");
  message.AttachHandlesFromSerializationContext(
      &serialization_context);
    return message;
  }


  void Dispatch(ConformanceTestInterface* impl) {
    impl->Method2(
        std::move(param_param0_),
        std::move(param_param1_));
  }

 private:
  // mojo::internal::UnserializedMessageContext:
  void Serialize(mojo::internal::SerializationContext* serialization_context,
                 mojo::internal::Buffer* buffer) override {
    ::mojo::test::internal::ConformanceTestInterface_Method2_Params_Data::BufferWriter
        params;
    params.Allocate(buffer);
  typename decltype(params->param0)::BaseType::BufferWriter
      param0_writer;
  mojo::internal::Serialize<::mojo::test::StructBDataView>(
      param_param0_, buffer, &param0_writer, serialization_context);
  params->param0.Set(
      param0_writer.is_null() ? nullptr : param0_writer.data());
  MOJO_INTERNAL_DLOG_SERIALIZATION_WARNING(
      params->param0.is_null(),
      mojo::internal::VALIDATION_ERROR_UNEXPECTED_NULL_POINTER,
      "null param0 in ConformanceTestInterface.Method2 request");
  typename decltype(params->param1)::BaseType::BufferWriter
      param1_writer;
  mojo::internal::Serialize<::mojo::test::StructADataView>(
      param_param1_, buffer, &param1_writer, serialization_context);
  params->param1.Set(
      param1_writer.is_null() ? nullptr : param1_writer.data());
  MOJO_INTERNAL_DLOG_SERIALIZATION_WARNING(
      params->param1.is_null(),
      mojo::internal::VALIDATION_ERROR_UNEXPECTED_NULL_POINTER,
      "null param1 in ConformanceTestInterface.Method2 request");
  }
  StructBPtr param_param0_;
  StructAPtr param_param1_;

  DISALLOW_COPY_AND_ASSIGN(ConformanceTestInterfaceProxy_Method2_Message);
};

const mojo::internal::UnserializedMessageContext::Tag
ConformanceTestInterfaceProxy_Method2_Message::kMessageTag = {};

void ConformanceTestInterfaceProxy::Method2(
    StructBPtr in_param0, StructAPtr in_param1) {
#if BUILDFLAG(MOJO_TRACE_ENABLED)
  TRACE_EVENT0("mojom", "<class 'jinja2::utils::Namespace'>::ConformanceTestInterface::Method2");
#endif
  const bool kExpectsResponse = false;
  const bool kIsSync = false;
  const bool kSerialize = receiver_->PrefersSerializedMessages();
  auto message = ConformanceTestInterfaceProxy_Method2_Message::Build(
      kSerialize, kExpectsResponse, kIsSync, std::move(in_param0), std::move(in_param1));

#if defined(ENABLE_IPC_FUZZER)
  message.set_interface_name(ConformanceTestInterface::Name_);
  message.set_method_name("Method2");
#endif
  // This return value may be ignored as false implies the Connector has
  // encountered an error, which will be visible through other means.
  ignore_result(receiver_->Accept(&message));
}
class ConformanceTestInterfaceProxy_Method3_Message
    : public mojo::internal::UnserializedMessageContext {
 public:
  static const mojo::internal::UnserializedMessageContext::Tag kMessageTag;

  explicit ConformanceTestInterfaceProxy_Method3_Message(
      uint32_t message_flags
      , const WTF::Vector<bool>& param_param0
  )
      : mojo::internal::UnserializedMessageContext(
          &kMessageTag,
          internal::kConformanceTestInterface_Method3_Name,
          message_flags)
      , param_param0_(std::move(param_param0)){}
  ~ConformanceTestInterfaceProxy_Method3_Message() override = default;

  static mojo::Message Build(
      bool serialize,
      bool expects_response,
      bool is_sync,
      const WTF::Vector<bool>& param_param0) {

    
  const uint32_t kFlags =
      ((expects_response) ? mojo::Message::kFlagExpectsResponse : 0) |
      ((is_sync) ? mojo::Message::kFlagIsSync : 0);

    if (!serialize) {
      return mojo::Message(std::make_unique<ConformanceTestInterfaceProxy_Method3_Message>(
          kFlags
          , std::move(param_param0)
          ));
    }

    DCHECK(serialize);
    
  mojo::Message message(
      internal::kConformanceTestInterface_Method3_Name, kFlags, 0, 0, nullptr);
  auto* buffer = message.payload_buffer();
  ::mojo::test::internal::ConformanceTestInterface_Method3_Params_Data::BufferWriter
      params;
  mojo::internal::SerializationContext serialization_context;
  params.Allocate(buffer);
  typename decltype(params->param0)::BaseType::BufferWriter
      param0_writer;
  const mojo::internal::ContainerValidateParams param0_validate_params(
      0, false, nullptr);
  mojo::internal::Serialize<mojo::ArrayDataView<bool>>(
      param_param0, buffer, &param0_writer, &param0_validate_params,
      &serialization_context);
  params->param0.Set(
      param0_writer.is_null() ? nullptr : param0_writer.data());
  MOJO_INTERNAL_DLOG_SERIALIZATION_WARNING(
      params->param0.is_null(),
      mojo::internal::VALIDATION_ERROR_UNEXPECTED_NULL_POINTER,
      "null param0 in ConformanceTestInterface.Method3 request");
  message.AttachHandlesFromSerializationContext(
      &serialization_context);
    return message;
  }


  void Dispatch(ConformanceTestInterface* impl) {
    impl->Method3(
        std::move(param_param0_));
  }

 private:
  // mojo::internal::UnserializedMessageContext:
  void Serialize(mojo::internal::SerializationContext* serialization_context,
                 mojo::internal::Buffer* buffer) override {
    ::mojo::test::internal::ConformanceTestInterface_Method3_Params_Data::BufferWriter
        params;
    params.Allocate(buffer);
  typename decltype(params->param0)::BaseType::BufferWriter
      param0_writer;
  const mojo::internal::ContainerValidateParams param0_validate_params(
      0, false, nullptr);
  mojo::internal::Serialize<mojo::ArrayDataView<bool>>(
      param_param0_, buffer, &param0_writer, &param0_validate_params,
      serialization_context);
  params->param0.Set(
      param0_writer.is_null() ? nullptr : param0_writer.data());
  MOJO_INTERNAL_DLOG_SERIALIZATION_WARNING(
      params->param0.is_null(),
      mojo::internal::VALIDATION_ERROR_UNEXPECTED_NULL_POINTER,
      "null param0 in ConformanceTestInterface.Method3 request");
  }
  WTF::Vector<bool> param_param0_;

  DISALLOW_COPY_AND_ASSIGN(ConformanceTestInterfaceProxy_Method3_Message);
};

const mojo::internal::UnserializedMessageContext::Tag
ConformanceTestInterfaceProxy_Method3_Message::kMessageTag = {};

void ConformanceTestInterfaceProxy::Method3(
    const WTF::Vector<bool>& in_param0) {
#if BUILDFLAG(MOJO_TRACE_ENABLED)
  TRACE_EVENT0("mojom", "<class 'jinja2::utils::Namespace'>::ConformanceTestInterface::Method3");
#endif
  const bool kExpectsResponse = false;
  const bool kIsSync = false;
  const bool kSerialize = receiver_->PrefersSerializedMessages();
  auto message = ConformanceTestInterfaceProxy_Method3_Message::Build(
      kSerialize, kExpectsResponse, kIsSync, std::move(in_param0));

#if defined(ENABLE_IPC_FUZZER)
  message.set_interface_name(ConformanceTestInterface::Name_);
  message.set_method_name("Method3");
#endif
  // This return value may be ignored as false implies the Connector has
  // encountered an error, which will be visible through other means.
  ignore_result(receiver_->Accept(&message));
}
class ConformanceTestInterfaceProxy_Method4_Message
    : public mojo::internal::UnserializedMessageContext {
 public:
  static const mojo::internal::UnserializedMessageContext::Tag kMessageTag;

  explicit ConformanceTestInterfaceProxy_Method4_Message(
      uint32_t message_flags
      , StructCPtr param_param0
      , const WTF::Vector<uint8_t>& param_param1
  )
      : mojo::internal::UnserializedMessageContext(
          &kMessageTag,
          internal::kConformanceTestInterface_Method4_Name,
          message_flags)
      , param_param0_(std::move(param_param0))
      , param_param1_(std::move(param_param1)){}
  ~ConformanceTestInterfaceProxy_Method4_Message() override = default;

  static mojo::Message Build(
      bool serialize,
      bool expects_response,
      bool is_sync,
      StructCPtr param_param0, const WTF::Vector<uint8_t>& param_param1) {

    
  const uint32_t kFlags =
      ((expects_response) ? mojo::Message::kFlagExpectsResponse : 0) |
      ((is_sync) ? mojo::Message::kFlagIsSync : 0);

    if (!serialize) {
      return mojo::Message(std::make_unique<ConformanceTestInterfaceProxy_Method4_Message>(
          kFlags
          , std::move(param_param0)
          , std::move(param_param1)
          ));
    }

    DCHECK(serialize);
    
  mojo::Message message(
      internal::kConformanceTestInterface_Method4_Name, kFlags, 0, 0, nullptr);
  auto* buffer = message.payload_buffer();
  ::mojo::test::internal::ConformanceTestInterface_Method4_Params_Data::BufferWriter
      params;
  mojo::internal::SerializationContext serialization_context;
  params.Allocate(buffer);
  typename decltype(params->param0)::BaseType::BufferWriter
      param0_writer;
  mojo::internal::Serialize<::mojo::test::StructCDataView>(
      param_param0, buffer, &param0_writer, &serialization_context);
  params->param0.Set(
      param0_writer.is_null() ? nullptr : param0_writer.data());
  MOJO_INTERNAL_DLOG_SERIALIZATION_WARNING(
      params->param0.is_null(),
      mojo::internal::VALIDATION_ERROR_UNEXPECTED_NULL_POINTER,
      "null param0 in ConformanceTestInterface.Method4 request");
  typename decltype(params->param1)::BaseType::BufferWriter
      param1_writer;
  const mojo::internal::ContainerValidateParams param1_validate_params(
      0, false, nullptr);
  mojo::internal::Serialize<mojo::ArrayDataView<uint8_t>>(
      param_param1, buffer, &param1_writer, &param1_validate_params,
      &serialization_context);
  params->param1.Set(
      param1_writer.is_null() ? nullptr : param1_writer.data());
  MOJO_INTERNAL_DLOG_SERIALIZATION_WARNING(
      params->param1.is_null(),
      mojo::internal::VALIDATION_ERROR_UNEXPECTED_NULL_POINTER,
      "null param1 in ConformanceTestInterface.Method4 request");
  message.AttachHandlesFromSerializationContext(
      &serialization_context);
    return message;
  }


  void Dispatch(ConformanceTestInterface* impl) {
    impl->Method4(
        std::move(param_param0_),
        std::move(param_param1_));
  }

 private:
  // mojo::internal::UnserializedMessageContext:
  void Serialize(mojo::internal::SerializationContext* serialization_context,
                 mojo::internal::Buffer* buffer) override {
    ::mojo::test::internal::ConformanceTestInterface_Method4_Params_Data::BufferWriter
        params;
    params.Allocate(buffer);
  typename decltype(params->param0)::BaseType::BufferWriter
      param0_writer;
  mojo::internal::Serialize<::mojo::test::StructCDataView>(
      param_param0_, buffer, &param0_writer, serialization_context);
  params->param0.Set(
      param0_writer.is_null() ? nullptr : param0_writer.data());
  MOJO_INTERNAL_DLOG_SERIALIZATION_WARNING(
      params->param0.is_null(),
      mojo::internal::VALIDATION_ERROR_UNEXPECTED_NULL_POINTER,
      "null param0 in ConformanceTestInterface.Method4 request");
  typename decltype(params->param1)::BaseType::BufferWriter
      param1_writer;
  const mojo::internal::ContainerValidateParams param1_validate_params(
      0, false, nullptr);
  mojo::internal::Serialize<mojo::ArrayDataView<uint8_t>>(
      param_param1_, buffer, &param1_writer, &param1_validate_params,
      serialization_context);
  params->param1.Set(
      param1_writer.is_null() ? nullptr : param1_writer.data());
  MOJO_INTERNAL_DLOG_SERIALIZATION_WARNING(
      params->param1.is_null(),
      mojo::internal::VALIDATION_ERROR_UNEXPECTED_NULL_POINTER,
      "null param1 in ConformanceTestInterface.Method4 request");
  }
  StructCPtr param_param0_;
  WTF::Vector<uint8_t> param_param1_;

  DISALLOW_COPY_AND_ASSIGN(ConformanceTestInterfaceProxy_Method4_Message);
};

const mojo::internal::UnserializedMessageContext::Tag
ConformanceTestInterfaceProxy_Method4_Message::kMessageTag = {};

void ConformanceTestInterfaceProxy::Method4(
    StructCPtr in_param0, const WTF::Vector<uint8_t>& in_param1) {
#if BUILDFLAG(MOJO_TRACE_ENABLED)
  TRACE_EVENT0("mojom", "<class 'jinja2::utils::Namespace'>::ConformanceTestInterface::Method4");
#endif
  const bool kExpectsResponse = false;
  const bool kIsSync = false;
  const bool kSerialize = receiver_->PrefersSerializedMessages();
  auto message = ConformanceTestInterfaceProxy_Method4_Message::Build(
      kSerialize, kExpectsResponse, kIsSync, std::move(in_param0), std::move(in_param1));

#if defined(ENABLE_IPC_FUZZER)
  message.set_interface_name(ConformanceTestInterface::Name_);
  message.set_method_name("Method4");
#endif
  // This return value may be ignored as false implies the Connector has
  // encountered an error, which will be visible through other means.
  ignore_result(receiver_->Accept(&message));
}
class ConformanceTestInterfaceProxy_Method5_Message
    : public mojo::internal::UnserializedMessageContext {
 public:
  static const mojo::internal::UnserializedMessageContext::Tag kMessageTag;

  explicit ConformanceTestInterfaceProxy_Method5_Message(
      uint32_t message_flags
      , StructEPtr param_param0
      , mojo::ScopedDataPipeProducerHandle param_param1
  )
      : mojo::internal::UnserializedMessageContext(
          &kMessageTag,
          internal::kConformanceTestInterface_Method5_Name,
          message_flags)
      , param_param0_(std::move(param_param0))
      , param_param1_(std::move(param_param1)){}
  ~ConformanceTestInterfaceProxy_Method5_Message() override = default;

  static mojo::Message Build(
      bool serialize,
      bool expects_response,
      bool is_sync,
      StructEPtr param_param0, mojo::ScopedDataPipeProducerHandle param_param1) {

    
  const uint32_t kFlags =
      ((expects_response) ? mojo::Message::kFlagExpectsResponse : 0) |
      ((is_sync) ? mojo::Message::kFlagIsSync : 0);

    if (!serialize) {
      return mojo::Message(std::make_unique<ConformanceTestInterfaceProxy_Method5_Message>(
          kFlags
          , std::move(param_param0)
          , std::move(param_param1)
          ));
    }

    DCHECK(serialize);
    
  mojo::Message message(
      internal::kConformanceTestInterface_Method5_Name, kFlags, 0, 0, nullptr);
  auto* buffer = message.payload_buffer();
  ::mojo::test::internal::ConformanceTestInterface_Method5_Params_Data::BufferWriter
      params;
  mojo::internal::SerializationContext serialization_context;
  params.Allocate(buffer);
  typename decltype(params->param0)::BaseType::BufferWriter
      param0_writer;
  mojo::internal::Serialize<::mojo::test::StructEDataView>(
      param_param0, buffer, &param0_writer, &serialization_context);
  params->param0.Set(
      param0_writer.is_null() ? nullptr : param0_writer.data());
  MOJO_INTERNAL_DLOG_SERIALIZATION_WARNING(
      params->param0.is_null(),
      mojo::internal::VALIDATION_ERROR_UNEXPECTED_NULL_POINTER,
      "null param0 in ConformanceTestInterface.Method5 request");
  mojo::internal::Serialize<mojo::ScopedDataPipeProducerHandle>(
      param_param1, &params->param1, &serialization_context);
  MOJO_INTERNAL_DLOG_SERIALIZATION_WARNING(
      !mojo::internal::IsHandleOrInterfaceValid(params->param1),
      mojo::internal::VALIDATION_ERROR_UNEXPECTED_INVALID_HANDLE,
      "invalid param1 in ConformanceTestInterface.Method5 request");
  message.AttachHandlesFromSerializationContext(
      &serialization_context);
    return message;
  }


  void Dispatch(ConformanceTestInterface* impl) {
    impl->Method5(
        std::move(param_param0_),
        std::move(param_param1_));
  }

 private:
  // mojo::internal::UnserializedMessageContext:
  void Serialize(mojo::internal::SerializationContext* serialization_context,
                 mojo::internal::Buffer* buffer) override {
    ::mojo::test::internal::ConformanceTestInterface_Method5_Params_Data::BufferWriter
        params;
    params.Allocate(buffer);
  typename decltype(params->param0)::BaseType::BufferWriter
      param0_writer;
  mojo::internal::Serialize<::mojo::test::StructEDataView>(
      param_param0_, buffer, &param0_writer, serialization_context);
  params->param0.Set(
      param0_writer.is_null() ? nullptr : param0_writer.data());
  MOJO_INTERNAL_DLOG_SERIALIZATION_WARNING(
      params->param0.is_null(),
      mojo::internal::VALIDATION_ERROR_UNEXPECTED_NULL_POINTER,
      "null param0 in ConformanceTestInterface.Method5 request");
  mojo::internal::Serialize<mojo::ScopedDataPipeProducerHandle>(
      param_param1_, &params->param1, serialization_context);
  MOJO_INTERNAL_DLOG_SERIALIZATION_WARNING(
      !mojo::internal::IsHandleOrInterfaceValid(params->param1),
      mojo::internal::VALIDATION_ERROR_UNEXPECTED_INVALID_HANDLE,
      "invalid param1 in ConformanceTestInterface.Method5 request");
  }
  StructEPtr param_param0_;
  mojo::ScopedDataPipeProducerHandle param_param1_;

  DISALLOW_COPY_AND_ASSIGN(ConformanceTestInterfaceProxy_Method5_Message);
};

const mojo::internal::UnserializedMessageContext::Tag
ConformanceTestInterfaceProxy_Method5_Message::kMessageTag = {};

void ConformanceTestInterfaceProxy::Method5(
    StructEPtr in_param0, mojo::ScopedDataPipeProducerHandle in_param1) {
#if BUILDFLAG(MOJO_TRACE_ENABLED)
  TRACE_EVENT0("mojom", "<class 'jinja2::utils::Namespace'>::ConformanceTestInterface::Method5");
#endif
  const bool kExpectsResponse = false;
  const bool kIsSync = false;
  const bool kSerialize = receiver_->PrefersSerializedMessages();
  auto message = ConformanceTestInterfaceProxy_Method5_Message::Build(
      kSerialize, kExpectsResponse, kIsSync, std::move(in_param0), std::move(in_param1));

#if defined(ENABLE_IPC_FUZZER)
  message.set_interface_name(ConformanceTestInterface::Name_);
  message.set_method_name("Method5");
#endif
  // This return value may be ignored as false implies the Connector has
  // encountered an error, which will be visible through other means.
  ignore_result(receiver_->Accept(&message));
}
class ConformanceTestInterfaceProxy_Method6_Message
    : public mojo::internal::UnserializedMessageContext {
 public:
  static const mojo::internal::UnserializedMessageContext::Tag kMessageTag;

  explicit ConformanceTestInterfaceProxy_Method6_Message(
      uint32_t message_flags
      , const WTF::Vector<WTF::Vector<uint8_t>>& param_param0
  )
      : mojo::internal::UnserializedMessageContext(
          &kMessageTag,
          internal::kConformanceTestInterface_Method6_Name,
          message_flags)
      , param_param0_(std::move(param_param0)){}
  ~ConformanceTestInterfaceProxy_Method6_Message() override = default;

  static mojo::Message Build(
      bool serialize,
      bool expects_response,
      bool is_sync,
      const WTF::Vector<WTF::Vector<uint8_t>>& param_param0) {

    
  const uint32_t kFlags =
      ((expects_response) ? mojo::Message::kFlagExpectsResponse : 0) |
      ((is_sync) ? mojo::Message::kFlagIsSync : 0);

    if (!serialize) {
      return mojo::Message(std::make_unique<ConformanceTestInterfaceProxy_Method6_Message>(
          kFlags
          , std::move(param_param0)
          ));
    }

    DCHECK(serialize);
    
  mojo::Message message(
      internal::kConformanceTestInterface_Method6_Name, kFlags, 0, 0, nullptr);
  auto* buffer = message.payload_buffer();
  ::mojo::test::internal::ConformanceTestInterface_Method6_Params_Data::BufferWriter
      params;
  mojo::internal::SerializationContext serialization_context;
  params.Allocate(buffer);
  typename decltype(params->param0)::BaseType::BufferWriter
      param0_writer;
  const mojo::internal::ContainerValidateParams param0_validate_params(
      0, false, new mojo::internal::ContainerValidateParams(0, false, nullptr));
  mojo::internal::Serialize<mojo::ArrayDataView<mojo::ArrayDataView<uint8_t>>>(
      param_param0, buffer, &param0_writer, &param0_validate_params,
      &serialization_context);
  params->param0.Set(
      param0_writer.is_null() ? nullptr : param0_writer.data());
  MOJO_INTERNAL_DLOG_SERIALIZATION_WARNING(
      params->param0.is_null(),
      mojo::internal::VALIDATION_ERROR_UNEXPECTED_NULL_POINTER,
      "null param0 in ConformanceTestInterface.Method6 request");
  message.AttachHandlesFromSerializationContext(
      &serialization_context);
    return message;
  }


  void Dispatch(ConformanceTestInterface* impl) {
    impl->Method6(
        std::move(param_param0_));
  }

 private:
  // mojo::internal::UnserializedMessageContext:
  void Serialize(mojo::internal::SerializationContext* serialization_context,
                 mojo::internal::Buffer* buffer) override {
    ::mojo::test::internal::ConformanceTestInterface_Method6_Params_Data::BufferWriter
        params;
    params.Allocate(buffer);
  typename decltype(params->param0)::BaseType::BufferWriter
      param0_writer;
  const mojo::internal::ContainerValidateParams param0_validate_params(
      0, false, new mojo::internal::ContainerValidateParams(0, false, nullptr));
  mojo::internal::Serialize<mojo::ArrayDataView<mojo::ArrayDataView<uint8_t>>>(
      param_param0_, buffer, &param0_writer, &param0_validate_params,
      serialization_context);
  params->param0.Set(
      param0_writer.is_null() ? nullptr : param0_writer.data());
  MOJO_INTERNAL_DLOG_SERIALIZATION_WARNING(
      params->param0.is_null(),
      mojo::internal::VALIDATION_ERROR_UNEXPECTED_NULL_POINTER,
      "null param0 in ConformanceTestInterface.Method6 request");
  }
  WTF::Vector<WTF::Vector<uint8_t>> param_param0_;

  DISALLOW_COPY_AND_ASSIGN(ConformanceTestInterfaceProxy_Method6_Message);
};

const mojo::internal::UnserializedMessageContext::Tag
ConformanceTestInterfaceProxy_Method6_Message::kMessageTag = {};

void ConformanceTestInterfaceProxy::Method6(
    const WTF::Vector<WTF::Vector<uint8_t>>& in_param0) {
#if BUILDFLAG(MOJO_TRACE_ENABLED)
  TRACE_EVENT0("mojom", "<class 'jinja2::utils::Namespace'>::ConformanceTestInterface::Method6");
#endif
  const bool kExpectsResponse = false;
  const bool kIsSync = false;
  const bool kSerialize = receiver_->PrefersSerializedMessages();
  auto message = ConformanceTestInterfaceProxy_Method6_Message::Build(
      kSerialize, kExpectsResponse, kIsSync, std::move(in_param0));

#if defined(ENABLE_IPC_FUZZER)
  message.set_interface_name(ConformanceTestInterface::Name_);
  message.set_method_name("Method6");
#endif
  // This return value may be ignored as false implies the Connector has
  // encountered an error, which will be visible through other means.
  ignore_result(receiver_->Accept(&message));
}
class ConformanceTestInterfaceProxy_Method7_Message
    : public mojo::internal::UnserializedMessageContext {
 public:
  static const mojo::internal::UnserializedMessageContext::Tag kMessageTag;

  explicit ConformanceTestInterfaceProxy_Method7_Message(
      uint32_t message_flags
      , StructFPtr param_param0
      , const WTF::Vector<base::Optional<WTF::Vector<uint8_t>>>& param_param1
  )
      : mojo::internal::UnserializedMessageContext(
          &kMessageTag,
          internal::kConformanceTestInterface_Method7_Name,
          message_flags)
      , param_param0_(std::move(param_param0))
      , param_param1_(std::move(param_param1)){}
  ~ConformanceTestInterfaceProxy_Method7_Message() override = default;

  static mojo::Message Build(
      bool serialize,
      bool expects_response,
      bool is_sync,
      StructFPtr param_param0, const WTF::Vector<base::Optional<WTF::Vector<uint8_t>>>& param_param1) {

    
  const uint32_t kFlags =
      ((expects_response) ? mojo::Message::kFlagExpectsResponse : 0) |
      ((is_sync) ? mojo::Message::kFlagIsSync : 0);

    if (!serialize) {
      return mojo::Message(std::make_unique<ConformanceTestInterfaceProxy_Method7_Message>(
          kFlags
          , std::move(param_param0)
          , std::move(param_param1)
          ));
    }

    DCHECK(serialize);
    
  mojo::Message message(
      internal::kConformanceTestInterface_Method7_Name, kFlags, 0, 0, nullptr);
  auto* buffer = message.payload_buffer();
  ::mojo::test::internal::ConformanceTestInterface_Method7_Params_Data::BufferWriter
      params;
  mojo::internal::SerializationContext serialization_context;
  params.Allocate(buffer);
  typename decltype(params->param0)::BaseType::BufferWriter
      param0_writer;
  mojo::internal::Serialize<::mojo::test::StructFDataView>(
      param_param0, buffer, &param0_writer, &serialization_context);
  params->param0.Set(
      param0_writer.is_null() ? nullptr : param0_writer.data());
  MOJO_INTERNAL_DLOG_SERIALIZATION_WARNING(
      params->param0.is_null(),
      mojo::internal::VALIDATION_ERROR_UNEXPECTED_NULL_POINTER,
      "null param0 in ConformanceTestInterface.Method7 request");
  typename decltype(params->param1)::BaseType::BufferWriter
      param1_writer;
  const mojo::internal::ContainerValidateParams param1_validate_params(
      2, true, new mojo::internal::ContainerValidateParams(3, false, nullptr));
  mojo::internal::Serialize<mojo::ArrayDataView<mojo::ArrayDataView<uint8_t>>>(
      param_param1, buffer, &param1_writer, &param1_validate_params,
      &serialization_context);
  params->param1.Set(
      param1_writer.is_null() ? nullptr : param1_writer.data());
  MOJO_INTERNAL_DLOG_SERIALIZATION_WARNING(
      params->param1.is_null(),
      mojo::internal::VALIDATION_ERROR_UNEXPECTED_NULL_POINTER,
      "null param1 in ConformanceTestInterface.Method7 request");
  message.AttachHandlesFromSerializationContext(
      &serialization_context);
    return message;
  }


  void Dispatch(ConformanceTestInterface* impl) {
    impl->Method7(
        std::move(param_param0_),
        std::move(param_param1_));
  }

 private:
  // mojo::internal::UnserializedMessageContext:
  void Serialize(mojo::internal::SerializationContext* serialization_context,
                 mojo::internal::Buffer* buffer) override {
    ::mojo::test::internal::ConformanceTestInterface_Method7_Params_Data::BufferWriter
        params;
    params.Allocate(buffer);
  typename decltype(params->param0)::BaseType::BufferWriter
      param0_writer;
  mojo::internal::Serialize<::mojo::test::StructFDataView>(
      param_param0_, buffer, &param0_writer, serialization_context);
  params->param0.Set(
      param0_writer.is_null() ? nullptr : param0_writer.data());
  MOJO_INTERNAL_DLOG_SERIALIZATION_WARNING(
      params->param0.is_null(),
      mojo::internal::VALIDATION_ERROR_UNEXPECTED_NULL_POINTER,
      "null param0 in ConformanceTestInterface.Method7 request");
  typename decltype(params->param1)::BaseType::BufferWriter
      param1_writer;
  const mojo::internal::ContainerValidateParams param1_validate_params(
      2, true, new mojo::internal::ContainerValidateParams(3, false, nullptr));
  mojo::internal::Serialize<mojo::ArrayDataView<mojo::ArrayDataView<uint8_t>>>(
      param_param1_, buffer, &param1_writer, &param1_validate_params,
      serialization_context);
  params->param1.Set(
      param1_writer.is_null() ? nullptr : param1_writer.data());
  MOJO_INTERNAL_DLOG_SERIALIZATION_WARNING(
      params->param1.is_null(),
      mojo::internal::VALIDATION_ERROR_UNEXPECTED_NULL_POINTER,
      "null param1 in ConformanceTestInterface.Method7 request");
  }
  StructFPtr param_param0_;
  WTF::Vector<base::Optional<WTF::Vector<uint8_t>>> param_param1_;

  DISALLOW_COPY_AND_ASSIGN(ConformanceTestInterfaceProxy_Method7_Message);
};

const mojo::internal::UnserializedMessageContext::Tag
ConformanceTestInterfaceProxy_Method7_Message::kMessageTag = {};

void ConformanceTestInterfaceProxy::Method7(
    StructFPtr in_param0, const WTF::Vector<base::Optional<WTF::Vector<uint8_t>>>& in_param1) {
#if BUILDFLAG(MOJO_TRACE_ENABLED)
  TRACE_EVENT0("mojom", "<class 'jinja2::utils::Namespace'>::ConformanceTestInterface::Method7");
#endif
  const bool kExpectsResponse = false;
  const bool kIsSync = false;
  const bool kSerialize = receiver_->PrefersSerializedMessages();
  auto message = ConformanceTestInterfaceProxy_Method7_Message::Build(
      kSerialize, kExpectsResponse, kIsSync, std::move(in_param0), std::move(in_param1));

#if defined(ENABLE_IPC_FUZZER)
  message.set_interface_name(ConformanceTestInterface::Name_);
  message.set_method_name("Method7");
#endif
  // This return value may be ignored as false implies the Connector has
  // encountered an error, which will be visible through other means.
  ignore_result(receiver_->Accept(&message));
}
class ConformanceTestInterfaceProxy_Method8_Message
    : public mojo::internal::UnserializedMessageContext {
 public:
  static const mojo::internal::UnserializedMessageContext::Tag kMessageTag;

  explicit ConformanceTestInterfaceProxy_Method8_Message(
      uint32_t message_flags
      , const WTF::Vector<base::Optional<WTF::Vector<WTF::String>>>& param_param0
  )
      : mojo::internal::UnserializedMessageContext(
          &kMessageTag,
          internal::kConformanceTestInterface_Method8_Name,
          message_flags)
      , param_param0_(std::move(param_param0)){}
  ~ConformanceTestInterfaceProxy_Method8_Message() override = default;

  static mojo::Message Build(
      bool serialize,
      bool expects_response,
      bool is_sync,
      const WTF::Vector<base::Optional<WTF::Vector<WTF::String>>>& param_param0) {

    
  const uint32_t kFlags =
      ((expects_response) ? mojo::Message::kFlagExpectsResponse : 0) |
      ((is_sync) ? mojo::Message::kFlagIsSync : 0);

    if (!serialize) {
      return mojo::Message(std::make_unique<ConformanceTestInterfaceProxy_Method8_Message>(
          kFlags
          , std::move(param_param0)
          ));
    }

    DCHECK(serialize);
    
  mojo::Message message(
      internal::kConformanceTestInterface_Method8_Name, kFlags, 0, 0, nullptr);
  auto* buffer = message.payload_buffer();
  ::mojo::test::internal::ConformanceTestInterface_Method8_Params_Data::BufferWriter
      params;
  mojo::internal::SerializationContext serialization_context;
  params.Allocate(buffer);
  typename decltype(params->param0)::BaseType::BufferWriter
      param0_writer;
  const mojo::internal::ContainerValidateParams param0_validate_params(
      0, true, new mojo::internal::ContainerValidateParams(0, false, new mojo::internal::ContainerValidateParams(0, false, nullptr)));
  mojo::internal::Serialize<mojo::ArrayDataView<mojo::ArrayDataView<mojo::StringDataView>>>(
      param_param0, buffer, &param0_writer, &param0_validate_params,
      &serialization_context);
  params->param0.Set(
      param0_writer.is_null() ? nullptr : param0_writer.data());
  MOJO_INTERNAL_DLOG_SERIALIZATION_WARNING(
      params->param0.is_null(),
      mojo::internal::VALIDATION_ERROR_UNEXPECTED_NULL_POINTER,
      "null param0 in ConformanceTestInterface.Method8 request");
  message.AttachHandlesFromSerializationContext(
      &serialization_context);
    return message;
  }


  void Dispatch(ConformanceTestInterface* impl) {
    impl->Method8(
        std::move(param_param0_));
  }

 private:
  // mojo::internal::UnserializedMessageContext:
  void Serialize(mojo::internal::SerializationContext* serialization_context,
                 mojo::internal::Buffer* buffer) override {
    ::mojo::test::internal::ConformanceTestInterface_Method8_Params_Data::BufferWriter
        params;
    params.Allocate(buffer);
  typename decltype(params->param0)::BaseType::BufferWriter
      param0_writer;
  const mojo::internal::ContainerValidateParams param0_validate_params(
      0, true, new mojo::internal::ContainerValidateParams(0, false, new mojo::internal::ContainerValidateParams(0, false, nullptr)));
  mojo::internal::Serialize<mojo::ArrayDataView<mojo::ArrayDataView<mojo::StringDataView>>>(
      param_param0_, buffer, &param0_writer, &param0_validate_params,
      serialization_context);
  params->param0.Set(
      param0_writer.is_null() ? nullptr : param0_writer.data());
  MOJO_INTERNAL_DLOG_SERIALIZATION_WARNING(
      params->param0.is_null(),
      mojo::internal::VALIDATION_ERROR_UNEXPECTED_NULL_POINTER,
      "null param0 in ConformanceTestInterface.Method8 request");
  }
  WTF::Vector<base::Optional<WTF::Vector<WTF::String>>> param_param0_;

  DISALLOW_COPY_AND_ASSIGN(ConformanceTestInterfaceProxy_Method8_Message);
};

const mojo::internal::UnserializedMessageContext::Tag
ConformanceTestInterfaceProxy_Method8_Message::kMessageTag = {};

void ConformanceTestInterfaceProxy::Method8(
    const WTF::Vector<base::Optional<WTF::Vector<WTF::String>>>& in_param0) {
#if BUILDFLAG(MOJO_TRACE_ENABLED)
  TRACE_EVENT0("mojom", "<class 'jinja2::utils::Namespace'>::ConformanceTestInterface::Method8");
#endif
  const bool kExpectsResponse = false;
  const bool kIsSync = false;
  const bool kSerialize = receiver_->PrefersSerializedMessages();
  auto message = ConformanceTestInterfaceProxy_Method8_Message::Build(
      kSerialize, kExpectsResponse, kIsSync, std::move(in_param0));

#if defined(ENABLE_IPC_FUZZER)
  message.set_interface_name(ConformanceTestInterface::Name_);
  message.set_method_name("Method8");
#endif
  // This return value may be ignored as false implies the Connector has
  // encountered an error, which will be visible through other means.
  ignore_result(receiver_->Accept(&message));
}
class ConformanceTestInterfaceProxy_Method9_Message
    : public mojo::internal::UnserializedMessageContext {
 public:
  static const mojo::internal::UnserializedMessageContext::Tag kMessageTag;

  explicit ConformanceTestInterfaceProxy_Method9_Message(
      uint32_t message_flags
      , base::Optional<WTF::Vector<WTF::Vector<mojo::ScopedHandle>>> param_param0
  )
      : mojo::internal::UnserializedMessageContext(
          &kMessageTag,
          internal::kConformanceTestInterface_Method9_Name,
          message_flags)
      , param_param0_(std::move(param_param0)){}
  ~ConformanceTestInterfaceProxy_Method9_Message() override = default;

  static mojo::Message Build(
      bool serialize,
      bool expects_response,
      bool is_sync,
      base::Optional<WTF::Vector<WTF::Vector<mojo::ScopedHandle>>> param_param0) {

    
  const uint32_t kFlags =
      ((expects_response) ? mojo::Message::kFlagExpectsResponse : 0) |
      ((is_sync) ? mojo::Message::kFlagIsSync : 0);

    if (!serialize) {
      return mojo::Message(std::make_unique<ConformanceTestInterfaceProxy_Method9_Message>(
          kFlags
          , std::move(param_param0)
          ));
    }

    DCHECK(serialize);
    
  mojo::Message message(
      internal::kConformanceTestInterface_Method9_Name, kFlags, 0, 0, nullptr);
  auto* buffer = message.payload_buffer();
  ::mojo::test::internal::ConformanceTestInterface_Method9_Params_Data::BufferWriter
      params;
  mojo::internal::SerializationContext serialization_context;
  params.Allocate(buffer);
  typename decltype(params->param0)::BaseType::BufferWriter
      param0_writer;
  const mojo::internal::ContainerValidateParams param0_validate_params(
      0, false, new mojo::internal::ContainerValidateParams(0, true, nullptr));
  mojo::internal::Serialize<mojo::ArrayDataView<mojo::ArrayDataView<mojo::ScopedHandle>>>(
      param_param0, buffer, &param0_writer, &param0_validate_params,
      &serialization_context);
  params->param0.Set(
      param0_writer.is_null() ? nullptr : param0_writer.data());
  message.AttachHandlesFromSerializationContext(
      &serialization_context);
    return message;
  }


  void Dispatch(ConformanceTestInterface* impl) {
    impl->Method9(
        std::move(param_param0_));
  }

 private:
  // mojo::internal::UnserializedMessageContext:
  void Serialize(mojo::internal::SerializationContext* serialization_context,
                 mojo::internal::Buffer* buffer) override {
    ::mojo::test::internal::ConformanceTestInterface_Method9_Params_Data::BufferWriter
        params;
    params.Allocate(buffer);
  typename decltype(params->param0)::BaseType::BufferWriter
      param0_writer;
  const mojo::internal::ContainerValidateParams param0_validate_params(
      0, false, new mojo::internal::ContainerValidateParams(0, true, nullptr));
  mojo::internal::Serialize<mojo::ArrayDataView<mojo::ArrayDataView<mojo::ScopedHandle>>>(
      param_param0_, buffer, &param0_writer, &param0_validate_params,
      serialization_context);
  params->param0.Set(
      param0_writer.is_null() ? nullptr : param0_writer.data());
  }
  base::Optional<WTF::Vector<WTF::Vector<mojo::ScopedHandle>>> param_param0_;

  DISALLOW_COPY_AND_ASSIGN(ConformanceTestInterfaceProxy_Method9_Message);
};

const mojo::internal::UnserializedMessageContext::Tag
ConformanceTestInterfaceProxy_Method9_Message::kMessageTag = {};

void ConformanceTestInterfaceProxy::Method9(
    base::Optional<WTF::Vector<WTF::Vector<mojo::ScopedHandle>>> in_param0) {
#if BUILDFLAG(MOJO_TRACE_ENABLED)
  TRACE_EVENT0("mojom", "<class 'jinja2::utils::Namespace'>::ConformanceTestInterface::Method9");
#endif
  const bool kExpectsResponse = false;
  const bool kIsSync = false;
  const bool kSerialize = receiver_->PrefersSerializedMessages();
  auto message = ConformanceTestInterfaceProxy_Method9_Message::Build(
      kSerialize, kExpectsResponse, kIsSync, std::move(in_param0));

#if defined(ENABLE_IPC_FUZZER)
  message.set_interface_name(ConformanceTestInterface::Name_);
  message.set_method_name("Method9");
#endif
  // This return value may be ignored as false implies the Connector has
  // encountered an error, which will be visible through other means.
  ignore_result(receiver_->Accept(&message));
}
class ConformanceTestInterfaceProxy_Method10_Message
    : public mojo::internal::UnserializedMessageContext {
 public:
  static const mojo::internal::UnserializedMessageContext::Tag kMessageTag;

  explicit ConformanceTestInterfaceProxy_Method10_Message(
      uint32_t message_flags
      , const WTF::HashMap<WTF::String, uint8_t>& param_param0
  )
      : mojo::internal::UnserializedMessageContext(
          &kMessageTag,
          internal::kConformanceTestInterface_Method10_Name,
          message_flags)
      , param_param0_(std::move(param_param0)){}
  ~ConformanceTestInterfaceProxy_Method10_Message() override = default;

  static mojo::Message Build(
      bool serialize,
      bool expects_response,
      bool is_sync,
      const WTF::HashMap<WTF::String, uint8_t>& param_param0) {

    
  const uint32_t kFlags =
      ((expects_response) ? mojo::Message::kFlagExpectsResponse : 0) |
      ((is_sync) ? mojo::Message::kFlagIsSync : 0);

    if (!serialize) {
      return mojo::Message(std::make_unique<ConformanceTestInterfaceProxy_Method10_Message>(
          kFlags
          , std::move(param_param0)
          ));
    }

    DCHECK(serialize);
    
  mojo::Message message(
      internal::kConformanceTestInterface_Method10_Name, kFlags, 0, 0, nullptr);
  auto* buffer = message.payload_buffer();
  ::mojo::test::internal::ConformanceTestInterface_Method10_Params_Data::BufferWriter
      params;
  mojo::internal::SerializationContext serialization_context;
  params.Allocate(buffer);
  typename decltype(params->param0)::BaseType::BufferWriter
      param0_writer;
  const mojo::internal::ContainerValidateParams param0_validate_params(
      new mojo::internal::ContainerValidateParams(0, false, new mojo::internal::ContainerValidateParams(0, false, nullptr)), new mojo::internal::ContainerValidateParams(0, false, nullptr));
  mojo::internal::Serialize<mojo::MapDataView<mojo::StringDataView, uint8_t>>(
      param_param0, buffer, &param0_writer, &param0_validate_params,
      &serialization_context);
  params->param0.Set(
      param0_writer.is_null() ? nullptr : param0_writer.data());
  MOJO_INTERNAL_DLOG_SERIALIZATION_WARNING(
      params->param0.is_null(),
      mojo::internal::VALIDATION_ERROR_UNEXPECTED_NULL_POINTER,
      "null param0 in ConformanceTestInterface.Method10 request");
  message.AttachHandlesFromSerializationContext(
      &serialization_context);
    return message;
  }


  void Dispatch(ConformanceTestInterface* impl) {
    impl->Method10(
        std::move(param_param0_));
  }

 private:
  // mojo::internal::UnserializedMessageContext:
  void Serialize(mojo::internal::SerializationContext* serialization_context,
                 mojo::internal::Buffer* buffer) override {
    ::mojo::test::internal::ConformanceTestInterface_Method10_Params_Data::BufferWriter
        params;
    params.Allocate(buffer);
  typename decltype(params->param0)::BaseType::BufferWriter
      param0_writer;
  const mojo::internal::ContainerValidateParams param0_validate_params(
      new mojo::internal::ContainerValidateParams(0, false, new mojo::internal::ContainerValidateParams(0, false, nullptr)), new mojo::internal::ContainerValidateParams(0, false, nullptr));
  mojo::internal::Serialize<mojo::MapDataView<mojo::StringDataView, uint8_t>>(
      param_param0_, buffer, &param0_writer, &param0_validate_params,
      serialization_context);
  params->param0.Set(
      param0_writer.is_null() ? nullptr : param0_writer.data());
  MOJO_INTERNAL_DLOG_SERIALIZATION_WARNING(
      params->param0.is_null(),
      mojo::internal::VALIDATION_ERROR_UNEXPECTED_NULL_POINTER,
      "null param0 in ConformanceTestInterface.Method10 request");
  }
  WTF::HashMap<WTF::String, uint8_t> param_param0_;

  DISALLOW_COPY_AND_ASSIGN(ConformanceTestInterfaceProxy_Method10_Message);
};

const mojo::internal::UnserializedMessageContext::Tag
ConformanceTestInterfaceProxy_Method10_Message::kMessageTag = {};

void ConformanceTestInterfaceProxy::Method10(
    const WTF::HashMap<WTF::String, uint8_t>& in_param0) {
#if BUILDFLAG(MOJO_TRACE_ENABLED)
  TRACE_EVENT0("mojom", "<class 'jinja2::utils::Namespace'>::ConformanceTestInterface::Method10");
#endif
  const bool kExpectsResponse = false;
  const bool kIsSync = false;
  const bool kSerialize = receiver_->PrefersSerializedMessages();
  auto message = ConformanceTestInterfaceProxy_Method10_Message::Build(
      kSerialize, kExpectsResponse, kIsSync, std::move(in_param0));

#if defined(ENABLE_IPC_FUZZER)
  message.set_interface_name(ConformanceTestInterface::Name_);
  message.set_method_name("Method10");
#endif
  // This return value may be ignored as false implies the Connector has
  // encountered an error, which will be visible through other means.
  ignore_result(receiver_->Accept(&message));
}
class ConformanceTestInterfaceProxy_Method11_Message
    : public mojo::internal::UnserializedMessageContext {
 public:
  static const mojo::internal::UnserializedMessageContext::Tag kMessageTag;

  explicit ConformanceTestInterfaceProxy_Method11_Message(
      uint32_t message_flags
      , StructGPtr param_param0
  )
      : mojo::internal::UnserializedMessageContext(
          &kMessageTag,
          internal::kConformanceTestInterface_Method11_Name,
          message_flags)
      , param_param0_(std::move(param_param0)){}
  ~ConformanceTestInterfaceProxy_Method11_Message() override = default;

  static mojo::Message Build(
      bool serialize,
      bool expects_response,
      bool is_sync,
      StructGPtr param_param0) {

    
  const uint32_t kFlags =
      ((expects_response) ? mojo::Message::kFlagExpectsResponse : 0) |
      ((is_sync) ? mojo::Message::kFlagIsSync : 0);

    if (!serialize) {
      return mojo::Message(std::make_unique<ConformanceTestInterfaceProxy_Method11_Message>(
          kFlags
          , std::move(param_param0)
          ));
    }

    DCHECK(serialize);
    
  mojo::Message message(
      internal::kConformanceTestInterface_Method11_Name, kFlags, 0, 0, nullptr);
  auto* buffer = message.payload_buffer();
  ::mojo::test::internal::ConformanceTestInterface_Method11_Params_Data::BufferWriter
      params;
  mojo::internal::SerializationContext serialization_context;
  params.Allocate(buffer);
  typename decltype(params->param0)::BaseType::BufferWriter
      param0_writer;
  mojo::internal::Serialize<::mojo::test::StructGDataView>(
      param_param0, buffer, &param0_writer, &serialization_context);
  params->param0.Set(
      param0_writer.is_null() ? nullptr : param0_writer.data());
  MOJO_INTERNAL_DLOG_SERIALIZATION_WARNING(
      params->param0.is_null(),
      mojo::internal::VALIDATION_ERROR_UNEXPECTED_NULL_POINTER,
      "null param0 in ConformanceTestInterface.Method11 request");
  message.AttachHandlesFromSerializationContext(
      &serialization_context);
    return message;
  }


  void Dispatch(ConformanceTestInterface* impl) {
    impl->Method11(
        std::move(param_param0_));
  }

 private:
  // mojo::internal::UnserializedMessageContext:
  void Serialize(mojo::internal::SerializationContext* serialization_context,
                 mojo::internal::Buffer* buffer) override {
    ::mojo::test::internal::ConformanceTestInterface_Method11_Params_Data::BufferWriter
        params;
    params.Allocate(buffer);
  typename decltype(params->param0)::BaseType::BufferWriter
      param0_writer;
  mojo::internal::Serialize<::mojo::test::StructGDataView>(
      param_param0_, buffer, &param0_writer, serialization_context);
  params->param0.Set(
      param0_writer.is_null() ? nullptr : param0_writer.data());
  MOJO_INTERNAL_DLOG_SERIALIZATION_WARNING(
      params->param0.is_null(),
      mojo::internal::VALIDATION_ERROR_UNEXPECTED_NULL_POINTER,
      "null param0 in ConformanceTestInterface.Method11 request");
  }
  StructGPtr param_param0_;

  DISALLOW_COPY_AND_ASSIGN(ConformanceTestInterfaceProxy_Method11_Message);
};

const mojo::internal::UnserializedMessageContext::Tag
ConformanceTestInterfaceProxy_Method11_Message::kMessageTag = {};

void ConformanceTestInterfaceProxy::Method11(
    StructGPtr in_param0) {
#if BUILDFLAG(MOJO_TRACE_ENABLED)
  TRACE_EVENT0("mojom", "<class 'jinja2::utils::Namespace'>::ConformanceTestInterface::Method11");
#endif
  const bool kExpectsResponse = false;
  const bool kIsSync = false;
  const bool kSerialize = receiver_->PrefersSerializedMessages();
  auto message = ConformanceTestInterfaceProxy_Method11_Message::Build(
      kSerialize, kExpectsResponse, kIsSync, std::move(in_param0));

#if defined(ENABLE_IPC_FUZZER)
  message.set_interface_name(ConformanceTestInterface::Name_);
  message.set_method_name("Method11");
#endif
  // This return value may be ignored as false implies the Connector has
  // encountered an error, which will be visible through other means.
  ignore_result(receiver_->Accept(&message));
}
class ConformanceTestInterfaceProxy_Method12_Message
    : public mojo::internal::UnserializedMessageContext {
 public:
  static const mojo::internal::UnserializedMessageContext::Tag kMessageTag;

  explicit ConformanceTestInterfaceProxy_Method12_Message(
      uint32_t message_flags
      , float param_param0
  )
      : mojo::internal::UnserializedMessageContext(
          &kMessageTag,
          internal::kConformanceTestInterface_Method12_Name,
          message_flags)
      , param_param0_(std::move(param_param0)){}
  ~ConformanceTestInterfaceProxy_Method12_Message() override = default;

  static mojo::Message Build(
      bool serialize,
      bool expects_response,
      bool is_sync,
      float param_param0) {

    
  const uint32_t kFlags =
      ((expects_response) ? mojo::Message::kFlagExpectsResponse : 0) |
      ((is_sync) ? mojo::Message::kFlagIsSync : 0);

    if (!serialize) {
      return mojo::Message(std::make_unique<ConformanceTestInterfaceProxy_Method12_Message>(
          kFlags
          , std::move(param_param0)
          ));
    }

    DCHECK(serialize);
    
  mojo::Message message(
      internal::kConformanceTestInterface_Method12_Name, kFlags, 0, 0, nullptr);
  auto* buffer = message.payload_buffer();
  ::mojo::test::internal::ConformanceTestInterface_Method12_Params_Data::BufferWriter
      params;
  mojo::internal::SerializationContext serialization_context;
  params.Allocate(buffer);
  params->param0 = param_param0;
  message.AttachHandlesFromSerializationContext(
      &serialization_context);
    return message;
  }


  void Dispatch(ConformanceTestInterface* impl, ConformanceTestInterface::Method12Callback callback) {
    impl->Method12(
        std::move(param_param0_), std::move(callback));
  }

 private:
  // mojo::internal::UnserializedMessageContext:
  void Serialize(mojo::internal::SerializationContext* serialization_context,
                 mojo::internal::Buffer* buffer) override {
    ::mojo::test::internal::ConformanceTestInterface_Method12_Params_Data::BufferWriter
        params;
    params.Allocate(buffer);
  params->param0 = param_param0_;
  }
  float param_param0_;

  DISALLOW_COPY_AND_ASSIGN(ConformanceTestInterfaceProxy_Method12_Message);
};

const mojo::internal::UnserializedMessageContext::Tag
ConformanceTestInterfaceProxy_Method12_Message::kMessageTag = {};

void ConformanceTestInterfaceProxy::Method12(
    float in_param0, Method12Callback callback) {
#if BUILDFLAG(MOJO_TRACE_ENABLED)
  TRACE_EVENT0("mojom", "<class 'jinja2::utils::Namespace'>::ConformanceTestInterface::Method12");
#endif
  const bool kExpectsResponse = true;
  const bool kIsSync = false;
  const bool kSerialize = receiver_->PrefersSerializedMessages();
  auto message = ConformanceTestInterfaceProxy_Method12_Message::Build(
      kSerialize, kExpectsResponse, kIsSync, std::move(in_param0));

#if defined(ENABLE_IPC_FUZZER)
  message.set_interface_name(ConformanceTestInterface::Name_);
  message.set_method_name("Method12");
#endif
  std::unique_ptr<mojo::MessageReceiver> responder(
      new ConformanceTestInterface_Method12_ForwardToCallback(
          std::move(callback)));
  ignore_result(receiver_->AcceptWithResponder(&message, std::move(responder)));
}
class ConformanceTestInterfaceProxy_Method13_Message
    : public mojo::internal::UnserializedMessageContext {
 public:
  static const mojo::internal::UnserializedMessageContext::Tag kMessageTag;

  explicit ConformanceTestInterfaceProxy_Method13_Message(
      uint32_t message_flags
      , InterfaceAPtr param_param0
      , uint32_t param_param1
      , InterfaceAPtr param_param2
  )
      : mojo::internal::UnserializedMessageContext(
          &kMessageTag,
          internal::kConformanceTestInterface_Method13_Name,
          message_flags)
      , param_param0_(param_param0.PassInterface())
      , param_param1_(std::move(param_param1))
      , param_param2_(param_param2.PassInterface()){}
  ~ConformanceTestInterfaceProxy_Method13_Message() override = default;

  static mojo::Message Build(
      bool serialize,
      bool expects_response,
      bool is_sync,
      InterfaceAPtr param_param0, uint32_t param_param1, InterfaceAPtr param_param2) {

    
  const uint32_t kFlags =
      ((expects_response) ? mojo::Message::kFlagExpectsResponse : 0) |
      ((is_sync) ? mojo::Message::kFlagIsSync : 0);

    if (!serialize) {
      return mojo::Message(std::make_unique<ConformanceTestInterfaceProxy_Method13_Message>(
          kFlags
          , std::move(param_param0)
          , std::move(param_param1)
          , std::move(param_param2)
          ));
    }

    DCHECK(serialize);
    
  mojo::Message message(
      internal::kConformanceTestInterface_Method13_Name, kFlags, 0, 0, nullptr);
  auto* buffer = message.payload_buffer();
  ::mojo::test::internal::ConformanceTestInterface_Method13_Params_Data::BufferWriter
      params;
  mojo::internal::SerializationContext serialization_context;
  params.Allocate(buffer);
  mojo::internal::Serialize<::mojo::test::InterfaceAPtrDataView>(
      param_param0, &params->param0, &serialization_context);
  params->param1 = param_param1;
  mojo::internal::Serialize<::mojo::test::InterfaceAPtrDataView>(
      param_param2, &params->param2, &serialization_context);
  message.AttachHandlesFromSerializationContext(
      &serialization_context);
    return message;
  }


  void Dispatch(ConformanceTestInterface* impl) {
    impl->Method13(
        InterfaceAPtr(std::move(param_param0_)),
        std::move(param_param1_),
        InterfaceAPtr(std::move(param_param2_)));
  }

 private:
  // mojo::internal::UnserializedMessageContext:
  void Serialize(mojo::internal::SerializationContext* serialization_context,
                 mojo::internal::Buffer* buffer) override {
    ::mojo::test::internal::ConformanceTestInterface_Method13_Params_Data::BufferWriter
        params;
    params.Allocate(buffer);
  mojo::internal::Serialize<::mojo::test::InterfaceAPtrDataView>(
      param_param0_, &params->param0, serialization_context);
  params->param1 = param_param1_;
  mojo::internal::Serialize<::mojo::test::InterfaceAPtrDataView>(
      param_param2_, &params->param2, serialization_context);
  }
  InterfaceAPtrInfo param_param0_;
  uint32_t param_param1_;
  InterfaceAPtrInfo param_param2_;

  DISALLOW_COPY_AND_ASSIGN(ConformanceTestInterfaceProxy_Method13_Message);
};

const mojo::internal::UnserializedMessageContext::Tag
ConformanceTestInterfaceProxy_Method13_Message::kMessageTag = {};

void ConformanceTestInterfaceProxy::Method13(
    InterfaceAPtr in_param0, uint32_t in_param1, InterfaceAPtr in_param2) {
#if BUILDFLAG(MOJO_TRACE_ENABLED)
  TRACE_EVENT0("mojom", "<class 'jinja2::utils::Namespace'>::ConformanceTestInterface::Method13");
#endif
  const bool kExpectsResponse = false;
  const bool kIsSync = false;
  const bool kSerialize = receiver_->PrefersSerializedMessages();
  auto message = ConformanceTestInterfaceProxy_Method13_Message::Build(
      kSerialize, kExpectsResponse, kIsSync, std::move(in_param0), std::move(in_param1), std::move(in_param2));

#if defined(ENABLE_IPC_FUZZER)
  message.set_interface_name(ConformanceTestInterface::Name_);
  message.set_method_name("Method13");
#endif
  // This return value may be ignored as false implies the Connector has
  // encountered an error, which will be visible through other means.
  ignore_result(receiver_->Accept(&message));
}
class ConformanceTestInterfaceProxy_Method14_Message
    : public mojo::internal::UnserializedMessageContext {
 public:
  static const mojo::internal::UnserializedMessageContext::Tag kMessageTag;

  explicit ConformanceTestInterfaceProxy_Method14_Message(
      uint32_t message_flags
      , EnumA param_param0
      , EnumB param_param1
  )
      : mojo::internal::UnserializedMessageContext(
          &kMessageTag,
          internal::kConformanceTestInterface_Method14_Name,
          message_flags)
      , param_param0_(std::move(param_param0))
      , param_param1_(std::move(param_param1)){}
  ~ConformanceTestInterfaceProxy_Method14_Message() override = default;

  static mojo::Message Build(
      bool serialize,
      bool expects_response,
      bool is_sync,
      EnumA param_param0, EnumB param_param1) {

    
  const uint32_t kFlags =
      ((expects_response) ? mojo::Message::kFlagExpectsResponse : 0) |
      ((is_sync) ? mojo::Message::kFlagIsSync : 0);

    if (!serialize) {
      return mojo::Message(std::make_unique<ConformanceTestInterfaceProxy_Method14_Message>(
          kFlags
          , std::move(param_param0)
          , std::move(param_param1)
          ));
    }

    DCHECK(serialize);
    
  mojo::Message message(
      internal::kConformanceTestInterface_Method14_Name, kFlags, 0, 0, nullptr);
  auto* buffer = message.payload_buffer();
  ::mojo::test::internal::ConformanceTestInterface_Method14_Params_Data::BufferWriter
      params;
  mojo::internal::SerializationContext serialization_context;
  params.Allocate(buffer);
  mojo::internal::Serialize<::mojo::test::EnumA>(
      param_param0, &params->param0);
  mojo::internal::Serialize<::mojo::test::EnumB>(
      param_param1, &params->param1);
  message.AttachHandlesFromSerializationContext(
      &serialization_context);
    return message;
  }


  void Dispatch(ConformanceTestInterface* impl) {
    impl->Method14(
        std::move(param_param0_),
        std::move(param_param1_));
  }

 private:
  // mojo::internal::UnserializedMessageContext:
  void Serialize(mojo::internal::SerializationContext* serialization_context,
                 mojo::internal::Buffer* buffer) override {
    ::mojo::test::internal::ConformanceTestInterface_Method14_Params_Data::BufferWriter
        params;
    params.Allocate(buffer);
  mojo::internal::Serialize<::mojo::test::EnumA>(
      param_param0_, &params->param0);
  mojo::internal::Serialize<::mojo::test::EnumB>(
      param_param1_, &params->param1);
  }
  EnumA param_param0_;
  EnumB param_param1_;

  DISALLOW_COPY_AND_ASSIGN(ConformanceTestInterfaceProxy_Method14_Message);
};

const mojo::internal::UnserializedMessageContext::Tag
ConformanceTestInterfaceProxy_Method14_Message::kMessageTag = {};

void ConformanceTestInterfaceProxy::Method14(
    EnumA in_param0, EnumB in_param1) {
#if BUILDFLAG(MOJO_TRACE_ENABLED)
  TRACE_EVENT0("mojom", "<class 'jinja2::utils::Namespace'>::ConformanceTestInterface::Method14");
#endif
  const bool kExpectsResponse = false;
  const bool kIsSync = false;
  const bool kSerialize = receiver_->PrefersSerializedMessages();
  auto message = ConformanceTestInterfaceProxy_Method14_Message::Build(
      kSerialize, kExpectsResponse, kIsSync, std::move(in_param0), std::move(in_param1));

#if defined(ENABLE_IPC_FUZZER)
  message.set_interface_name(ConformanceTestInterface::Name_);
  message.set_method_name("Method14");
#endif
  // This return value may be ignored as false implies the Connector has
  // encountered an error, which will be visible through other means.
  ignore_result(receiver_->Accept(&message));
}
class ConformanceTestInterfaceProxy_Method15_Message
    : public mojo::internal::UnserializedMessageContext {
 public:
  static const mojo::internal::UnserializedMessageContext::Tag kMessageTag;

  explicit ConformanceTestInterfaceProxy_Method15_Message(
      uint32_t message_flags
      , const base::Optional<WTF::Vector<EnumA>>& param_param0
      , const base::Optional<WTF::Vector<EnumB>>& param_param1
  )
      : mojo::internal::UnserializedMessageContext(
          &kMessageTag,
          internal::kConformanceTestInterface_Method15_Name,
          message_flags)
      , param_param0_(std::move(param_param0))
      , param_param1_(std::move(param_param1)){}
  ~ConformanceTestInterfaceProxy_Method15_Message() override = default;

  static mojo::Message Build(
      bool serialize,
      bool expects_response,
      bool is_sync,
      const base::Optional<WTF::Vector<EnumA>>& param_param0, const base::Optional<WTF::Vector<EnumB>>& param_param1) {

    
  const uint32_t kFlags =
      ((expects_response) ? mojo::Message::kFlagExpectsResponse : 0) |
      ((is_sync) ? mojo::Message::kFlagIsSync : 0);

    if (!serialize) {
      return mojo::Message(std::make_unique<ConformanceTestInterfaceProxy_Method15_Message>(
          kFlags
          , std::move(param_param0)
          , std::move(param_param1)
          ));
    }

    DCHECK(serialize);
    
  mojo::Message message(
      internal::kConformanceTestInterface_Method15_Name, kFlags, 0, 0, nullptr);
  auto* buffer = message.payload_buffer();
  ::mojo::test::internal::ConformanceTestInterface_Method15_Params_Data::BufferWriter
      params;
  mojo::internal::SerializationContext serialization_context;
  params.Allocate(buffer);
  typename decltype(params->param0)::BaseType::BufferWriter
      param0_writer;
  const mojo::internal::ContainerValidateParams param0_validate_params(
      0, ::mojo::test::internal::EnumA_Data::Validate);
  mojo::internal::Serialize<mojo::ArrayDataView<::mojo::test::EnumA>>(
      param_param0, buffer, &param0_writer, &param0_validate_params,
      &serialization_context);
  params->param0.Set(
      param0_writer.is_null() ? nullptr : param0_writer.data());
  typename decltype(params->param1)::BaseType::BufferWriter
      param1_writer;
  const mojo::internal::ContainerValidateParams param1_validate_params(
      0, ::mojo::test::internal::EnumB_Data::Validate);
  mojo::internal::Serialize<mojo::ArrayDataView<::mojo::test::EnumB>>(
      param_param1, buffer, &param1_writer, &param1_validate_params,
      &serialization_context);
  params->param1.Set(
      param1_writer.is_null() ? nullptr : param1_writer.data());
  message.AttachHandlesFromSerializationContext(
      &serialization_context);
    return message;
  }


  void Dispatch(ConformanceTestInterface* impl) {
    impl->Method15(
        std::move(param_param0_),
        std::move(param_param1_));
  }

 private:
  // mojo::internal::UnserializedMessageContext:
  void Serialize(mojo::internal::SerializationContext* serialization_context,
                 mojo::internal::Buffer* buffer) override {
    ::mojo::test::internal::ConformanceTestInterface_Method15_Params_Data::BufferWriter
        params;
    params.Allocate(buffer);
  typename decltype(params->param0)::BaseType::BufferWriter
      param0_writer;
  const mojo::internal::ContainerValidateParams param0_validate_params(
      0, ::mojo::test::internal::EnumA_Data::Validate);
  mojo::internal::Serialize<mojo::ArrayDataView<::mojo::test::EnumA>>(
      param_param0_, buffer, &param0_writer, &param0_validate_params,
      serialization_context);
  params->param0.Set(
      param0_writer.is_null() ? nullptr : param0_writer.data());
  typename decltype(params->param1)::BaseType::BufferWriter
      param1_writer;
  const mojo::internal::ContainerValidateParams param1_validate_params(
      0, ::mojo::test::internal::EnumB_Data::Validate);
  mojo::internal::Serialize<mojo::ArrayDataView<::mojo::test::EnumB>>(
      param_param1_, buffer, &param1_writer, &param1_validate_params,
      serialization_context);
  params->param1.Set(
      param1_writer.is_null() ? nullptr : param1_writer.data());
  }
  base::Optional<WTF::Vector<EnumA>> param_param0_;
  base::Optional<WTF::Vector<EnumB>> param_param1_;

  DISALLOW_COPY_AND_ASSIGN(ConformanceTestInterfaceProxy_Method15_Message);
};

const mojo::internal::UnserializedMessageContext::Tag
ConformanceTestInterfaceProxy_Method15_Message::kMessageTag = {};

void ConformanceTestInterfaceProxy::Method15(
    const base::Optional<WTF::Vector<EnumA>>& in_param0, const base::Optional<WTF::Vector<EnumB>>& in_param1) {
#if BUILDFLAG(MOJO_TRACE_ENABLED)
  TRACE_EVENT0("mojom", "<class 'jinja2::utils::Namespace'>::ConformanceTestInterface::Method15");
#endif
  const bool kExpectsResponse = false;
  const bool kIsSync = false;
  const bool kSerialize = receiver_->PrefersSerializedMessages();
  auto message = ConformanceTestInterfaceProxy_Method15_Message::Build(
      kSerialize, kExpectsResponse, kIsSync, std::move(in_param0), std::move(in_param1));

#if defined(ENABLE_IPC_FUZZER)
  message.set_interface_name(ConformanceTestInterface::Name_);
  message.set_method_name("Method15");
#endif
  // This return value may be ignored as false implies the Connector has
  // encountered an error, which will be visible through other means.
  ignore_result(receiver_->Accept(&message));
}
class ConformanceTestInterfaceProxy_Method16_Message
    : public mojo::internal::UnserializedMessageContext {
 public:
  static const mojo::internal::UnserializedMessageContext::Tag kMessageTag;

  explicit ConformanceTestInterfaceProxy_Method16_Message(
      uint32_t message_flags
      , const base::Optional<WTF::HashMap<EnumA, EnumA>>& param_param0
  )
      : mojo::internal::UnserializedMessageContext(
          &kMessageTag,
          internal::kConformanceTestInterface_Method16_Name,
          message_flags)
      , param_param0_(std::move(param_param0)){}
  ~ConformanceTestInterfaceProxy_Method16_Message() override = default;

  static mojo::Message Build(
      bool serialize,
      bool expects_response,
      bool is_sync,
      const base::Optional<WTF::HashMap<EnumA, EnumA>>& param_param0) {

    
  const uint32_t kFlags =
      ((expects_response) ? mojo::Message::kFlagExpectsResponse : 0) |
      ((is_sync) ? mojo::Message::kFlagIsSync : 0);

    if (!serialize) {
      return mojo::Message(std::make_unique<ConformanceTestInterfaceProxy_Method16_Message>(
          kFlags
          , std::move(param_param0)
          ));
    }

    DCHECK(serialize);
    
  mojo::Message message(
      internal::kConformanceTestInterface_Method16_Name, kFlags, 0, 0, nullptr);
  auto* buffer = message.payload_buffer();
  ::mojo::test::internal::ConformanceTestInterface_Method16_Params_Data::BufferWriter
      params;
  mojo::internal::SerializationContext serialization_context;
  params.Allocate(buffer);
  typename decltype(params->param0)::BaseType::BufferWriter
      param0_writer;
  const mojo::internal::ContainerValidateParams param0_validate_params(
      new mojo::internal::ContainerValidateParams(0, ::mojo::test::internal::EnumA_Data::Validate), new mojo::internal::ContainerValidateParams(0, ::mojo::test::internal::EnumA_Data::Validate));
  mojo::internal::Serialize<mojo::MapDataView<::mojo::test::EnumA, ::mojo::test::EnumA>>(
      param_param0, buffer, &param0_writer, &param0_validate_params,
      &serialization_context);
  params->param0.Set(
      param0_writer.is_null() ? nullptr : param0_writer.data());
  message.AttachHandlesFromSerializationContext(
      &serialization_context);
    return message;
  }


  void Dispatch(ConformanceTestInterface* impl) {
    impl->Method16(
        std::move(param_param0_));
  }

 private:
  // mojo::internal::UnserializedMessageContext:
  void Serialize(mojo::internal::SerializationContext* serialization_context,
                 mojo::internal::Buffer* buffer) override {
    ::mojo::test::internal::ConformanceTestInterface_Method16_Params_Data::BufferWriter
        params;
    params.Allocate(buffer);
  typename decltype(params->param0)::BaseType::BufferWriter
      param0_writer;
  const mojo::internal::ContainerValidateParams param0_validate_params(
      new mojo::internal::ContainerValidateParams(0, ::mojo::test::internal::EnumA_Data::Validate), new mojo::internal::ContainerValidateParams(0, ::mojo::test::internal::EnumA_Data::Validate));
  mojo::internal::Serialize<mojo::MapDataView<::mojo::test::EnumA, ::mojo::test::EnumA>>(
      param_param0_, buffer, &param0_writer, &param0_validate_params,
      serialization_context);
  params->param0.Set(
      param0_writer.is_null() ? nullptr : param0_writer.data());
  }
  base::Optional<WTF::HashMap<EnumA, EnumA>> param_param0_;

  DISALLOW_COPY_AND_ASSIGN(ConformanceTestInterfaceProxy_Method16_Message);
};

const mojo::internal::UnserializedMessageContext::Tag
ConformanceTestInterfaceProxy_Method16_Message::kMessageTag = {};

void ConformanceTestInterfaceProxy::Method16(
    const base::Optional<WTF::HashMap<EnumA, EnumA>>& in_param0) {
#if BUILDFLAG(MOJO_TRACE_ENABLED)
  TRACE_EVENT0("mojom", "<class 'jinja2::utils::Namespace'>::ConformanceTestInterface::Method16");
#endif
  const bool kExpectsResponse = false;
  const bool kIsSync = false;
  const bool kSerialize = receiver_->PrefersSerializedMessages();
  auto message = ConformanceTestInterfaceProxy_Method16_Message::Build(
      kSerialize, kExpectsResponse, kIsSync, std::move(in_param0));

#if defined(ENABLE_IPC_FUZZER)
  message.set_interface_name(ConformanceTestInterface::Name_);
  message.set_method_name("Method16");
#endif
  // This return value may be ignored as false implies the Connector has
  // encountered an error, which will be visible through other means.
  ignore_result(receiver_->Accept(&message));
}
class ConformanceTestInterfaceProxy_Method17_Message
    : public mojo::internal::UnserializedMessageContext {
 public:
  static const mojo::internal::UnserializedMessageContext::Tag kMessageTag;

  explicit ConformanceTestInterfaceProxy_Method17_Message(
      uint32_t message_flags
      , WTF::Vector<mojo::PendingRemote<InterfaceA>> param_param0
  )
      : mojo::internal::UnserializedMessageContext(
          &kMessageTag,
          internal::kConformanceTestInterface_Method17_Name,
          message_flags)
      , param_param0_(std::move(param_param0)){}
  ~ConformanceTestInterfaceProxy_Method17_Message() override = default;

  static mojo::Message Build(
      bool serialize,
      bool expects_response,
      bool is_sync,
      WTF::Vector<mojo::PendingRemote<InterfaceA>> param_param0) {

    
  const uint32_t kFlags =
      ((expects_response) ? mojo::Message::kFlagExpectsResponse : 0) |
      ((is_sync) ? mojo::Message::kFlagIsSync : 0);

    if (!serialize) {
      return mojo::Message(std::make_unique<ConformanceTestInterfaceProxy_Method17_Message>(
          kFlags
          , std::move(param_param0)
          ));
    }

    DCHECK(serialize);
    
  mojo::Message message(
      internal::kConformanceTestInterface_Method17_Name, kFlags, 0, 0, nullptr);
  auto* buffer = message.payload_buffer();
  ::mojo::test::internal::ConformanceTestInterface_Method17_Params_Data::BufferWriter
      params;
  mojo::internal::SerializationContext serialization_context;
  params.Allocate(buffer);
  typename decltype(params->param0)::BaseType::BufferWriter
      param0_writer;
  const mojo::internal::ContainerValidateParams param0_validate_params(
      0, false, nullptr);
  mojo::internal::Serialize<mojo::ArrayDataView<mojo::InterfacePtrDataView<::mojo::test::InterfaceAInterfaceBase>>>(
      param_param0, buffer, &param0_writer, &param0_validate_params,
      &serialization_context);
  params->param0.Set(
      param0_writer.is_null() ? nullptr : param0_writer.data());
  MOJO_INTERNAL_DLOG_SERIALIZATION_WARNING(
      params->param0.is_null(),
      mojo::internal::VALIDATION_ERROR_UNEXPECTED_NULL_POINTER,
      "null param0 in ConformanceTestInterface.Method17 request");
  message.AttachHandlesFromSerializationContext(
      &serialization_context);
    return message;
  }


  void Dispatch(ConformanceTestInterface* impl) {
    impl->Method17(
        std::move(param_param0_));
  }

 private:
  // mojo::internal::UnserializedMessageContext:
  void Serialize(mojo::internal::SerializationContext* serialization_context,
                 mojo::internal::Buffer* buffer) override {
    ::mojo::test::internal::ConformanceTestInterface_Method17_Params_Data::BufferWriter
        params;
    params.Allocate(buffer);
  typename decltype(params->param0)::BaseType::BufferWriter
      param0_writer;
  const mojo::internal::ContainerValidateParams param0_validate_params(
      0, false, nullptr);
  mojo::internal::Serialize<mojo::ArrayDataView<mojo::InterfacePtrDataView<::mojo::test::InterfaceAInterfaceBase>>>(
      param_param0_, buffer, &param0_writer, &param0_validate_params,
      serialization_context);
  params->param0.Set(
      param0_writer.is_null() ? nullptr : param0_writer.data());
  MOJO_INTERNAL_DLOG_SERIALIZATION_WARNING(
      params->param0.is_null(),
      mojo::internal::VALIDATION_ERROR_UNEXPECTED_NULL_POINTER,
      "null param0 in ConformanceTestInterface.Method17 request");
  }
  WTF::Vector<mojo::PendingRemote<InterfaceA>> param_param0_;

  DISALLOW_COPY_AND_ASSIGN(ConformanceTestInterfaceProxy_Method17_Message);
};

const mojo::internal::UnserializedMessageContext::Tag
ConformanceTestInterfaceProxy_Method17_Message::kMessageTag = {};

void ConformanceTestInterfaceProxy::Method17(
    WTF::Vector<mojo::PendingRemote<InterfaceA>> in_param0) {
#if BUILDFLAG(MOJO_TRACE_ENABLED)
  TRACE_EVENT0("mojom", "<class 'jinja2::utils::Namespace'>::ConformanceTestInterface::Method17");
#endif
  const bool kExpectsResponse = false;
  const bool kIsSync = false;
  const bool kSerialize = receiver_->PrefersSerializedMessages();
  auto message = ConformanceTestInterfaceProxy_Method17_Message::Build(
      kSerialize, kExpectsResponse, kIsSync, std::move(in_param0));

#if defined(ENABLE_IPC_FUZZER)
  message.set_interface_name(ConformanceTestInterface::Name_);
  message.set_method_name("Method17");
#endif
  // This return value may be ignored as false implies the Connector has
  // encountered an error, which will be visible through other means.
  ignore_result(receiver_->Accept(&message));
}
class ConformanceTestInterfaceProxy_Method18_Message
    : public mojo::internal::UnserializedMessageContext {
 public:
  static const mojo::internal::UnserializedMessageContext::Tag kMessageTag;

  explicit ConformanceTestInterfaceProxy_Method18_Message(
      uint32_t message_flags
      , UnionAPtr param_param0
  )
      : mojo::internal::UnserializedMessageContext(
          &kMessageTag,
          internal::kConformanceTestInterface_Method18_Name,
          message_flags)
      , param_param0_(std::move(param_param0)){}
  ~ConformanceTestInterfaceProxy_Method18_Message() override = default;

  static mojo::Message Build(
      bool serialize,
      bool expects_response,
      bool is_sync,
      UnionAPtr param_param0) {

    
  const uint32_t kFlags =
      ((expects_response) ? mojo::Message::kFlagExpectsResponse : 0) |
      ((is_sync) ? mojo::Message::kFlagIsSync : 0);

    if (!serialize) {
      return mojo::Message(std::make_unique<ConformanceTestInterfaceProxy_Method18_Message>(
          kFlags
          , std::move(param_param0)
          ));
    }

    DCHECK(serialize);
    
  mojo::Message message(
      internal::kConformanceTestInterface_Method18_Name, kFlags, 0, 0, nullptr);
  auto* buffer = message.payload_buffer();
  ::mojo::test::internal::ConformanceTestInterface_Method18_Params_Data::BufferWriter
      params;
  mojo::internal::SerializationContext serialization_context;
  params.Allocate(buffer);
  typename decltype(params->param0)::BufferWriter param0_writer;
  param0_writer.AllocateInline(buffer, &params->param0);
  mojo::internal::Serialize<::mojo::test::UnionADataView>(
      param_param0, buffer, &param0_writer, true, &serialization_context);
  message.AttachHandlesFromSerializationContext(
      &serialization_context);
    return message;
  }


  void Dispatch(ConformanceTestInterface* impl) {
    impl->Method18(
        std::move(param_param0_));
  }

 private:
  // mojo::internal::UnserializedMessageContext:
  void Serialize(mojo::internal::SerializationContext* serialization_context,
                 mojo::internal::Buffer* buffer) override {
    ::mojo::test::internal::ConformanceTestInterface_Method18_Params_Data::BufferWriter
        params;
    params.Allocate(buffer);
  typename decltype(params->param0)::BufferWriter param0_writer;
  param0_writer.AllocateInline(buffer, &params->param0);
  mojo::internal::Serialize<::mojo::test::UnionADataView>(
      param_param0_, buffer, &param0_writer, true, serialization_context);
  }
  UnionAPtr param_param0_;

  DISALLOW_COPY_AND_ASSIGN(ConformanceTestInterfaceProxy_Method18_Message);
};

const mojo::internal::UnserializedMessageContext::Tag
ConformanceTestInterfaceProxy_Method18_Message::kMessageTag = {};

void ConformanceTestInterfaceProxy::Method18(
    UnionAPtr in_param0) {
#if BUILDFLAG(MOJO_TRACE_ENABLED)
  TRACE_EVENT0("mojom", "<class 'jinja2::utils::Namespace'>::ConformanceTestInterface::Method18");
#endif
  const bool kExpectsResponse = false;
  const bool kIsSync = false;
  const bool kSerialize = receiver_->PrefersSerializedMessages();
  auto message = ConformanceTestInterfaceProxy_Method18_Message::Build(
      kSerialize, kExpectsResponse, kIsSync, std::move(in_param0));

#if defined(ENABLE_IPC_FUZZER)
  message.set_interface_name(ConformanceTestInterface::Name_);
  message.set_method_name("Method18");
#endif
  // This return value may be ignored as false implies the Connector has
  // encountered an error, which will be visible through other means.
  ignore_result(receiver_->Accept(&message));
}
class ConformanceTestInterfaceProxy_Method19_Message
    : public mojo::internal::UnserializedMessageContext {
 public:
  static const mojo::internal::UnserializedMessageContext::Tag kMessageTag;

  explicit ConformanceTestInterfaceProxy_Method19_Message(
      uint32_t message_flags
      , RecursivePtr param_recursive
  )
      : mojo::internal::UnserializedMessageContext(
          &kMessageTag,
          internal::kConformanceTestInterface_Method19_Name,
          message_flags)
      , param_recursive_(std::move(param_recursive)){}
  ~ConformanceTestInterfaceProxy_Method19_Message() override = default;

  static mojo::Message Build(
      bool serialize,
      bool expects_response,
      bool is_sync,
      RecursivePtr param_recursive) {

    
  const uint32_t kFlags =
      ((expects_response) ? mojo::Message::kFlagExpectsResponse : 0) |
      ((is_sync) ? mojo::Message::kFlagIsSync : 0);

    if (!serialize) {
      return mojo::Message(std::make_unique<ConformanceTestInterfaceProxy_Method19_Message>(
          kFlags
          , std::move(param_recursive)
          ));
    }

    DCHECK(serialize);
    
  mojo::Message message(
      internal::kConformanceTestInterface_Method19_Name, kFlags, 0, 0, nullptr);
  auto* buffer = message.payload_buffer();
  ::mojo::test::internal::ConformanceTestInterface_Method19_Params_Data::BufferWriter
      params;
  mojo::internal::SerializationContext serialization_context;
  params.Allocate(buffer);
  typename decltype(params->recursive)::BaseType::BufferWriter
      recursive_writer;
  mojo::internal::Serialize<::mojo::test::RecursiveDataView>(
      param_recursive, buffer, &recursive_writer, &serialization_context);
  params->recursive.Set(
      recursive_writer.is_null() ? nullptr : recursive_writer.data());
  MOJO_INTERNAL_DLOG_SERIALIZATION_WARNING(
      params->recursive.is_null(),
      mojo::internal::VALIDATION_ERROR_UNEXPECTED_NULL_POINTER,
      "null recursive in ConformanceTestInterface.Method19 request");
  message.AttachHandlesFromSerializationContext(
      &serialization_context);
    return message;
  }


  void Dispatch(ConformanceTestInterface* impl) {
    impl->Method19(
        std::move(param_recursive_));
  }

 private:
  // mojo::internal::UnserializedMessageContext:
  void Serialize(mojo::internal::SerializationContext* serialization_context,
                 mojo::internal::Buffer* buffer) override {
    ::mojo::test::internal::ConformanceTestInterface_Method19_Params_Data::BufferWriter
        params;
    params.Allocate(buffer);
  typename decltype(params->recursive)::BaseType::BufferWriter
      recursive_writer;
  mojo::internal::Serialize<::mojo::test::RecursiveDataView>(
      param_recursive_, buffer, &recursive_writer, serialization_context);
  params->recursive.Set(
      recursive_writer.is_null() ? nullptr : recursive_writer.data());
  MOJO_INTERNAL_DLOG_SERIALIZATION_WARNING(
      params->recursive.is_null(),
      mojo::internal::VALIDATION_ERROR_UNEXPECTED_NULL_POINTER,
      "null recursive in ConformanceTestInterface.Method19 request");
  }
  RecursivePtr param_recursive_;

  DISALLOW_COPY_AND_ASSIGN(ConformanceTestInterfaceProxy_Method19_Message);
};

const mojo::internal::UnserializedMessageContext::Tag
ConformanceTestInterfaceProxy_Method19_Message::kMessageTag = {};

void ConformanceTestInterfaceProxy::Method19(
    RecursivePtr in_recursive) {
#if BUILDFLAG(MOJO_TRACE_ENABLED)
  TRACE_EVENT0("mojom", "<class 'jinja2::utils::Namespace'>::ConformanceTestInterface::Method19");
#endif
  const bool kExpectsResponse = false;
  const bool kIsSync = false;
  const bool kSerialize = receiver_->PrefersSerializedMessages();
  auto message = ConformanceTestInterfaceProxy_Method19_Message::Build(
      kSerialize, kExpectsResponse, kIsSync, std::move(in_recursive));

#if defined(ENABLE_IPC_FUZZER)
  message.set_interface_name(ConformanceTestInterface::Name_);
  message.set_method_name("Method19");
#endif
  // This return value may be ignored as false implies the Connector has
  // encountered an error, which will be visible through other means.
  ignore_result(receiver_->Accept(&message));
}
class ConformanceTestInterfaceProxy_Method20_Message
    : public mojo::internal::UnserializedMessageContext {
 public:
  static const mojo::internal::UnserializedMessageContext::Tag kMessageTag;

  explicit ConformanceTestInterfaceProxy_Method20_Message(
      uint32_t message_flags
      , WTF::HashMap<StructBPtr, uint8_t> param_param0
  )
      : mojo::internal::UnserializedMessageContext(
          &kMessageTag,
          internal::kConformanceTestInterface_Method20_Name,
          message_flags)
      , param_param0_(std::move(param_param0)){}
  ~ConformanceTestInterfaceProxy_Method20_Message() override = default;

  static mojo::Message Build(
      bool serialize,
      bool expects_response,
      bool is_sync,
      WTF::HashMap<StructBPtr, uint8_t> param_param0) {

    
  const uint32_t kFlags =
      ((expects_response) ? mojo::Message::kFlagExpectsResponse : 0) |
      ((is_sync) ? mojo::Message::kFlagIsSync : 0);

    if (!serialize) {
      return mojo::Message(std::make_unique<ConformanceTestInterfaceProxy_Method20_Message>(
          kFlags
          , std::move(param_param0)
          ));
    }

    DCHECK(serialize);
    
  mojo::Message message(
      internal::kConformanceTestInterface_Method20_Name, kFlags, 0, 0, nullptr);
  auto* buffer = message.payload_buffer();
  ::mojo::test::internal::ConformanceTestInterface_Method20_Params_Data::BufferWriter
      params;
  mojo::internal::SerializationContext serialization_context;
  params.Allocate(buffer);
  typename decltype(params->param0)::BaseType::BufferWriter
      param0_writer;
  const mojo::internal::ContainerValidateParams param0_validate_params(
      new mojo::internal::ContainerValidateParams(0, false, nullptr), new mojo::internal::ContainerValidateParams(0, false, nullptr));
  mojo::internal::Serialize<mojo::MapDataView<::mojo::test::StructBDataView, uint8_t>>(
      param_param0, buffer, &param0_writer, &param0_validate_params,
      &serialization_context);
  params->param0.Set(
      param0_writer.is_null() ? nullptr : param0_writer.data());
  MOJO_INTERNAL_DLOG_SERIALIZATION_WARNING(
      params->param0.is_null(),
      mojo::internal::VALIDATION_ERROR_UNEXPECTED_NULL_POINTER,
      "null param0 in ConformanceTestInterface.Method20 request");
  message.AttachHandlesFromSerializationContext(
      &serialization_context);
    return message;
  }


  void Dispatch(ConformanceTestInterface* impl) {
    impl->Method20(
        std::move(param_param0_));
  }

 private:
  // mojo::internal::UnserializedMessageContext:
  void Serialize(mojo::internal::SerializationContext* serialization_context,
                 mojo::internal::Buffer* buffer) override {
    ::mojo::test::internal::ConformanceTestInterface_Method20_Params_Data::BufferWriter
        params;
    params.Allocate(buffer);
  typename decltype(params->param0)::BaseType::BufferWriter
      param0_writer;
  const mojo::internal::ContainerValidateParams param0_validate_params(
      new mojo::internal::ContainerValidateParams(0, false, nullptr), new mojo::internal::ContainerValidateParams(0, false, nullptr));
  mojo::internal::Serialize<mojo::MapDataView<::mojo::test::StructBDataView, uint8_t>>(
      param_param0_, buffer, &param0_writer, &param0_validate_params,
      serialization_context);
  params->param0.Set(
      param0_writer.is_null() ? nullptr : param0_writer.data());
  MOJO_INTERNAL_DLOG_SERIALIZATION_WARNING(
      params->param0.is_null(),
      mojo::internal::VALIDATION_ERROR_UNEXPECTED_NULL_POINTER,
      "null param0 in ConformanceTestInterface.Method20 request");
  }
  WTF::HashMap<StructBPtr, uint8_t> param_param0_;

  DISALLOW_COPY_AND_ASSIGN(ConformanceTestInterfaceProxy_Method20_Message);
};

const mojo::internal::UnserializedMessageContext::Tag
ConformanceTestInterfaceProxy_Method20_Message::kMessageTag = {};

void ConformanceTestInterfaceProxy::Method20(
    WTF::HashMap<StructBPtr, uint8_t> in_param0) {
#if BUILDFLAG(MOJO_TRACE_ENABLED)
  TRACE_EVENT0("mojom", "<class 'jinja2::utils::Namespace'>::ConformanceTestInterface::Method20");
#endif
  const bool kExpectsResponse = false;
  const bool kIsSync = false;
  const bool kSerialize = receiver_->PrefersSerializedMessages();
  auto message = ConformanceTestInterfaceProxy_Method20_Message::Build(
      kSerialize, kExpectsResponse, kIsSync, std::move(in_param0));

#if defined(ENABLE_IPC_FUZZER)
  message.set_interface_name(ConformanceTestInterface::Name_);
  message.set_method_name("Method20");
#endif
  // This return value may be ignored as false implies the Connector has
  // encountered an error, which will be visible through other means.
  ignore_result(receiver_->Accept(&message));
}
class ConformanceTestInterfaceProxy_Method21_Message
    : public mojo::internal::UnserializedMessageContext {
 public:
  static const mojo::internal::UnserializedMessageContext::Tag kMessageTag;

  explicit ConformanceTestInterfaceProxy_Method21_Message(
      uint32_t message_flags
      , ExtensibleEmptyEnum param_param0
  )
      : mojo::internal::UnserializedMessageContext(
          &kMessageTag,
          internal::kConformanceTestInterface_Method21_Name,
          message_flags)
      , param_param0_(std::move(param_param0)){}
  ~ConformanceTestInterfaceProxy_Method21_Message() override = default;

  static mojo::Message Build(
      bool serialize,
      bool expects_response,
      bool is_sync,
      ExtensibleEmptyEnum param_param0) {

    
  const uint32_t kFlags =
      ((expects_response) ? mojo::Message::kFlagExpectsResponse : 0) |
      ((is_sync) ? mojo::Message::kFlagIsSync : 0);

    if (!serialize) {
      return mojo::Message(std::make_unique<ConformanceTestInterfaceProxy_Method21_Message>(
          kFlags
          , std::move(param_param0)
          ));
    }

    DCHECK(serialize);
    
  mojo::Message message(
      internal::kConformanceTestInterface_Method21_Name, kFlags, 0, 0, nullptr);
  auto* buffer = message.payload_buffer();
  ::mojo::test::internal::ConformanceTestInterface_Method21_Params_Data::BufferWriter
      params;
  mojo::internal::SerializationContext serialization_context;
  params.Allocate(buffer);
  mojo::internal::Serialize<::mojo::test::ExtensibleEmptyEnum>(
      param_param0, &params->param0);
  message.AttachHandlesFromSerializationContext(
      &serialization_context);
    return message;
  }


  void Dispatch(ConformanceTestInterface* impl) {
    impl->Method21(
        std::move(param_param0_));
  }

 private:
  // mojo::internal::UnserializedMessageContext:
  void Serialize(mojo::internal::SerializationContext* serialization_context,
                 mojo::internal::Buffer* buffer) override {
    ::mojo::test::internal::ConformanceTestInterface_Method21_Params_Data::BufferWriter
        params;
    params.Allocate(buffer);
  mojo::internal::Serialize<::mojo::test::ExtensibleEmptyEnum>(
      param_param0_, &params->param0);
  }
  ExtensibleEmptyEnum param_param0_;

  DISALLOW_COPY_AND_ASSIGN(ConformanceTestInterfaceProxy_Method21_Message);
};

const mojo::internal::UnserializedMessageContext::Tag
ConformanceTestInterfaceProxy_Method21_Message::kMessageTag = {};

void ConformanceTestInterfaceProxy::Method21(
    ExtensibleEmptyEnum in_param0) {
#if BUILDFLAG(MOJO_TRACE_ENABLED)
  TRACE_EVENT0("mojom", "<class 'jinja2::utils::Namespace'>::ConformanceTestInterface::Method21");
#endif
  const bool kExpectsResponse = false;
  const bool kIsSync = false;
  const bool kSerialize = receiver_->PrefersSerializedMessages();
  auto message = ConformanceTestInterfaceProxy_Method21_Message::Build(
      kSerialize, kExpectsResponse, kIsSync, std::move(in_param0));

#if defined(ENABLE_IPC_FUZZER)
  message.set_interface_name(ConformanceTestInterface::Name_);
  message.set_method_name("Method21");
#endif
  // This return value may be ignored as false implies the Connector has
  // encountered an error, which will be visible through other means.
  ignore_result(receiver_->Accept(&message));
}
class ConformanceTestInterfaceProxy_Method22_Message
    : public mojo::internal::UnserializedMessageContext {
 public:
  static const mojo::internal::UnserializedMessageContext::Tag kMessageTag;

  explicit ConformanceTestInterfaceProxy_Method22_Message(
      uint32_t message_flags
      , EmptyEnum param_param0
  )
      : mojo::internal::UnserializedMessageContext(
          &kMessageTag,
          internal::kConformanceTestInterface_Method22_Name,
          message_flags)
      , param_param0_(std::move(param_param0)){}
  ~ConformanceTestInterfaceProxy_Method22_Message() override = default;

  static mojo::Message Build(
      bool serialize,
      bool expects_response,
      bool is_sync,
      EmptyEnum param_param0) {

    
  const uint32_t kFlags =
      ((expects_response) ? mojo::Message::kFlagExpectsResponse : 0) |
      ((is_sync) ? mojo::Message::kFlagIsSync : 0);

    if (!serialize) {
      return mojo::Message(std::make_unique<ConformanceTestInterfaceProxy_Method22_Message>(
          kFlags
          , std::move(param_param0)
          ));
    }

    DCHECK(serialize);
    
  mojo::Message message(
      internal::kConformanceTestInterface_Method22_Name, kFlags, 0, 0, nullptr);
  auto* buffer = message.payload_buffer();
  ::mojo::test::internal::ConformanceTestInterface_Method22_Params_Data::BufferWriter
      params;
  mojo::internal::SerializationContext serialization_context;
  params.Allocate(buffer);
  mojo::internal::Serialize<::mojo::test::EmptyEnum>(
      param_param0, &params->param0);
  message.AttachHandlesFromSerializationContext(
      &serialization_context);
    return message;
  }


  void Dispatch(ConformanceTestInterface* impl) {
    impl->Method22(
        std::move(param_param0_));
  }

 private:
  // mojo::internal::UnserializedMessageContext:
  void Serialize(mojo::internal::SerializationContext* serialization_context,
                 mojo::internal::Buffer* buffer) override {
    ::mojo::test::internal::ConformanceTestInterface_Method22_Params_Data::BufferWriter
        params;
    params.Allocate(buffer);
  mojo::internal::Serialize<::mojo::test::EmptyEnum>(
      param_param0_, &params->param0);
  }
  EmptyEnum param_param0_;

  DISALLOW_COPY_AND_ASSIGN(ConformanceTestInterfaceProxy_Method22_Message);
};

const mojo::internal::UnserializedMessageContext::Tag
ConformanceTestInterfaceProxy_Method22_Message::kMessageTag = {};

void ConformanceTestInterfaceProxy::Method22(
    EmptyEnum in_param0) {
#if BUILDFLAG(MOJO_TRACE_ENABLED)
  TRACE_EVENT0("mojom", "<class 'jinja2::utils::Namespace'>::ConformanceTestInterface::Method22");
#endif
  const bool kExpectsResponse = false;
  const bool kIsSync = false;
  const bool kSerialize = receiver_->PrefersSerializedMessages();
  auto message = ConformanceTestInterfaceProxy_Method22_Message::Build(
      kSerialize, kExpectsResponse, kIsSync, std::move(in_param0));

#if defined(ENABLE_IPC_FUZZER)
  message.set_interface_name(ConformanceTestInterface::Name_);
  message.set_method_name("Method22");
#endif
  // This return value may be ignored as false implies the Connector has
  // encountered an error, which will be visible through other means.
  ignore_result(receiver_->Accept(&message));
}
class ConformanceTestInterface_Method12_ProxyToResponder {
 public:
  static ConformanceTestInterface::Method12Callback CreateCallback(
      uint64_t request_id,
      bool is_sync,
      std::unique_ptr<mojo::MessageReceiverWithStatus> responder) {
    std::unique_ptr<ConformanceTestInterface_Method12_ProxyToResponder> proxy(
        new ConformanceTestInterface_Method12_ProxyToResponder(
            request_id, is_sync, std::move(responder)));
    return base::BindOnce(&ConformanceTestInterface_Method12_ProxyToResponder::Run,
                          std::move(proxy));
  }

  ~ConformanceTestInterface_Method12_ProxyToResponder() {
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
  ConformanceTestInterface_Method12_ProxyToResponder(
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
        << "ConformanceTestInterface::Method12Callback was destroyed without "
        << "first either being run or its corresponding binding being closed. "
        << "It is an error to drop response callbacks which still correspond "
        << "to an open interface pipe.";
  }
#endif

  void Run(
      float in_param0);

  uint64_t request_id_;
  bool is_sync_;
  std::unique_ptr<mojo::MessageReceiverWithStatus> responder_;

  DISALLOW_COPY_AND_ASSIGN(ConformanceTestInterface_Method12_ProxyToResponder);
};
class ConformanceTestInterface_Method12_Response_Message
    : public mojo::internal::UnserializedMessageContext {
 public:
  static const mojo::internal::UnserializedMessageContext::Tag kMessageTag;

  explicit ConformanceTestInterface_Method12_Response_Message(
      uint32_t message_flags
      , float param_param0
  )
      : mojo::internal::UnserializedMessageContext(
          &kMessageTag,
          internal::kConformanceTestInterface_Method12_Name,
          message_flags)
      , param_param0_(std::move(param_param0)){}
  ~ConformanceTestInterface_Method12_Response_Message() override = default;

  static mojo::Message Build(
      bool serialize,
      bool is_sync,
      float param_param0) {

    
  const uint32_t kFlags = mojo::Message::kFlagIsResponse |
      ((is_sync) ? mojo::Message::kFlagIsSync : 0);

    if (!serialize) {
      return mojo::Message(std::make_unique<ConformanceTestInterface_Method12_Response_Message>(
          kFlags
          , std::move(param_param0)
          ));
    }

    DCHECK(serialize);
    
  mojo::Message message(
      internal::kConformanceTestInterface_Method12_Name, kFlags, 0, 0, nullptr);
  auto* buffer = message.payload_buffer();
  ::mojo::test::internal::ConformanceTestInterface_Method12_ResponseParams_Data::BufferWriter
      params;
  mojo::internal::SerializationContext serialization_context;
  params.Allocate(buffer);
  params->param0 = param_param0;
  message.AttachHandlesFromSerializationContext(
      &serialization_context);
    return message;
  }


  void Dispatch(ConformanceTestInterface::Method12Callback* callback) {
    std::move(*callback).Run(
        std::move(param_param0_));
  }



 private:
  // mojo::internal::UnserializedMessageContext:
  void Serialize(mojo::internal::SerializationContext* serialization_context,
                 mojo::internal::Buffer* buffer) override {
    ::mojo::test::internal::ConformanceTestInterface_Method12_ResponseParams_Data::BufferWriter
        params;
    params.Allocate(buffer);
  params->param0 = param_param0_;
  }
  float param_param0_;

  DISALLOW_COPY_AND_ASSIGN(ConformanceTestInterface_Method12_Response_Message);
};

const mojo::internal::UnserializedMessageContext::Tag
ConformanceTestInterface_Method12_Response_Message::kMessageTag = {};

bool ConformanceTestInterface_Method12_ForwardToCallback::Accept(
    mojo::Message* message) {
#if BUILDFLAG(MOJO_TRACE_ENABLED)
  TRACE_EVENT1("mojom", "<class 'jinja2::utils::Namespace'>::ConformanceTestInterface::Method12Callback",
               "message", message->name());
#endif
  mojo::internal::MessageDispatchContext dispatch_context(message);
  if (!message->is_serialized()) {
    auto context =
        message->TakeUnserializedContext<ConformanceTestInterface_Method12_Response_Message>();
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
  internal::ConformanceTestInterface_Method12_ResponseParams_Data* params =
      reinterpret_cast<
          internal::ConformanceTestInterface_Method12_ResponseParams_Data*>(
              message->mutable_payload());
  
  mojo::internal::SerializationContext serialization_context;
  serialization_context.TakeHandlesFromMessage(message);
  bool success = true;
  float p_param0{};
  ConformanceTestInterface_Method12_ResponseParamsDataView input_data_view(params, &serialization_context);
  
  p_param0 = input_data_view.param0();
  if (!success) {
    ReportValidationErrorForMessage(
        message,
        mojo::internal::VALIDATION_ERROR_DESERIALIZATION_FAILED,
        ConformanceTestInterface::Name_, 12, true);
    return false;
  }
  if (!callback_.is_null())
    std::move(callback_).Run(
std::move(p_param0));
  return true;
}

void ConformanceTestInterface_Method12_ProxyToResponder::Run(
    float in_param0) {
  const bool kSerialize = responder_->PrefersSerializedMessages();
  auto message = ConformanceTestInterface_Method12_Response_Message::Build(kSerialize, is_sync_, std::move(in_param0));

#if BUILDFLAG(MOJO_TRACE_ENABLED)
  TRACE_EVENT1("mojom", "(Impl)<class 'jinja2::utils::Namespace'>::ConformanceTestInterface::Method12Callback",
               "message", message.name());
#endif

#if defined(ENABLE_IPC_FUZZER)
  message.set_interface_name(ConformanceTestInterface::Name_);
  message.set_method_name("Method12");
#endif

  message.set_request_id(request_id_);
  ignore_result(responder_->Accept(&message));
  // TODO(darin): Accept() returning false indicates a malformed message, and
  // that may be good reason to close the connection. However, we don't have a
  // way to do that from here. We should add a way.
  responder_ = nullptr;
}

// static
bool ConformanceTestInterfaceStubDispatch::Accept(
    ConformanceTestInterface* impl,
    mojo::Message* message) {
  switch (message->header()->name) {
    case internal::kConformanceTestInterface_Method0_Name: {
#if BUILDFLAG(MOJO_TRACE_ENABLED)
      TRACE_EVENT1(
          "mojom",
          "(Impl)<class 'jinja2::utils::Namespace'>::ConformanceTestInterface::Method0",
          "message", message->name());
#endif
      base::TaskAnnotator::ScopedSetIpcProgramCounter scoped_ipc_pc(
          base::GetProgramCounter());
      mojo::internal::MessageDispatchContext context(message);
      if (!message->is_serialized()) {
        auto context = message->TakeUnserializedContext<
            ConformanceTestInterfaceProxy_Method0_Message>();
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
      internal::ConformanceTestInterface_Method0_Params_Data* params =
          reinterpret_cast<internal::ConformanceTestInterface_Method0_Params_Data*>(
              message->mutable_payload());
      
      mojo::internal::SerializationContext serialization_context;
      serialization_context.TakeHandlesFromMessage(message);
      bool success = true;
      float p_param0{};
      ConformanceTestInterface_Method0_ParamsDataView input_data_view(params, &serialization_context);
      
      p_param0 = input_data_view.param0();
      if (!success) {
        ReportValidationErrorForMessage(
            message,
            mojo::internal::VALIDATION_ERROR_DESERIALIZATION_FAILED,
            ConformanceTestInterface::Name_, 0, false);
        return false;
      }
      // A null |impl| means no implementation was bound.
      DCHECK(impl);
      impl->Method0(
std::move(p_param0));
      return true;
    }
    case internal::kConformanceTestInterface_Method1_Name: {
#if BUILDFLAG(MOJO_TRACE_ENABLED)
      TRACE_EVENT1(
          "mojom",
          "(Impl)<class 'jinja2::utils::Namespace'>::ConformanceTestInterface::Method1",
          "message", message->name());
#endif
      base::TaskAnnotator::ScopedSetIpcProgramCounter scoped_ipc_pc(
          base::GetProgramCounter());
      mojo::internal::MessageDispatchContext context(message);
      if (!message->is_serialized()) {
        auto context = message->TakeUnserializedContext<
            ConformanceTestInterfaceProxy_Method1_Message>();
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
      internal::ConformanceTestInterface_Method1_Params_Data* params =
          reinterpret_cast<internal::ConformanceTestInterface_Method1_Params_Data*>(
              message->mutable_payload());
      
      mojo::internal::SerializationContext serialization_context;
      serialization_context.TakeHandlesFromMessage(message);
      bool success = true;
      StructAPtr p_param0{};
      ConformanceTestInterface_Method1_ParamsDataView input_data_view(params, &serialization_context);
      
      if (!input_data_view.ReadParam0(&p_param0))
        success = false;
      if (!success) {
        ReportValidationErrorForMessage(
            message,
            mojo::internal::VALIDATION_ERROR_DESERIALIZATION_FAILED,
            ConformanceTestInterface::Name_, 1, false);
        return false;
      }
      // A null |impl| means no implementation was bound.
      DCHECK(impl);
      impl->Method1(
std::move(p_param0));
      return true;
    }
    case internal::kConformanceTestInterface_Method2_Name: {
#if BUILDFLAG(MOJO_TRACE_ENABLED)
      TRACE_EVENT1(
          "mojom",
          "(Impl)<class 'jinja2::utils::Namespace'>::ConformanceTestInterface::Method2",
          "message", message->name());
#endif
      base::TaskAnnotator::ScopedSetIpcProgramCounter scoped_ipc_pc(
          base::GetProgramCounter());
      mojo::internal::MessageDispatchContext context(message);
      if (!message->is_serialized()) {
        auto context = message->TakeUnserializedContext<
            ConformanceTestInterfaceProxy_Method2_Message>();
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
      internal::ConformanceTestInterface_Method2_Params_Data* params =
          reinterpret_cast<internal::ConformanceTestInterface_Method2_Params_Data*>(
              message->mutable_payload());
      
      mojo::internal::SerializationContext serialization_context;
      serialization_context.TakeHandlesFromMessage(message);
      bool success = true;
      StructBPtr p_param0{};
      StructAPtr p_param1{};
      ConformanceTestInterface_Method2_ParamsDataView input_data_view(params, &serialization_context);
      
      if (!input_data_view.ReadParam0(&p_param0))
        success = false;
      if (!input_data_view.ReadParam1(&p_param1))
        success = false;
      if (!success) {
        ReportValidationErrorForMessage(
            message,
            mojo::internal::VALIDATION_ERROR_DESERIALIZATION_FAILED,
            ConformanceTestInterface::Name_, 2, false);
        return false;
      }
      // A null |impl| means no implementation was bound.
      DCHECK(impl);
      impl->Method2(
std::move(p_param0), 
std::move(p_param1));
      return true;
    }
    case internal::kConformanceTestInterface_Method3_Name: {
#if BUILDFLAG(MOJO_TRACE_ENABLED)
      TRACE_EVENT1(
          "mojom",
          "(Impl)<class 'jinja2::utils::Namespace'>::ConformanceTestInterface::Method3",
          "message", message->name());
#endif
      base::TaskAnnotator::ScopedSetIpcProgramCounter scoped_ipc_pc(
          base::GetProgramCounter());
      mojo::internal::MessageDispatchContext context(message);
      if (!message->is_serialized()) {
        auto context = message->TakeUnserializedContext<
            ConformanceTestInterfaceProxy_Method3_Message>();
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
      internal::ConformanceTestInterface_Method3_Params_Data* params =
          reinterpret_cast<internal::ConformanceTestInterface_Method3_Params_Data*>(
              message->mutable_payload());
      
      mojo::internal::SerializationContext serialization_context;
      serialization_context.TakeHandlesFromMessage(message);
      bool success = true;
      WTF::Vector<bool> p_param0{};
      ConformanceTestInterface_Method3_ParamsDataView input_data_view(params, &serialization_context);
      
      if (!input_data_view.ReadParam0(&p_param0))
        success = false;
      if (!success) {
        ReportValidationErrorForMessage(
            message,
            mojo::internal::VALIDATION_ERROR_DESERIALIZATION_FAILED,
            ConformanceTestInterface::Name_, 3, false);
        return false;
      }
      // A null |impl| means no implementation was bound.
      DCHECK(impl);
      impl->Method3(
std::move(p_param0));
      return true;
    }
    case internal::kConformanceTestInterface_Method4_Name: {
#if BUILDFLAG(MOJO_TRACE_ENABLED)
      TRACE_EVENT1(
          "mojom",
          "(Impl)<class 'jinja2::utils::Namespace'>::ConformanceTestInterface::Method4",
          "message", message->name());
#endif
      base::TaskAnnotator::ScopedSetIpcProgramCounter scoped_ipc_pc(
          base::GetProgramCounter());
      mojo::internal::MessageDispatchContext context(message);
      if (!message->is_serialized()) {
        auto context = message->TakeUnserializedContext<
            ConformanceTestInterfaceProxy_Method4_Message>();
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
      internal::ConformanceTestInterface_Method4_Params_Data* params =
          reinterpret_cast<internal::ConformanceTestInterface_Method4_Params_Data*>(
              message->mutable_payload());
      
      mojo::internal::SerializationContext serialization_context;
      serialization_context.TakeHandlesFromMessage(message);
      bool success = true;
      StructCPtr p_param0{};
      WTF::Vector<uint8_t> p_param1{};
      ConformanceTestInterface_Method4_ParamsDataView input_data_view(params, &serialization_context);
      
      if (!input_data_view.ReadParam0(&p_param0))
        success = false;
      if (!input_data_view.ReadParam1(&p_param1))
        success = false;
      if (!success) {
        ReportValidationErrorForMessage(
            message,
            mojo::internal::VALIDATION_ERROR_DESERIALIZATION_FAILED,
            ConformanceTestInterface::Name_, 4, false);
        return false;
      }
      // A null |impl| means no implementation was bound.
      DCHECK(impl);
      impl->Method4(
std::move(p_param0), 
std::move(p_param1));
      return true;
    }
    case internal::kConformanceTestInterface_Method5_Name: {
#if BUILDFLAG(MOJO_TRACE_ENABLED)
      TRACE_EVENT1(
          "mojom",
          "(Impl)<class 'jinja2::utils::Namespace'>::ConformanceTestInterface::Method5",
          "message", message->name());
#endif
      base::TaskAnnotator::ScopedSetIpcProgramCounter scoped_ipc_pc(
          base::GetProgramCounter());
      mojo::internal::MessageDispatchContext context(message);
      if (!message->is_serialized()) {
        auto context = message->TakeUnserializedContext<
            ConformanceTestInterfaceProxy_Method5_Message>();
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
      internal::ConformanceTestInterface_Method5_Params_Data* params =
          reinterpret_cast<internal::ConformanceTestInterface_Method5_Params_Data*>(
              message->mutable_payload());
      
      mojo::internal::SerializationContext serialization_context;
      serialization_context.TakeHandlesFromMessage(message);
      bool success = true;
      StructEPtr p_param0{};
      mojo::ScopedDataPipeProducerHandle p_param1{};
      ConformanceTestInterface_Method5_ParamsDataView input_data_view(params, &serialization_context);
      
      if (!input_data_view.ReadParam0(&p_param0))
        success = false;
      p_param1 = input_data_view.TakeParam1();
      if (!success) {
        ReportValidationErrorForMessage(
            message,
            mojo::internal::VALIDATION_ERROR_DESERIALIZATION_FAILED,
            ConformanceTestInterface::Name_, 5, false);
        return false;
      }
      // A null |impl| means no implementation was bound.
      DCHECK(impl);
      impl->Method5(
std::move(p_param0), 
std::move(p_param1));
      return true;
    }
    case internal::kConformanceTestInterface_Method6_Name: {
#if BUILDFLAG(MOJO_TRACE_ENABLED)
      TRACE_EVENT1(
          "mojom",
          "(Impl)<class 'jinja2::utils::Namespace'>::ConformanceTestInterface::Method6",
          "message", message->name());
#endif
      base::TaskAnnotator::ScopedSetIpcProgramCounter scoped_ipc_pc(
          base::GetProgramCounter());
      mojo::internal::MessageDispatchContext context(message);
      if (!message->is_serialized()) {
        auto context = message->TakeUnserializedContext<
            ConformanceTestInterfaceProxy_Method6_Message>();
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
      internal::ConformanceTestInterface_Method6_Params_Data* params =
          reinterpret_cast<internal::ConformanceTestInterface_Method6_Params_Data*>(
              message->mutable_payload());
      
      mojo::internal::SerializationContext serialization_context;
      serialization_context.TakeHandlesFromMessage(message);
      bool success = true;
      WTF::Vector<WTF::Vector<uint8_t>> p_param0{};
      ConformanceTestInterface_Method6_ParamsDataView input_data_view(params, &serialization_context);
      
      if (!input_data_view.ReadParam0(&p_param0))
        success = false;
      if (!success) {
        ReportValidationErrorForMessage(
            message,
            mojo::internal::VALIDATION_ERROR_DESERIALIZATION_FAILED,
            ConformanceTestInterface::Name_, 6, false);
        return false;
      }
      // A null |impl| means no implementation was bound.
      DCHECK(impl);
      impl->Method6(
std::move(p_param0));
      return true;
    }
    case internal::kConformanceTestInterface_Method7_Name: {
#if BUILDFLAG(MOJO_TRACE_ENABLED)
      TRACE_EVENT1(
          "mojom",
          "(Impl)<class 'jinja2::utils::Namespace'>::ConformanceTestInterface::Method7",
          "message", message->name());
#endif
      base::TaskAnnotator::ScopedSetIpcProgramCounter scoped_ipc_pc(
          base::GetProgramCounter());
      mojo::internal::MessageDispatchContext context(message);
      if (!message->is_serialized()) {
        auto context = message->TakeUnserializedContext<
            ConformanceTestInterfaceProxy_Method7_Message>();
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
      internal::ConformanceTestInterface_Method7_Params_Data* params =
          reinterpret_cast<internal::ConformanceTestInterface_Method7_Params_Data*>(
              message->mutable_payload());
      
      mojo::internal::SerializationContext serialization_context;
      serialization_context.TakeHandlesFromMessage(message);
      bool success = true;
      StructFPtr p_param0{};
      WTF::Vector<base::Optional<WTF::Vector<uint8_t>>> p_param1{};
      ConformanceTestInterface_Method7_ParamsDataView input_data_view(params, &serialization_context);
      
      if (!input_data_view.ReadParam0(&p_param0))
        success = false;
      if (!input_data_view.ReadParam1(&p_param1))
        success = false;
      if (!success) {
        ReportValidationErrorForMessage(
            message,
            mojo::internal::VALIDATION_ERROR_DESERIALIZATION_FAILED,
            ConformanceTestInterface::Name_, 7, false);
        return false;
      }
      // A null |impl| means no implementation was bound.
      DCHECK(impl);
      impl->Method7(
std::move(p_param0), 
std::move(p_param1));
      return true;
    }
    case internal::kConformanceTestInterface_Method8_Name: {
#if BUILDFLAG(MOJO_TRACE_ENABLED)
      TRACE_EVENT1(
          "mojom",
          "(Impl)<class 'jinja2::utils::Namespace'>::ConformanceTestInterface::Method8",
          "message", message->name());
#endif
      base::TaskAnnotator::ScopedSetIpcProgramCounter scoped_ipc_pc(
          base::GetProgramCounter());
      mojo::internal::MessageDispatchContext context(message);
      if (!message->is_serialized()) {
        auto context = message->TakeUnserializedContext<
            ConformanceTestInterfaceProxy_Method8_Message>();
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
      internal::ConformanceTestInterface_Method8_Params_Data* params =
          reinterpret_cast<internal::ConformanceTestInterface_Method8_Params_Data*>(
              message->mutable_payload());
      
      mojo::internal::SerializationContext serialization_context;
      serialization_context.TakeHandlesFromMessage(message);
      bool success = true;
      WTF::Vector<base::Optional<WTF::Vector<WTF::String>>> p_param0{};
      ConformanceTestInterface_Method8_ParamsDataView input_data_view(params, &serialization_context);
      
      if (!input_data_view.ReadParam0(&p_param0))
        success = false;
      if (!success) {
        ReportValidationErrorForMessage(
            message,
            mojo::internal::VALIDATION_ERROR_DESERIALIZATION_FAILED,
            ConformanceTestInterface::Name_, 8, false);
        return false;
      }
      // A null |impl| means no implementation was bound.
      DCHECK(impl);
      impl->Method8(
std::move(p_param0));
      return true;
    }
    case internal::kConformanceTestInterface_Method9_Name: {
#if BUILDFLAG(MOJO_TRACE_ENABLED)
      TRACE_EVENT1(
          "mojom",
          "(Impl)<class 'jinja2::utils::Namespace'>::ConformanceTestInterface::Method9",
          "message", message->name());
#endif
      base::TaskAnnotator::ScopedSetIpcProgramCounter scoped_ipc_pc(
          base::GetProgramCounter());
      mojo::internal::MessageDispatchContext context(message);
      if (!message->is_serialized()) {
        auto context = message->TakeUnserializedContext<
            ConformanceTestInterfaceProxy_Method9_Message>();
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
      internal::ConformanceTestInterface_Method9_Params_Data* params =
          reinterpret_cast<internal::ConformanceTestInterface_Method9_Params_Data*>(
              message->mutable_payload());
      
      mojo::internal::SerializationContext serialization_context;
      serialization_context.TakeHandlesFromMessage(message);
      bool success = true;
      base::Optional<WTF::Vector<WTF::Vector<mojo::ScopedHandle>>> p_param0{};
      ConformanceTestInterface_Method9_ParamsDataView input_data_view(params, &serialization_context);
      
      if (!input_data_view.ReadParam0(&p_param0))
        success = false;
      if (!success) {
        ReportValidationErrorForMessage(
            message,
            mojo::internal::VALIDATION_ERROR_DESERIALIZATION_FAILED,
            ConformanceTestInterface::Name_, 9, false);
        return false;
      }
      // A null |impl| means no implementation was bound.
      DCHECK(impl);
      impl->Method9(
std::move(p_param0));
      return true;
    }
    case internal::kConformanceTestInterface_Method10_Name: {
#if BUILDFLAG(MOJO_TRACE_ENABLED)
      TRACE_EVENT1(
          "mojom",
          "(Impl)<class 'jinja2::utils::Namespace'>::ConformanceTestInterface::Method10",
          "message", message->name());
#endif
      base::TaskAnnotator::ScopedSetIpcProgramCounter scoped_ipc_pc(
          base::GetProgramCounter());
      mojo::internal::MessageDispatchContext context(message);
      if (!message->is_serialized()) {
        auto context = message->TakeUnserializedContext<
            ConformanceTestInterfaceProxy_Method10_Message>();
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
      internal::ConformanceTestInterface_Method10_Params_Data* params =
          reinterpret_cast<internal::ConformanceTestInterface_Method10_Params_Data*>(
              message->mutable_payload());
      
      mojo::internal::SerializationContext serialization_context;
      serialization_context.TakeHandlesFromMessage(message);
      bool success = true;
      WTF::HashMap<WTF::String, uint8_t> p_param0{};
      ConformanceTestInterface_Method10_ParamsDataView input_data_view(params, &serialization_context);
      
      if (!input_data_view.ReadParam0(&p_param0))
        success = false;
      if (!success) {
        ReportValidationErrorForMessage(
            message,
            mojo::internal::VALIDATION_ERROR_DESERIALIZATION_FAILED,
            ConformanceTestInterface::Name_, 10, false);
        return false;
      }
      // A null |impl| means no implementation was bound.
      DCHECK(impl);
      impl->Method10(
std::move(p_param0));
      return true;
    }
    case internal::kConformanceTestInterface_Method11_Name: {
#if BUILDFLAG(MOJO_TRACE_ENABLED)
      TRACE_EVENT1(
          "mojom",
          "(Impl)<class 'jinja2::utils::Namespace'>::ConformanceTestInterface::Method11",
          "message", message->name());
#endif
      base::TaskAnnotator::ScopedSetIpcProgramCounter scoped_ipc_pc(
          base::GetProgramCounter());
      mojo::internal::MessageDispatchContext context(message);
      if (!message->is_serialized()) {
        auto context = message->TakeUnserializedContext<
            ConformanceTestInterfaceProxy_Method11_Message>();
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
      internal::ConformanceTestInterface_Method11_Params_Data* params =
          reinterpret_cast<internal::ConformanceTestInterface_Method11_Params_Data*>(
              message->mutable_payload());
      
      mojo::internal::SerializationContext serialization_context;
      serialization_context.TakeHandlesFromMessage(message);
      bool success = true;
      StructGPtr p_param0{};
      ConformanceTestInterface_Method11_ParamsDataView input_data_view(params, &serialization_context);
      
      if (!input_data_view.ReadParam0(&p_param0))
        success = false;
      if (!success) {
        ReportValidationErrorForMessage(
            message,
            mojo::internal::VALIDATION_ERROR_DESERIALIZATION_FAILED,
            ConformanceTestInterface::Name_, 11, false);
        return false;
      }
      // A null |impl| means no implementation was bound.
      DCHECK(impl);
      impl->Method11(
std::move(p_param0));
      return true;
    }
    case internal::kConformanceTestInterface_Method12_Name: {
      break;
    }
    case internal::kConformanceTestInterface_Method13_Name: {
#if BUILDFLAG(MOJO_TRACE_ENABLED)
      TRACE_EVENT1(
          "mojom",
          "(Impl)<class 'jinja2::utils::Namespace'>::ConformanceTestInterface::Method13",
          "message", message->name());
#endif
      base::TaskAnnotator::ScopedSetIpcProgramCounter scoped_ipc_pc(
          base::GetProgramCounter());
      mojo::internal::MessageDispatchContext context(message);
      if (!message->is_serialized()) {
        auto context = message->TakeUnserializedContext<
            ConformanceTestInterfaceProxy_Method13_Message>();
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
      internal::ConformanceTestInterface_Method13_Params_Data* params =
          reinterpret_cast<internal::ConformanceTestInterface_Method13_Params_Data*>(
              message->mutable_payload());
      
      mojo::internal::SerializationContext serialization_context;
      serialization_context.TakeHandlesFromMessage(message);
      bool success = true;
      InterfaceAPtr p_param0{};
      uint32_t p_param1{};
      InterfaceAPtr p_param2{};
      ConformanceTestInterface_Method13_ParamsDataView input_data_view(params, &serialization_context);
      
      p_param0 =
          input_data_view.TakeParam0<decltype(p_param0)>();
      p_param1 = input_data_view.param1();
      p_param2 =
          input_data_view.TakeParam2<decltype(p_param2)>();
      if (!success) {
        ReportValidationErrorForMessage(
            message,
            mojo::internal::VALIDATION_ERROR_DESERIALIZATION_FAILED,
            ConformanceTestInterface::Name_, 13, false);
        return false;
      }
      // A null |impl| means no implementation was bound.
      DCHECK(impl);
      impl->Method13(
std::move(p_param0), 
std::move(p_param1), 
std::move(p_param2));
      return true;
    }
    case internal::kConformanceTestInterface_Method14_Name: {
#if BUILDFLAG(MOJO_TRACE_ENABLED)
      TRACE_EVENT1(
          "mojom",
          "(Impl)<class 'jinja2::utils::Namespace'>::ConformanceTestInterface::Method14",
          "message", message->name());
#endif
      base::TaskAnnotator::ScopedSetIpcProgramCounter scoped_ipc_pc(
          base::GetProgramCounter());
      mojo::internal::MessageDispatchContext context(message);
      if (!message->is_serialized()) {
        auto context = message->TakeUnserializedContext<
            ConformanceTestInterfaceProxy_Method14_Message>();
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
      internal::ConformanceTestInterface_Method14_Params_Data* params =
          reinterpret_cast<internal::ConformanceTestInterface_Method14_Params_Data*>(
              message->mutable_payload());
      
      mojo::internal::SerializationContext serialization_context;
      serialization_context.TakeHandlesFromMessage(message);
      bool success = true;
      EnumA p_param0{};
      EnumB p_param1{};
      ConformanceTestInterface_Method14_ParamsDataView input_data_view(params, &serialization_context);
      
      if (!input_data_view.ReadParam0(&p_param0))
        success = false;
      if (!input_data_view.ReadParam1(&p_param1))
        success = false;
      if (!success) {
        ReportValidationErrorForMessage(
            message,
            mojo::internal::VALIDATION_ERROR_DESERIALIZATION_FAILED,
            ConformanceTestInterface::Name_, 14, false);
        return false;
      }
      // A null |impl| means no implementation was bound.
      DCHECK(impl);
      impl->Method14(
std::move(p_param0), 
std::move(p_param1));
      return true;
    }
    case internal::kConformanceTestInterface_Method15_Name: {
#if BUILDFLAG(MOJO_TRACE_ENABLED)
      TRACE_EVENT1(
          "mojom",
          "(Impl)<class 'jinja2::utils::Namespace'>::ConformanceTestInterface::Method15",
          "message", message->name());
#endif
      base::TaskAnnotator::ScopedSetIpcProgramCounter scoped_ipc_pc(
          base::GetProgramCounter());
      mojo::internal::MessageDispatchContext context(message);
      if (!message->is_serialized()) {
        auto context = message->TakeUnserializedContext<
            ConformanceTestInterfaceProxy_Method15_Message>();
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
      internal::ConformanceTestInterface_Method15_Params_Data* params =
          reinterpret_cast<internal::ConformanceTestInterface_Method15_Params_Data*>(
              message->mutable_payload());
      
      mojo::internal::SerializationContext serialization_context;
      serialization_context.TakeHandlesFromMessage(message);
      bool success = true;
      base::Optional<WTF::Vector<EnumA>> p_param0{};
      base::Optional<WTF::Vector<EnumB>> p_param1{};
      ConformanceTestInterface_Method15_ParamsDataView input_data_view(params, &serialization_context);
      
      if (!input_data_view.ReadParam0(&p_param0))
        success = false;
      if (!input_data_view.ReadParam1(&p_param1))
        success = false;
      if (!success) {
        ReportValidationErrorForMessage(
            message,
            mojo::internal::VALIDATION_ERROR_DESERIALIZATION_FAILED,
            ConformanceTestInterface::Name_, 15, false);
        return false;
      }
      // A null |impl| means no implementation was bound.
      DCHECK(impl);
      impl->Method15(
std::move(p_param0), 
std::move(p_param1));
      return true;
    }
    case internal::kConformanceTestInterface_Method16_Name: {
#if BUILDFLAG(MOJO_TRACE_ENABLED)
      TRACE_EVENT1(
          "mojom",
          "(Impl)<class 'jinja2::utils::Namespace'>::ConformanceTestInterface::Method16",
          "message", message->name());
#endif
      base::TaskAnnotator::ScopedSetIpcProgramCounter scoped_ipc_pc(
          base::GetProgramCounter());
      mojo::internal::MessageDispatchContext context(message);
      if (!message->is_serialized()) {
        auto context = message->TakeUnserializedContext<
            ConformanceTestInterfaceProxy_Method16_Message>();
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
      internal::ConformanceTestInterface_Method16_Params_Data* params =
          reinterpret_cast<internal::ConformanceTestInterface_Method16_Params_Data*>(
              message->mutable_payload());
      
      mojo::internal::SerializationContext serialization_context;
      serialization_context.TakeHandlesFromMessage(message);
      bool success = true;
      base::Optional<WTF::HashMap<EnumA, EnumA>> p_param0{};
      ConformanceTestInterface_Method16_ParamsDataView input_data_view(params, &serialization_context);
      
      if (!input_data_view.ReadParam0(&p_param0))
        success = false;
      if (!success) {
        ReportValidationErrorForMessage(
            message,
            mojo::internal::VALIDATION_ERROR_DESERIALIZATION_FAILED,
            ConformanceTestInterface::Name_, 16, false);
        return false;
      }
      // A null |impl| means no implementation was bound.
      DCHECK(impl);
      impl->Method16(
std::move(p_param0));
      return true;
    }
    case internal::kConformanceTestInterface_Method17_Name: {
#if BUILDFLAG(MOJO_TRACE_ENABLED)
      TRACE_EVENT1(
          "mojom",
          "(Impl)<class 'jinja2::utils::Namespace'>::ConformanceTestInterface::Method17",
          "message", message->name());
#endif
      base::TaskAnnotator::ScopedSetIpcProgramCounter scoped_ipc_pc(
          base::GetProgramCounter());
      mojo::internal::MessageDispatchContext context(message);
      if (!message->is_serialized()) {
        auto context = message->TakeUnserializedContext<
            ConformanceTestInterfaceProxy_Method17_Message>();
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
      internal::ConformanceTestInterface_Method17_Params_Data* params =
          reinterpret_cast<internal::ConformanceTestInterface_Method17_Params_Data*>(
              message->mutable_payload());
      
      mojo::internal::SerializationContext serialization_context;
      serialization_context.TakeHandlesFromMessage(message);
      bool success = true;
      WTF::Vector<mojo::PendingRemote<InterfaceA>> p_param0{};
      ConformanceTestInterface_Method17_ParamsDataView input_data_view(params, &serialization_context);
      
      if (!input_data_view.ReadParam0(&p_param0))
        success = false;
      if (!success) {
        ReportValidationErrorForMessage(
            message,
            mojo::internal::VALIDATION_ERROR_DESERIALIZATION_FAILED,
            ConformanceTestInterface::Name_, 17, false);
        return false;
      }
      // A null |impl| means no implementation was bound.
      DCHECK(impl);
      impl->Method17(
std::move(p_param0));
      return true;
    }
    case internal::kConformanceTestInterface_Method18_Name: {
#if BUILDFLAG(MOJO_TRACE_ENABLED)
      TRACE_EVENT1(
          "mojom",
          "(Impl)<class 'jinja2::utils::Namespace'>::ConformanceTestInterface::Method18",
          "message", message->name());
#endif
      base::TaskAnnotator::ScopedSetIpcProgramCounter scoped_ipc_pc(
          base::GetProgramCounter());
      mojo::internal::MessageDispatchContext context(message);
      if (!message->is_serialized()) {
        auto context = message->TakeUnserializedContext<
            ConformanceTestInterfaceProxy_Method18_Message>();
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
      internal::ConformanceTestInterface_Method18_Params_Data* params =
          reinterpret_cast<internal::ConformanceTestInterface_Method18_Params_Data*>(
              message->mutable_payload());
      
      mojo::internal::SerializationContext serialization_context;
      serialization_context.TakeHandlesFromMessage(message);
      bool success = true;
      UnionAPtr p_param0{};
      ConformanceTestInterface_Method18_ParamsDataView input_data_view(params, &serialization_context);
      
      if (!input_data_view.ReadParam0(&p_param0))
        success = false;
      if (!success) {
        ReportValidationErrorForMessage(
            message,
            mojo::internal::VALIDATION_ERROR_DESERIALIZATION_FAILED,
            ConformanceTestInterface::Name_, 18, false);
        return false;
      }
      // A null |impl| means no implementation was bound.
      DCHECK(impl);
      impl->Method18(
std::move(p_param0));
      return true;
    }
    case internal::kConformanceTestInterface_Method19_Name: {
#if BUILDFLAG(MOJO_TRACE_ENABLED)
      TRACE_EVENT1(
          "mojom",
          "(Impl)<class 'jinja2::utils::Namespace'>::ConformanceTestInterface::Method19",
          "message", message->name());
#endif
      base::TaskAnnotator::ScopedSetIpcProgramCounter scoped_ipc_pc(
          base::GetProgramCounter());
      mojo::internal::MessageDispatchContext context(message);
      if (!message->is_serialized()) {
        auto context = message->TakeUnserializedContext<
            ConformanceTestInterfaceProxy_Method19_Message>();
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
      internal::ConformanceTestInterface_Method19_Params_Data* params =
          reinterpret_cast<internal::ConformanceTestInterface_Method19_Params_Data*>(
              message->mutable_payload());
      
      mojo::internal::SerializationContext serialization_context;
      serialization_context.TakeHandlesFromMessage(message);
      bool success = true;
      RecursivePtr p_recursive{};
      ConformanceTestInterface_Method19_ParamsDataView input_data_view(params, &serialization_context);
      
      if (!input_data_view.ReadRecursive(&p_recursive))
        success = false;
      if (!success) {
        ReportValidationErrorForMessage(
            message,
            mojo::internal::VALIDATION_ERROR_DESERIALIZATION_FAILED,
            ConformanceTestInterface::Name_, 19, false);
        return false;
      }
      // A null |impl| means no implementation was bound.
      DCHECK(impl);
      impl->Method19(
std::move(p_recursive));
      return true;
    }
    case internal::kConformanceTestInterface_Method20_Name: {
#if BUILDFLAG(MOJO_TRACE_ENABLED)
      TRACE_EVENT1(
          "mojom",
          "(Impl)<class 'jinja2::utils::Namespace'>::ConformanceTestInterface::Method20",
          "message", message->name());
#endif
      base::TaskAnnotator::ScopedSetIpcProgramCounter scoped_ipc_pc(
          base::GetProgramCounter());
      mojo::internal::MessageDispatchContext context(message);
      if (!message->is_serialized()) {
        auto context = message->TakeUnserializedContext<
            ConformanceTestInterfaceProxy_Method20_Message>();
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
      internal::ConformanceTestInterface_Method20_Params_Data* params =
          reinterpret_cast<internal::ConformanceTestInterface_Method20_Params_Data*>(
              message->mutable_payload());
      
      mojo::internal::SerializationContext serialization_context;
      serialization_context.TakeHandlesFromMessage(message);
      bool success = true;
      WTF::HashMap<StructBPtr, uint8_t> p_param0{};
      ConformanceTestInterface_Method20_ParamsDataView input_data_view(params, &serialization_context);
      
      if (!input_data_view.ReadParam0(&p_param0))
        success = false;
      if (!success) {
        ReportValidationErrorForMessage(
            message,
            mojo::internal::VALIDATION_ERROR_DESERIALIZATION_FAILED,
            ConformanceTestInterface::Name_, 20, false);
        return false;
      }
      // A null |impl| means no implementation was bound.
      DCHECK(impl);
      impl->Method20(
std::move(p_param0));
      return true;
    }
    case internal::kConformanceTestInterface_Method21_Name: {
#if BUILDFLAG(MOJO_TRACE_ENABLED)
      TRACE_EVENT1(
          "mojom",
          "(Impl)<class 'jinja2::utils::Namespace'>::ConformanceTestInterface::Method21",
          "message", message->name());
#endif
      base::TaskAnnotator::ScopedSetIpcProgramCounter scoped_ipc_pc(
          base::GetProgramCounter());
      mojo::internal::MessageDispatchContext context(message);
      if (!message->is_serialized()) {
        auto context = message->TakeUnserializedContext<
            ConformanceTestInterfaceProxy_Method21_Message>();
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
      internal::ConformanceTestInterface_Method21_Params_Data* params =
          reinterpret_cast<internal::ConformanceTestInterface_Method21_Params_Data*>(
              message->mutable_payload());
      
      mojo::internal::SerializationContext serialization_context;
      serialization_context.TakeHandlesFromMessage(message);
      bool success = true;
      ExtensibleEmptyEnum p_param0{};
      ConformanceTestInterface_Method21_ParamsDataView input_data_view(params, &serialization_context);
      
      if (!input_data_view.ReadParam0(&p_param0))
        success = false;
      if (!success) {
        ReportValidationErrorForMessage(
            message,
            mojo::internal::VALIDATION_ERROR_DESERIALIZATION_FAILED,
            ConformanceTestInterface::Name_, 21, false);
        return false;
      }
      // A null |impl| means no implementation was bound.
      DCHECK(impl);
      impl->Method21(
std::move(p_param0));
      return true;
    }
    case internal::kConformanceTestInterface_Method22_Name: {
#if BUILDFLAG(MOJO_TRACE_ENABLED)
      TRACE_EVENT1(
          "mojom",
          "(Impl)<class 'jinja2::utils::Namespace'>::ConformanceTestInterface::Method22",
          "message", message->name());
#endif
      base::TaskAnnotator::ScopedSetIpcProgramCounter scoped_ipc_pc(
          base::GetProgramCounter());
      mojo::internal::MessageDispatchContext context(message);
      if (!message->is_serialized()) {
        auto context = message->TakeUnserializedContext<
            ConformanceTestInterfaceProxy_Method22_Message>();
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
      internal::ConformanceTestInterface_Method22_Params_Data* params =
          reinterpret_cast<internal::ConformanceTestInterface_Method22_Params_Data*>(
              message->mutable_payload());
      
      mojo::internal::SerializationContext serialization_context;
      serialization_context.TakeHandlesFromMessage(message);
      bool success = true;
      EmptyEnum p_param0{};
      ConformanceTestInterface_Method22_ParamsDataView input_data_view(params, &serialization_context);
      
      if (!input_data_view.ReadParam0(&p_param0))
        success = false;
      if (!success) {
        ReportValidationErrorForMessage(
            message,
            mojo::internal::VALIDATION_ERROR_DESERIALIZATION_FAILED,
            ConformanceTestInterface::Name_, 22, false);
        return false;
      }
      // A null |impl| means no implementation was bound.
      DCHECK(impl);
      impl->Method22(
std::move(p_param0));
      return true;
    }
  }
  return false;
}

// static
bool ConformanceTestInterfaceStubDispatch::AcceptWithResponder(
    ConformanceTestInterface* impl,
    mojo::Message* message,
    std::unique_ptr<mojo::MessageReceiverWithStatus> responder) {
  switch (message->header()->name) {
    case internal::kConformanceTestInterface_Method0_Name: {
      break;
    }
    case internal::kConformanceTestInterface_Method1_Name: {
      break;
    }
    case internal::kConformanceTestInterface_Method2_Name: {
      break;
    }
    case internal::kConformanceTestInterface_Method3_Name: {
      break;
    }
    case internal::kConformanceTestInterface_Method4_Name: {
      break;
    }
    case internal::kConformanceTestInterface_Method5_Name: {
      break;
    }
    case internal::kConformanceTestInterface_Method6_Name: {
      break;
    }
    case internal::kConformanceTestInterface_Method7_Name: {
      break;
    }
    case internal::kConformanceTestInterface_Method8_Name: {
      break;
    }
    case internal::kConformanceTestInterface_Method9_Name: {
      break;
    }
    case internal::kConformanceTestInterface_Method10_Name: {
      break;
    }
    case internal::kConformanceTestInterface_Method11_Name: {
      break;
    }
    case internal::kConformanceTestInterface_Method12_Name: {
#if BUILDFLAG(MOJO_TRACE_ENABLED)
      TRACE_EVENT1(
          "mojom",
          "(Impl)<class 'jinja2::utils::Namespace'>::ConformanceTestInterface::Method12",
          "message", message->name());
#endif
      base::TaskAnnotator::ScopedSetIpcProgramCounter scoped_ipc_pc(
          base::GetProgramCounter());
      mojo::internal::MessageDispatchContext context(message);
      if (!message->is_serialized()) {
        auto context = message->TakeUnserializedContext<
            ConformanceTestInterfaceProxy_Method12_Message>();
        if (!context) {
          // The Message was not of the expected type. It may be a valid message
          // which was built using a different variant of these bindings. Force
          // serialization before dispatch in this case.
          message->SerializeIfNecessary();
        } else {
          ConformanceTestInterface::Method12Callback callback =
              ConformanceTestInterface_Method12_ProxyToResponder::CreateCallback(
                  message->request_id(),
                  message->has_flag(mojo::Message::kFlagIsSync),
                  std::move(responder));
          context->Dispatch(impl, std::move(callback));
          return true;
        }
      }

      internal::ConformanceTestInterface_Method12_Params_Data* params =
          reinterpret_cast<
              internal::ConformanceTestInterface_Method12_Params_Data*>(
                  message->mutable_payload());
      
      mojo::internal::SerializationContext serialization_context;
      serialization_context.TakeHandlesFromMessage(message);
      bool success = true;
      float p_param0{};
      ConformanceTestInterface_Method12_ParamsDataView input_data_view(params, &serialization_context);
      
      p_param0 = input_data_view.param0();
      if (!success) {
        ReportValidationErrorForMessage(
            message,
            mojo::internal::VALIDATION_ERROR_DESERIALIZATION_FAILED,
            ConformanceTestInterface::Name_, 12, false);
        return false;
      }
      ConformanceTestInterface::Method12Callback callback =
          ConformanceTestInterface_Method12_ProxyToResponder::CreateCallback(
              message->request_id(),
              message->has_flag(mojo::Message::kFlagIsSync),
              std::move(responder));
      // A null |impl| means no implementation was bound.
      DCHECK(impl);
      impl->Method12(
std::move(p_param0), std::move(callback));
      return true;
    }
    case internal::kConformanceTestInterface_Method13_Name: {
      break;
    }
    case internal::kConformanceTestInterface_Method14_Name: {
      break;
    }
    case internal::kConformanceTestInterface_Method15_Name: {
      break;
    }
    case internal::kConformanceTestInterface_Method16_Name: {
      break;
    }
    case internal::kConformanceTestInterface_Method17_Name: {
      break;
    }
    case internal::kConformanceTestInterface_Method18_Name: {
      break;
    }
    case internal::kConformanceTestInterface_Method19_Name: {
      break;
    }
    case internal::kConformanceTestInterface_Method20_Name: {
      break;
    }
    case internal::kConformanceTestInterface_Method21_Name: {
      break;
    }
    case internal::kConformanceTestInterface_Method22_Name: {
      break;
    }
  }
  return false;
}

bool ConformanceTestInterfaceRequestValidator::Accept(mojo::Message* message) {
  if (!message->is_serialized() ||
      mojo::internal::ControlMessageHandler::IsControlMessage(message)) {
    return true;
  }

  mojo::internal::ValidationContext validation_context(
    message->payload(), message->payload_num_bytes(),
    message->handles()->size(), message->payload_num_interface_ids(), message,
    "ConformanceTestInterface RequestValidator");

  switch (message->header()->name) {
    case internal::kConformanceTestInterface_Method0_Name: {
      if (!mojo::internal::ValidateMessageIsRequestWithoutResponse(
              message, &validation_context)) {
        return false;
      }
      if (!mojo::internal::ValidateMessagePayload<
               internal::ConformanceTestInterface_Method0_Params_Data>(
                  message, &validation_context)) {
        return false;
      }
      return true;
    }
    case internal::kConformanceTestInterface_Method1_Name: {
      if (!mojo::internal::ValidateMessageIsRequestWithoutResponse(
              message, &validation_context)) {
        return false;
      }
      if (!mojo::internal::ValidateMessagePayload<
               internal::ConformanceTestInterface_Method1_Params_Data>(
                  message, &validation_context)) {
        return false;
      }
      return true;
    }
    case internal::kConformanceTestInterface_Method2_Name: {
      if (!mojo::internal::ValidateMessageIsRequestWithoutResponse(
              message, &validation_context)) {
        return false;
      }
      if (!mojo::internal::ValidateMessagePayload<
               internal::ConformanceTestInterface_Method2_Params_Data>(
                  message, &validation_context)) {
        return false;
      }
      return true;
    }
    case internal::kConformanceTestInterface_Method3_Name: {
      if (!mojo::internal::ValidateMessageIsRequestWithoutResponse(
              message, &validation_context)) {
        return false;
      }
      if (!mojo::internal::ValidateMessagePayload<
               internal::ConformanceTestInterface_Method3_Params_Data>(
                  message, &validation_context)) {
        return false;
      }
      return true;
    }
    case internal::kConformanceTestInterface_Method4_Name: {
      if (!mojo::internal::ValidateMessageIsRequestWithoutResponse(
              message, &validation_context)) {
        return false;
      }
      if (!mojo::internal::ValidateMessagePayload<
               internal::ConformanceTestInterface_Method4_Params_Data>(
                  message, &validation_context)) {
        return false;
      }
      return true;
    }
    case internal::kConformanceTestInterface_Method5_Name: {
      if (!mojo::internal::ValidateMessageIsRequestWithoutResponse(
              message, &validation_context)) {
        return false;
      }
      if (!mojo::internal::ValidateMessagePayload<
               internal::ConformanceTestInterface_Method5_Params_Data>(
                  message, &validation_context)) {
        return false;
      }
      return true;
    }
    case internal::kConformanceTestInterface_Method6_Name: {
      if (!mojo::internal::ValidateMessageIsRequestWithoutResponse(
              message, &validation_context)) {
        return false;
      }
      if (!mojo::internal::ValidateMessagePayload<
               internal::ConformanceTestInterface_Method6_Params_Data>(
                  message, &validation_context)) {
        return false;
      }
      return true;
    }
    case internal::kConformanceTestInterface_Method7_Name: {
      if (!mojo::internal::ValidateMessageIsRequestWithoutResponse(
              message, &validation_context)) {
        return false;
      }
      if (!mojo::internal::ValidateMessagePayload<
               internal::ConformanceTestInterface_Method7_Params_Data>(
                  message, &validation_context)) {
        return false;
      }
      return true;
    }
    case internal::kConformanceTestInterface_Method8_Name: {
      if (!mojo::internal::ValidateMessageIsRequestWithoutResponse(
              message, &validation_context)) {
        return false;
      }
      if (!mojo::internal::ValidateMessagePayload<
               internal::ConformanceTestInterface_Method8_Params_Data>(
                  message, &validation_context)) {
        return false;
      }
      return true;
    }
    case internal::kConformanceTestInterface_Method9_Name: {
      if (!mojo::internal::ValidateMessageIsRequestWithoutResponse(
              message, &validation_context)) {
        return false;
      }
      if (!mojo::internal::ValidateMessagePayload<
               internal::ConformanceTestInterface_Method9_Params_Data>(
                  message, &validation_context)) {
        return false;
      }
      return true;
    }
    case internal::kConformanceTestInterface_Method10_Name: {
      if (!mojo::internal::ValidateMessageIsRequestWithoutResponse(
              message, &validation_context)) {
        return false;
      }
      if (!mojo::internal::ValidateMessagePayload<
               internal::ConformanceTestInterface_Method10_Params_Data>(
                  message, &validation_context)) {
        return false;
      }
      return true;
    }
    case internal::kConformanceTestInterface_Method11_Name: {
      if (!mojo::internal::ValidateMessageIsRequestWithoutResponse(
              message, &validation_context)) {
        return false;
      }
      if (!mojo::internal::ValidateMessagePayload<
               internal::ConformanceTestInterface_Method11_Params_Data>(
                  message, &validation_context)) {
        return false;
      }
      return true;
    }
    case internal::kConformanceTestInterface_Method12_Name: {
      if (!mojo::internal::ValidateMessageIsRequestExpectingResponse(
              message, &validation_context)) {
        return false;
      }
      if (!mojo::internal::ValidateMessagePayload<
               internal::ConformanceTestInterface_Method12_Params_Data>(
                  message, &validation_context)) {
        return false;
      }
      return true;
    }
    case internal::kConformanceTestInterface_Method13_Name: {
      if (!mojo::internal::ValidateMessageIsRequestWithoutResponse(
              message, &validation_context)) {
        return false;
      }
      if (!mojo::internal::ValidateMessagePayload<
               internal::ConformanceTestInterface_Method13_Params_Data>(
                  message, &validation_context)) {
        return false;
      }
      return true;
    }
    case internal::kConformanceTestInterface_Method14_Name: {
      if (!mojo::internal::ValidateMessageIsRequestWithoutResponse(
              message, &validation_context)) {
        return false;
      }
      if (!mojo::internal::ValidateMessagePayload<
               internal::ConformanceTestInterface_Method14_Params_Data>(
                  message, &validation_context)) {
        return false;
      }
      return true;
    }
    case internal::kConformanceTestInterface_Method15_Name: {
      if (!mojo::internal::ValidateMessageIsRequestWithoutResponse(
              message, &validation_context)) {
        return false;
      }
      if (!mojo::internal::ValidateMessagePayload<
               internal::ConformanceTestInterface_Method15_Params_Data>(
                  message, &validation_context)) {
        return false;
      }
      return true;
    }
    case internal::kConformanceTestInterface_Method16_Name: {
      if (!mojo::internal::ValidateMessageIsRequestWithoutResponse(
              message, &validation_context)) {
        return false;
      }
      if (!mojo::internal::ValidateMessagePayload<
               internal::ConformanceTestInterface_Method16_Params_Data>(
                  message, &validation_context)) {
        return false;
      }
      return true;
    }
    case internal::kConformanceTestInterface_Method17_Name: {
      if (!mojo::internal::ValidateMessageIsRequestWithoutResponse(
              message, &validation_context)) {
        return false;
      }
      if (!mojo::internal::ValidateMessagePayload<
               internal::ConformanceTestInterface_Method17_Params_Data>(
                  message, &validation_context)) {
        return false;
      }
      return true;
    }
    case internal::kConformanceTestInterface_Method18_Name: {
      if (!mojo::internal::ValidateMessageIsRequestWithoutResponse(
              message, &validation_context)) {
        return false;
      }
      if (!mojo::internal::ValidateMessagePayload<
               internal::ConformanceTestInterface_Method18_Params_Data>(
                  message, &validation_context)) {
        return false;
      }
      return true;
    }
    case internal::kConformanceTestInterface_Method19_Name: {
      if (!mojo::internal::ValidateMessageIsRequestWithoutResponse(
              message, &validation_context)) {
        return false;
      }
      if (!mojo::internal::ValidateMessagePayload<
               internal::ConformanceTestInterface_Method19_Params_Data>(
                  message, &validation_context)) {
        return false;
      }
      return true;
    }
    case internal::kConformanceTestInterface_Method20_Name: {
      if (!mojo::internal::ValidateMessageIsRequestWithoutResponse(
              message, &validation_context)) {
        return false;
      }
      if (!mojo::internal::ValidateMessagePayload<
               internal::ConformanceTestInterface_Method20_Params_Data>(
                  message, &validation_context)) {
        return false;
      }
      return true;
    }
    case internal::kConformanceTestInterface_Method21_Name: {
      if (!mojo::internal::ValidateMessageIsRequestWithoutResponse(
              message, &validation_context)) {
        return false;
      }
      if (!mojo::internal::ValidateMessagePayload<
               internal::ConformanceTestInterface_Method21_Params_Data>(
                  message, &validation_context)) {
        return false;
      }
      return true;
    }
    case internal::kConformanceTestInterface_Method22_Name: {
      if (!mojo::internal::ValidateMessageIsRequestWithoutResponse(
              message, &validation_context)) {
        return false;
      }
      if (!mojo::internal::ValidateMessagePayload<
               internal::ConformanceTestInterface_Method22_Params_Data>(
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

bool ConformanceTestInterfaceResponseValidator::Accept(mojo::Message* message) {
  if (!message->is_serialized() ||
      mojo::internal::ControlMessageHandler::IsControlMessage(message)) {
    return true;
  }

  mojo::internal::ValidationContext validation_context(
    message->payload(), message->payload_num_bytes(),
    message->handles()->size(), message->payload_num_interface_ids(), message,
    "ConformanceTestInterface ResponseValidator");

  if (!mojo::internal::ValidateMessageIsResponse(message, &validation_context))
    return false;
  switch (message->header()->name) {
    case internal::kConformanceTestInterface_Method12_Name: {
      if (!mojo::internal::ValidateMessagePayload<
               internal::ConformanceTestInterface_Method12_ResponseParams_Data>(
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
const char IntegrationTestInterface::Name_[] = "mojo.test.IntegrationTestInterface";

class IntegrationTestInterface_Method0_ForwardToCallback
    : public mojo::MessageReceiver {
 public:
  IntegrationTestInterface_Method0_ForwardToCallback(
      IntegrationTestInterface::Method0Callback callback
      ) : callback_(std::move(callback)) {
  }
  bool Accept(mojo::Message* message) override;
 private:
  IntegrationTestInterface::Method0Callback callback_;
  DISALLOW_COPY_AND_ASSIGN(IntegrationTestInterface_Method0_ForwardToCallback);
};

IntegrationTestInterfaceProxy::IntegrationTestInterfaceProxy(mojo::MessageReceiverWithResponder* receiver)
    : receiver_(receiver) {
}
class IntegrationTestInterfaceProxy_Method0_Message
    : public mojo::internal::UnserializedMessageContext {
 public:
  static const mojo::internal::UnserializedMessageContext::Tag kMessageTag;

  explicit IntegrationTestInterfaceProxy_Method0_Message(
      uint32_t message_flags
      , BasicStructPtr param_param0
  )
      : mojo::internal::UnserializedMessageContext(
          &kMessageTag,
          internal::kIntegrationTestInterface_Method0_Name,
          message_flags)
      , param_param0_(std::move(param_param0)){}
  ~IntegrationTestInterfaceProxy_Method0_Message() override = default;

  static mojo::Message Build(
      bool serialize,
      bool expects_response,
      bool is_sync,
      BasicStructPtr param_param0) {

    
  const uint32_t kFlags =
      ((expects_response) ? mojo::Message::kFlagExpectsResponse : 0) |
      ((is_sync) ? mojo::Message::kFlagIsSync : 0);

    if (!serialize) {
      return mojo::Message(std::make_unique<IntegrationTestInterfaceProxy_Method0_Message>(
          kFlags
          , std::move(param_param0)
          ));
    }

    DCHECK(serialize);
    
  mojo::Message message(
      internal::kIntegrationTestInterface_Method0_Name, kFlags, 0, 0, nullptr);
  auto* buffer = message.payload_buffer();
  ::mojo::test::internal::IntegrationTestInterface_Method0_Params_Data::BufferWriter
      params;
  mojo::internal::SerializationContext serialization_context;
  params.Allocate(buffer);
  typename decltype(params->param0)::BaseType::BufferWriter
      param0_writer;
  mojo::internal::Serialize<::mojo::test::BasicStructDataView>(
      param_param0, buffer, &param0_writer, &serialization_context);
  params->param0.Set(
      param0_writer.is_null() ? nullptr : param0_writer.data());
  MOJO_INTERNAL_DLOG_SERIALIZATION_WARNING(
      params->param0.is_null(),
      mojo::internal::VALIDATION_ERROR_UNEXPECTED_NULL_POINTER,
      "null param0 in IntegrationTestInterface.Method0 request");
  message.AttachHandlesFromSerializationContext(
      &serialization_context);
    return message;
  }


  void Dispatch(IntegrationTestInterface* impl, IntegrationTestInterface::Method0Callback callback) {
    impl->Method0(
        std::move(param_param0_), std::move(callback));
  }

 private:
  // mojo::internal::UnserializedMessageContext:
  void Serialize(mojo::internal::SerializationContext* serialization_context,
                 mojo::internal::Buffer* buffer) override {
    ::mojo::test::internal::IntegrationTestInterface_Method0_Params_Data::BufferWriter
        params;
    params.Allocate(buffer);
  typename decltype(params->param0)::BaseType::BufferWriter
      param0_writer;
  mojo::internal::Serialize<::mojo::test::BasicStructDataView>(
      param_param0_, buffer, &param0_writer, serialization_context);
  params->param0.Set(
      param0_writer.is_null() ? nullptr : param0_writer.data());
  MOJO_INTERNAL_DLOG_SERIALIZATION_WARNING(
      params->param0.is_null(),
      mojo::internal::VALIDATION_ERROR_UNEXPECTED_NULL_POINTER,
      "null param0 in IntegrationTestInterface.Method0 request");
  }
  BasicStructPtr param_param0_;

  DISALLOW_COPY_AND_ASSIGN(IntegrationTestInterfaceProxy_Method0_Message);
};

const mojo::internal::UnserializedMessageContext::Tag
IntegrationTestInterfaceProxy_Method0_Message::kMessageTag = {};

void IntegrationTestInterfaceProxy::Method0(
    BasicStructPtr in_param0, Method0Callback callback) {
#if BUILDFLAG(MOJO_TRACE_ENABLED)
  TRACE_EVENT0("mojom", "<class 'jinja2::utils::Namespace'>::IntegrationTestInterface::Method0");
#endif
  const bool kExpectsResponse = true;
  const bool kIsSync = false;
  const bool kSerialize = receiver_->PrefersSerializedMessages();
  auto message = IntegrationTestInterfaceProxy_Method0_Message::Build(
      kSerialize, kExpectsResponse, kIsSync, std::move(in_param0));

#if defined(ENABLE_IPC_FUZZER)
  message.set_interface_name(IntegrationTestInterface::Name_);
  message.set_method_name("Method0");
#endif
  std::unique_ptr<mojo::MessageReceiver> responder(
      new IntegrationTestInterface_Method0_ForwardToCallback(
          std::move(callback)));
  ignore_result(receiver_->AcceptWithResponder(&message, std::move(responder)));
}
class IntegrationTestInterface_Method0_ProxyToResponder {
 public:
  static IntegrationTestInterface::Method0Callback CreateCallback(
      uint64_t request_id,
      bool is_sync,
      std::unique_ptr<mojo::MessageReceiverWithStatus> responder) {
    std::unique_ptr<IntegrationTestInterface_Method0_ProxyToResponder> proxy(
        new IntegrationTestInterface_Method0_ProxyToResponder(
            request_id, is_sync, std::move(responder)));
    return base::BindOnce(&IntegrationTestInterface_Method0_ProxyToResponder::Run,
                          std::move(proxy));
  }

  ~IntegrationTestInterface_Method0_ProxyToResponder() {
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
  IntegrationTestInterface_Method0_ProxyToResponder(
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
        << "IntegrationTestInterface::Method0Callback was destroyed without "
        << "first either being run or its corresponding binding being closed. "
        << "It is an error to drop response callbacks which still correspond "
        << "to an open interface pipe.";
  }
#endif

  void Run(
      const WTF::Vector<uint8_t>& in_param0);

  uint64_t request_id_;
  bool is_sync_;
  std::unique_ptr<mojo::MessageReceiverWithStatus> responder_;

  DISALLOW_COPY_AND_ASSIGN(IntegrationTestInterface_Method0_ProxyToResponder);
};
class IntegrationTestInterface_Method0_Response_Message
    : public mojo::internal::UnserializedMessageContext {
 public:
  static const mojo::internal::UnserializedMessageContext::Tag kMessageTag;

  explicit IntegrationTestInterface_Method0_Response_Message(
      uint32_t message_flags
      , const WTF::Vector<uint8_t>& param_param0
  )
      : mojo::internal::UnserializedMessageContext(
          &kMessageTag,
          internal::kIntegrationTestInterface_Method0_Name,
          message_flags)
      , param_param0_(std::move(param_param0)){}
  ~IntegrationTestInterface_Method0_Response_Message() override = default;

  static mojo::Message Build(
      bool serialize,
      bool is_sync,
      const WTF::Vector<uint8_t>& param_param0) {

    
  const uint32_t kFlags = mojo::Message::kFlagIsResponse |
      ((is_sync) ? mojo::Message::kFlagIsSync : 0);

    if (!serialize) {
      return mojo::Message(std::make_unique<IntegrationTestInterface_Method0_Response_Message>(
          kFlags
          , std::move(param_param0)
          ));
    }

    DCHECK(serialize);
    
  mojo::Message message(
      internal::kIntegrationTestInterface_Method0_Name, kFlags, 0, 0, nullptr);
  auto* buffer = message.payload_buffer();
  ::mojo::test::internal::IntegrationTestInterface_Method0_ResponseParams_Data::BufferWriter
      params;
  mojo::internal::SerializationContext serialization_context;
  params.Allocate(buffer);
  typename decltype(params->param0)::BaseType::BufferWriter
      param0_writer;
  const mojo::internal::ContainerValidateParams param0_validate_params(
      0, false, nullptr);
  mojo::internal::Serialize<mojo::ArrayDataView<uint8_t>>(
      param_param0, buffer, &param0_writer, &param0_validate_params,
      &serialization_context);
  params->param0.Set(
      param0_writer.is_null() ? nullptr : param0_writer.data());
  MOJO_INTERNAL_DLOG_SERIALIZATION_WARNING(
      params->param0.is_null(),
      mojo::internal::VALIDATION_ERROR_UNEXPECTED_NULL_POINTER,
      "null param0 in IntegrationTestInterface.Method0 response");
  message.AttachHandlesFromSerializationContext(
      &serialization_context);
    return message;
  }


  void Dispatch(IntegrationTestInterface::Method0Callback* callback) {
    std::move(*callback).Run(
        std::move(param_param0_));
  }



 private:
  // mojo::internal::UnserializedMessageContext:
  void Serialize(mojo::internal::SerializationContext* serialization_context,
                 mojo::internal::Buffer* buffer) override {
    ::mojo::test::internal::IntegrationTestInterface_Method0_ResponseParams_Data::BufferWriter
        params;
    params.Allocate(buffer);
  typename decltype(params->param0)::BaseType::BufferWriter
      param0_writer;
  const mojo::internal::ContainerValidateParams param0_validate_params(
      0, false, nullptr);
  mojo::internal::Serialize<mojo::ArrayDataView<uint8_t>>(
      param_param0_, buffer, &param0_writer, &param0_validate_params,
      serialization_context);
  params->param0.Set(
      param0_writer.is_null() ? nullptr : param0_writer.data());
  MOJO_INTERNAL_DLOG_SERIALIZATION_WARNING(
      params->param0.is_null(),
      mojo::internal::VALIDATION_ERROR_UNEXPECTED_NULL_POINTER,
      "null param0 in IntegrationTestInterface.Method0 response");
  }
  WTF::Vector<uint8_t> param_param0_;

  DISALLOW_COPY_AND_ASSIGN(IntegrationTestInterface_Method0_Response_Message);
};

const mojo::internal::UnserializedMessageContext::Tag
IntegrationTestInterface_Method0_Response_Message::kMessageTag = {};

bool IntegrationTestInterface_Method0_ForwardToCallback::Accept(
    mojo::Message* message) {
#if BUILDFLAG(MOJO_TRACE_ENABLED)
  TRACE_EVENT1("mojom", "<class 'jinja2::utils::Namespace'>::IntegrationTestInterface::Method0Callback",
               "message", message->name());
#endif
  mojo::internal::MessageDispatchContext dispatch_context(message);
  if (!message->is_serialized()) {
    auto context =
        message->TakeUnserializedContext<IntegrationTestInterface_Method0_Response_Message>();
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
  internal::IntegrationTestInterface_Method0_ResponseParams_Data* params =
      reinterpret_cast<
          internal::IntegrationTestInterface_Method0_ResponseParams_Data*>(
              message->mutable_payload());
  
  mojo::internal::SerializationContext serialization_context;
  serialization_context.TakeHandlesFromMessage(message);
  bool success = true;
  WTF::Vector<uint8_t> p_param0{};
  IntegrationTestInterface_Method0_ResponseParamsDataView input_data_view(params, &serialization_context);
  
  if (!input_data_view.ReadParam0(&p_param0))
    success = false;
  if (!success) {
    ReportValidationErrorForMessage(
        message,
        mojo::internal::VALIDATION_ERROR_DESERIALIZATION_FAILED,
        IntegrationTestInterface::Name_, 0, true);
    return false;
  }
  if (!callback_.is_null())
    std::move(callback_).Run(
std::move(p_param0));
  return true;
}

void IntegrationTestInterface_Method0_ProxyToResponder::Run(
    const WTF::Vector<uint8_t>& in_param0) {
  const bool kSerialize = responder_->PrefersSerializedMessages();
  auto message = IntegrationTestInterface_Method0_Response_Message::Build(kSerialize, is_sync_, std::move(in_param0));

#if BUILDFLAG(MOJO_TRACE_ENABLED)
  TRACE_EVENT1("mojom", "(Impl)<class 'jinja2::utils::Namespace'>::IntegrationTestInterface::Method0Callback",
               "message", message.name());
#endif

#if defined(ENABLE_IPC_FUZZER)
  message.set_interface_name(IntegrationTestInterface::Name_);
  message.set_method_name("Method0");
#endif

  message.set_request_id(request_id_);
  ignore_result(responder_->Accept(&message));
  // TODO(darin): Accept() returning false indicates a malformed message, and
  // that may be good reason to close the connection. However, we don't have a
  // way to do that from here. We should add a way.
  responder_ = nullptr;
}

// static
bool IntegrationTestInterfaceStubDispatch::Accept(
    IntegrationTestInterface* impl,
    mojo::Message* message) {
  switch (message->header()->name) {
    case internal::kIntegrationTestInterface_Method0_Name: {
      break;
    }
  }
  return false;
}

// static
bool IntegrationTestInterfaceStubDispatch::AcceptWithResponder(
    IntegrationTestInterface* impl,
    mojo::Message* message,
    std::unique_ptr<mojo::MessageReceiverWithStatus> responder) {
  switch (message->header()->name) {
    case internal::kIntegrationTestInterface_Method0_Name: {
#if BUILDFLAG(MOJO_TRACE_ENABLED)
      TRACE_EVENT1(
          "mojom",
          "(Impl)<class 'jinja2::utils::Namespace'>::IntegrationTestInterface::Method0",
          "message", message->name());
#endif
      base::TaskAnnotator::ScopedSetIpcProgramCounter scoped_ipc_pc(
          base::GetProgramCounter());
      mojo::internal::MessageDispatchContext context(message);
      if (!message->is_serialized()) {
        auto context = message->TakeUnserializedContext<
            IntegrationTestInterfaceProxy_Method0_Message>();
        if (!context) {
          // The Message was not of the expected type. It may be a valid message
          // which was built using a different variant of these bindings. Force
          // serialization before dispatch in this case.
          message->SerializeIfNecessary();
        } else {
          IntegrationTestInterface::Method0Callback callback =
              IntegrationTestInterface_Method0_ProxyToResponder::CreateCallback(
                  message->request_id(),
                  message->has_flag(mojo::Message::kFlagIsSync),
                  std::move(responder));
          context->Dispatch(impl, std::move(callback));
          return true;
        }
      }

      internal::IntegrationTestInterface_Method0_Params_Data* params =
          reinterpret_cast<
              internal::IntegrationTestInterface_Method0_Params_Data*>(
                  message->mutable_payload());
      
      mojo::internal::SerializationContext serialization_context;
      serialization_context.TakeHandlesFromMessage(message);
      bool success = true;
      BasicStructPtr p_param0{};
      IntegrationTestInterface_Method0_ParamsDataView input_data_view(params, &serialization_context);
      
      if (!input_data_view.ReadParam0(&p_param0))
        success = false;
      if (!success) {
        ReportValidationErrorForMessage(
            message,
            mojo::internal::VALIDATION_ERROR_DESERIALIZATION_FAILED,
            IntegrationTestInterface::Name_, 0, false);
        return false;
      }
      IntegrationTestInterface::Method0Callback callback =
          IntegrationTestInterface_Method0_ProxyToResponder::CreateCallback(
              message->request_id(),
              message->has_flag(mojo::Message::kFlagIsSync),
              std::move(responder));
      // A null |impl| means no implementation was bound.
      DCHECK(impl);
      impl->Method0(
std::move(p_param0), std::move(callback));
      return true;
    }
  }
  return false;
}

bool IntegrationTestInterfaceRequestValidator::Accept(mojo::Message* message) {
  if (!message->is_serialized() ||
      mojo::internal::ControlMessageHandler::IsControlMessage(message)) {
    return true;
  }

  mojo::internal::ValidationContext validation_context(
    message->payload(), message->payload_num_bytes(),
    message->handles()->size(), message->payload_num_interface_ids(), message,
    "IntegrationTestInterface RequestValidator");

  switch (message->header()->name) {
    case internal::kIntegrationTestInterface_Method0_Name: {
      if (!mojo::internal::ValidateMessageIsRequestExpectingResponse(
              message, &validation_context)) {
        return false;
      }
      if (!mojo::internal::ValidateMessagePayload<
               internal::IntegrationTestInterface_Method0_Params_Data>(
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

bool IntegrationTestInterfaceResponseValidator::Accept(mojo::Message* message) {
  if (!message->is_serialized() ||
      mojo::internal::ControlMessageHandler::IsControlMessage(message)) {
    return true;
  }

  mojo::internal::ValidationContext validation_context(
    message->payload(), message->payload_num_bytes(),
    message->handles()->size(), message->payload_num_interface_ids(), message,
    "IntegrationTestInterface ResponseValidator");

  if (!mojo::internal::ValidateMessageIsResponse(message, &validation_context))
    return false;
  switch (message->header()->name) {
    case internal::kIntegrationTestInterface_Method0_Name: {
      if (!mojo::internal::ValidateMessagePayload<
               internal::IntegrationTestInterface_Method0_ResponseParams_Data>(
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
bool StructTraits<::mojo::test::blink::StructA::DataView, ::mojo::test::blink::StructAPtr>::Read(
    ::mojo::test::blink::StructA::DataView input,
    ::mojo::test::blink::StructAPtr* output) {
  bool success = true;
  ::mojo::test::blink::StructAPtr result(::mojo::test::blink::StructA::New());
  
      result->i = input.i();
  *output = std::move(result);
  return success;
}


// static
bool StructTraits<::mojo::test::blink::StructB::DataView, ::mojo::test::blink::StructBPtr>::Read(
    ::mojo::test::blink::StructB::DataView input,
    ::mojo::test::blink::StructBPtr* output) {
  bool success = true;
  ::mojo::test::blink::StructBPtr result(::mojo::test::blink::StructB::New());
  
      if (!input.ReadStructA(&result->struct_a))
        success = false;
  *output = std::move(result);
  return success;
}


// static
bool StructTraits<::mojo::test::blink::StructC::DataView, ::mojo::test::blink::StructCPtr>::Read(
    ::mojo::test::blink::StructC::DataView input,
    ::mojo::test::blink::StructCPtr* output) {
  bool success = true;
  ::mojo::test::blink::StructCPtr result(::mojo::test::blink::StructC::New());
  
      if (!input.ReadData(&result->data))
        success = false;
  *output = std::move(result);
  return success;
}


// static
bool StructTraits<::mojo::test::blink::StructD::DataView, ::mojo::test::blink::StructDPtr>::Read(
    ::mojo::test::blink::StructD::DataView input,
    ::mojo::test::blink::StructDPtr* output) {
  bool success = true;
  ::mojo::test::blink::StructDPtr result(::mojo::test::blink::StructD::New());
  
      if (!input.ReadMessagePipes(&result->message_pipes))
        success = false;
  *output = std::move(result);
  return success;
}


// static
bool StructTraits<::mojo::test::blink::StructE::DataView, ::mojo::test::blink::StructEPtr>::Read(
    ::mojo::test::blink::StructE::DataView input,
    ::mojo::test::blink::StructEPtr* output) {
  bool success = true;
  ::mojo::test::blink::StructEPtr result(::mojo::test::blink::StructE::New());
  
      if (!input.ReadStructD(&result->struct_d))
        success = false;
      result->data_pipe_consumer = input.TakeDataPipeConsumer();
  *output = std::move(result);
  return success;
}


// static
bool StructTraits<::mojo::test::blink::StructF::DataView, ::mojo::test::blink::StructFPtr>::Read(
    ::mojo::test::blink::StructF::DataView input,
    ::mojo::test::blink::StructFPtr* output) {
  bool success = true;
  ::mojo::test::blink::StructFPtr result(::mojo::test::blink::StructF::New());
  
      if (!input.ReadFixedSizeArray(&result->fixed_size_array))
        success = false;
  *output = std::move(result);
  return success;
}


// static
bool StructTraits<::mojo::test::blink::StructG::DataView, ::mojo::test::blink::StructGPtr>::Read(
    ::mojo::test::blink::StructG::DataView input,
    ::mojo::test::blink::StructGPtr* output) {
  bool success = true;
  ::mojo::test::blink::StructGPtr result(::mojo::test::blink::StructG::New());
  
      result->i = input.i();
      if (!input.ReadStructA(&result->struct_a))
        success = false;
      if (!input.ReadStr(&result->str))
        success = false;
      result->b = input.b();
  *output = std::move(result);
  return success;
}


// static
bool StructTraits<::mojo::test::blink::BasicStruct::DataView, ::mojo::test::blink::BasicStructPtr>::Read(
    ::mojo::test::blink::BasicStruct::DataView input,
    ::mojo::test::blink::BasicStructPtr* output) {
  bool success = true;
  ::mojo::test::blink::BasicStructPtr result(::mojo::test::blink::BasicStruct::New());
  
      result->a = input.a();
  *output = std::move(result);
  return success;
}


// static
bool StructTraits<::mojo::test::blink::StructWithEnum::DataView, ::mojo::test::blink::StructWithEnumPtr>::Read(
    ::mojo::test::blink::StructWithEnum::DataView input,
    ::mojo::test::blink::StructWithEnumPtr* output) {
  bool success = true;
  ::mojo::test::blink::StructWithEnumPtr result(::mojo::test::blink::StructWithEnum::New());
  
  *output = std::move(result);
  return success;
}


// static
bool StructTraits<::mojo::test::blink::Recursive::DataView, ::mojo::test::blink::RecursivePtr>::Read(
    ::mojo::test::blink::Recursive::DataView input,
    ::mojo::test::blink::RecursivePtr* output) {
  bool success = true;
  ::mojo::test::blink::RecursivePtr result(::mojo::test::blink::Recursive::New());
  
      if (!input.ReadRecursive(&result->recursive))
        success = false;
  *output = std::move(result);
  return success;
}

// static
bool UnionTraits<::mojo::test::blink::UnionA::DataView, ::mojo::test::blink::UnionAPtr>::Read(
    ::mojo::test::blink::UnionA::DataView input,
    ::mojo::test::blink::UnionAPtr* output) {
  using UnionType = ::mojo::test::blink::UnionA;
  using Tag = UnionType::Tag;

  switch (input.tag()) {
    case Tag::STRUCT_A: {
      ::mojo::test::blink::StructAPtr result_struct_a;
      if (!input.ReadStructA(&result_struct_a))
        return false;

      *output = UnionType::NewStructA(
          std::move(result_struct_a));
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