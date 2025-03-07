// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef THIRD_PARTY_BLINK_PUBLIC_MOJOM_NATIVE_FILE_SYSTEM_NATIVE_FILE_SYSTEM_MANAGER_MOJOM_H_
#define THIRD_PARTY_BLINK_PUBLIC_MOJOM_NATIVE_FILE_SYSTEM_NATIVE_FILE_SYSTEM_MANAGER_MOJOM_H_

#include <stdint.h>

#include <limits>
#include <type_traits>
#include <utility>

#include "base/callback.h"
#include "base/macros.h"
#include "base/optional.h"

#include "mojo/public/cpp/bindings/mojo_buildflags.h"
#if BUILDFLAG(MOJO_TRACE_ENABLED)
#include "base/trace_event/trace_event.h"
#endif
#include "mojo/public/cpp/bindings/clone_traits.h"
#include "mojo/public/cpp/bindings/equals_traits.h"
#include "mojo/public/cpp/bindings/lib/serialization.h"
#include "mojo/public/cpp/bindings/struct_ptr.h"
#include "mojo/public/cpp/bindings/struct_traits.h"
#include "mojo/public/cpp/bindings/union_traits.h"
#include "third_party/blink/public/mojom/native_file_system/native_file_system_manager.mojom-shared.h"
#include "third_party/blink/public/mojom/native_file_system/native_file_system_manager.mojom-forward.h"
#include "mojo/public/mojom/base/string16.mojom.h"
#include "third_party/blink/public/mojom/native_file_system/native_file_system_directory_handle.mojom.h"
#include "third_party/blink/public/mojom/native_file_system/native_file_system_file_handle.mojom.h"
#include "third_party/blink/public/mojom/native_file_system/native_file_system_error.mojom.h"
#include "third_party/blink/public/mojom/native_file_system/native_file_system_transfer_token.mojom.h"
#include <string>
#include <vector>

#include "mojo/public/cpp/bindings/associated_interface_ptr.h"
#include "mojo/public/cpp/bindings/associated_interface_ptr_info.h"
#include "mojo/public/cpp/bindings/associated_interface_request.h"
#include "mojo/public/cpp/bindings/interface_ptr.h"
#include "mojo/public/cpp/bindings/interface_request.h"
#include "mojo/public/cpp/bindings/lib/control_message_handler.h"
#include "mojo/public/cpp/bindings/raw_ptr_impl_ref_traits.h"
#include "mojo/public/cpp/bindings/thread_safe_interface_ptr.h"


#include "mojo/public/cpp/bindings/lib/native_enum_serialization.h"
#include "mojo/public/cpp/bindings/lib/native_struct_serialization.h"
#include "third_party/blink/public/common/common_export.h"




namespace blink {
namespace mojom {

class NativeFileSystemManagerProxy;

template <typename ImplRefTraits>
class NativeFileSystemManagerStub;

class NativeFileSystemManagerRequestValidator;
class NativeFileSystemManagerResponseValidator;


class BLINK_COMMON_EXPORT NativeFileSystemManager
    : public NativeFileSystemManagerInterfaceBase {
 public:
  static const char Name_[];
  static constexpr uint32_t Version_ = 0;
  static constexpr bool PassesAssociatedKinds_ = false;
  static constexpr bool HasSyncMethods_ = false;

  using Base_ = NativeFileSystemManagerInterfaceBase;
  using Proxy_ = NativeFileSystemManagerProxy;

  template <typename ImplRefTraits>
  using Stub_ = NativeFileSystemManagerStub<ImplRefTraits>;

  using RequestValidator_ = NativeFileSystemManagerRequestValidator;
  using ResponseValidator_ = NativeFileSystemManagerResponseValidator;
  enum MethodMinVersions : uint32_t {
    kGetSandboxedFileSystemMinVersion = 0,
    kChooseEntriesMinVersion = 0,
  };
  virtual ~NativeFileSystemManager() {}


  using GetSandboxedFileSystemCallback = base::OnceCallback<void(::blink::mojom::NativeFileSystemErrorPtr, ::blink::mojom::NativeFileSystemDirectoryHandlePtr)>;
  
  virtual void GetSandboxedFileSystem(GetSandboxedFileSystemCallback callback) = 0;


  using ChooseEntriesCallback = base::OnceCallback<void(::blink::mojom::NativeFileSystemErrorPtr, std::vector<::blink::mojom::NativeFileSystemEntryPtr>)>;
  
  virtual void ChooseEntries(ChooseFileSystemEntryType type, std::vector<ChooseFileSystemEntryAcceptsOptionPtr> accepts, bool include_accepts_all, ChooseEntriesCallback callback) = 0;
};

class BLINK_COMMON_EXPORT NativeFileSystemManagerProxy
    : public NativeFileSystemManager {
 public:
  using InterfaceType = NativeFileSystemManager;

  explicit NativeFileSystemManagerProxy(mojo::MessageReceiverWithResponder* receiver);
  void GetSandboxedFileSystem(GetSandboxedFileSystemCallback callback) final;
  void ChooseEntries(ChooseFileSystemEntryType type, std::vector<ChooseFileSystemEntryAcceptsOptionPtr> accepts, bool include_accepts_all, ChooseEntriesCallback callback) final;

 private:
  mojo::MessageReceiverWithResponder* receiver_;
};
class BLINK_COMMON_EXPORT NativeFileSystemManagerStubDispatch {
 public:
  static bool Accept(NativeFileSystemManager* impl, mojo::Message* message);
  static bool AcceptWithResponder(
      NativeFileSystemManager* impl,
      mojo::Message* message,
      std::unique_ptr<mojo::MessageReceiverWithStatus> responder);
};

template <typename ImplRefTraits =
              mojo::RawPtrImplRefTraits<NativeFileSystemManager>>
class NativeFileSystemManagerStub
    : public mojo::MessageReceiverWithResponderStatus {
 public:
  using ImplPointerType = typename ImplRefTraits::PointerType;

  NativeFileSystemManagerStub() {}
  ~NativeFileSystemManagerStub() override {}

  void set_sink(ImplPointerType sink) { sink_ = std::move(sink); }
  ImplPointerType& sink() { return sink_; }

  bool Accept(mojo::Message* message) override {
    if (ImplRefTraits::IsNull(sink_))
      return false;
    return NativeFileSystemManagerStubDispatch::Accept(
        ImplRefTraits::GetRawPointer(&sink_), message);
  }

  bool AcceptWithResponder(
      mojo::Message* message,
      std::unique_ptr<mojo::MessageReceiverWithStatus> responder) override {
    if (ImplRefTraits::IsNull(sink_))
      return false;
    return NativeFileSystemManagerStubDispatch::AcceptWithResponder(
        ImplRefTraits::GetRawPointer(&sink_), message, std::move(responder));
  }

 private:
  ImplPointerType sink_;
};
class BLINK_COMMON_EXPORT NativeFileSystemManagerRequestValidator : public mojo::MessageReceiver {
 public:
  bool Accept(mojo::Message* message) override;
};
class BLINK_COMMON_EXPORT NativeFileSystemManagerResponseValidator : public mojo::MessageReceiver {
 public:
  bool Accept(mojo::Message* message) override;
};








class BLINK_COMMON_EXPORT ChooseFileSystemEntryAcceptsOption {
 public:
  using DataView = ChooseFileSystemEntryAcceptsOptionDataView;
  using Data_ = internal::ChooseFileSystemEntryAcceptsOption_Data;

  template <typename... Args>
  static ChooseFileSystemEntryAcceptsOptionPtr New(Args&&... args) {
    return ChooseFileSystemEntryAcceptsOptionPtr(
        base::in_place, std::forward<Args>(args)...);
  }

  template <typename U>
  static ChooseFileSystemEntryAcceptsOptionPtr From(const U& u) {
    return mojo::TypeConverter<ChooseFileSystemEntryAcceptsOptionPtr, U>::Convert(u);
  }

  template <typename U>
  U To() const {
    return mojo::TypeConverter<U, ChooseFileSystemEntryAcceptsOption>::Convert(*this);
  }


  ChooseFileSystemEntryAcceptsOption();

  ChooseFileSystemEntryAcceptsOption(
      const base::string16& description,
      const std::vector<std::string>& mime_types,
      const std::vector<std::string>& extensions);

  ~ChooseFileSystemEntryAcceptsOption();

  // Clone() is a template so it is only instantiated if it is used. Thus, the
  // bindings generator does not need to know whether Clone() or copy
  // constructor/assignment are available for members.
  template <typename StructPtrType = ChooseFileSystemEntryAcceptsOptionPtr>
  ChooseFileSystemEntryAcceptsOptionPtr Clone() const;

  // Equals() is a template so it is only instantiated if it is used. Thus, the
  // bindings generator does not need to know whether Equals() or == operator
  // are available for members.
  template <typename T,
            typename std::enable_if<std::is_same<
                T, ChooseFileSystemEntryAcceptsOption>::value>::type* = nullptr>
  bool Equals(const T& other) const;

  template <typename UserType>
  static std::vector<uint8_t> Serialize(UserType* input) {
    return mojo::internal::SerializeImpl<
        ChooseFileSystemEntryAcceptsOption::DataView, std::vector<uint8_t>>(input);
  }

  template <typename UserType>
  static mojo::Message SerializeAsMessage(UserType* input) {
    return mojo::internal::SerializeAsMessageImpl<
        ChooseFileSystemEntryAcceptsOption::DataView>(input);
  }

  // The returned Message is serialized only if the message is moved
  // cross-process or cross-language. Otherwise if the message is Deserialized
  // as the same UserType |input| will just be moved to |output| in
  // DeserializeFromMessage.
  template <typename UserType>
  static mojo::Message WrapAsMessage(UserType input) {
    return mojo::Message(std::make_unique<
        internal::ChooseFileSystemEntryAcceptsOption_UnserializedMessageContext<
            UserType, ChooseFileSystemEntryAcceptsOption::DataView>>(0, 0, std::move(input)));
  }

  template <typename UserType>
  static bool Deserialize(const void* data,
                          size_t data_num_bytes,
                          UserType* output) {
    return mojo::internal::DeserializeImpl<ChooseFileSystemEntryAcceptsOption::DataView>(
        data, data_num_bytes, std::vector<mojo::ScopedHandle>(), output, Validate);
  }

  template <typename UserType>
  static bool Deserialize(const std::vector<uint8_t>& input,
                          UserType* output) {
    return ChooseFileSystemEntryAcceptsOption::Deserialize(
        input.size() == 0 ? nullptr : &input.front(), input.size(), output);
  }

  template <typename UserType>
  static bool DeserializeFromMessage(mojo::Message input,
                                     UserType* output) {
    auto context = input.TakeUnserializedContext<
        internal::ChooseFileSystemEntryAcceptsOption_UnserializedMessageContext<
            UserType, ChooseFileSystemEntryAcceptsOption::DataView>>();
    if (context) {
      *output = std::move(context->TakeData());
      return true;
    }
    input.SerializeIfNecessary();
    return mojo::internal::DeserializeImpl<ChooseFileSystemEntryAcceptsOption::DataView>(
        input.payload(), input.payload_num_bytes(),
        std::move(*input.mutable_handles()), output, Validate);
  }

  
  base::string16 description;
  
  std::vector<std::string> mime_types;
  
  std::vector<std::string> extensions;

 private:
  static bool Validate(const void* data,
                       mojo::internal::ValidationContext* validation_context);
};

template <typename StructPtrType>
ChooseFileSystemEntryAcceptsOptionPtr ChooseFileSystemEntryAcceptsOption::Clone() const {
  return New(
      mojo::Clone(description),
      mojo::Clone(mime_types),
      mojo::Clone(extensions)
  );
}

template <typename T,
          typename std::enable_if<std::is_same<
              T, ChooseFileSystemEntryAcceptsOption>::value>::type*>
bool ChooseFileSystemEntryAcceptsOption::Equals(const T& other_struct) const {
  if (!mojo::Equals(this->description, other_struct.description))
    return false;
  if (!mojo::Equals(this->mime_types, other_struct.mime_types))
    return false;
  if (!mojo::Equals(this->extensions, other_struct.extensions))
    return false;
  return true;
}


}  // namespace mojom
}  // namespace blink

namespace mojo {


template <>
struct BLINK_COMMON_EXPORT StructTraits<::blink::mojom::ChooseFileSystemEntryAcceptsOption::DataView,
                                         ::blink::mojom::ChooseFileSystemEntryAcceptsOptionPtr> {
  static bool IsNull(const ::blink::mojom::ChooseFileSystemEntryAcceptsOptionPtr& input) { return !input; }
  static void SetToNull(::blink::mojom::ChooseFileSystemEntryAcceptsOptionPtr* output) { output->reset(); }

  static const decltype(::blink::mojom::ChooseFileSystemEntryAcceptsOption::description)& description(
      const ::blink::mojom::ChooseFileSystemEntryAcceptsOptionPtr& input) {
    return input->description;
  }

  static const decltype(::blink::mojom::ChooseFileSystemEntryAcceptsOption::mime_types)& mime_types(
      const ::blink::mojom::ChooseFileSystemEntryAcceptsOptionPtr& input) {
    return input->mime_types;
  }

  static const decltype(::blink::mojom::ChooseFileSystemEntryAcceptsOption::extensions)& extensions(
      const ::blink::mojom::ChooseFileSystemEntryAcceptsOptionPtr& input) {
    return input->extensions;
  }

  static bool Read(::blink::mojom::ChooseFileSystemEntryAcceptsOption::DataView input, ::blink::mojom::ChooseFileSystemEntryAcceptsOptionPtr* output);
};

}  // namespace mojo

#endif  // THIRD_PARTY_BLINK_PUBLIC_MOJOM_NATIVE_FILE_SYSTEM_NATIVE_FILE_SYSTEM_MANAGER_MOJOM_H_