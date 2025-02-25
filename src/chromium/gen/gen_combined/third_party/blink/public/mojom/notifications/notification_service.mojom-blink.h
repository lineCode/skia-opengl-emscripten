// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef THIRD_PARTY_BLINK_PUBLIC_MOJOM_NOTIFICATIONS_NOTIFICATION_SERVICE_MOJOM_BLINK_H_
#define THIRD_PARTY_BLINK_PUBLIC_MOJOM_NOTIFICATIONS_NOTIFICATION_SERVICE_MOJOM_BLINK_H_

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
#include "third_party/blink/public/mojom/notifications/notification_service.mojom-shared.h"
#include "third_party/blink/public/mojom/notifications/notification_service.mojom-blink-forward.h"
#include "third_party/blink/public/mojom/notifications/notification.mojom-blink-forward.h"
#include "third_party/blink/public/mojom/permissions/permission_status.mojom-blink-forward.h"

#include "mojo/public/cpp/bindings/lib/wtf_clone_equals_util.h"
#include "mojo/public/cpp/bindings/lib/wtf_hash_util.h"
#include "third_party/blink/renderer/platform/wtf/hash_functions.h"
#include "third_party/blink/renderer/platform/wtf/text/wtf_string.h"

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
#include "third_party/blink/renderer/platform/platform_export.h"




namespace WTF {
struct blink_mojom_internal_PersistentNotificationError_DataHashFn {
  static unsigned GetHash(const ::blink::mojom::PersistentNotificationError& value) {
    using utype = std::underlying_type<::blink::mojom::PersistentNotificationError>::type;
    return DefaultHash<utype>::Hash().GetHash(static_cast<utype>(value));
  }
  static bool Equal(const ::blink::mojom::PersistentNotificationError& left, const ::blink::mojom::PersistentNotificationError& right) {
    return left == right;
  }
  static const bool safe_to_compare_to_empty_or_deleted = true;
};

template <>
struct HashTraits<::blink::mojom::PersistentNotificationError>
    : public GenericHashTraits<::blink::mojom::PersistentNotificationError> {
  static_assert(true,
                "-1000000 is a reserved enum value");
  static_assert(true,
                "-1000001 is a reserved enum value");
  static const bool hasIsEmptyValueFunction = true;
  static bool IsEmptyValue(const ::blink::mojom::PersistentNotificationError& value) {
    return value == static_cast<::blink::mojom::PersistentNotificationError>(-1000000);
  }
  static void ConstructDeletedValue(::blink::mojom::PersistentNotificationError& slot, bool) {
    slot = static_cast<::blink::mojom::PersistentNotificationError>(-1000001);
  }
  static bool IsDeletedValue(const ::blink::mojom::PersistentNotificationError& value) {
    return value == static_cast<::blink::mojom::PersistentNotificationError>(-1000001);
  }
};
}  // namespace WTF


namespace blink {
namespace mojom {
namespace blink {

class NonPersistentNotificationListenerProxy;

template <typename ImplRefTraits>
class NonPersistentNotificationListenerStub;

class NonPersistentNotificationListenerRequestValidator;
class NonPersistentNotificationListenerResponseValidator;


class PLATFORM_EXPORT NonPersistentNotificationListener
    : public NonPersistentNotificationListenerInterfaceBase {
 public:
  static const char Name_[];
  static constexpr uint32_t Version_ = 0;
  static constexpr bool PassesAssociatedKinds_ = false;
  static constexpr bool HasSyncMethods_ = false;

  using Base_ = NonPersistentNotificationListenerInterfaceBase;
  using Proxy_ = NonPersistentNotificationListenerProxy;

  template <typename ImplRefTraits>
  using Stub_ = NonPersistentNotificationListenerStub<ImplRefTraits>;

  using RequestValidator_ = NonPersistentNotificationListenerRequestValidator;
  using ResponseValidator_ = NonPersistentNotificationListenerResponseValidator;
  enum MethodMinVersions : uint32_t {
    kOnShowMinVersion = 0,
    kOnClickMinVersion = 0,
    kOnCloseMinVersion = 0,
  };
  virtual ~NonPersistentNotificationListener() {}

  
  virtual void OnShow() = 0;


  using OnClickCallback = base::OnceCallback<void()>;
  
  virtual void OnClick(OnClickCallback callback) = 0;


  using OnCloseCallback = base::OnceCallback<void()>;
  
  virtual void OnClose(OnCloseCallback callback) = 0;
};

class NotificationServiceProxy;

template <typename ImplRefTraits>
class NotificationServiceStub;

class NotificationServiceRequestValidator;
class NotificationServiceResponseValidator;


class PLATFORM_EXPORT NotificationService
    : public NotificationServiceInterfaceBase {
 public:
  static const char Name_[];
  static constexpr uint32_t Version_ = 0;
  static constexpr bool PassesAssociatedKinds_ = false;
  static constexpr bool HasSyncMethods_ = true;

  using Base_ = NotificationServiceInterfaceBase;
  using Proxy_ = NotificationServiceProxy;

  template <typename ImplRefTraits>
  using Stub_ = NotificationServiceStub<ImplRefTraits>;

  using RequestValidator_ = NotificationServiceRequestValidator;
  using ResponseValidator_ = NotificationServiceResponseValidator;
  enum MethodMinVersions : uint32_t {
    kGetPermissionStatusMinVersion = 0,
    kDisplayNonPersistentNotificationMinVersion = 0,
    kCloseNonPersistentNotificationMinVersion = 0,
    kDisplayPersistentNotificationMinVersion = 0,
    kClosePersistentNotificationMinVersion = 0,
    kGetNotificationsMinVersion = 0,
  };
  virtual ~NotificationService() {}

  // Sync method. This signature is used by the client side; the service side
  // should implement the signature with callback below.
  
  virtual bool GetPermissionStatus(::blink::mojom::blink::PermissionStatus* out_status);

  using GetPermissionStatusCallback = base::OnceCallback<void(::blink::mojom::blink::PermissionStatus)>;
  
  virtual void GetPermissionStatus(GetPermissionStatusCallback callback) = 0;

  
  virtual void DisplayNonPersistentNotification(const WTF::String& token, ::blink::mojom::blink::NotificationDataPtr notification_data, ::blink::mojom::blink::NotificationResourcesPtr notification_resources, NonPersistentNotificationListenerPtr event_listener) = 0;

  
  virtual void CloseNonPersistentNotification(const WTF::String& token) = 0;


  using DisplayPersistentNotificationCallback = base::OnceCallback<void(PersistentNotificationError)>;
  
  virtual void DisplayPersistentNotification(int64_t service_worker_registration_id, ::blink::mojom::blink::NotificationDataPtr notification_data, ::blink::mojom::blink::NotificationResourcesPtr notification_resources, DisplayPersistentNotificationCallback callback) = 0;

  
  virtual void ClosePersistentNotification(const WTF::String& notification_id) = 0;


  using GetNotificationsCallback = base::OnceCallback<void(const WTF::Vector<WTF::String>&, WTF::Vector<::blink::mojom::blink::NotificationDataPtr>)>;
  
  virtual void GetNotifications(int64_t service_worker_registration_id, const WTF::String& filter_tag, bool include_triggered, GetNotificationsCallback callback) = 0;
};

class PLATFORM_EXPORT NonPersistentNotificationListenerProxy
    : public NonPersistentNotificationListener {
 public:
  using InterfaceType = NonPersistentNotificationListener;

  explicit NonPersistentNotificationListenerProxy(mojo::MessageReceiverWithResponder* receiver);
  void OnShow() final;
  void OnClick(OnClickCallback callback) final;
  void OnClose(OnCloseCallback callback) final;

 private:
  mojo::MessageReceiverWithResponder* receiver_;
};

class PLATFORM_EXPORT NotificationServiceProxy
    : public NotificationService {
 public:
  using InterfaceType = NotificationService;

  explicit NotificationServiceProxy(mojo::MessageReceiverWithResponder* receiver);
  bool GetPermissionStatus(::blink::mojom::blink::PermissionStatus* out_status) final;
  void GetPermissionStatus(GetPermissionStatusCallback callback) final;
  void DisplayNonPersistentNotification(const WTF::String& token, ::blink::mojom::blink::NotificationDataPtr notification_data, ::blink::mojom::blink::NotificationResourcesPtr notification_resources, NonPersistentNotificationListenerPtr event_listener) final;
  void CloseNonPersistentNotification(const WTF::String& token) final;
  void DisplayPersistentNotification(int64_t service_worker_registration_id, ::blink::mojom::blink::NotificationDataPtr notification_data, ::blink::mojom::blink::NotificationResourcesPtr notification_resources, DisplayPersistentNotificationCallback callback) final;
  void ClosePersistentNotification(const WTF::String& notification_id) final;
  void GetNotifications(int64_t service_worker_registration_id, const WTF::String& filter_tag, bool include_triggered, GetNotificationsCallback callback) final;

 private:
  mojo::MessageReceiverWithResponder* receiver_;
};
class PLATFORM_EXPORT NonPersistentNotificationListenerStubDispatch {
 public:
  static bool Accept(NonPersistentNotificationListener* impl, mojo::Message* message);
  static bool AcceptWithResponder(
      NonPersistentNotificationListener* impl,
      mojo::Message* message,
      std::unique_ptr<mojo::MessageReceiverWithStatus> responder);
};

template <typename ImplRefTraits =
              mojo::RawPtrImplRefTraits<NonPersistentNotificationListener>>
class NonPersistentNotificationListenerStub
    : public mojo::MessageReceiverWithResponderStatus {
 public:
  using ImplPointerType = typename ImplRefTraits::PointerType;

  NonPersistentNotificationListenerStub() {}
  ~NonPersistentNotificationListenerStub() override {}

  void set_sink(ImplPointerType sink) { sink_ = std::move(sink); }
  ImplPointerType& sink() { return sink_; }

  bool Accept(mojo::Message* message) override {
    if (ImplRefTraits::IsNull(sink_))
      return false;
    return NonPersistentNotificationListenerStubDispatch::Accept(
        ImplRefTraits::GetRawPointer(&sink_), message);
  }

  bool AcceptWithResponder(
      mojo::Message* message,
      std::unique_ptr<mojo::MessageReceiverWithStatus> responder) override {
    if (ImplRefTraits::IsNull(sink_))
      return false;
    return NonPersistentNotificationListenerStubDispatch::AcceptWithResponder(
        ImplRefTraits::GetRawPointer(&sink_), message, std::move(responder));
  }

 private:
  ImplPointerType sink_;
};
class PLATFORM_EXPORT NotificationServiceStubDispatch {
 public:
  static bool Accept(NotificationService* impl, mojo::Message* message);
  static bool AcceptWithResponder(
      NotificationService* impl,
      mojo::Message* message,
      std::unique_ptr<mojo::MessageReceiverWithStatus> responder);
};

template <typename ImplRefTraits =
              mojo::RawPtrImplRefTraits<NotificationService>>
class NotificationServiceStub
    : public mojo::MessageReceiverWithResponderStatus {
 public:
  using ImplPointerType = typename ImplRefTraits::PointerType;

  NotificationServiceStub() {}
  ~NotificationServiceStub() override {}

  void set_sink(ImplPointerType sink) { sink_ = std::move(sink); }
  ImplPointerType& sink() { return sink_; }

  bool Accept(mojo::Message* message) override {
    if (ImplRefTraits::IsNull(sink_))
      return false;
    return NotificationServiceStubDispatch::Accept(
        ImplRefTraits::GetRawPointer(&sink_), message);
  }

  bool AcceptWithResponder(
      mojo::Message* message,
      std::unique_ptr<mojo::MessageReceiverWithStatus> responder) override {
    if (ImplRefTraits::IsNull(sink_))
      return false;
    return NotificationServiceStubDispatch::AcceptWithResponder(
        ImplRefTraits::GetRawPointer(&sink_), message, std::move(responder));
  }

 private:
  ImplPointerType sink_;
};
class PLATFORM_EXPORT NonPersistentNotificationListenerRequestValidator : public mojo::MessageReceiver {
 public:
  bool Accept(mojo::Message* message) override;
};
class PLATFORM_EXPORT NotificationServiceRequestValidator : public mojo::MessageReceiver {
 public:
  bool Accept(mojo::Message* message) override;
};
class PLATFORM_EXPORT NonPersistentNotificationListenerResponseValidator : public mojo::MessageReceiver {
 public:
  bool Accept(mojo::Message* message) override;
};
class PLATFORM_EXPORT NotificationServiceResponseValidator : public mojo::MessageReceiver {
 public:
  bool Accept(mojo::Message* message) override;
};





}  // namespace blink
}  // namespace mojom
}  // namespace blink

namespace mojo {

}  // namespace mojo

#endif  // THIRD_PARTY_BLINK_PUBLIC_MOJOM_NOTIFICATIONS_NOTIFICATION_SERVICE_MOJOM_BLINK_H_