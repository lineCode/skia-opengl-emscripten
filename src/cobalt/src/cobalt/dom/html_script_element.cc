﻿// Copyright 2014 The Cobalt Authors. All Rights Reserved.
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

#include "cobalt/dom/html_script_element.h"

#include <deque>
#include <memory>
#include <utility>

#include "base/bind.h"
#include "base/compiler_specific.h"
#include "base/strings/string_util.h"
#include "base/trace_event/trace_event.h"
#include "cobalt/base/tokens.h"
#include "cobalt/dom/csp_delegate.h"
#include "cobalt/dom/document.h"
#include "cobalt/dom/global_stats.h"
#include "cobalt/dom/html_element_context.h"

#ifdef ENABLE_LOADER
#include "cobalt/loader/decoder.h"
#include "cobalt/loader/fetcher_factory.h"
#include "cobalt/loader/sync_loader.h"
#include "cobalt/loader/text_decoder.h"
#endif

#include "cobalt/script/global_environment.h"
#include "cobalt/script/script_runner.h"
#include "nb/memory_scope.h"
#include "url/gurl.h"

namespace cobalt {
namespace dom {

namespace {

bool PermitAnyURL(const GURL&, bool) { return true; }

#ifdef ENABLE_LOADER
loader::RequestMode GetRequestMode(
    const base::Optional<std::string>& cross_origin_attribute) {
  // https://html.spec.whatwg.org/#cors-settings-attribute
  if (cross_origin_attribute) {
    if (*cross_origin_attribute == "use-credentials") {
      return loader::kCORSModeIncludeCredentials;
    } else {
      // The invalid value default of crossorigin is Anonymous state, leading to
      // "same-origin" credentials mode.
      return loader::kCORSModeSameOriginCredentials;
    }
  }
  // crossorigin attribute's missing value default is No CORS state, leading to
  // "no-cors" request mode.
  return loader::kNoCORSMode;
}
#endif

}  // namespace

// static
const char HTMLScriptElement::kTagName[] = "script";

HTMLScriptElement::HTMLScriptElement(Document* document)
    : HTMLElement(document, base::CobToken(kTagName)),
      is_already_started_(false),
      is_parser_inserted_(false),
      is_ready_(false),
      load_option_(0),
      inline_script_location_(GetSourceLocationName(), 1, 1),
      is_sync_load_successful_(false),
      should_execute_(true)
#if defined(ENABLE_LOADER)
      ,
      synchronous_loader_interrupt_(
          document->html_element_context()->synchronous_loader_interrupt())
#endif
          {
#if defined(ENABLE_SCRIPT_RUNNER)
  DCHECK(document->html_element_context()->script_runner());
#endif
}

base::Optional<std::string> HTMLScriptElement::cross_origin() const {
  base::Optional<std::string> cross_origin_attribute =
      GetAttribute("crossOrigin");
  if (cross_origin_attribute &&
      (*cross_origin_attribute != "anonymous" &&
       *cross_origin_attribute != "use-credentials")) {
    return std::string();
  }
  return cross_origin_attribute;
}

void HTMLScriptElement::set_cross_origin(
    const base::Optional<std::string>& value) {
  if (value) {
    SetAttribute("crossOrigin", *value);
  } else {
    RemoveAttribute("crossOrigin");
  }
}

void HTMLScriptElement::OnInsertedIntoDocument() {
  HTMLElement::OnInsertedIntoDocument();
  if (!is_parser_inserted_) {
    Prepare();
  }
}

void HTMLScriptElement::OnParserStartTag(
    const base::SourceLocation& opening_tag_location) {
  inline_script_location_ = opening_tag_location;
  ++inline_script_location_.column_number;  // JavaScript code starts after ">".
  is_parser_inserted_ = true;
}

void HTMLScriptElement::OnParserEndTag() { Prepare(); }

scoped_refptr<HTMLScriptElement> HTMLScriptElement::AsHTMLScriptElement() {
  return this;
}

scoped_refptr<Node> HTMLScriptElement::Duplicate() const {
  // The cloning steps for script elements must set the "already started" flag
  // on the copy if it is set on the element being cloned.
  //   https://www.w3.org/TR/html5/scripting-1.html#already-started
  scoped_refptr<HTMLScriptElement> new_script = HTMLElement::Duplicate()
                                                    ->AsElement()
                                                    ->AsHTMLElement()
                                                    ->AsHTMLScriptElement();
  new_script->is_already_started_ = is_already_started_;
  return new_script;
}

HTMLScriptElement::~HTMLScriptElement() {
  // Remove the script from the list of scripts that will execute in order as
  // soon as possible associated with the Document, only if the document still
  // exists when the script element is destroyed.
  // NOTE: While the garbage collection prevention logic will typically protect
  // from this, it is still possible during shutdown.
  if (node_document()) {
    std::deque<HTMLScriptElement*>* scripts_to_be_executed =
        node_document()->scripts_to_be_executed();

    std::deque<HTMLScriptElement*>::iterator it = std::find(
        scripts_to_be_executed->begin(), scripts_to_be_executed->end(), this);
    if (it != scripts_to_be_executed->end()) {
      scripts_to_be_executed->erase(it);
    }
  }
}

// Algorithm for Prepare:
//   https://www.w3.org/TR/html5/scripting-1.html#prepare-a-script
void HTMLScriptElement::Prepare() {
  TRACK_MEMORY_SCOPE("DOM");
  // Custom, not in any spec.
  DCHECK(thread_checker_.CalledOnValidThread());
  //DCHECK(base::MessageLoop::current());
  DCHECK(base::MessageLoopCurrent::Get());
#ifdef ENABLE_LOADER
  DCHECK(!loader_ || is_already_started_);
#endif
  TRACE_EVENT0("cobalt::dom", "HTMLScriptElement::Prepare()");

  // If the script element is marked as having "already started", then the user
  // agent must abort these steps at this point. The script is not executed.
  if (is_already_started_) {
    return;
  }

  // 2. 3. 4. Not needed by Cobalt.

  // 5. If the element is not in a Document, then the user agent must abort
  // these steps at this point. The script is not executed.
  if (!node_document()) {
    return;
  }
  document_ = base::AsWeakPtr<Document>(node_document());

  // 6. If either:
  //    the script element has a type attribute and its value is the empty
  //      string, or
  //    the script element has no type attribute but it has a language attribute
  //      and that attribute's value is the empty string, or
  //    the script element has neither a type attribute nor a language
  //      attribute, then
  // ...let the script block's type for this script element be
  // "text/javascript".
  // Otherwise, if the script element has a type attribute, let the script
  // block's type for this script element be the value of that attribute with
  // any leading or trailing sequences of space characters removed.
  // Otherwise, the element has a non-empty language attribute; let the script
  // block's type for this script element be the concatenation of the string
  // "text/" followed by the value of the language attribute.
  if (type() == "") {
    set_type("text/javascript");
  } else {
    std::string trimmed_type;
    TrimWhitespaceASCII(type(), base::TRIM_ALL, &trimmed_type);
    set_type(trimmed_type);
  }

  // 7. If the user agent does not support the scripting language given by the
  // script block's type for this script element, then the user agent must abort
  // these steps at this point. The script is not executed.
  if (type() != "text/javascript") return;

  // 8. Not needed by Cobalt.

  // 9. The user agent must set the element's "already started" flag.
  is_already_started_ = true;

  // 10. ~ 13. Not needed by Cobalt.

  // 14. If the element has a src content attribute, run these substeps:
  //   1. Let src be the value of the element's src attribute.
  //   2. If src is the empty string, queue a task to fire a simple event
  // named error at the element, and abort these steps.
  if (HasAttribute("src") && src() == "") {
    LOG(WARNING) << "src attribute of script element is empty.";

    PreventGarbageCollectionAndPostToDispatchEvent(
        FROM_HERE, base::Tokens::error(),
        &prevent_gc_until_error_event_dispatch_);
    return;
  }

  //   3. Resolve src relative to the element.
  //   4. If the previous step failed, queue a task to fire a simple event named
  // error at the element, and abort these steps.
  const GURL& base_url = document_->url_as_gurl();
  url_ = base_url.Resolve(src());
  if (!url_.is_valid()) {
    LOG(WARNING) << src() << " cannot be resolved based on " << base_url << ".";

    PreventGarbageCollectionAndPostToDispatchEvent(
        FROM_HERE, base::Tokens::error(),
        &prevent_gc_until_error_event_dispatch_);
    return;
  }

  //   5. Do a potentially CORS-enabled fetch of the resulting absolute URL,
  // with the mode being the current state of the element's crossorigin
  // content attribute, the origin being the origin of the script element's
  // Document, and the default origin behaviour set to taint.

  // 15. Then, the first of the following options that describes the situation
  // must be followed:

  // Option 1 and Option 3 are not needed by Cobalt.
  if (HasAttribute("src") && is_parser_inserted_ && !async()) {
    // Option 2
    // If the element has a src attribute, and the element has been flagged as
    // "parser-inserted", and the element does not have an async attribute.
    load_option_ = 2;
  } else if (HasAttribute("src") && !async()) {
    // Option 4
    // If the element has a src attribute, does not have an async attribute, and
    // does not have the "force-async" flag set.
    load_option_ = 4;
  } else if (HasAttribute("src")) {
    // Option 5
    // If the element has a src attribute.
    load_option_ = 5;
  } else {
    // Option 6
    // Otherwise.
    load_option_ = 6;
  }

  // https://www.w3.org/TR/CSP2/#directive-script-src

#if defined(ENABLE_COBALT_CSP)
  CspDelegate* csp_delegate = document_->csp_delegate();
  // If the script element has a valid nonce, we always permit it, regardless
  // of its URL or inline nature.
  const bool bypass_csp =
      csp_delegate->IsValidNonce(CspDelegate::kScript, nonce());

  csp::SecurityCallback csp_callback;
  if (bypass_csp) {
    csp_callback = base::Bind(&PermitAnyURL);
  } else {
    csp_callback =
        base::Bind(&CspDelegate::CanLoad, base::Unretained(csp_delegate),
                   CspDelegate::kScript);
  }
#endif

#ifdef ENABLE_LOADER
  // Clear fetched resource's origin before start.
  fetched_last_url_origin_ = loader::Origin();

  // Determine request mode from crossorigin attribute.
  request_mode_ = GetRequestMode(GetAttribute("crossOrigin"));
#endif

  switch (load_option_) {
    case 2: {
      // If the element has a src attribute, and the element has been flagged as
      // "parser-inserted", and the element does not have an async attribute.

      // The element is the pending parsing-blocking script of the Document of
      // the parser that created the element. (There can only be one such script
      // per Document at a time.)

      // This variable will be set to true in the completion callback for the
      // loader below.  If that completion callback never fires, the variable
      // will stay false.  This can happen if the loader was interrupted, or
      // failed for another reason.
      is_sync_load_successful_ = false;

#ifdef ENABLE_LOADER
      loader::LoadSynchronously(
          //html_element_context()->sync_load_thread()->task_runner()->,
          base::MessageLoopCurrent::Get(),
          synchronous_loader_interrupt_,
          base::Bind(
              &loader::FetcherFactory::CreateSecureFetcher,
              base::Unretained(html_element_context()->fetcher_factory()), url_,
              csp_callback, request_mode_,
              document_->location() ? document_->location()->GetOriginAsObject()
                                    : loader::Origin()),
          base::Bind(&loader::TextDecoder::Create,
                     base::Bind(&HTMLScriptElement::OnSyncContentProduced,
                                base::Unretained(this)),
                     loader::Decoder::OnCompleteFunction()),
          base::Bind(&HTMLScriptElement::OnSyncLoadingComplete,
                     base::Unretained(this)));
#endif
#if defined(ENABLE_SCRIPT_RUNNER)
      if (is_sync_load_successful_) {
        script::GlobalEnvironment::ScopedPreventGarbageCollection
            scoped_prevent_gc(
                html_element_context()->script_runner()->GetGlobalEnvironment(),
                this);
        ExecuteExternal();
        // Release the content string now that we're finished with it.
        content_.reset();
      } else
#endif
      {
        // Executing the script block must just consist of firing a simple event
        // named error at the element.
        DispatchEvent(new Event(base::Tokens::error()));
      }
    } break;
    case 4: {
      // This is an asynchronous script. Prevent garbage collection until
      // loading completes and the script potentially executes.
      PreventGCUntilLoadComplete();

      // If the element has a src attribute, does not have an async attribute,
      // and does not have the "force-async" flag set.

      // The element must be added to the end of the list of scripts that will
      // execute in order as soon as possible associated with the Document of
      // the script element at the time the prepare a script algorithm started.
      std::deque<HTMLScriptElement*>* scripts_to_be_executed =
          document_->scripts_to_be_executed();
      scripts_to_be_executed->push_back(this);

      // Fetching an external script must delay the load event of the element's
      // document until the task that is queued by the networking task source
      // once the resource has been fetched (defined above) has been run.
      document_->IncreaseLoadingCounter();

#ifdef ENABLE_LOADER
      loader::Origin origin = document_->location()
                                  ? document_->location()->GetOriginAsObject()
                                  : loader::Origin();

      loader_ = html_element_context()->loader_factory()->CreateScriptLoader(
          url_, origin, csp_callback,
          base::Bind(&HTMLScriptElement::OnContentProduced,
                     base::Unretained(this)),
          base::Bind(&HTMLScriptElement::OnLoadingComplete,
                     base::Unretained(this)));
#endif

    } break;
    case 5: {
      // This is an asynchronous script. Prevent garbage collection until
      // loading completes and the script potentially executes.
      PreventGCUntilLoadComplete();

      // If the element has a src attribute.

      // Fetching an external script must delay the load event of the element's
      // document until the task that is queued by the networking task source
      // once the resource has been fetched (defined above) has been run.
      document_->IncreaseLoadingCounter();

#ifdef ENABLE_LOADER
      // The element must be added to the set of scripts that will execute as
      // soon as possible of the Document of the script element at the time the
      // prepare a script algorithm started.
      DCHECK(!loader_);

      loader::Origin origin = document_->location()
                                  ? document_->location()->GetOriginAsObject()
                                  : loader::Origin();

      loader_ = html_element_context()->loader_factory()->CreateScriptLoader(
          url_, origin, csp_callback,
          base::Bind(&HTMLScriptElement::OnContentProduced,
                     base::Unretained(this)),
          base::Bind(&HTMLScriptElement::OnLoadingComplete,
                     base::Unretained(this)));
#endif

    } break;
    case 6: {
      // Otherwise.

      // The user agent must immediately execute the script block, even if other
      // scripts are already executing.
      base::Optional<std::string> content = text_content();
      const std::string& text = content.value_or(base::EmptyString());
#if defined(ENABLE_COBALT_CSP)
      if (bypass_csp || text.empty() ||
          csp_delegate->AllowInline(CspDelegate::kScript,
                                    inline_script_location_,
                                    text))
#endif
                                    {
#ifdef ENABLE_LOADER
        fetched_last_url_origin_ = document_->location()->GetOriginAsObject();
#endif
        ExecuteInternal();
      }
#if defined(ENABLE_COBALT_CSP)
      else {
        PreventGarbageCollectionAndPostToDispatchEvent(
            FROM_HERE, base::Tokens::error(),
            &prevent_gc_until_error_event_dispatch_);
      }
#endif
    } break;
    default: { NOTREACHED(); }
  }
}

#ifdef ENABLE_LOADER
void HTMLScriptElement::OnSyncContentProduced(
    const loader::Origin& last_url_origin,
    std::unique_ptr<std::string> content) {
  TRACE_EVENT0("cobalt::dom", "HTMLScriptElement::OnSyncContentProduced()");
  fetched_last_url_origin_ = last_url_origin;
  content_ = std::move(content);
  is_sync_load_successful_ = true;
}
#endif

void HTMLScriptElement::OnSyncLoadingComplete(
    const base::Optional<std::string>& error) {
  if (!error) return;

  TRACE_EVENT0("cobalt::dom", "HTMLScriptElement::OnSyncLoadingComplete()");
  LOG(ERROR) << *error;
}

#ifdef ENABLE_LOADER
// Algorithm for OnContentProduced:
//   https://www.w3.org/TR/html5/scripting-1.html#prepare-a-script
void HTMLScriptElement::OnContentProduced(
    const loader::Origin& last_url_origin,
    std::unique_ptr<std::string> content) {
  DCHECK(thread_checker_.CalledOnValidThread());
  DCHECK(load_option_ == 4 || load_option_ == 5);
  DCHECK(content);
  TRACE_EVENT0("cobalt::dom", "HTMLScriptElement::OnContentProduced()");
  if (!document_) {
    AllowGCAfterLoadComplete();
    return;
  }

  fetched_last_url_origin_ = last_url_origin;
  content_ = std::move(content);

  switch (load_option_) {
    case 4: {
      // If the element has a src attribute, does not have an async attribute,
      // and does not have the "force-async" flag set.

      // The task that the networking task source places on the task queue once
      // the fetching algorithm has completed must run the following steps:
      //   1. If the element is not now the first element in the list of scripts
      //   that will execute in order as soon as possible to which it was added
      //   above, then mark the element as ready but abort these steps without
      //   executing the script yet.
      std::deque<HTMLScriptElement*>* scripts_to_be_executed =
          document_->scripts_to_be_executed();
      if (scripts_to_be_executed->front() != this) {
        is_ready_ = true;
        return;
      }
      while (true) {
        // 2. Execution: Execute the script block corresponding to the first
        // script element in this list of scripts that will execute in order as
        // soon as possible.
        HTMLScriptElement* script = scripts_to_be_executed->front();
        script->ExecuteExternal();

        // NOTE: Must disable warning 6011 on Windows. It mysteriously believes
        // that a NULL pointer is being dereferenced with this comparison.
        MSVC_PUSH_DISABLE_WARNING(6011);
        // If this script isn't the current object, then allow it to be garbage
        // collected now that it has executed.
        if (script != this) {
          script->AllowGCAfterLoadComplete();
        }
        MSVC_POP_WARNING();

        // 3. Remove the first element from this list of scripts that will
        // execute in order as soon as possible.
        scripts_to_be_executed->pop_front();

        // Fetching an external script must delay the load event of the
        //  element's document until the task that is queued by the networking
        // task source once the resource has been fetched (defined above)
        // has been run.
        document_->DecreaseLoadingCounterAndMaybeDispatchLoadEvent();

        // 4. If this list of scripts that will execute in order as soon as
        // possible is still not empty and the first entry has already been
        // marked
        // as ready, then jump back to the step labeled execution.
        if (scripts_to_be_executed->empty() ||
            !scripts_to_be_executed->front()->is_ready_) {
          break;
        }
      }
      // Allow garbage collection on the current script object now that it has
      // finished executing both itself and other pending scripts.
      AllowGCAfterLoadComplete();
    } break;
    case 5: {
      // If the element has a src attribute.

      // The task that the networking task source places on the task queue once
      // the fetching algorithm has completed must execute the script block and
      // then remove the element from the set of scripts that will execute as
      // soon as possible.
      ExecuteExternal();

      // Allow garbage collection on the current object now that it has finished
      // executing.
      AllowGCAfterLoadComplete();

      // Fetching an external script must delay the load event of the element's
      // document until the task that is queued by the networking task source
      // once the resource has been fetched (defined above) has been run.
      document_->DecreaseLoadingCounterAndMaybeDispatchLoadEvent();
    } break;
  }

  // Release the content string now that we're finished with it.
  content_.reset();

  // Post a task to release the loader.
  DCHECK(base::MessageLoopCurrent::Get()); // TODO
  //base::MessageLoop::current()->task_runner()->PostTask(
  base::MessageLoopCurrent::Get()->task_runner()->PostTask(
      FROM_HERE, base::Bind(&HTMLScriptElement::ReleaseLoader, this));
}
#endif

// Algorithm for OnLoadingComplete:
//   https://www.w3.org/TR/html5/scripting-1.html#prepare-a-script
void HTMLScriptElement::OnLoadingComplete(
    const base::Optional<std::string>& error) {
  P_LOG("HTMLScriptElement::OnLoadingComplete\n");
  if (!error) return;

  DCHECK(thread_checker_.CalledOnValidThread());
  DCHECK(load_option_ == 4 || load_option_ == 5);
  TRACE_EVENT0("cobalt::dom", "HTMLScriptElement::OnLoadingComplete()");

  if (!document_) {
    AllowGCAfterLoadComplete();
    return;
  }

  LOG(ERROR) << *error;

  // Executing the script block must just consist of firing a simple event
  // named error at the element.
  DispatchEvent(new Event(base::Tokens::error()));
  AllowGCAfterLoadComplete();

  switch (load_option_) {
    case 4: {
      // If the element has a src attribute, does not have an async attribute,
      // and does not have the "force-async" flag set.
      std::deque<HTMLScriptElement*>* scripts_to_be_executed =
          document_->scripts_to_be_executed();

      std::deque<HTMLScriptElement*>::iterator it = std::find(
          scripts_to_be_executed->begin(), scripts_to_be_executed->end(), this);
      if (it != scripts_to_be_executed->end()) {
        scripts_to_be_executed->erase(it);
      }
    } break;
    case 5: {
      // If the element has a src attribute.
    } break;
  }

  // Fetching an external script must delay the load event of the element's
  // document until the task that is queued by the networking task source
  // once the resource has been fetched (defined above) has been run.
  document_->DecreaseLoadingCounterAndMaybeDispatchLoadEvent();

  // Post a task to release the loader.
  DCHECK(base::MessageLoopCurrent::Get()); // TODO
  //base::MessageLoop::current()->task_runner()->PostTask(
  base::MessageLoopCurrent::Get()->task_runner()->PostTask(
      FROM_HERE, base::Bind(&HTMLScriptElement::ReleaseLoader, this));
}

void HTMLScriptElement::ExecuteExternal() {
  DCHECK(content_);
  Execute(*content_, base::SourceLocation(url_.spec(), 1, 1), true);
}

void HTMLScriptElement::ExecuteInternal() {
  Execute(text_content().value(), inline_script_location_, false);
}

// Algorithm for Execute:
//   https://www.w3.org/TR/html5/scripting-1.html#execute-the-script-block
void HTMLScriptElement::Execute(const std::string& content,
                                const base::SourceLocation& script_location,
                                bool is_external) {
  TRACK_MEMORY_SCOPE("DOM");
  // If should_execute_ is set to false for whatever reason, then we
  // immediately exit.
  // When inserted using the document.write() method, script elements execute
  // (typically synchronously), but when inserted using innerHTML and
  // outerHTML attributes, they do not execute at all.
  // https://www.w3.org/TR/html5/scripting-1.html#the-script-element.
  if (!should_execute_) {
    return;
  }

  // The script is now being run. Track it in the global stats.
  GlobalStats::GetInstance()->StartJavaScriptEvent();

  TRACE_EVENT2("cobalt::dom", "HTMLScriptElement::Execute()", "file_path",
               script_location.file_path, "line_number",
               script_location.line_number);
  // Since error is already handled, it is guaranteed the load is successful.

  // 1. 2. 3. Not needed by Cobalt.

  // 4. Create a script, using the script block's source, the URL from which the
  // script was obtained, the script block's type as the scripting language, and
  // the script settings object of the script element's Document's Window
  // object.
  bool mute_errors =
#ifdef ENABLE_LOADER
      request_mode_ == loader::kNoCORSMode &&
      fetched_last_url_origin_ != document_->location()->GetOriginAsObject();
#else
      false;
#endif
#if defined(ENABLE_SCRIPT_RUNNER)
  html_element_context()->script_runner()->Execute(
      content, script_location, mute_errors, NULL /*out_succeeded*/);
#endif

  // 5. 6. Not needed by Cobalt.

  // 7. If the script is from an external file, fire a simple event named load
  // at the script element.
  // Otherwise, the script is internal; queue a task to fire a simple event
  // named load at the script element.
  // TODO: Remove the firing of readystatechange once we support Promise.
  if (is_external) {
    DispatchEvent(new Event(base::Tokens::load()));
    DispatchEvent(new Event(base::Tokens::readystatechange()));
  } else {
    PreventGarbageCollectionAndPostToDispatchEvent(
        FROM_HERE, base::Tokens::load(),
        &prevent_gc_until_load_event_dispatch_);
    PreventGarbageCollectionAndPostToDispatchEvent(
        FROM_HERE, base::Tokens::readystatechange(),
        &prevent_gc_until_ready_event_dispatch_);
  }

  // The script is done running. Stop tracking it in the global stats.
  GlobalStats::GetInstance()->StopJavaScriptEvent();

  // Notify the DomStatTracker of the execution.
  dom_stat_tracker_->OnHtmlScriptElementExecuted(content.size());
}

void HTMLScriptElement::PreventGarbageCollectionAndPostToDispatchEvent(
    const base::Location& location, const base::CobToken& token,
    std::unique_ptr<script::GlobalEnvironment::ScopedPreventGarbageCollection>*
        scoped_prevent_gc) {
  // Ensure that this HTMLScriptElement is not garbage collected until the event
  // has been processed.
#if defined(ENABLE_SCRIPT_RUNNER)
  DCHECK(!(*scoped_prevent_gc));
  scoped_prevent_gc->reset(
      new script::GlobalEnvironment::ScopedPreventGarbageCollection(
          html_element_context()->script_runner()->GetGlobalEnvironment(),
          this));
  PostToDispatchEventNameAndRunCallback(
      location, token,
      base::Bind(&HTMLScriptElement::AllowGCAfterEventDispatch, this,
                 scoped_prevent_gc));
#endif
}

void HTMLScriptElement::AllowGCAfterEventDispatch(
    std::unique_ptr<script::GlobalEnvironment::ScopedPreventGarbageCollection>*
        scoped_prevent_gc) {
  scoped_prevent_gc->reset();
}

void HTMLScriptElement::PreventGCUntilLoadComplete() {
#if defined(ENABLE_SCRIPT_RUNNER)
  DCHECK(!prevent_gc_until_load_complete_);
  prevent_gc_until_load_complete_.reset(
      new script::GlobalEnvironment::ScopedPreventGarbageCollection(
          html_element_context()->script_runner()->GetGlobalEnvironment(),
          this));
#endif
}

void HTMLScriptElement::AllowGCAfterLoadComplete() {
  prevent_gc_until_load_complete_.reset();
}

void HTMLScriptElement::ReleaseLoader() {
#ifdef ENABLE_LOADER
  DCHECK(thread_checker_.CalledOnValidThread());
  DCHECK(loader_);
  loader_.reset();
#endif
}

}  // namespace dom
}  // namespace cobalt
