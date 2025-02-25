// Copyright 2016 The Cobalt Authors. All Rights Reserved.
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

// Module Overview: Starboard Audio Sink API
//
// Provides an interface to output raw audio data.

#ifndef STARBOARD_AUDIO_SINK_H_
#define STARBOARD_AUDIO_SINK_H_

#include "starboard/configuration.h"

#include "starboard/export.h"
#include "starboard/media.h"

#ifdef __cplusplus
extern "C" {
#endif

// --- Types -----------------------------------------------------------------

// An array of frame buffers.  For interleaved audio streams, there will be
// only one element in the array.  For planar audio streams, the number of
// elements in the array equal to the number of channels.
typedef void** SbAudioSinkFrameBuffers;

// An opaque handle to an implementation-private structure representing an
// audio sink.
typedef struct SbAudioSinkPrivate* SbAudioSink;

// Callback being called periodically to retrieve the status of the audio
// source.  The first two output parameters indicating the filling level of the
// audio frame buffer passed to SbAudioSinkCreate as |frame_buffers|.  As
// |frame_buffers| is a circular buffer, |offset_in_frames| can be used to
// calculate the number of continuous frames towards the end of the buffer.
// The audio sink can play the frames only when |is_playing| is true.  It should
// pause the playback when |is_playing| is false.
// The audio sink may cache certain amount of audio frames before start
// playback.  It will start playback immediately when |is_eos_reached| is true
// even if the amount of cached audio frames are not enough as no more frames
// will be appended into the buffer when |is_eos_reached| is true.
// It is possible for a host to set |is_eos_reached| to false after setting it
// to true, usually this is caused by a seek.
// All parameters except |context| cannot be NULL.
// Note that this function only reports the status of the source, it doesn't
// remove audio data from the source frame buffer.
typedef void (*SbAudioSinkUpdateSourceStatusFunc)(int* frames_in_buffer,
                                                  int* offset_in_frames,
                                                  bool* is_playing,
                                                  bool* is_eos_reached,
                                                  void* context);

// Callback used to report frames consumed.  The consumed frames will be
// removed from the source frame buffer to free space for new audio frames.
#if SB_HAS(ASYNC_AUDIO_FRAMES_REPORTING)
// When |frames_consumed| is updated asynchnously and the last time that it has
// been updated is known, it can be passed in |frames_consumed_at| so the audio
// time calculating can be more accurate.
#endif  // SB_HAS(ASYNC_AUDIO_FRAMES_REPORTING)
typedef void (*SbAudioSinkConsumeFramesFunc)(int frames_consumed,
#if SB_HAS(ASYNC_AUDIO_FRAMES_REPORTING)
                                             SbTime frames_consumed_at,
#endif  // SB_HAS(ASYNC_AUDIO_FRAMES_REPORTING)
                                             void* context);

// Well-defined value for an invalid audio sink.
#define kSbAudioSinkInvalid ((SbAudioSink)NULL)

// --- Functions -------------------------------------------------------------

// Indicates whether the given audio sink handle is valid.
//
// |audio_sink|: The audio sink handle to check.
SB_EXPORT bool SbAudioSinkIsValid(SbAudioSink audio_sink);

// Creates an audio sink for the specified |channels| and
// |sampling_frequency_hz|, acquires all resources needed to operate the
// audio sink, and returns an opaque handle to the audio sink.
//
// If the particular platform doesn't support the requested audio sink, the
// function returns |kSbAudioSinkInvalid| without calling any of the callbacks.
// If there is a platform limitation on how many audio sinks can coexist
// simultaneously, then calls made to this function that attempt to exceed
// that limit must return |kSbAudioSinkInvalid|. Multiple calls to
// SbAudioSinkCreate must not cause a crash.
//
// |channels|: The number of audio channels, such as left and right channels
// in stereo audio.
// |sampling_frequency_hz|: The sample frequency of the audio data being
// streamed. For example, 22,000 Hz means 22,000 sample elements represents
// one second of audio data.
// |audio_sample_type|: The type of each sample of the audio data --
// |int16|, |float32|, etc.
// |audio_frame_storage_type|: Indicates whether frames are interleaved or
// planar.
// |frame_buffers|: An array of pointers to sample data.
// - If the sink is operating in interleaved mode, the array contains only
//   one element, which is an array containing (|frames_per_channel| *
//   |channels|) samples.
// - If the sink is operating in planar mode, the number of elements in the
//   array is the same as |channels|, and each element is an array of
//   |frames_per_channel| samples. The caller has to ensure that
//   |frame_buffers| is valid until SbAudioSinkDestroy is called.
// |frames_per_channel|: The size of the frame buffers, in units of the
// number of samples per channel. The frame, in this case, represents a
// group of samples at the same media time, one for each channel.
// |update_source_status_func|: The audio sink calls this function on an
// internal thread to query the status of the source. It is called
// immediately after SbAudioSinkCreate is called, even before it returns.
// The caller has to ensure that the callback functions above return
// meaningful values in this case. The value cannot be NULL.
// |consume_frames_func|: The audio sink calls this function on an internal
// thread to report consumed frames. The value cannot be NULL.
// |context|: A value that is passed back to all callbacks and is generally
// used to point at a class or struct that contains state associated with the
// audio sink.

SB_EXPORT SbAudioSink
SbAudioSinkCreate(int channels,
                  int sampling_frequency_hz,
                  SbMediaAudioSampleType audio_sample_type,
                  SbMediaAudioFrameStorageType audio_frame_storage_type,
                  SbAudioSinkFrameBuffers frame_buffers,
                  int frames_per_channel,
                  SbAudioSinkUpdateSourceStatusFunc update_source_status_func,
                  SbAudioSinkConsumeFramesFunc consume_frames_func,
                  void* context);

// Destroys |audio_sink|, freeing all associated resources. Before
// returning, the function waits until all callbacks that are in progress
// have finished. After the function returns, no further calls are made
// callbacks passed into SbAudioSinkCreate. In addition, you can not pass
// |audio_sink| to any other SbAudioSink functions after SbAudioSinkDestroy
// has been called on it.
//
// This function can be called on any thread. However, it cannot be called
// within any of the callbacks passed into SbAudioSinkCreate.
//
// |audio_sink|: The audio sink to destroy.
SB_EXPORT void SbAudioSinkDestroy(SbAudioSink audio_sink);

// Returns the maximum number of channels supported on the platform. For
// example, the number would be |2| if the platform only supports stereo.
SB_EXPORT int SbAudioSinkGetMaxChannels();

// Returns the supported sample rate closest to |sampling_frequency_hz|.
// On platforms that don't support all sample rates, it is the caller's
// responsibility to resample the audio frames into the supported sample rate
// returned by this function.
SB_EXPORT int SbAudioSinkGetNearestSupportedSampleFrequency(
    int sampling_frequency_hz);

// Indicates whether |audio_sample_type| is supported on this platform.
SB_EXPORT bool SbAudioSinkIsAudioSampleTypeSupported(
    SbMediaAudioSampleType audio_sample_type);

// Indicates whether |audio_frame_storage_type| is supported on this
// platform.
SB_EXPORT bool SbAudioSinkIsAudioFrameStorageTypeSupported(
    SbMediaAudioFrameStorageType audio_frame_storage_type);

#ifdef __cplusplus
}  // extern "C"
#endif

#endif  // STARBOARD_AUDIO_SINK_H_
