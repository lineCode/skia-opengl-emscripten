// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MEDIA_MOJO_INTERFACES_AUDIO_OUTPUT_STREAM_MOJOM_BLINK_H_
#define MEDIA_MOJO_INTERFACES_AUDIO_OUTPUT_STREAM_MOJOM_BLINK_H_

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
#include "media/mojo/interfaces/audio_output_stream.mojom-shared.h"
#include "media/mojo/interfaces/audio_output_stream.mojom-blink-forward.h"
#include "mojo/public/mojom/base/unguessable_token.mojom-blink-forward.h"
#include "media/mojo/interfaces/audio_data_pipe.mojom-blink-forward.h"
#include "media/mojo/interfaces/audio_parameters.mojom-blink-forward.h"
#include "media/mojo/interfaces/media_types.mojom-blink-forward.h"

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
#include "third_party/blink/public/platform/web_common.h"




namespace WTF {
struct media_mojom_internal_AudioOutputStreamObserver_DisconnectReason_DataHashFn {
  static unsigned GetHash(const ::media::mojom::AudioOutputStreamObserver_DisconnectReason& value) {
    using utype = std::underlying_type<::media::mojom::AudioOutputStreamObserver_DisconnectReason>::type;
    return DefaultHash<utype>::Hash().GetHash(static_cast<utype>(value));
  }
  static bool Equal(const ::media::mojom::AudioOutputStreamObserver_DisconnectReason& left, const ::media::mojom::AudioOutputStreamObserver_DisconnectReason& right) {
    return left == right;
  }
  static const bool safe_to_compare_to_empty_or_deleted = true;
};

template <>
struct HashTraits<::media::mojom::AudioOutputStreamObserver_DisconnectReason>
    : public GenericHashTraits<::media::mojom::AudioOutputStreamObserver_DisconnectReason> {
  static_assert(true,
                "-1000000 is a reserved enum value");
  static_assert(true,
                "-1000001 is a reserved enum value");
  static const bool hasIsEmptyValueFunction = true;
  static bool IsEmptyValue(const ::media::mojom::AudioOutputStreamObserver_DisconnectReason& value) {
    return value == static_cast<::media::mojom::AudioOutputStreamObserver_DisconnectReason>(-1000000);
  }
  static void ConstructDeletedValue(::media::mojom::AudioOutputStreamObserver_DisconnectReason& slot, bool) {
    slot = static_cast<::media::mojom::AudioOutputStreamObserver_DisconnectReason>(-1000001);
  }
  static bool IsDeletedValue(const ::media::mojom::AudioOutputStreamObserver_DisconnectReason& value) {
    return value == static_cast<::media::mojom::AudioOutputStreamObserver_DisconnectReason>(-1000001);
  }
};
}  // namespace WTF


namespace media {
namespace mojom {
namespace blink {

class AudioOutputStreamProxy;

template <typename ImplRefTraits>
class AudioOutputStreamStub;

class AudioOutputStreamRequestValidator;


class BLINK_PLATFORM_EXPORT AudioOutputStream
    : public AudioOutputStreamInterfaceBase {
 public:
  static const char Name_[];
  static constexpr uint32_t Version_ = 0;
  static constexpr bool PassesAssociatedKinds_ = false;
  static constexpr bool HasSyncMethods_ = false;

  using Base_ = AudioOutputStreamInterfaceBase;
  using Proxy_ = AudioOutputStreamProxy;

  template <typename ImplRefTraits>
  using Stub_ = AudioOutputStreamStub<ImplRefTraits>;

  using RequestValidator_ = AudioOutputStreamRequestValidator;
  using ResponseValidator_ = mojo::PassThroughFilter;
  enum MethodMinVersions : uint32_t {
    kPlayMinVersion = 0,
    kPauseMinVersion = 0,
    kSetVolumeMinVersion = 0,
  };
  virtual ~AudioOutputStream() {}

  
  virtual void Play() = 0;

  
  virtual void Pause() = 0;

  
  virtual void SetVolume(double volume) = 0;
};

class AudioOutputStreamObserverProxy;

template <typename ImplRefTraits>
class AudioOutputStreamObserverStub;

class AudioOutputStreamObserverRequestValidator;


class BLINK_PLATFORM_EXPORT AudioOutputStreamObserver
    : public AudioOutputStreamObserverInterfaceBase {
 public:
  static const char Name_[];
  static constexpr uint32_t Version_ = 0;
  static constexpr bool PassesAssociatedKinds_ = false;
  static constexpr bool HasSyncMethods_ = false;

  using Base_ = AudioOutputStreamObserverInterfaceBase;
  using Proxy_ = AudioOutputStreamObserverProxy;

  template <typename ImplRefTraits>
  using Stub_ = AudioOutputStreamObserverStub<ImplRefTraits>;

  using RequestValidator_ = AudioOutputStreamObserverRequestValidator;
  using ResponseValidator_ = mojo::PassThroughFilter;
  enum MethodMinVersions : uint32_t {
    kDidStartPlayingMinVersion = 0,
    kDidStopPlayingMinVersion = 0,
    kDidChangeAudibleStateMinVersion = 0,
  };
  
  using DisconnectReason = AudioOutputStreamObserver_DisconnectReason;
  virtual ~AudioOutputStreamObserver() {}

  
  virtual void DidStartPlaying() = 0;

  
  virtual void DidStopPlaying() = 0;

  
  virtual void DidChangeAudibleState(bool is_audible) = 0;
};

class AudioOutputStreamProviderProxy;

template <typename ImplRefTraits>
class AudioOutputStreamProviderStub;

class AudioOutputStreamProviderRequestValidator;


class BLINK_PLATFORM_EXPORT AudioOutputStreamProvider
    : public AudioOutputStreamProviderInterfaceBase {
 public:
  static const char Name_[];
  static constexpr uint32_t Version_ = 0;
  static constexpr bool PassesAssociatedKinds_ = false;
  static constexpr bool HasSyncMethods_ = false;

  using Base_ = AudioOutputStreamProviderInterfaceBase;
  using Proxy_ = AudioOutputStreamProviderProxy;

  template <typename ImplRefTraits>
  using Stub_ = AudioOutputStreamProviderStub<ImplRefTraits>;

  using RequestValidator_ = AudioOutputStreamProviderRequestValidator;
  using ResponseValidator_ = mojo::PassThroughFilter;
  enum MethodMinVersions : uint32_t {
    kAcquireMinVersion = 0,
  };
  virtual ~AudioOutputStreamProvider() {}

  
  virtual void Acquire(::media::mojom::blink::AudioParametersPtr params, AudioOutputStreamProviderClientPtr client, const base::Optional<base::UnguessableToken>& processing_id) = 0;
};

class AudioOutputStreamProviderClientProxy;

template <typename ImplRefTraits>
class AudioOutputStreamProviderClientStub;

class AudioOutputStreamProviderClientRequestValidator;


class BLINK_PLATFORM_EXPORT AudioOutputStreamProviderClient
    : public AudioOutputStreamProviderClientInterfaceBase {
 public:
  static const char Name_[];
  static constexpr uint32_t Version_ = 0;
  static constexpr bool PassesAssociatedKinds_ = false;
  static constexpr bool HasSyncMethods_ = false;

  using Base_ = AudioOutputStreamProviderClientInterfaceBase;
  using Proxy_ = AudioOutputStreamProviderClientProxy;

  template <typename ImplRefTraits>
  using Stub_ = AudioOutputStreamProviderClientStub<ImplRefTraits>;

  using RequestValidator_ = AudioOutputStreamProviderClientRequestValidator;
  using ResponseValidator_ = mojo::PassThroughFilter;
  enum MethodMinVersions : uint32_t {
    kCreatedMinVersion = 0,
  };
  virtual ~AudioOutputStreamProviderClient() {}

  
  virtual void Created(AudioOutputStreamPtr stream, ::media::mojom::blink::ReadWriteAudioDataPipePtr data_pipe) = 0;
};

class BLINK_PLATFORM_EXPORT AudioOutputStreamProxy
    : public AudioOutputStream {
 public:
  using InterfaceType = AudioOutputStream;

  explicit AudioOutputStreamProxy(mojo::MessageReceiverWithResponder* receiver);
  void Play() final;
  void Pause() final;
  void SetVolume(double volume) final;

 private:
  mojo::MessageReceiverWithResponder* receiver_;
};

class BLINK_PLATFORM_EXPORT AudioOutputStreamObserverProxy
    : public AudioOutputStreamObserver {
 public:
  using InterfaceType = AudioOutputStreamObserver;

  explicit AudioOutputStreamObserverProxy(mojo::MessageReceiverWithResponder* receiver);
  void DidStartPlaying() final;
  void DidStopPlaying() final;
  void DidChangeAudibleState(bool is_audible) final;

 private:
  mojo::MessageReceiverWithResponder* receiver_;
};

class BLINK_PLATFORM_EXPORT AudioOutputStreamProviderProxy
    : public AudioOutputStreamProvider {
 public:
  using InterfaceType = AudioOutputStreamProvider;

  explicit AudioOutputStreamProviderProxy(mojo::MessageReceiverWithResponder* receiver);
  void Acquire(::media::mojom::blink::AudioParametersPtr params, AudioOutputStreamProviderClientPtr client, const base::Optional<base::UnguessableToken>& processing_id) final;

 private:
  mojo::MessageReceiverWithResponder* receiver_;
};

class BLINK_PLATFORM_EXPORT AudioOutputStreamProviderClientProxy
    : public AudioOutputStreamProviderClient {
 public:
  using InterfaceType = AudioOutputStreamProviderClient;

  explicit AudioOutputStreamProviderClientProxy(mojo::MessageReceiverWithResponder* receiver);
  void Created(AudioOutputStreamPtr stream, ::media::mojom::blink::ReadWriteAudioDataPipePtr data_pipe) final;

 private:
  mojo::MessageReceiverWithResponder* receiver_;
};
class BLINK_PLATFORM_EXPORT AudioOutputStreamStubDispatch {
 public:
  static bool Accept(AudioOutputStream* impl, mojo::Message* message);
  static bool AcceptWithResponder(
      AudioOutputStream* impl,
      mojo::Message* message,
      std::unique_ptr<mojo::MessageReceiverWithStatus> responder);
};

template <typename ImplRefTraits =
              mojo::RawPtrImplRefTraits<AudioOutputStream>>
class AudioOutputStreamStub
    : public mojo::MessageReceiverWithResponderStatus {
 public:
  using ImplPointerType = typename ImplRefTraits::PointerType;

  AudioOutputStreamStub() {}
  ~AudioOutputStreamStub() override {}

  void set_sink(ImplPointerType sink) { sink_ = std::move(sink); }
  ImplPointerType& sink() { return sink_; }

  bool Accept(mojo::Message* message) override {
    if (ImplRefTraits::IsNull(sink_))
      return false;
    return AudioOutputStreamStubDispatch::Accept(
        ImplRefTraits::GetRawPointer(&sink_), message);
  }

  bool AcceptWithResponder(
      mojo::Message* message,
      std::unique_ptr<mojo::MessageReceiverWithStatus> responder) override {
    if (ImplRefTraits::IsNull(sink_))
      return false;
    return AudioOutputStreamStubDispatch::AcceptWithResponder(
        ImplRefTraits::GetRawPointer(&sink_), message, std::move(responder));
  }

 private:
  ImplPointerType sink_;
};
class BLINK_PLATFORM_EXPORT AudioOutputStreamObserverStubDispatch {
 public:
  static bool Accept(AudioOutputStreamObserver* impl, mojo::Message* message);
  static bool AcceptWithResponder(
      AudioOutputStreamObserver* impl,
      mojo::Message* message,
      std::unique_ptr<mojo::MessageReceiverWithStatus> responder);
};

template <typename ImplRefTraits =
              mojo::RawPtrImplRefTraits<AudioOutputStreamObserver>>
class AudioOutputStreamObserverStub
    : public mojo::MessageReceiverWithResponderStatus {
 public:
  using ImplPointerType = typename ImplRefTraits::PointerType;

  AudioOutputStreamObserverStub() {}
  ~AudioOutputStreamObserverStub() override {}

  void set_sink(ImplPointerType sink) { sink_ = std::move(sink); }
  ImplPointerType& sink() { return sink_; }

  bool Accept(mojo::Message* message) override {
    if (ImplRefTraits::IsNull(sink_))
      return false;
    return AudioOutputStreamObserverStubDispatch::Accept(
        ImplRefTraits::GetRawPointer(&sink_), message);
  }

  bool AcceptWithResponder(
      mojo::Message* message,
      std::unique_ptr<mojo::MessageReceiverWithStatus> responder) override {
    if (ImplRefTraits::IsNull(sink_))
      return false;
    return AudioOutputStreamObserverStubDispatch::AcceptWithResponder(
        ImplRefTraits::GetRawPointer(&sink_), message, std::move(responder));
  }

 private:
  ImplPointerType sink_;
};
class BLINK_PLATFORM_EXPORT AudioOutputStreamProviderStubDispatch {
 public:
  static bool Accept(AudioOutputStreamProvider* impl, mojo::Message* message);
  static bool AcceptWithResponder(
      AudioOutputStreamProvider* impl,
      mojo::Message* message,
      std::unique_ptr<mojo::MessageReceiverWithStatus> responder);
};

template <typename ImplRefTraits =
              mojo::RawPtrImplRefTraits<AudioOutputStreamProvider>>
class AudioOutputStreamProviderStub
    : public mojo::MessageReceiverWithResponderStatus {
 public:
  using ImplPointerType = typename ImplRefTraits::PointerType;

  AudioOutputStreamProviderStub() {}
  ~AudioOutputStreamProviderStub() override {}

  void set_sink(ImplPointerType sink) { sink_ = std::move(sink); }
  ImplPointerType& sink() { return sink_; }

  bool Accept(mojo::Message* message) override {
    if (ImplRefTraits::IsNull(sink_))
      return false;
    return AudioOutputStreamProviderStubDispatch::Accept(
        ImplRefTraits::GetRawPointer(&sink_), message);
  }

  bool AcceptWithResponder(
      mojo::Message* message,
      std::unique_ptr<mojo::MessageReceiverWithStatus> responder) override {
    if (ImplRefTraits::IsNull(sink_))
      return false;
    return AudioOutputStreamProviderStubDispatch::AcceptWithResponder(
        ImplRefTraits::GetRawPointer(&sink_), message, std::move(responder));
  }

 private:
  ImplPointerType sink_;
};
class BLINK_PLATFORM_EXPORT AudioOutputStreamProviderClientStubDispatch {
 public:
  static bool Accept(AudioOutputStreamProviderClient* impl, mojo::Message* message);
  static bool AcceptWithResponder(
      AudioOutputStreamProviderClient* impl,
      mojo::Message* message,
      std::unique_ptr<mojo::MessageReceiverWithStatus> responder);
};

template <typename ImplRefTraits =
              mojo::RawPtrImplRefTraits<AudioOutputStreamProviderClient>>
class AudioOutputStreamProviderClientStub
    : public mojo::MessageReceiverWithResponderStatus {
 public:
  using ImplPointerType = typename ImplRefTraits::PointerType;

  AudioOutputStreamProviderClientStub() {}
  ~AudioOutputStreamProviderClientStub() override {}

  void set_sink(ImplPointerType sink) { sink_ = std::move(sink); }
  ImplPointerType& sink() { return sink_; }

  bool Accept(mojo::Message* message) override {
    if (ImplRefTraits::IsNull(sink_))
      return false;
    return AudioOutputStreamProviderClientStubDispatch::Accept(
        ImplRefTraits::GetRawPointer(&sink_), message);
  }

  bool AcceptWithResponder(
      mojo::Message* message,
      std::unique_ptr<mojo::MessageReceiverWithStatus> responder) override {
    if (ImplRefTraits::IsNull(sink_))
      return false;
    return AudioOutputStreamProviderClientStubDispatch::AcceptWithResponder(
        ImplRefTraits::GetRawPointer(&sink_), message, std::move(responder));
  }

 private:
  ImplPointerType sink_;
};
class BLINK_PLATFORM_EXPORT AudioOutputStreamRequestValidator : public mojo::MessageReceiver {
 public:
  bool Accept(mojo::Message* message) override;
};
class BLINK_PLATFORM_EXPORT AudioOutputStreamObserverRequestValidator : public mojo::MessageReceiver {
 public:
  bool Accept(mojo::Message* message) override;
};
class BLINK_PLATFORM_EXPORT AudioOutputStreamProviderRequestValidator : public mojo::MessageReceiver {
 public:
  bool Accept(mojo::Message* message) override;
};
class BLINK_PLATFORM_EXPORT AudioOutputStreamProviderClientRequestValidator : public mojo::MessageReceiver {
 public:
  bool Accept(mojo::Message* message) override;
};





}  // namespace blink
}  // namespace mojom
}  // namespace media

namespace mojo {

}  // namespace mojo

#endif  // MEDIA_MOJO_INTERFACES_AUDIO_OUTPUT_STREAM_MOJOM_BLINK_H_