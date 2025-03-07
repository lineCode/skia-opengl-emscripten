// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef THIRD_PARTY_BLINK_PUBLIC_MOJOM_USB_WEB_USB_SERVICE_MOJOM_H_
#define THIRD_PARTY_BLINK_PUBLIC_MOJOM_USB_WEB_USB_SERVICE_MOJOM_H_

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
#include "third_party/blink/public/mojom/usb/web_usb_service.mojom-shared.h"
#include "third_party/blink/public/mojom/usb/web_usb_service.mojom-forward.h"
#include "device/usb/public/mojom/device.mojom-forward.h"
#include "device/usb/public/mojom/device_enumeration_options.mojom-forward.h"
#include "device/usb/public/mojom/device_manager_client.mojom-forward.h"
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

class WebUsbServiceProxy;

template <typename ImplRefTraits>
class WebUsbServiceStub;

class WebUsbServiceRequestValidator;
class WebUsbServiceResponseValidator;


class BLINK_COMMON_EXPORT WebUsbService
    : public WebUsbServiceInterfaceBase {
 public:
  static const char Name_[];
  static constexpr uint32_t Version_ = 0;
  static constexpr bool PassesAssociatedKinds_ = true;
  static constexpr bool HasSyncMethods_ = false;

  using Base_ = WebUsbServiceInterfaceBase;
  using Proxy_ = WebUsbServiceProxy;

  template <typename ImplRefTraits>
  using Stub_ = WebUsbServiceStub<ImplRefTraits>;

  using RequestValidator_ = WebUsbServiceRequestValidator;
  using ResponseValidator_ = WebUsbServiceResponseValidator;
  enum MethodMinVersions : uint32_t {
    kGetDevicesMinVersion = 0,
    kGetDeviceMinVersion = 0,
    kGetPermissionMinVersion = 0,
    kSetClientMinVersion = 0,
  };
  virtual ~WebUsbService() {}


  using GetDevicesCallback = base::OnceCallback<void(std::vector<::device::mojom::UsbDeviceInfoPtr>)>;
  
  virtual void GetDevices(GetDevicesCallback callback) = 0;

  
  virtual void GetDevice(const std::string& guid, ::device::mojom::UsbDeviceRequest device_requestd) = 0;


  using GetPermissionCallback = base::OnceCallback<void(::device::mojom::UsbDeviceInfoPtr)>;
  
  virtual void GetPermission(std::vector<::device::mojom::UsbDeviceFilterPtr> device_filters, GetPermissionCallback callback) = 0;

  
  virtual void SetClient(::device::mojom::UsbDeviceManagerClientAssociatedPtrInfo client) = 0;
};

class BLINK_COMMON_EXPORT WebUsbServiceProxy
    : public WebUsbService {
 public:
  using InterfaceType = WebUsbService;

  explicit WebUsbServiceProxy(mojo::MessageReceiverWithResponder* receiver);
  void GetDevices(GetDevicesCallback callback) final;
  void GetDevice(const std::string& guid, ::device::mojom::UsbDeviceRequest device_requestd) final;
  void GetPermission(std::vector<::device::mojom::UsbDeviceFilterPtr> device_filters, GetPermissionCallback callback) final;
  void SetClient(::device::mojom::UsbDeviceManagerClientAssociatedPtrInfo client) final;

 private:
  mojo::MessageReceiverWithResponder* receiver_;
};
class BLINK_COMMON_EXPORT WebUsbServiceStubDispatch {
 public:
  static bool Accept(WebUsbService* impl, mojo::Message* message);
  static bool AcceptWithResponder(
      WebUsbService* impl,
      mojo::Message* message,
      std::unique_ptr<mojo::MessageReceiverWithStatus> responder);
};

template <typename ImplRefTraits =
              mojo::RawPtrImplRefTraits<WebUsbService>>
class WebUsbServiceStub
    : public mojo::MessageReceiverWithResponderStatus {
 public:
  using ImplPointerType = typename ImplRefTraits::PointerType;

  WebUsbServiceStub() {}
  ~WebUsbServiceStub() override {}

  void set_sink(ImplPointerType sink) { sink_ = std::move(sink); }
  ImplPointerType& sink() { return sink_; }

  bool Accept(mojo::Message* message) override {
    if (ImplRefTraits::IsNull(sink_))
      return false;
    return WebUsbServiceStubDispatch::Accept(
        ImplRefTraits::GetRawPointer(&sink_), message);
  }

  bool AcceptWithResponder(
      mojo::Message* message,
      std::unique_ptr<mojo::MessageReceiverWithStatus> responder) override {
    if (ImplRefTraits::IsNull(sink_))
      return false;
    return WebUsbServiceStubDispatch::AcceptWithResponder(
        ImplRefTraits::GetRawPointer(&sink_), message, std::move(responder));
  }

 private:
  ImplPointerType sink_;
};
class BLINK_COMMON_EXPORT WebUsbServiceRequestValidator : public mojo::MessageReceiver {
 public:
  bool Accept(mojo::Message* message) override;
};
class BLINK_COMMON_EXPORT WebUsbServiceResponseValidator : public mojo::MessageReceiver {
 public:
  bool Accept(mojo::Message* message) override;
};





}  // namespace mojom
}  // namespace blink

namespace mojo {

}  // namespace mojo

#endif  // THIRD_PARTY_BLINK_PUBLIC_MOJOM_USB_WEB_USB_SERVICE_MOJOM_H_