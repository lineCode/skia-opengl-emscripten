﻿// Copyright 2015 The Cobalt Authors. All Rights Reserved.
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

#include "cobalt/dom/html_image_element.h"

#include <memory>
#include <string>

#include "base/message_loop/message_loop.h"
#include "base/trace_event/trace_event.h"
#include "cobalt/base/polymorphic_downcast.h"
#include "cobalt/dom/csp_delegate.h"
#include "cobalt/dom/document.h"
#include "cobalt/dom/dom_settings.h"
#include "cobalt/dom/html_element_context.h"
#include "cobalt/dom/window.h"
#include "cobalt/script/global_environment.h"
#include "url/gurl.h"

namespace cobalt {
namespace dom {

// static
const char HTMLImageElement::kTagName[] = "img";

HTMLImageElement::HTMLImageElement(script::EnvironmentSettings* env_settings)
    : HTMLElement(base::polymorphic_downcast<DOMSettings*>(env_settings)
                      ->window()
                      ->document()
                      .get(),
                  base::CobToken(kTagName)) {}

void HTMLImageElement::OnSetAttribute(const std::string& name,
                                      const std::string& /* value */) {
  printf("HTMLImageElement::OnSetAttribute 1 %s", name.c_str());
  // A user agent that obtains images immediately must synchronously update the
  // image data of an img element whenever that element is created with a src
  // attribute. A user agent that obtains images immediately must also
  // synchronously update the image data of an img element whenever that element
  // has its src or crossorigin attribute set, changed, or removed.
  if (name == "src") {
    UpdateImageData();
  }
}

void HTMLImageElement::OnRemoveAttribute(const std::string& name) {
  printf("HTMLImageElement::OnRemoveAttribute 1 %s", name.c_str());
  // A user agent that obtains images immediately must synchronously update the
  // image data of an img element whenever that element is created with a src
  // attribute. A user agent that obtains images immediately must also
  // synchronously update the image data of an img element whenever that element
  // has its src or crossorigin attribute set, changed, or removed.
  if (name == "src") {
    UpdateImageData();
  }
}

// Algorithm for UpdateTheImageData:
//   https://www.w3.org/TR/html5/embedded-content-0.html#update-the-image-data
void HTMLImageElement::UpdateImageData() {
  printf("HTMLImageElement::UpdateImageData 1\n");
  DCHECK(base::MessageLoopCurrent::Get());
  DCHECK(node_document());
  TRACE_EVENT0("cobalt::dom", "HTMLImageElement::UpdateImageData()");

  // 1. Not needed by Cobalt.

  // 2. If an instance of the fetching algorithm is still running for this
  // element, then abort that algorithm, discarding any pending tasks generated
  // by that algorithm.
  // 3. Forget the img element's current image data, if any.
  if (cached_image_loaded_callback_handler_) {
    cached_image_loaded_callback_handler_.reset();
    prevent_gc_until_load_complete_.reset();
    node_document()->DecreaseLoadingCounter();
  }

  // 4. If the user agent cannot support images, or its support for images has
  // been disabled, then abort these steps.
  // 5. Otherwise, if the element has a src attribute specified and its value is
  // not the empty string, let selected source be the value of the element's src
  // attribute, and selected pixel density be 1.0. Otherwise, let selected
  // source be null and selected pixel density be undefined.
  const std::string src = GetAttribute("src").value_or("");

  // 6. Not needed by Cobalt.

  // 7. If selected source is not null, run these substeps:
  scoped_refptr<loader::image::CachedImage> cached_image;

  printf("HTMLImageElement::UpdateImageData 2\n");

  if (!src.empty()) {
    printf("HTMLImageElement::UpdateImageData 2.1\n");
    // 7.1. Resolve selected source, relative to the element. If that is not
    // successful, abort these steps.
    const GURL& base_url = node_document()->url_as_gurl();
    const GURL selected_source = base_url.Resolve(src);
    if (!selected_source.is_valid()) {
      printf("HTMLImageElement::UpdateImageData 2.2\n");
      LOG(WARNING) << src << " cannot be resolved based on " << base_url << ".";
      return;
    }

    // 7.2 Let key be a tuple consisting of the resulting absolute URL, the img
    // element's cross origin attribute's mode, and, if that mode is not No
    // CORS, the Document object's origin.
    // 7.3 If the list of available images contains an entry for key, then set
    // the img element to the completely available state, update the
    // presentation of the image appropriately, queue a task to fire a simple
    // event named load at the img element, and abort these steps.
    cached_image =
        node_document()
            ->html_element_context()
            ->image_cache()
            ->CreateCachedResource(selected_source, loader::Origin());
    printf("HTMLImageElement::UpdateImageData 2.3\n");
    if (cached_image->TryGetResource()) {
      printf("HTMLImageElement::UpdateImageData 2.4\n");
      PreventGarbageCollectionUntilEventIsDispatched(base::Tokens::load());
      printf("HTMLImageElement::UpdateImageData 2.5 %s\n", cached_image->url().path().c_str());
      return;
    }
    printf("HTMLImageElement::UpdateImageData 2.6 %s\n", cached_image->url().path().c_str());
  } else {
    printf("HTMLImageElement::UpdateImageData 2.6\n");
    // 8. 9. Not needed by Cobalt.
    // 10. If selected source is null, then set the element to the broken state,
    // queue a task to fire a simple event named error at the img element, and
    // abort these steps.
    PreventGarbageCollectionUntilEventIsDispatched(base::Tokens::error());
    return;
  }

  printf("HTMLImageElement::UpdateImageData 3\n");

  // 11. Not needed by Cobalt.

  // 12. Do a potentially CORS-enabled fetch of the absolute URL that resulted
  // from the earlier step, with the mode being the current state of the
  // element's crossorigin content attribute, the origin being the origin of the
  // img element's Document, and the default origin behaviour set to taint.
  // 13. Not needed by Cobalt.
  // 14. If the download was successful, fire a simple event named load at the
  // img element. Otherwise, queue a task to first fire a simple event named
  // error at the img element.
  DCHECK(!prevent_gc_until_load_complete_);
  prevent_gc_until_load_complete_.reset(
      new script::GlobalEnvironment::ScopedPreventGarbageCollection(
          html_element_context()->script_runner()->GetGlobalEnvironment(),
          this));
  node_document()->IncreaseLoadingCounter();
  cached_image_loaded_callback_handler_.reset(
      new loader::image::CachedImage::OnLoadedCallbackHandler(
          cached_image, base::Bind(&HTMLImageElement::OnLoadingSuccess,
                                   base::Unretained(this)),
          base::Bind(&HTMLImageElement::OnLoadingError,
                     base::Unretained(this))));

  printf("HTMLImageElement::UpdateImageData 4\n");
}

void HTMLImageElement::OnLoadingSuccess() {
  P_LOG("HTMLImageElement::OnLoadingSuccess\n");
  TRACE_EVENT0("cobalt::dom", "HTMLImageElement::OnLoadingSuccess()");
  AllowGarbageCollectionAfterEventIsDispatched(
      base::Tokens::load(), std::move(prevent_gc_until_load_complete_));
  if (node_document()) {
    node_document()->DecreaseLoadingCounterAndMaybeDispatchLoadEvent();
  }
  cached_image_loaded_callback_handler_.reset();
}

void HTMLImageElement::OnLoadingError() {
  P_LOG("HTMLImageElement::OnLoadingError\n");
  TRACE_EVENT0("cobalt::dom", "HTMLImageElement::OnLoadingError()");
  AllowGarbageCollectionAfterEventIsDispatched(
      base::Tokens::error(), std::move(prevent_gc_until_load_complete_));
  if (node_document()) {
    node_document()->DecreaseLoadingCounterAndMaybeDispatchLoadEvent();
  }
  cached_image_loaded_callback_handler_.reset();
}

void HTMLImageElement::PreventGarbageCollectionUntilEventIsDispatched(
    base::CobToken event_name) {
  P_LOG("HTMLImageElement::PreventGarbageCollectionUntilEventIsDispatched\n");
  std::unique_ptr<script::GlobalEnvironment::ScopedPreventGarbageCollection>
      prevent_gc_until_event_dispatch(
          new script::GlobalEnvironment::ScopedPreventGarbageCollection(
              html_element_context()->script_runner()->GetGlobalEnvironment(),
              this));
  AllowGarbageCollectionAfterEventIsDispatched(
      event_name, std::move(prevent_gc_until_event_dispatch));
}

void HTMLImageElement::AllowGarbageCollectionAfterEventIsDispatched(
    base::CobToken event_name,
    std::unique_ptr<script::GlobalEnvironment::ScopedPreventGarbageCollection>
        scoped_prevent_gc) {
  P_LOG("HTMLImageElement::AllowGarbageCollectionAfterEventIsDispatched\n");
  PostToDispatchEventNameAndRunCallback(
      FROM_HERE, event_name,
      base::Bind(&HTMLImageElement::DestroyScopedPreventGC,
                 base::AsWeakPtr<HTMLImageElement>(this),
                 base::Passed(&scoped_prevent_gc)));
}

void HTMLImageElement::DestroyScopedPreventGC(
    std::unique_ptr<script::GlobalEnvironment::ScopedPreventGarbageCollection>
        scoped_prevent_gc) {
  P_LOG("HTMLImageElement::DestroyScopedPreventGC\n");
  scoped_prevent_gc.reset();
}

}  // namespace dom
}  // namespace cobalt
