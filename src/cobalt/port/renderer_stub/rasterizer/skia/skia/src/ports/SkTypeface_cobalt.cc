﻿// Copyright 2017 The Cobalt Authors. All Rights Reserved.
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

#include <memory>

#include "renderer_stub/rasterizer/skia/skia/src/ports/SkTypeface_cobalt.h"

#include "SkFontDescriptor.h"
#include "SkFontStyle.h"
#include "SkTypefaceCache.h"

//#include <iostream> /// __TODO__

SkTypeface_Cobalt::SkTypeface_Cobalt(int face_index, SkFontStyle style,
                                     bool is_fixed_pitch,
                                     const SkString& family_name)
    : //INHERITED(SkFontStyle::FromOldStyle(style), is_fixed_pitch),
      INHERITED(style, is_fixed_pitch),
      face_index_(face_index),
      family_name_(family_name),
      synthesizes_bold_(!isBold()) {
    printf("SkTypeface_Cobalt 1\n");
}

void SkTypeface_Cobalt::onGetFamilyName(SkString* family_name) const {
  *family_name = family_name_;
}

SkTypeface_CobaltStream::SkTypeface_CobaltStream(std::unique_ptr<SkStreamAsset> stream,
                                                 int face_index, SkFontStyle style,
                                                 bool is_fixed_pitch,
                                                 const SkString& family_name)
    : INHERITED(face_index, style, is_fixed_pitch, family_name),
      stream_(std::move(stream)) {
    DCHECK(stream_);
  /*LOG(INFO) << "Created SkTypeface_CobaltStream: " << family_name.c_str() << "("
            << style << "); Size: " << stream_->getLength() << " bytes";*/
}

void SkTypeface_CobaltStream::onGetFontDescriptor(SkFontDescriptor* descriptor,
                                                  bool* serialize) const {
  SkASSERT(descriptor);
  SkASSERT(serialize);
  descriptor->setFamilyName(family_name_.c_str());
  *serialize = true;
}

std::unique_ptr<SkStreamAsset> SkTypeface_CobaltStream::onOpenStream(int* face_index) const {
  *face_index = face_index_;
  DCHECK(stream_);
  return stream_->duplicate();
}

size_t SkTypeface_CobaltStream::GetStreamLength() const {
  ///std::cout << "SkTypeface_CobaltStream::GetStreamLength() 1" << std::endl;
  DCHECK(stream_);
  return stream_->getLength();
}

SkTypeface_CobaltStreamProvider::SkTypeface_CobaltStreamProvider(
    SkFileMemoryChunkStreamProvider* stream_provider, int face_index,
    SkFontStyle style, bool is_fixed_pitch, const SkString& family_name,
    bool disable_synthetic_bolding)
    : INHERITED(face_index, style, is_fixed_pitch, family_name),
      stream_provider_(stream_provider) {
    DCHECK(stream_provider_);
  if (disable_synthetic_bolding) {
    synthesizes_bold_ = false;
  }
  /*LOG(INFO) << "Created SkTypeface_CobaltStreamProvider: "
            << family_name.c_str() << "(" << style << "); File: \""
            << stream_provider->file_path() << "\"";*/
}

void SkTypeface_CobaltStreamProvider::onGetFontDescriptor(
    SkFontDescriptor* descriptor, bool* serialize) const {
  SkASSERT(descriptor);
  SkASSERT(serialize);
  descriptor->setFamilyName(family_name_.c_str());
  *serialize = false;
}

std::unique_ptr<SkStreamAsset> SkTypeface_CobaltStreamProvider::onOpenStream(
    int* face_index) const {
  *face_index = face_index_;
  DCHECK(stream_provider_);

  //DCHECK(stream_);
  //return stream_provider_->stream_->duplicate();

  std::unique_ptr<SkFileMemoryChunkStream> stream(
      stream_provider_->OpenStream());
  return std::move(stream);

  /// __TODO__
  ///return stream_provider_->OpenStream();
  //SkFileMemoryChunkStream* res = stream_provider_->OpenStream();

  //return SkStream::MakeFromFile()
  //return SkStream::MakeFromFile(stream_provider_->file_path().c_str());
}

size_t SkTypeface_CobaltStreamProvider::GetStreamLength() const {
  ///std::cout << "SkTypeface_CobaltStreamProvider::GetStreamLength() 1" << std::endl;
  DLOG(WARNING)
      << "Requesting stream length of SkTypeface_CobaltStreamProvider. "
         "This requires a file load and should be used sparingly.";
  DCHECK(stream_provider_);
  std::unique_ptr<SkFileMemoryChunkStream> stream(
      stream_provider_->OpenStream());
  return stream->getLength();

  /// __TODO__: FIXME

  //return 0;//this->memory_chunks_
}
