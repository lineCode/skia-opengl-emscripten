/*
 * Copyright (C) 2013 Google Inc. All Rights Reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *     * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above
 * copyright notice, this list of conditions and the following disclaimer
 * in the documentation and/or other materials provided with the
 * distribution.
 *     * Neither the name of Google Inc. nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

// Modifications Copyright 2017 The Cobalt Authors. All Rights Reserved.
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

#ifndef COBALT_DOM_SOURCE_BUFFER_H_
#define COBALT_DOM_SOURCE_BUFFER_H_

#include <memory>
#include <string>
#include <vector>

#include "base/compiler_specific.h"
#include "base/memory/ref_counted.h"
#include "base/optional.h"
#if !(defined(OS_EMSCRIPTEN) && defined(DISABLE_PTHREADS))
#include "base/timer/timer.h"
#endif

#include "cobalt/base/cobalt_token.h"
//#include "base/token.h"

#include "cobalt/dom/audio_track_list.h"
#include "cobalt/dom/event_queue.h"
#include "cobalt/dom/event_target.h"
#include "cobalt/dom/source_buffer_append_mode.h"
#include "cobalt/dom/time_ranges.h"
#include "cobalt/dom/track_default_list.h"
#include "cobalt/dom/video_track_list.h"
#include "cobalt/media/base/media_tracks.h"
#include "cobalt/media/filters/chunk_demuxer.h"
#include "cobalt/script/array_buffer.h"
#include "cobalt/script/array_buffer_view.h"
#include "cobalt/script/exception_state.h"

namespace cobalt {
namespace dom {

class MediaSource;

// The SourceBuffer interface exposes the media source buffer so its user can
// append media data to.
//   https://www.w3.org/TR/2016/CR-media-source-20160705/#sourcebuffer
class SourceBuffer : public dom::EventTarget {
 public:
  // Custom, not in any spec.
  //
  SourceBuffer(const std::string& id, MediaSource* media_source,
               media::ChunkDemuxer* chunk_demuxer, EventQueue* event_queue);

  // Web API: SourceBuffer
  //
  SourceBufferAppendMode mode(script::ExceptionState* exception_state) const {
    SB_UNREFERENCED_PARAMETER(exception_state);
    return mode_;
  }
  void set_mode(SourceBufferAppendMode mode,
                script::ExceptionState* exception_state);
  bool updating() const { return updating_; }
  scoped_refptr<TimeRanges> buffered(
      script::ExceptionState* exception_state) const;
  double timestamp_offset(script::ExceptionState* exception_state) const {
    SB_UNREFERENCED_PARAMETER(exception_state);
    return timestamp_offset_;
  }
  void set_timestamp_offset(double offset,
                            script::ExceptionState* exception_state);
  scoped_refptr<AudioTrackList> audio_tracks() const { return audio_tracks_; }
  scoped_refptr<VideoTrackList> video_tracks() const { return video_tracks_; }
  double append_window_start(script::ExceptionState* exception_state) const {
    SB_UNREFERENCED_PARAMETER(exception_state);
    return append_window_start_;
  }
  void set_append_window_start(double start,
                               script::ExceptionState* exception_state);
  double append_window_end(script::ExceptionState* exception_state) const {
    SB_UNREFERENCED_PARAMETER(exception_state);
    return append_window_end_;
  }
  void set_append_window_end(double start,
                             script::ExceptionState* exception_state);
  void AppendBuffer(const script::Handle<script::ArrayBuffer>& data,
                    script::ExceptionState* exception_state);
  void AppendBuffer(const script::Handle<script::ArrayBufferView>& data,
                    script::ExceptionState* exception_state);
  void Abort(script::ExceptionState* exception_state);
  void Remove(double start, double end,
              script::ExceptionState* exception_state);
  scoped_refptr<TrackDefaultList> track_defaults(
      script::ExceptionState* exception_state) const {
    SB_UNREFERENCED_PARAMETER(exception_state);
    return track_defaults_;
  }
  void set_track_defaults(const scoped_refptr<TrackDefaultList>& track_defaults,
                          script::ExceptionState* exception_state);

  // Custom, not in any spec.
  //
  void OnRemovedFromMediaSource();
  double GetHighestPresentationTimestamp() const;

  DEFINE_WRAPPABLE_TYPE(SourceBuffer);
  void TraceMembers(script::Tracer* tracer) override;

 private:
  typedef media::MediaTracks MediaTracks;

  void InitSegmentReceived(std::unique_ptr<MediaTracks> tracks);
  void ScheduleEvent(base::CobToken event_name);
  bool PrepareAppend(size_t new_data_size,
                     script::ExceptionState* exception_state);
  bool EvictCodedFrames(size_t new_data_size);
  void AppendBufferInternal(const unsigned char* data, size_t size,
                            script::ExceptionState* exception_state);
  void OnAppendTimer();
  void AppendError();

  void OnRemoveTimer();

  void CancelRemove();
  void AbortIfUpdating();

  void RemoveMediaTracks();

  const TrackDefault* GetTrackDefault(
      const std::string& track_type,
      const std::string& byte_stream_track_id) const;
  std::string DefaultTrackLabel(const std::string& track_type,
                                const std::string& byte_stream_track_id) const;
  std::string DefaultTrackLanguage(
      const std::string& track_type,
      const std::string& byte_stream_track_id) const;

  const std::string id_;
  media::ChunkDemuxer* chunk_demuxer_;
  MediaSource* media_source_;
  scoped_refptr<TrackDefaultList> track_defaults_;
  EventQueue* event_queue_;

  SourceBufferAppendMode mode_;
  bool updating_;
  double timestamp_offset_;
  scoped_refptr<AudioTrackList> audio_tracks_;
  scoped_refptr<VideoTrackList> video_tracks_;
  double append_window_start_;
  double append_window_end_;

  /// TODO: !(defined(OS_EMSCRIPTEN) && defined(DISABLE_PTHREADS))
  base::OneShotTimer append_timer_;
  bool first_initialization_segment_received_;
  std::vector<uint8_t> pending_append_data_;
  size_t pending_append_data_offset_;

  base::OneShotTimer remove_timer_;
  double pending_remove_start_;
  double pending_remove_end_;
};

}  // namespace dom
}  // namespace cobalt

#endif  // COBALT_DOM_SOURCE_BUFFER_H_
