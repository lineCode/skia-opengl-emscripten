﻿// Copyright 2016 The Cobalt Authors. All Rights Reserved.
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

#include "cobalt/loader/image/threaded_image_decoder_proxy.h"

#include <memory>
#include <string>

#include "base/bind.h"
#include "base/threading/thread.h"
#include "cobalt/loader/image/image_decoder.h"

namespace cobalt {
namespace loader {
namespace image {

namespace {

// Helper function that is run on the WebModule thread to run a Callback if
// and only if |threaded_image_decoder_proxy| is still alive.
template <typename Callback, typename Arg>
void MaybeRun(
    base::WeakPtr<ThreadedImageDecoderProxy> threaded_image_decoder_proxy,
    const Callback& callback, const Arg& arg) {
  if (threaded_image_decoder_proxy) {
    callback.Run(arg);
  }
}

// Helper function to post a Callback back to the WebModule thread, that
// checks whether the ThreadedImageDecoderProxy it came from is alive or not
// before it runs.
template <typename Callback, typename Arg>
void PostToMessageLoopChecked(
    base::WeakPtr<ThreadedImageDecoderProxy> threaded_image_decoder_proxy,
    const Callback& callback,
    //base::MessageLoop* message_loop,
    scoped_refptr<base::SingleThreadTaskRunner> message_loop,
    const Arg& arg) {
#if !(defined(OS_EMSCRIPTEN) && defined(DISABLE_PTHREADS))
  message_loop->PostTask(
      FROM_HERE, base::Bind(&MaybeRun<Callback, Arg>,
                            threaded_image_decoder_proxy, callback, arg));
#else
  std::move(base::Bind(&MaybeRun<Callback, Arg>,
                       threaded_image_decoder_proxy, callback, arg)).Run();
#endif
}

}  // namespace

ThreadedImageDecoderProxy::ThreadedImageDecoderProxy(
    render_tree::ResourceProvider* resource_provider,
    const ImageAvailableCallback& image_available_callback,
    //base::MessageLoop* load_message_loop,
    scoped_refptr<base::SingleThreadTaskRunner> load_message_loop,
    const loader::Decoder::OnCompleteFunction& load_complete_callback)
    : ALLOW_THIS_IN_INITIALIZER_LIST(weak_ptr_factory_(this)),
      ALLOW_THIS_IN_INITIALIZER_LIST(
          weak_this_(weak_ptr_factory_.GetWeakPtr())),
      load_message_loop_(load_message_loop),
#if !(defined(OS_EMSCRIPTEN) && defined(DISABLE_PTHREADS))
      result_message_loop_(base::MessageLoopCurrent::Get().task_runner()),
#else
      result_message_loop_(),
#endif
      image_decoder_(new ImageDecoder(
          resource_provider,
          base::Bind(&PostToMessageLoopChecked<ImageAvailableCallback,
                                               scoped_refptr<Image>>,
                     weak_this_, image_available_callback,
                     result_message_loop_),
          base::Bind(
              &PostToMessageLoopChecked<loader::Decoder::OnCompleteFunction,
                                        base::Optional<std::string>>,
              weak_this_, load_complete_callback, result_message_loop_))) {
#if !(defined(OS_EMSCRIPTEN) && defined(DISABLE_PTHREADS))
  DCHECK(load_message_loop_);
  DCHECK(result_message_loop_); // TODO
#else
  DCHECK(!load_message_loop_);
  DCHECK(!load_message_loop_);
#endif
  DCHECK(image_decoder_);
}

// We're dying, but |image_decoder_| might still be doing work on the load
// thread.  Because of this, we transfer ownership of it into the load message
// loop, where it will be deleted after any pending tasks involving it are
// done.
ThreadedImageDecoderProxy::~ThreadedImageDecoderProxy() {
  ///printf("ThreadedImageDecoderProxy::~ThreadedImageDecoderProxy...\n");
  // Notify the ImageDecoder that there's a pending deletion to ensure that no
  // additional work is done decoding the image.
  DCHECK(image_decoder_);
  image_decoder_->SetDeletionPending();
#if !(defined(OS_EMSCRIPTEN) && defined(DISABLE_PTHREADS))
  DCHECK(load_message_loop_);
  load_message_loop_->DeleteSoon(FROM_HERE,
                                                image_decoder_.release());
#else
  DCHECK(!load_message_loop_);
  image_decoder_.release();
#endif
}

LoadResponseType ThreadedImageDecoderProxy::OnResponseStarted(
    Fetcher* fetcher
#if defined(ENABLE_GNET)
    ,
    const scoped_refptr<net::HttpResponseHeaders>& headers
#endif
    ) {
  ///printf("ThreadedImageDecoderProxy::OnResponseStarted 1...\n");
  SB_UNREFERENCED_PARAMETER(fetcher);
  return image_decoder_->OnResponseStarted(fetcher
#if defined(ENABLE_GNET)
  , headers
#endif
  );
}

void ThreadedImageDecoderProxy::DecodeChunk(const char* data, size_t size) {
  ///printf("ThreadedImageDecoderProxy::DecodeChunk 1...\n");
  std::unique_ptr<std::string> scoped_data(new std::string(data, size));
#if !(defined(OS_EMSCRIPTEN) && defined(DISABLE_PTHREADS))
  DCHECK(load_message_loop_);
  load_message_loop_->PostTask(
      FROM_HERE, base::Bind(&Decoder::DecodeChunkPassed,
                            base::Unretained(image_decoder_.get()),
                            base::Passed(&scoped_data)));
#else
  std::move(base::Bind(&Decoder::DecodeChunkPassed,
                       base::Unretained(image_decoder_.get()),
                       base::Passed(&scoped_data))).Run();
#endif
}

void ThreadedImageDecoderProxy::DecodeChunkPassed(
    std::unique_ptr<std::string> data) {
  ///printf("ThreadedImageDecoderProxy::DecodeChunkPassed 1...\n");
#if !(defined(OS_EMSCRIPTEN) && defined(DISABLE_PTHREADS))
  DCHECK(load_message_loop_);
  load_message_loop_->PostTask(
      FROM_HERE,
      base::Bind(&Decoder::DecodeChunkPassed,
                 base::Unretained(image_decoder_.get()), base::Passed(&data)));
#else
  std::move(base::Bind(&Decoder::DecodeChunkPassed,
                       base::Unretained(image_decoder_.get()), base::Passed(&data))).Run();
#endif
}

void ThreadedImageDecoderProxy::Finish() {
  ///printf("ThreadedImageDecoderProxy::Finish 1...\n");
#if !(defined(OS_EMSCRIPTEN) && defined(DISABLE_PTHREADS))
  DCHECK(load_message_loop_);
  load_message_loop_->PostTask(
      FROM_HERE, base::Bind(&ImageDecoder::Finish,
                            base::Unretained(image_decoder_.get())));
#else
  std::move(base::Bind(&ImageDecoder::Finish,
                       base::Unretained(image_decoder_.get()))).Run();
#endif
  ///printf("ThreadedImageDecoderProxy::Finish 2...\n");
}

bool ThreadedImageDecoderProxy::Suspend() {
  ///printf("ThreadedImageDecoderProxy::Suspend 1...\n");
#if !(defined(OS_EMSCRIPTEN) && defined(DISABLE_PTHREADS))
  DCHECK(load_message_loop_);
  load_message_loop_->PostTask(
      FROM_HERE, base::Bind(base::IgnoreResult(&ImageDecoder::Suspend),
                            base::Unretained(image_decoder_.get())));
#else
  std::move(base::Bind(base::IgnoreResult(&ImageDecoder::Suspend),
                       base::Unretained(image_decoder_.get()))).Run();
#endif
  return true;
}

void ThreadedImageDecoderProxy::Resume(
    render_tree::ResourceProvider* resource_provider) {
  ///printf("ThreadedImageDecoderProxy::Resume 1...\n");
#if !(defined(OS_EMSCRIPTEN) && defined(DISABLE_PTHREADS))
  DCHECK(load_message_loop_);
  load_message_loop_->PostTask(
      FROM_HERE,
      base::Bind(&ImageDecoder::Resume, base::Unretained(image_decoder_.get()),
                 resource_provider));
#else
  std::move(base::Bind(&ImageDecoder::Resume, base::Unretained(image_decoder_.get()),
                       resource_provider)).Run();
#endif
}

}  // namespace image
}  // namespace loader
}  // namespace cobalt
