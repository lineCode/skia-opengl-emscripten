// Copyright 2015 The Cobalt Authors. All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "cobalt/media/base/shell_audio_bus.h"

#include <algorithm>
#include <limits>

#include "starboard/memory.h"

namespace cobalt {
namespace media {

namespace {

typedef ShellAudioBus::StorageType StorageType;
typedef ShellAudioBus::SampleType SampleType;

const float kFloat32ToInt16Factor = 32768.f;

inline void ConvertSample(ShellAudioBus::SampleType src_type,
                          const uint8* src_ptr,
                          ShellAudioBus::SampleType dest_type,
                          uint8* dest_ptr) {
  if (src_type == dest_type) {
    SbMemoryCopy(
        dest_ptr, src_ptr,
        src_type == ShellAudioBus::kInt16 ? sizeof(int16) : sizeof(float));
  } else if (src_type == ShellAudioBus::kFloat32) {
    float sample_in_float = *reinterpret_cast<const float*>(src_ptr);
    int32 sample_in_int32 =
        static_cast<int32>(sample_in_float * kFloat32ToInt16Factor);
    sample_in_int32 =
        std::max<int32>(sample_in_int32, std::numeric_limits<int16>::min());
    sample_in_int32 =
        std::min<int32>(sample_in_int32, std::numeric_limits<int16>::max());
    *reinterpret_cast<int16*>(dest_ptr) = static_cast<int16>(sample_in_int32);
  } else {
    int16 sample = *reinterpret_cast<const int16*>(src_ptr);
    *reinterpret_cast<float*>(dest_ptr) =
        static_cast<float>(sample) / kFloat32ToInt16Factor;
  }
}

void Sum(const float* source, float* destination, size_t size) {
  for (int i = 0; i < size; ++i) {
    *destination += *source;
    ++source;
    ++destination;
  }
}

void Sum(const int16* source, int16* destination, size_t size) {
  for (int i = 0; i < size; ++i) {
    int sample_in_int32 = *destination + static_cast<int>(*source);
    if (sample_in_int32 > std::numeric_limits<int16>::max()) {
      *destination = std::numeric_limits<int16>::max();
    } else if (sample_in_int32 < std::numeric_limits<int16>::min()) {
      *destination = std::numeric_limits<int16>::min();
    } else {
      *destination = static_cast<int16>(sample_in_int32);
    }
    ++source;
    ++destination;
  }
}

}  // namespace

ShellAudioBus::ShellAudioBus(size_t channels, size_t frames,
                             SampleType sample_type, StorageType storage_type)
    : channels_(channels),
      frames_(frames),
      sample_type_(sample_type),
      storage_type_(storage_type) {
  DCHECK_GT(channels_, 0);

  if (storage_type_ == kInterleaved) {
    data_.reset(static_cast<uint8*>(base::AlignedAlloc(
        GetSampleSizeInBytes() * frames * channels, kChannelAlignmentInBytes)));
    channel_data_.push_back(data_.get());
  } else {
    DCHECK_EQ(storage_type_, kPlanar);
    size_t aligned_per_channel_size_in_bytes =
        (GetSampleSizeInBytes() * frames + kChannelAlignmentInBytes - 1) /
        kChannelAlignmentInBytes * kChannelAlignmentInBytes;
    data_.reset(static_cast<uint8*>(
        base::AlignedAlloc(aligned_per_channel_size_in_bytes * channels,
                           kChannelAlignmentInBytes)));
    channel_data_.reserve(channels);
    for (size_t i = 0; i < channels_; ++i) {
      channel_data_.push_back(data_.get() +
                              aligned_per_channel_size_in_bytes * i);
    }
  }
}

ShellAudioBus::ShellAudioBus(size_t frames, const std::vector<float*>& samples)
    : channels_(samples.size()),
      frames_(frames),
      sample_type_(kFloat32),
      storage_type_(kPlanar) {
  DCHECK_GT(channels_, 0);

  channel_data_.reserve(samples.size());
  for (size_t i = 0; i < samples.size(); ++i) {
    channel_data_.push_back(reinterpret_cast<uint8*>(samples[i]));
  }
}

ShellAudioBus::ShellAudioBus(size_t channels, size_t frames, float* samples)
    : channels_(channels),
      frames_(frames),
      sample_type_(kFloat32),
      storage_type_(kInterleaved) {
  DCHECK_GT(channels_, 0);

  channel_data_.push_back(reinterpret_cast<uint8*>(samples));
}

ShellAudioBus::ShellAudioBus(size_t frames, const std::vector<int16*>& samples)
    : channels_(samples.size()),
      frames_(frames),
      sample_type_(kInt16),
      storage_type_(kPlanar) {
  DCHECK_GT(channels_, 0);

  channel_data_.reserve(samples.size());
  for (size_t i = 0; i < samples.size(); ++i) {
    channel_data_.push_back(reinterpret_cast<uint8*>(samples[i]));
  }
}

ShellAudioBus::ShellAudioBus(size_t channels, size_t frames, int16* samples)
    : channels_(channels),
      frames_(frames),
      sample_type_(kInt16),
      storage_type_(kInterleaved) {
  DCHECK_GT(channels_, 0);

  channel_data_.push_back(reinterpret_cast<uint8*>(samples));
}

size_t ShellAudioBus::GetSampleSizeInBytes() const {
  if (sample_type_ == kInt16) {
    return sizeof(int16);
  }
  DCHECK_EQ(sample_type_, kFloat32);
  return sizeof(float);
}

const uint8* ShellAudioBus::interleaved_data() const {
  DCHECK_EQ(storage_type_, kInterleaved);
  return channel_data_[0];
}

const uint8* ShellAudioBus::planar_data(size_t channel) const {
  DCHECK_LT(channel, channels_);
  DCHECK_EQ(storage_type_, kPlanar);
  return channel_data_[channel];
}

uint8* ShellAudioBus::interleaved_data() {
  DCHECK_EQ(storage_type_, kInterleaved);
  return channel_data_[0];
}

uint8* ShellAudioBus::planar_data(size_t channel) {
  DCHECK_LT(channel, channels_);
  DCHECK_EQ(storage_type_, kPlanar);
  return channel_data_[channel];
}

void ShellAudioBus::ZeroFrames(size_t start_frame, size_t end_frame) {
  DCHECK_LE(start_frame, end_frame);
  DCHECK_LE(end_frame, frames_);
  end_frame = std::min(end_frame, frames_);
  start_frame = std::min(start_frame, end_frame);
  if (start_frame >= end_frame) {
    return;
  }
  if (storage_type_ == kInterleaved) {
    SbMemorySet(GetSamplePtr(0, start_frame), 0,
                GetSampleSizeInBytes() * (end_frame - start_frame) * channels_);
  } else {
    for (size_t channel = 0; channel < channels_; ++channel) {
      SbMemorySet(GetSamplePtr(channel, start_frame), 0,
                  GetSampleSizeInBytes() * (end_frame - start_frame));
    }
  }
}

void ShellAudioBus::Assign(const ShellAudioBus& source) {
  DCHECK_EQ(channels_, source.channels_);
  if (channels_ != source.channels_) {
    ZeroAllFrames();
    return;
  }

  if (sample_type_ == source.sample_type_ &&
      storage_type_ == source.storage_type_) {
    size_t frames = std::min(frames_, source.frames_);
    if (storage_type_ == kInterleaved) {
      SbMemoryCopy(GetSamplePtr(0, 0), source.GetSamplePtr(0, 0),
                   GetSampleSizeInBytes() * frames * channels_);
    } else {
      for (size_t channel = 0; channel < channels_; ++channel) {
        SbMemoryCopy(GetSamplePtr(channel, 0), source.GetSamplePtr(channel, 0),
                     GetSampleSizeInBytes() * frames);
      }
    }
    return;
  }

  size_t frames = std::min(frames_, source.frames_);
  for (size_t channel = 0; channel < channels_; ++channel) {
    for (size_t frame = 0; frame < frames; ++frame) {
      ConvertSample(source.sample_type_, source.GetSamplePtr(channel, frame),
                    sample_type_, GetSamplePtr(channel, frame));
    }
  }
}

void ShellAudioBus::Assign(const ShellAudioBus& source,
                           const std::vector<float>& matrix) {
  DCHECK_EQ(channels() * source.channels(), matrix.size());
  DCHECK_EQ(sample_type_, kFloat32);
  DCHECK_EQ(source.sample_type_, kFloat32);
  if (channels() * source.channels() != matrix.size() ||
      sample_type_ != kFloat32 || source.sample_type_ != kFloat32) {
    ZeroAllFrames();
    return;
  }

  size_t frames = std::min(frames_, source.frames_);
  for (size_t dest_channel = 0; dest_channel < channels_; ++dest_channel) {
    for (size_t frame = 0; frame < frames; ++frame) {
      float mixed_sample = 0.f;
      for (size_t src_channel = 0; src_channel < source.channels_;
           ++src_channel) {
        mixed_sample += source.GetFloat32Sample(src_channel, frame) *
                        matrix[dest_channel * source.channels_ + src_channel];
      }
      SetFloat32Sample(dest_channel, frame, mixed_sample);
    }
  }
}

template <StorageType SourceStorageType, StorageType DestStorageType>
void ShellAudioBus::MixFloatSamples(const ShellAudioBus& source) {
  const size_t frames = std::min(frames_, source.frames_);

  if (SourceStorageType == DestStorageType) {
    if (SourceStorageType == kInterleaved) {
      Sum(reinterpret_cast<const float*>(source.interleaved_data()),
          reinterpret_cast<float*>(interleaved_data()), frames * channels_);
      return;
    }
    for (size_t channel = 0; channel < channels_; ++channel) {
      Sum(reinterpret_cast<const float*>(source.planar_data(channel)),
          reinterpret_cast<float*>(planar_data(channel)), frames);
    }
    return;
  }

  for (size_t channel = 0; channel < channels_; ++channel) {
    for (size_t frame = 0; frame < frames; ++frame) {
      *reinterpret_cast<float*>(
          GetSamplePtrForType<float, DestStorageType>(channel, frame)) +=
          source.GetSampleForType<float, SourceStorageType>(channel, frame);
    }
  }
}

template <StorageType SourceStorageType, StorageType DestStorageType>
void ShellAudioBus::MixInt16Samples(const ShellAudioBus& source) {
  const size_t frames = std::min(frames_, source.frames_);

  if (SourceStorageType == DestStorageType) {
    if (SourceStorageType == kInterleaved) {
      Sum(reinterpret_cast<const int16*>(source.interleaved_data()),
          reinterpret_cast<int16*>(interleaved_data()), frames * channels_);
      return;
    }
    for (size_t channel = 0; channel < channels_; ++channel) {
      Sum(reinterpret_cast<const int16*>(source.planar_data(channel)),
          reinterpret_cast<int16*>(planar_data(channel)), frames);
    }
    return;
  }

  for (size_t channel = 0; channel < channels_; ++channel) {
    for (size_t frame = 0; frame < frames; ++frame) {
      auto& dest_sample = *reinterpret_cast<int16*>(
          GetSamplePtrForType<int16, DestStorageType>(channel, frame));
      int source_sample =
          source.GetSampleForType<int16, SourceStorageType>(channel, frame);
      if (dest_sample + source_sample > std::numeric_limits<int16>::max()) {
        dest_sample = std::numeric_limits<int16>::max();
      } else if (dest_sample + source_sample <
                 std::numeric_limits<int16>::min()) {
        dest_sample = std::numeric_limits<int16>::min();
      } else {
        dest_sample += source_sample;
      }
    }
  }
}

void ShellAudioBus::Mix(const ShellAudioBus& source) {
  DCHECK_EQ(channels_, source.channels_);
  DCHECK_EQ(sample_type_, source.sample_type_);

  if (channels_ != source.channels_ || sample_type_ != source.sample_type_) {
    ZeroAllFrames();
    return;
  }

  // Profiling has identified this area of code as hot, so instead of calling
  // GetSamplePtr, which branches each time it is called, we branch once before
  // we loop and inline the branch of the function we want.
  if (source.sample_type_ == kInt16 && source.storage_type_ == kInterleaved &&
      storage_type_ == kInterleaved) {
    MixInt16Samples<kInterleaved, kInterleaved>(source);
  } else if (sample_type_ == kInt16 && source.storage_type_ == kInterleaved &&
             storage_type_ == kPlanar) {
    MixInt16Samples<kInterleaved, kPlanar>(source);
  } else if (sample_type_ == kInt16 && source.storage_type_ == kPlanar &&
             storage_type_ == kInterleaved) {
    MixInt16Samples<kPlanar, kInterleaved>(source);
  } else if (sample_type_ == kInt16 && source.storage_type_ == kPlanar &&
             storage_type_ == kPlanar) {
    MixInt16Samples<kPlanar, kPlanar>(source);
  } else if (sample_type_ == kFloat32 && source.storage_type_ == kInterleaved &&
             storage_type_ == kInterleaved) {
    MixFloatSamples<kInterleaved, kInterleaved>(source);
  } else if (sample_type_ == kFloat32 && source.storage_type_ == kInterleaved &&
             storage_type_ == kPlanar) {
    MixFloatSamples<kInterleaved, kPlanar>(source);
  } else if (sample_type_ == kFloat32 && source.storage_type_ == kPlanar &&
             storage_type_ == kInterleaved) {
    MixFloatSamples<kPlanar, kInterleaved>(source);
  } else if (sample_type_ == kFloat32 && source.storage_type_ == kPlanar &&
             storage_type_ == kPlanar) {
    MixFloatSamples<kPlanar, kPlanar>(source);
  } else {
    NOTREACHED();
  }
}

void ShellAudioBus::Mix(const ShellAudioBus& source,
                        const std::vector<float>& matrix) {
  DCHECK_EQ(channels() * source.channels(), matrix.size());
  DCHECK_EQ(sample_type_, kFloat32);
  DCHECK_EQ(source.sample_type_, kFloat32);
  if (channels() * source.channels() != matrix.size() ||
      sample_type_ != kFloat32 || source.sample_type_ != kFloat32) {
    ZeroAllFrames();
    return;
  }

  size_t frames = std::min(frames_, source.frames_);
  for (size_t dest_channel = 0; dest_channel < channels_; ++dest_channel) {
    for (size_t frame = 0; frame < frames; ++frame) {
      float mixed_sample = 0.f;
      for (size_t src_channel = 0; src_channel < source.channels_;
           ++src_channel) {
        mixed_sample += source.GetFloat32Sample(src_channel, frame) *
                        matrix[dest_channel * source.channels_ + src_channel];
      }
      mixed_sample += GetFloat32Sample(dest_channel, frame);
      SetFloat32Sample(dest_channel, frame, mixed_sample);
    }
  }
}

uint8* ShellAudioBus::GetSamplePtr(size_t channel, size_t frame) {
  DCHECK_LT(channel, channels_);
  DCHECK_LT(frame, frames_);

  if (storage_type_ == kInterleaved) {
    return channel_data_[0] +
           GetSampleSizeInBytes() * (channels_ * frame + channel);
  } else {
    return channel_data_[channel] + GetSampleSizeInBytes() * frame;
  }
}

const uint8* ShellAudioBus::GetSamplePtr(size_t channel, size_t frame) const {
  DCHECK_LT(channel, channels_);
  DCHECK_LT(frame, frames_);

  if (storage_type_ == kInterleaved) {
    return channel_data_[0] +
           GetSampleSizeInBytes() * (channels_ * frame + channel);
  } else {
    return channel_data_[channel] + GetSampleSizeInBytes() * frame;
  }
}

}  // namespace media
}  // namespace cobalt
