// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MEDIA_MOJO_INTERFACES_VIDEO_ENCODE_ACCELERATOR_MOJOM_H_
#define MEDIA_MOJO_INTERFACES_VIDEO_ENCODE_ACCELERATOR_MOJOM_H_

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
#include "media/mojo/interfaces/video_encode_accelerator.mojom-shared.h"
#include "media/mojo/interfaces/video_encode_accelerator.mojom-forward.h"
#include "media/mojo/interfaces/media_types.mojom.h"
#include "mojo/public/mojom/base/time.mojom.h"
#include "ui/gfx/geometry/mojo/geometry.mojom.h"
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
#include "media/video/video_encode_accelerator.h"




namespace media {
namespace mojom {

class VideoEncodeAcceleratorProviderProxy;

template <typename ImplRefTraits>
class VideoEncodeAcceleratorProviderStub;

class VideoEncodeAcceleratorProviderRequestValidator;


class  VideoEncodeAcceleratorProvider
    : public VideoEncodeAcceleratorProviderInterfaceBase {
 public:
  static const char Name_[];
  static constexpr uint32_t Version_ = 0;
  static constexpr bool PassesAssociatedKinds_ = false;
  static constexpr bool HasSyncMethods_ = false;

  using Base_ = VideoEncodeAcceleratorProviderInterfaceBase;
  using Proxy_ = VideoEncodeAcceleratorProviderProxy;

  template <typename ImplRefTraits>
  using Stub_ = VideoEncodeAcceleratorProviderStub<ImplRefTraits>;

  using RequestValidator_ = VideoEncodeAcceleratorProviderRequestValidator;
  using ResponseValidator_ = mojo::PassThroughFilter;
  enum MethodMinVersions : uint32_t {
    kCreateVideoEncodeAcceleratorMinVersion = 0,
  };
  virtual ~VideoEncodeAcceleratorProvider() {}

  
  virtual void CreateVideoEncodeAccelerator(VideoEncodeAcceleratorRequest request) = 0;
};

class VideoEncodeAcceleratorProxy;

template <typename ImplRefTraits>
class VideoEncodeAcceleratorStub;

class VideoEncodeAcceleratorRequestValidator;
class VideoEncodeAcceleratorResponseValidator;


class  VideoEncodeAccelerator
    : public VideoEncodeAcceleratorInterfaceBase {
 public:
  static const char Name_[];
  static constexpr uint32_t Version_ = 0;
  static constexpr bool PassesAssociatedKinds_ = false;
  static constexpr bool HasSyncMethods_ = true;

  using Base_ = VideoEncodeAcceleratorInterfaceBase;
  using Proxy_ = VideoEncodeAcceleratorProxy;

  template <typename ImplRefTraits>
  using Stub_ = VideoEncodeAcceleratorStub<ImplRefTraits>;

  using RequestValidator_ = VideoEncodeAcceleratorRequestValidator;
  using ResponseValidator_ = VideoEncodeAcceleratorResponseValidator;
  enum MethodMinVersions : uint32_t {
    kInitializeMinVersion = 0,
    kEncodeMinVersion = 0,
    kUseOutputBitstreamBufferMinVersion = 0,
    kRequestEncodingParametersChangeMinVersion = 0,
  };
  
  using Error = VideoEncodeAccelerator_Error;
  virtual ~VideoEncodeAccelerator() {}

  // Sync method. This signature is used by the client side; the service side
  // should implement the signature with callback below.
  
  virtual bool Initialize(const media::VideoEncodeAccelerator::Config& config, VideoEncodeAcceleratorClientPtr client, bool* out_result);

  using InitializeCallback = base::OnceCallback<void(bool)>;
  
  virtual void Initialize(const media::VideoEncodeAccelerator::Config& config, VideoEncodeAcceleratorClientPtr client, InitializeCallback callback) = 0;


  using EncodeCallback = base::OnceCallback<void()>;
  
  virtual void Encode(const scoped_refptr<media::VideoFrame>& frame, bool force_keyframe, EncodeCallback callback) = 0;

  
  virtual void UseOutputBitstreamBuffer(int32_t bitstream_buffer_id, mojo::ScopedSharedBufferHandle buffer) = 0;

  
  virtual void RequestEncodingParametersChange(const media::VideoBitrateAllocation& bitrate_allocation, uint32_t framerate) = 0;
};

class VideoEncodeAcceleratorClientProxy;

template <typename ImplRefTraits>
class VideoEncodeAcceleratorClientStub;

class VideoEncodeAcceleratorClientRequestValidator;


class  VideoEncodeAcceleratorClient
    : public VideoEncodeAcceleratorClientInterfaceBase {
 public:
  static const char Name_[];
  static constexpr uint32_t Version_ = 0;
  static constexpr bool PassesAssociatedKinds_ = false;
  static constexpr bool HasSyncMethods_ = false;

  using Base_ = VideoEncodeAcceleratorClientInterfaceBase;
  using Proxy_ = VideoEncodeAcceleratorClientProxy;

  template <typename ImplRefTraits>
  using Stub_ = VideoEncodeAcceleratorClientStub<ImplRefTraits>;

  using RequestValidator_ = VideoEncodeAcceleratorClientRequestValidator;
  using ResponseValidator_ = mojo::PassThroughFilter;
  enum MethodMinVersions : uint32_t {
    kRequireBitstreamBuffersMinVersion = 0,
    kBitstreamBufferReadyMinVersion = 0,
    kNotifyErrorMinVersion = 0,
  };
  virtual ~VideoEncodeAcceleratorClient() {}

  
  virtual void RequireBitstreamBuffers(uint32_t input_count, const gfx::Size& input_coded_size, uint32_t output_buffer_size) = 0;

  
  virtual void BitstreamBufferReady(int32_t bitstream_buffer_id, const media::BitstreamBufferMetadata& metadata) = 0;

  
  virtual void NotifyError(media::VideoEncodeAccelerator::Error error) = 0;
};

class  VideoEncodeAcceleratorProviderProxy
    : public VideoEncodeAcceleratorProvider {
 public:
  using InterfaceType = VideoEncodeAcceleratorProvider;

  explicit VideoEncodeAcceleratorProviderProxy(mojo::MessageReceiverWithResponder* receiver);
  void CreateVideoEncodeAccelerator(VideoEncodeAcceleratorRequest request) final;

 private:
  mojo::MessageReceiverWithResponder* receiver_;
};

class  VideoEncodeAcceleratorProxy
    : public VideoEncodeAccelerator {
 public:
  using InterfaceType = VideoEncodeAccelerator;

  explicit VideoEncodeAcceleratorProxy(mojo::MessageReceiverWithResponder* receiver);
  bool Initialize(const media::VideoEncodeAccelerator::Config& config, VideoEncodeAcceleratorClientPtr client, bool* out_result) final;
  void Initialize(const media::VideoEncodeAccelerator::Config& config, VideoEncodeAcceleratorClientPtr client, InitializeCallback callback) final;
  void Encode(const scoped_refptr<media::VideoFrame>& frame, bool force_keyframe, EncodeCallback callback) final;
  void UseOutputBitstreamBuffer(int32_t bitstream_buffer_id, mojo::ScopedSharedBufferHandle buffer) final;
  void RequestEncodingParametersChange(const media::VideoBitrateAllocation& bitrate_allocation, uint32_t framerate) final;

 private:
  mojo::MessageReceiverWithResponder* receiver_;
};

class  VideoEncodeAcceleratorClientProxy
    : public VideoEncodeAcceleratorClient {
 public:
  using InterfaceType = VideoEncodeAcceleratorClient;

  explicit VideoEncodeAcceleratorClientProxy(mojo::MessageReceiverWithResponder* receiver);
  void RequireBitstreamBuffers(uint32_t input_count, const gfx::Size& input_coded_size, uint32_t output_buffer_size) final;
  void BitstreamBufferReady(int32_t bitstream_buffer_id, const media::BitstreamBufferMetadata& metadata) final;
  void NotifyError(media::VideoEncodeAccelerator::Error error) final;

 private:
  mojo::MessageReceiverWithResponder* receiver_;
};
class  VideoEncodeAcceleratorProviderStubDispatch {
 public:
  static bool Accept(VideoEncodeAcceleratorProvider* impl, mojo::Message* message);
  static bool AcceptWithResponder(
      VideoEncodeAcceleratorProvider* impl,
      mojo::Message* message,
      std::unique_ptr<mojo::MessageReceiverWithStatus> responder);
};

template <typename ImplRefTraits =
              mojo::RawPtrImplRefTraits<VideoEncodeAcceleratorProvider>>
class VideoEncodeAcceleratorProviderStub
    : public mojo::MessageReceiverWithResponderStatus {
 public:
  using ImplPointerType = typename ImplRefTraits::PointerType;

  VideoEncodeAcceleratorProviderStub() {}
  ~VideoEncodeAcceleratorProviderStub() override {}

  void set_sink(ImplPointerType sink) { sink_ = std::move(sink); }
  ImplPointerType& sink() { return sink_; }

  bool Accept(mojo::Message* message) override {
    if (ImplRefTraits::IsNull(sink_))
      return false;
    return VideoEncodeAcceleratorProviderStubDispatch::Accept(
        ImplRefTraits::GetRawPointer(&sink_), message);
  }

  bool AcceptWithResponder(
      mojo::Message* message,
      std::unique_ptr<mojo::MessageReceiverWithStatus> responder) override {
    if (ImplRefTraits::IsNull(sink_))
      return false;
    return VideoEncodeAcceleratorProviderStubDispatch::AcceptWithResponder(
        ImplRefTraits::GetRawPointer(&sink_), message, std::move(responder));
  }

 private:
  ImplPointerType sink_;
};
class  VideoEncodeAcceleratorStubDispatch {
 public:
  static bool Accept(VideoEncodeAccelerator* impl, mojo::Message* message);
  static bool AcceptWithResponder(
      VideoEncodeAccelerator* impl,
      mojo::Message* message,
      std::unique_ptr<mojo::MessageReceiverWithStatus> responder);
};

template <typename ImplRefTraits =
              mojo::RawPtrImplRefTraits<VideoEncodeAccelerator>>
class VideoEncodeAcceleratorStub
    : public mojo::MessageReceiverWithResponderStatus {
 public:
  using ImplPointerType = typename ImplRefTraits::PointerType;

  VideoEncodeAcceleratorStub() {}
  ~VideoEncodeAcceleratorStub() override {}

  void set_sink(ImplPointerType sink) { sink_ = std::move(sink); }
  ImplPointerType& sink() { return sink_; }

  bool Accept(mojo::Message* message) override {
    if (ImplRefTraits::IsNull(sink_))
      return false;
    return VideoEncodeAcceleratorStubDispatch::Accept(
        ImplRefTraits::GetRawPointer(&sink_), message);
  }

  bool AcceptWithResponder(
      mojo::Message* message,
      std::unique_ptr<mojo::MessageReceiverWithStatus> responder) override {
    if (ImplRefTraits::IsNull(sink_))
      return false;
    return VideoEncodeAcceleratorStubDispatch::AcceptWithResponder(
        ImplRefTraits::GetRawPointer(&sink_), message, std::move(responder));
  }

 private:
  ImplPointerType sink_;
};
class  VideoEncodeAcceleratorClientStubDispatch {
 public:
  static bool Accept(VideoEncodeAcceleratorClient* impl, mojo::Message* message);
  static bool AcceptWithResponder(
      VideoEncodeAcceleratorClient* impl,
      mojo::Message* message,
      std::unique_ptr<mojo::MessageReceiverWithStatus> responder);
};

template <typename ImplRefTraits =
              mojo::RawPtrImplRefTraits<VideoEncodeAcceleratorClient>>
class VideoEncodeAcceleratorClientStub
    : public mojo::MessageReceiverWithResponderStatus {
 public:
  using ImplPointerType = typename ImplRefTraits::PointerType;

  VideoEncodeAcceleratorClientStub() {}
  ~VideoEncodeAcceleratorClientStub() override {}

  void set_sink(ImplPointerType sink) { sink_ = std::move(sink); }
  ImplPointerType& sink() { return sink_; }

  bool Accept(mojo::Message* message) override {
    if (ImplRefTraits::IsNull(sink_))
      return false;
    return VideoEncodeAcceleratorClientStubDispatch::Accept(
        ImplRefTraits::GetRawPointer(&sink_), message);
  }

  bool AcceptWithResponder(
      mojo::Message* message,
      std::unique_ptr<mojo::MessageReceiverWithStatus> responder) override {
    if (ImplRefTraits::IsNull(sink_))
      return false;
    return VideoEncodeAcceleratorClientStubDispatch::AcceptWithResponder(
        ImplRefTraits::GetRawPointer(&sink_), message, std::move(responder));
  }

 private:
  ImplPointerType sink_;
};
class  VideoEncodeAcceleratorProviderRequestValidator : public mojo::MessageReceiver {
 public:
  bool Accept(mojo::Message* message) override;
};
class  VideoEncodeAcceleratorRequestValidator : public mojo::MessageReceiver {
 public:
  bool Accept(mojo::Message* message) override;
};
class  VideoEncodeAcceleratorClientRequestValidator : public mojo::MessageReceiver {
 public:
  bool Accept(mojo::Message* message) override;
};
class  VideoEncodeAcceleratorResponseValidator : public mojo::MessageReceiver {
 public:
  bool Accept(mojo::Message* message) override;
};







class  Vp8Metadata {
 public:
  using DataView = Vp8MetadataDataView;
  using Data_ = internal::Vp8Metadata_Data;

  template <typename... Args>
  static Vp8MetadataPtr New(Args&&... args) {
    return Vp8MetadataPtr(
        base::in_place, std::forward<Args>(args)...);
  }

  template <typename U>
  static Vp8MetadataPtr From(const U& u) {
    return mojo::TypeConverter<Vp8MetadataPtr, U>::Convert(u);
  }

  template <typename U>
  U To() const {
    return mojo::TypeConverter<U, Vp8Metadata>::Convert(*this);
  }


  Vp8Metadata();

  Vp8Metadata(
      bool non_reference,
      uint8_t temporal_idx,
      bool layer_sync);

  ~Vp8Metadata();

  // Clone() is a template so it is only instantiated if it is used. Thus, the
  // bindings generator does not need to know whether Clone() or copy
  // constructor/assignment are available for members.
  template <typename StructPtrType = Vp8MetadataPtr>
  Vp8MetadataPtr Clone() const;

  // Equals() is a template so it is only instantiated if it is used. Thus, the
  // bindings generator does not need to know whether Equals() or == operator
  // are available for members.
  template <typename T,
            typename std::enable_if<std::is_same<
                T, Vp8Metadata>::value>::type* = nullptr>
  bool Equals(const T& other) const;

  template <typename UserType>
  static std::vector<uint8_t> Serialize(UserType* input) {
    return mojo::internal::SerializeImpl<
        Vp8Metadata::DataView, std::vector<uint8_t>>(input);
  }

  template <typename UserType>
  static mojo::Message SerializeAsMessage(UserType* input) {
    return mojo::internal::SerializeAsMessageImpl<
        Vp8Metadata::DataView>(input);
  }

  // The returned Message is serialized only if the message is moved
  // cross-process or cross-language. Otherwise if the message is Deserialized
  // as the same UserType |input| will just be moved to |output| in
  // DeserializeFromMessage.
  template <typename UserType>
  static mojo::Message WrapAsMessage(UserType input) {
    return mojo::Message(std::make_unique<
        internal::Vp8Metadata_UnserializedMessageContext<
            UserType, Vp8Metadata::DataView>>(0, 0, std::move(input)));
  }

  template <typename UserType>
  static bool Deserialize(const void* data,
                          size_t data_num_bytes,
                          UserType* output) {
    return mojo::internal::DeserializeImpl<Vp8Metadata::DataView>(
        data, data_num_bytes, std::vector<mojo::ScopedHandle>(), output, Validate);
  }

  template <typename UserType>
  static bool Deserialize(const std::vector<uint8_t>& input,
                          UserType* output) {
    return Vp8Metadata::Deserialize(
        input.size() == 0 ? nullptr : &input.front(), input.size(), output);
  }

  template <typename UserType>
  static bool DeserializeFromMessage(mojo::Message input,
                                     UserType* output) {
    auto context = input.TakeUnserializedContext<
        internal::Vp8Metadata_UnserializedMessageContext<
            UserType, Vp8Metadata::DataView>>();
    if (context) {
      *output = std::move(context->TakeData());
      return true;
    }
    input.SerializeIfNecessary();
    return mojo::internal::DeserializeImpl<Vp8Metadata::DataView>(
        input.payload(), input.payload_num_bytes(),
        std::move(*input.mutable_handles()), output, Validate);
  }

  
  bool non_reference;
  
  uint8_t temporal_idx;
  
  bool layer_sync;

 private:
  static bool Validate(const void* data,
                       mojo::internal::ValidationContext* validation_context);
};








class  VideoBitrateAllocation {
 public:
  using DataView = VideoBitrateAllocationDataView;
  using Data_ = internal::VideoBitrateAllocation_Data;

  template <typename... Args>
  static VideoBitrateAllocationPtr New(Args&&... args) {
    return VideoBitrateAllocationPtr(
        base::in_place, std::forward<Args>(args)...);
  }

  template <typename U>
  static VideoBitrateAllocationPtr From(const U& u) {
    return mojo::TypeConverter<VideoBitrateAllocationPtr, U>::Convert(u);
  }

  template <typename U>
  U To() const {
    return mojo::TypeConverter<U, VideoBitrateAllocation>::Convert(*this);
  }


  VideoBitrateAllocation();

  explicit VideoBitrateAllocation(
      const std::vector<int32_t>& bitrates);

  ~VideoBitrateAllocation();

  // Clone() is a template so it is only instantiated if it is used. Thus, the
  // bindings generator does not need to know whether Clone() or copy
  // constructor/assignment are available for members.
  template <typename StructPtrType = VideoBitrateAllocationPtr>
  VideoBitrateAllocationPtr Clone() const;

  // Equals() is a template so it is only instantiated if it is used. Thus, the
  // bindings generator does not need to know whether Equals() or == operator
  // are available for members.
  template <typename T,
            typename std::enable_if<std::is_same<
                T, VideoBitrateAllocation>::value>::type* = nullptr>
  bool Equals(const T& other) const;

  template <typename UserType>
  static std::vector<uint8_t> Serialize(UserType* input) {
    return mojo::internal::SerializeImpl<
        VideoBitrateAllocation::DataView, std::vector<uint8_t>>(input);
  }

  template <typename UserType>
  static mojo::Message SerializeAsMessage(UserType* input) {
    return mojo::internal::SerializeAsMessageImpl<
        VideoBitrateAllocation::DataView>(input);
  }

  // The returned Message is serialized only if the message is moved
  // cross-process or cross-language. Otherwise if the message is Deserialized
  // as the same UserType |input| will just be moved to |output| in
  // DeserializeFromMessage.
  template <typename UserType>
  static mojo::Message WrapAsMessage(UserType input) {
    return mojo::Message(std::make_unique<
        internal::VideoBitrateAllocation_UnserializedMessageContext<
            UserType, VideoBitrateAllocation::DataView>>(0, 0, std::move(input)));
  }

  template <typename UserType>
  static bool Deserialize(const void* data,
                          size_t data_num_bytes,
                          UserType* output) {
    return mojo::internal::DeserializeImpl<VideoBitrateAllocation::DataView>(
        data, data_num_bytes, std::vector<mojo::ScopedHandle>(), output, Validate);
  }

  template <typename UserType>
  static bool Deserialize(const std::vector<uint8_t>& input,
                          UserType* output) {
    return VideoBitrateAllocation::Deserialize(
        input.size() == 0 ? nullptr : &input.front(), input.size(), output);
  }

  template <typename UserType>
  static bool DeserializeFromMessage(mojo::Message input,
                                     UserType* output) {
    auto context = input.TakeUnserializedContext<
        internal::VideoBitrateAllocation_UnserializedMessageContext<
            UserType, VideoBitrateAllocation::DataView>>();
    if (context) {
      *output = std::move(context->TakeData());
      return true;
    }
    input.SerializeIfNecessary();
    return mojo::internal::DeserializeImpl<VideoBitrateAllocation::DataView>(
        input.payload(), input.payload_num_bytes(),
        std::move(*input.mutable_handles()), output, Validate);
  }

  
  std::vector<int32_t> bitrates;

 private:
  static bool Validate(const void* data,
                       mojo::internal::ValidationContext* validation_context);
};





class  VideoEncodeAcceleratorConfig {
 public:
  using DataView = VideoEncodeAcceleratorConfigDataView;
  using Data_ = internal::VideoEncodeAcceleratorConfig_Data;
  using ContentType = VideoEncodeAcceleratorConfig_ContentType;

  template <typename... Args>
  static VideoEncodeAcceleratorConfigPtr New(Args&&... args) {
    return VideoEncodeAcceleratorConfigPtr(
        base::in_place, std::forward<Args>(args)...);
  }

  template <typename U>
  static VideoEncodeAcceleratorConfigPtr From(const U& u) {
    return mojo::TypeConverter<VideoEncodeAcceleratorConfigPtr, U>::Convert(u);
  }

  template <typename U>
  U To() const {
    return mojo::TypeConverter<U, VideoEncodeAcceleratorConfig>::Convert(*this);
  }


  VideoEncodeAcceleratorConfig();

  VideoEncodeAcceleratorConfig(
      media::VideoPixelFormat input_format,
      const gfx::Size& input_visible_size,
      media::VideoCodecProfile output_profile,
      uint32_t initial_bitrate,
      uint32_t initial_framerate,
      bool has_initial_framerate,
      uint32_t gop_length,
      bool has_gop_length,
      uint8_t h264_output_level,
      bool has_h264_output_level,
      VideoEncodeAcceleratorConfig::ContentType content_type);

  ~VideoEncodeAcceleratorConfig();

  // Clone() is a template so it is only instantiated if it is used. Thus, the
  // bindings generator does not need to know whether Clone() or copy
  // constructor/assignment are available for members.
  template <typename StructPtrType = VideoEncodeAcceleratorConfigPtr>
  VideoEncodeAcceleratorConfigPtr Clone() const;

  // Equals() is a template so it is only instantiated if it is used. Thus, the
  // bindings generator does not need to know whether Equals() or == operator
  // are available for members.
  template <typename T,
            typename std::enable_if<std::is_same<
                T, VideoEncodeAcceleratorConfig>::value>::type* = nullptr>
  bool Equals(const T& other) const;

  template <typename UserType>
  static std::vector<uint8_t> Serialize(UserType* input) {
    return mojo::internal::SerializeImpl<
        VideoEncodeAcceleratorConfig::DataView, std::vector<uint8_t>>(input);
  }

  template <typename UserType>
  static mojo::Message SerializeAsMessage(UserType* input) {
    return mojo::internal::SerializeAsMessageImpl<
        VideoEncodeAcceleratorConfig::DataView>(input);
  }

  // The returned Message is serialized only if the message is moved
  // cross-process or cross-language. Otherwise if the message is Deserialized
  // as the same UserType |input| will just be moved to |output| in
  // DeserializeFromMessage.
  template <typename UserType>
  static mojo::Message WrapAsMessage(UserType input) {
    return mojo::Message(std::make_unique<
        internal::VideoEncodeAcceleratorConfig_UnserializedMessageContext<
            UserType, VideoEncodeAcceleratorConfig::DataView>>(0, 0, std::move(input)));
  }

  template <typename UserType>
  static bool Deserialize(const void* data,
                          size_t data_num_bytes,
                          UserType* output) {
    return mojo::internal::DeserializeImpl<VideoEncodeAcceleratorConfig::DataView>(
        data, data_num_bytes, std::vector<mojo::ScopedHandle>(), output, Validate);
  }

  template <typename UserType>
  static bool Deserialize(const std::vector<uint8_t>& input,
                          UserType* output) {
    return VideoEncodeAcceleratorConfig::Deserialize(
        input.size() == 0 ? nullptr : &input.front(), input.size(), output);
  }

  template <typename UserType>
  static bool DeserializeFromMessage(mojo::Message input,
                                     UserType* output) {
    auto context = input.TakeUnserializedContext<
        internal::VideoEncodeAcceleratorConfig_UnserializedMessageContext<
            UserType, VideoEncodeAcceleratorConfig::DataView>>();
    if (context) {
      *output = std::move(context->TakeData());
      return true;
    }
    input.SerializeIfNecessary();
    return mojo::internal::DeserializeImpl<VideoEncodeAcceleratorConfig::DataView>(
        input.payload(), input.payload_num_bytes(),
        std::move(*input.mutable_handles()), output, Validate);
  }

  
  media::VideoPixelFormat input_format;
  
  gfx::Size input_visible_size;
  
  media::VideoCodecProfile output_profile;
  
  uint32_t initial_bitrate;
  
  uint32_t initial_framerate;
  
  bool has_initial_framerate;
  
  uint32_t gop_length;
  
  bool has_gop_length;
  
  uint8_t h264_output_level;
  
  bool has_h264_output_level;
  
  VideoEncodeAcceleratorConfig::ContentType content_type;

 private:
  static bool Validate(const void* data,
                       mojo::internal::ValidationContext* validation_context);
};






class  BitstreamBufferMetadata {
 public:
  using DataView = BitstreamBufferMetadataDataView;
  using Data_ = internal::BitstreamBufferMetadata_Data;

  template <typename... Args>
  static BitstreamBufferMetadataPtr New(Args&&... args) {
    return BitstreamBufferMetadataPtr(
        base::in_place, std::forward<Args>(args)...);
  }

  template <typename U>
  static BitstreamBufferMetadataPtr From(const U& u) {
    return mojo::TypeConverter<BitstreamBufferMetadataPtr, U>::Convert(u);
  }

  template <typename U>
  U To() const {
    return mojo::TypeConverter<U, BitstreamBufferMetadata>::Convert(*this);
  }


  BitstreamBufferMetadata();

  BitstreamBufferMetadata(
      uint32_t payload_size_bytes,
      bool key_frame,
      base::TimeDelta timestamp,
      const base::Optional<media::Vp8Metadata>& vp8);

  ~BitstreamBufferMetadata();

  // Clone() is a template so it is only instantiated if it is used. Thus, the
  // bindings generator does not need to know whether Clone() or copy
  // constructor/assignment are available for members.
  template <typename StructPtrType = BitstreamBufferMetadataPtr>
  BitstreamBufferMetadataPtr Clone() const;

  // Equals() is a template so it is only instantiated if it is used. Thus, the
  // bindings generator does not need to know whether Equals() or == operator
  // are available for members.
  template <typename T,
            typename std::enable_if<std::is_same<
                T, BitstreamBufferMetadata>::value>::type* = nullptr>
  bool Equals(const T& other) const;

  template <typename UserType>
  static std::vector<uint8_t> Serialize(UserType* input) {
    return mojo::internal::SerializeImpl<
        BitstreamBufferMetadata::DataView, std::vector<uint8_t>>(input);
  }

  template <typename UserType>
  static mojo::Message SerializeAsMessage(UserType* input) {
    return mojo::internal::SerializeAsMessageImpl<
        BitstreamBufferMetadata::DataView>(input);
  }

  // The returned Message is serialized only if the message is moved
  // cross-process or cross-language. Otherwise if the message is Deserialized
  // as the same UserType |input| will just be moved to |output| in
  // DeserializeFromMessage.
  template <typename UserType>
  static mojo::Message WrapAsMessage(UserType input) {
    return mojo::Message(std::make_unique<
        internal::BitstreamBufferMetadata_UnserializedMessageContext<
            UserType, BitstreamBufferMetadata::DataView>>(0, 0, std::move(input)));
  }

  template <typename UserType>
  static bool Deserialize(const void* data,
                          size_t data_num_bytes,
                          UserType* output) {
    return mojo::internal::DeserializeImpl<BitstreamBufferMetadata::DataView>(
        data, data_num_bytes, std::vector<mojo::ScopedHandle>(), output, Validate);
  }

  template <typename UserType>
  static bool Deserialize(const std::vector<uint8_t>& input,
                          UserType* output) {
    return BitstreamBufferMetadata::Deserialize(
        input.size() == 0 ? nullptr : &input.front(), input.size(), output);
  }

  template <typename UserType>
  static bool DeserializeFromMessage(mojo::Message input,
                                     UserType* output) {
    auto context = input.TakeUnserializedContext<
        internal::BitstreamBufferMetadata_UnserializedMessageContext<
            UserType, BitstreamBufferMetadata::DataView>>();
    if (context) {
      *output = std::move(context->TakeData());
      return true;
    }
    input.SerializeIfNecessary();
    return mojo::internal::DeserializeImpl<BitstreamBufferMetadata::DataView>(
        input.payload(), input.payload_num_bytes(),
        std::move(*input.mutable_handles()), output, Validate);
  }

  
  uint32_t payload_size_bytes;
  
  bool key_frame;
  
  base::TimeDelta timestamp;
  
  base::Optional<media::Vp8Metadata> vp8;

 private:
  static bool Validate(const void* data,
                       mojo::internal::ValidationContext* validation_context);
};

template <typename StructPtrType>
VideoBitrateAllocationPtr VideoBitrateAllocation::Clone() const {
  return New(
      mojo::Clone(bitrates)
  );
}

template <typename T,
          typename std::enable_if<std::is_same<
              T, VideoBitrateAllocation>::value>::type*>
bool VideoBitrateAllocation::Equals(const T& other_struct) const {
  if (!mojo::Equals(this->bitrates, other_struct.bitrates))
    return false;
  return true;
}
template <typename StructPtrType>
VideoEncodeAcceleratorConfigPtr VideoEncodeAcceleratorConfig::Clone() const {
  return New(
      mojo::Clone(input_format),
      mojo::Clone(input_visible_size),
      mojo::Clone(output_profile),
      mojo::Clone(initial_bitrate),
      mojo::Clone(initial_framerate),
      mojo::Clone(has_initial_framerate),
      mojo::Clone(gop_length),
      mojo::Clone(has_gop_length),
      mojo::Clone(h264_output_level),
      mojo::Clone(has_h264_output_level),
      mojo::Clone(content_type)
  );
}

template <typename T,
          typename std::enable_if<std::is_same<
              T, VideoEncodeAcceleratorConfig>::value>::type*>
bool VideoEncodeAcceleratorConfig::Equals(const T& other_struct) const {
  if (!mojo::Equals(this->input_format, other_struct.input_format))
    return false;
  if (!mojo::Equals(this->input_visible_size, other_struct.input_visible_size))
    return false;
  if (!mojo::Equals(this->output_profile, other_struct.output_profile))
    return false;
  if (!mojo::Equals(this->initial_bitrate, other_struct.initial_bitrate))
    return false;
  if (!mojo::Equals(this->initial_framerate, other_struct.initial_framerate))
    return false;
  if (!mojo::Equals(this->has_initial_framerate, other_struct.has_initial_framerate))
    return false;
  if (!mojo::Equals(this->gop_length, other_struct.gop_length))
    return false;
  if (!mojo::Equals(this->has_gop_length, other_struct.has_gop_length))
    return false;
  if (!mojo::Equals(this->h264_output_level, other_struct.h264_output_level))
    return false;
  if (!mojo::Equals(this->has_h264_output_level, other_struct.has_h264_output_level))
    return false;
  if (!mojo::Equals(this->content_type, other_struct.content_type))
    return false;
  return true;
}
template <typename StructPtrType>
Vp8MetadataPtr Vp8Metadata::Clone() const {
  return New(
      mojo::Clone(non_reference),
      mojo::Clone(temporal_idx),
      mojo::Clone(layer_sync)
  );
}

template <typename T,
          typename std::enable_if<std::is_same<
              T, Vp8Metadata>::value>::type*>
bool Vp8Metadata::Equals(const T& other_struct) const {
  if (!mojo::Equals(this->non_reference, other_struct.non_reference))
    return false;
  if (!mojo::Equals(this->temporal_idx, other_struct.temporal_idx))
    return false;
  if (!mojo::Equals(this->layer_sync, other_struct.layer_sync))
    return false;
  return true;
}
template <typename StructPtrType>
BitstreamBufferMetadataPtr BitstreamBufferMetadata::Clone() const {
  return New(
      mojo::Clone(payload_size_bytes),
      mojo::Clone(key_frame),
      mojo::Clone(timestamp),
      mojo::Clone(vp8)
  );
}

template <typename T,
          typename std::enable_if<std::is_same<
              T, BitstreamBufferMetadata>::value>::type*>
bool BitstreamBufferMetadata::Equals(const T& other_struct) const {
  if (!mojo::Equals(this->payload_size_bytes, other_struct.payload_size_bytes))
    return false;
  if (!mojo::Equals(this->key_frame, other_struct.key_frame))
    return false;
  if (!mojo::Equals(this->timestamp, other_struct.timestamp))
    return false;
  if (!mojo::Equals(this->vp8, other_struct.vp8))
    return false;
  return true;
}


}  // namespace mojom
}  // namespace media

namespace mojo {


template <>
struct  StructTraits<::media::mojom::VideoBitrateAllocation::DataView,
                                         ::media::mojom::VideoBitrateAllocationPtr> {
  static bool IsNull(const ::media::mojom::VideoBitrateAllocationPtr& input) { return !input; }
  static void SetToNull(::media::mojom::VideoBitrateAllocationPtr* output) { output->reset(); }

  static const decltype(::media::mojom::VideoBitrateAllocation::bitrates)& bitrates(
      const ::media::mojom::VideoBitrateAllocationPtr& input) {
    return input->bitrates;
  }

  static bool Read(::media::mojom::VideoBitrateAllocation::DataView input, ::media::mojom::VideoBitrateAllocationPtr* output);
};


template <>
struct  StructTraits<::media::mojom::VideoEncodeAcceleratorConfig::DataView,
                                         ::media::mojom::VideoEncodeAcceleratorConfigPtr> {
  static bool IsNull(const ::media::mojom::VideoEncodeAcceleratorConfigPtr& input) { return !input; }
  static void SetToNull(::media::mojom::VideoEncodeAcceleratorConfigPtr* output) { output->reset(); }

  static decltype(::media::mojom::VideoEncodeAcceleratorConfig::input_format) input_format(
      const ::media::mojom::VideoEncodeAcceleratorConfigPtr& input) {
    return input->input_format;
  }

  static const decltype(::media::mojom::VideoEncodeAcceleratorConfig::input_visible_size)& input_visible_size(
      const ::media::mojom::VideoEncodeAcceleratorConfigPtr& input) {
    return input->input_visible_size;
  }

  static decltype(::media::mojom::VideoEncodeAcceleratorConfig::output_profile) output_profile(
      const ::media::mojom::VideoEncodeAcceleratorConfigPtr& input) {
    return input->output_profile;
  }

  static decltype(::media::mojom::VideoEncodeAcceleratorConfig::initial_bitrate) initial_bitrate(
      const ::media::mojom::VideoEncodeAcceleratorConfigPtr& input) {
    return input->initial_bitrate;
  }

  static decltype(::media::mojom::VideoEncodeAcceleratorConfig::initial_framerate) initial_framerate(
      const ::media::mojom::VideoEncodeAcceleratorConfigPtr& input) {
    return input->initial_framerate;
  }

  static decltype(::media::mojom::VideoEncodeAcceleratorConfig::has_initial_framerate) has_initial_framerate(
      const ::media::mojom::VideoEncodeAcceleratorConfigPtr& input) {
    return input->has_initial_framerate;
  }

  static decltype(::media::mojom::VideoEncodeAcceleratorConfig::gop_length) gop_length(
      const ::media::mojom::VideoEncodeAcceleratorConfigPtr& input) {
    return input->gop_length;
  }

  static decltype(::media::mojom::VideoEncodeAcceleratorConfig::has_gop_length) has_gop_length(
      const ::media::mojom::VideoEncodeAcceleratorConfigPtr& input) {
    return input->has_gop_length;
  }

  static decltype(::media::mojom::VideoEncodeAcceleratorConfig::h264_output_level) h264_output_level(
      const ::media::mojom::VideoEncodeAcceleratorConfigPtr& input) {
    return input->h264_output_level;
  }

  static decltype(::media::mojom::VideoEncodeAcceleratorConfig::has_h264_output_level) has_h264_output_level(
      const ::media::mojom::VideoEncodeAcceleratorConfigPtr& input) {
    return input->has_h264_output_level;
  }

  static decltype(::media::mojom::VideoEncodeAcceleratorConfig::content_type) content_type(
      const ::media::mojom::VideoEncodeAcceleratorConfigPtr& input) {
    return input->content_type;
  }

  static bool Read(::media::mojom::VideoEncodeAcceleratorConfig::DataView input, ::media::mojom::VideoEncodeAcceleratorConfigPtr* output);
};


template <>
struct  StructTraits<::media::mojom::Vp8Metadata::DataView,
                                         ::media::mojom::Vp8MetadataPtr> {
  static bool IsNull(const ::media::mojom::Vp8MetadataPtr& input) { return !input; }
  static void SetToNull(::media::mojom::Vp8MetadataPtr* output) { output->reset(); }

  static decltype(::media::mojom::Vp8Metadata::non_reference) non_reference(
      const ::media::mojom::Vp8MetadataPtr& input) {
    return input->non_reference;
  }

  static decltype(::media::mojom::Vp8Metadata::temporal_idx) temporal_idx(
      const ::media::mojom::Vp8MetadataPtr& input) {
    return input->temporal_idx;
  }

  static decltype(::media::mojom::Vp8Metadata::layer_sync) layer_sync(
      const ::media::mojom::Vp8MetadataPtr& input) {
    return input->layer_sync;
  }

  static bool Read(::media::mojom::Vp8Metadata::DataView input, ::media::mojom::Vp8MetadataPtr* output);
};


template <>
struct  StructTraits<::media::mojom::BitstreamBufferMetadata::DataView,
                                         ::media::mojom::BitstreamBufferMetadataPtr> {
  static bool IsNull(const ::media::mojom::BitstreamBufferMetadataPtr& input) { return !input; }
  static void SetToNull(::media::mojom::BitstreamBufferMetadataPtr* output) { output->reset(); }

  static decltype(::media::mojom::BitstreamBufferMetadata::payload_size_bytes) payload_size_bytes(
      const ::media::mojom::BitstreamBufferMetadataPtr& input) {
    return input->payload_size_bytes;
  }

  static decltype(::media::mojom::BitstreamBufferMetadata::key_frame) key_frame(
      const ::media::mojom::BitstreamBufferMetadataPtr& input) {
    return input->key_frame;
  }

  static const decltype(::media::mojom::BitstreamBufferMetadata::timestamp)& timestamp(
      const ::media::mojom::BitstreamBufferMetadataPtr& input) {
    return input->timestamp;
  }

  static const decltype(::media::mojom::BitstreamBufferMetadata::vp8)& vp8(
      const ::media::mojom::BitstreamBufferMetadataPtr& input) {
    return input->vp8;
  }

  static bool Read(::media::mojom::BitstreamBufferMetadata::DataView input, ::media::mojom::BitstreamBufferMetadataPtr* output);
};

}  // namespace mojo

#endif  // MEDIA_MOJO_INTERFACES_VIDEO_ENCODE_ACCELERATOR_MOJOM_H_