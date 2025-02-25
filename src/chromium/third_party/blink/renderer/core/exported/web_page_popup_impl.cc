/*
 * Copyright (C) 2012 Google Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
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

#include "third_party/blink/renderer/core/exported/web_page_popup_impl.h"

#include <memory>

#include "cc/animation/animation_host.h"
#include "cc/layers/picture_layer.h"
#include "third_party/blink/public/platform/web_cursor_info.h"
#include "third_party/blink/public/platform/web_float_rect.h"
#include "third_party/blink/public/web/web_view_client.h"
#include "third_party/blink/renderer/core/accessibility/ax_object_cache_base.h"
#include "third_party/blink/renderer/core/dom/context_features.h"
#include "third_party/blink/renderer/core/dom/events/event_dispatch_forbidden_scope.h"
#include "third_party/blink/renderer/core/events/message_event.h"
#include "third_party/blink/renderer/core/events/web_input_event_conversion.h"
#include "third_party/blink/renderer/core/exported/web_settings_impl.h"
#include "third_party/blink/renderer/core/exported/web_view_impl.h"
#include "third_party/blink/renderer/core/frame/local_dom_window.h"
#include "third_party/blink/renderer/core/frame/local_frame.h"
#include "third_party/blink/renderer/core/frame/local_frame_client.h"
#include "third_party/blink/renderer/core/frame/local_frame_ukm_aggregator.h"
#include "third_party/blink/renderer/core/frame/local_frame_view.h"
#include "third_party/blink/renderer/core/frame/settings.h"
#include "third_party/blink/renderer/core/frame/visual_viewport.h"
#include "third_party/blink/renderer/core/frame/web_frame_widget_base.h"
#include "third_party/blink/renderer/core/frame/web_local_frame_impl.h"
#include "third_party/blink/renderer/core/input/event_handler.h"
#include "third_party/blink/renderer/core/layout/layout_view.h"
#include "third_party/blink/renderer/core/loader/empty_clients.h"
#include "third_party/blink/renderer/core/loader/frame_load_request.h"
#include "third_party/blink/renderer/core/page/focus_controller.h"
#include "third_party/blink/renderer/core/page/page.h"
#include "third_party/blink/renderer/core/page/page_popup_client.h"
#include "third_party/blink/renderer/core/page/page_popup_supplement.h"
#include "third_party/blink/renderer/platform/animation/compositor_animation_timeline.h"
#include "third_party/blink/renderer/platform/bindings/script_forbidden_scope.h"
#include "third_party/blink/renderer/platform/graphics/graphics_layer.h"
#include "third_party/blink/renderer/platform/heap/handle.h"
#include "third_party/blink/renderer/platform/instrumentation/tracing/trace_event.h"
#include "third_party/blink/renderer/platform/web_test_support.h"

namespace blink {

class PagePopupChromeClient final : public EmptyChromeClient {
 public:
  explicit PagePopupChromeClient(WebPagePopupImpl* popup) : popup_(popup) {}

  void SetWindowRect(const IntRect& rect, LocalFrame&) override {
    popup_->SetWindowRect(rect);
  }

  bool IsPopup() override { return true; }

 private:
  void CloseWindowSoon() override {
    // This skips past the PopupClient by calling ClosePopup() instead of
    // Cancel().
    popup_->ClosePopup();
  }

  IntRect RootWindowRect(LocalFrame&) override {
    // There is only one frame/widget in a WebPagePopup, so we can ignore the
    // param.
    return popup_->WindowRectInScreen();
  }

  IntRect ViewportToScreen(const IntRect& rect,
                           const LocalFrameView*) const override {
    WebRect rect_in_screen(rect);
    WebRect window_rect = popup_->WindowRectInScreen();
    popup_->WidgetClient()->ConvertViewportToWindow(&rect_in_screen);
    rect_in_screen.x += window_rect.x;
    rect_in_screen.y += window_rect.y;
    return rect_in_screen;
  }

  float WindowToViewportScalar(const float scalar_value) const override {
    WebFloatRect viewport_rect(0, 0, scalar_value, 0);
    popup_->WidgetClient()->ConvertWindowToViewport(&viewport_rect);
    return viewport_rect.width;
  }

  void AddMessageToConsole(LocalFrame*,
                           mojom::ConsoleMessageSource,
                           mojom::ConsoleMessageLevel,
                           const String& message,
                           unsigned line_number,
                           const String&,
                           const String&) override {
#ifndef NDEBUG
    fprintf(stderr, "CONSOLE MESSSAGE:%u: %s\n", line_number,
            message.Utf8().data());
#endif
  }

  void ScheduleAnimation(const LocalFrameView*) override {
    if (WebTestSupport::IsRunningWebTest()) {
      // In single threaded web tests, the main frame's WebWidgetClient
      // (provided by WebViewTestProxy or WebWidgetTestProxy) runs the composite
      // step for the current popup. We don't run popup tests with a compositor
      // thread.
      popup_->web_view_->WidgetClient()->ScheduleAnimation();
      return;
    }
    popup_->WidgetClient()->ScheduleAnimation();
  }

  void AttachCompositorAnimationTimeline(CompositorAnimationTimeline* timeline,
                                         LocalFrame*) override {
    popup_->animation_host_->AddAnimationTimeline(
        timeline->GetAnimationTimeline());
  }

  void DetachCompositorAnimationTimeline(CompositorAnimationTimeline* timeline,
                                         LocalFrame*) override {
    popup_->animation_host_->RemoveAnimationTimeline(
        timeline->GetAnimationTimeline());
  }

  WebScreenInfo GetScreenInfo() const override {
    return popup_->web_view_->Client()->GetScreenInfo();
  }

  WebViewImpl* GetWebView() const override { return popup_->web_view_; }

  IntSize MinimumWindowSize() const override { return IntSize(0, 0); }

  void SetCursor(const Cursor& cursor, LocalFrame* local_frame) override {
    popup_->WidgetClient()->DidChangeCursor(WebCursorInfo(cursor));
  }

  void SetEventListenerProperties(
      LocalFrame* frame,
      cc::EventListenerClass event_class,
      cc::EventListenerProperties properties) override {
    DCHECK(frame->IsMainFrame());
    WebWidgetClient* client = popup_->WidgetClient();
    if (WebLayerTreeView* layer_tree_view = popup_->layer_tree_view_) {
      layer_tree_view->SetEventListenerProperties(event_class, properties);
      if (event_class == cc::EventListenerClass::kTouchStartOrMove) {
        client->HasTouchEventHandlers(
            properties != cc::EventListenerProperties::kNone ||
            EventListenerProperties(
                frame, cc::EventListenerClass::kTouchEndOrCancel) !=
                cc::EventListenerProperties::kNone);
      } else if (event_class == cc::EventListenerClass::kTouchEndOrCancel) {
        client->HasTouchEventHandlers(
            properties != cc::EventListenerProperties::kNone ||
            EventListenerProperties(
                frame, cc::EventListenerClass::kTouchStartOrMove) !=
                cc::EventListenerProperties::kNone);
      }
    } else {
      client->HasTouchEventHandlers(true);
    }
  }
  cc::EventListenerProperties EventListenerProperties(
      LocalFrame*,
      cc::EventListenerClass event_class) const override {
    if (popup_->layer_tree_view_) {
      return popup_->layer_tree_view_->EventListenerProperties(event_class);
    }
    return cc::EventListenerProperties::kNone;
  }

  void SetHasScrollEventHandlers(LocalFrame* frame,
                                 bool has_event_handlers) override {
    DCHECK(frame->IsMainFrame());
    if (popup_->layer_tree_view_)
      popup_->layer_tree_view_->SetHaveScrollEventHandlers(has_event_handlers);
  }

  void SetTouchAction(LocalFrame* frame, TouchAction touch_action) override {
    DCHECK(frame);
    WebLocalFrameImpl* web_frame = WebLocalFrameImpl::FromFrame(frame);
    // TODO(https://crbug.com/844547): check if we are setting touch action on
    // pop up window or not.
    if (!web_frame)
      return;
    WebFrameWidget* widget = web_frame->LocalRoot()->FrameWidget();
    if (!widget)
      return;
    if (WebWidgetClient* client = To<WebFrameWidgetBase>(widget)->Client())
      client->SetTouchAction(static_cast<WebTouchAction>(touch_action));
  }

  void AttachRootGraphicsLayer(GraphicsLayer* graphics_layer,
                               LocalFrame* local_root) override {
    popup_->SetRootLayer(graphics_layer ? graphics_layer->CcLayer() : nullptr);
  }

  void AttachRootLayer(scoped_refptr<cc::Layer> layer,
                       LocalFrame* local_root) override {
    popup_->SetRootLayer(layer.get());
  }

  void SetToolTip(LocalFrame&,
                  const String& tooltip_text,
                  TextDirection dir) override {
    if (popup_->WidgetClient()) {
      popup_->WidgetClient()->SetToolTipText(tooltip_text,
                                             ToWebTextDirection(dir));
    }
  }

  void InjectGestureScrollEvent(const blink::WebFloatSize& delta,
                                blink::WebScrollGranularity granularity,
                                cc::ElementId scrollable_area_element_id,
                                WebInputEvent::Type injected_type) override {
    popup_->WidgetClient()->InjectGestureScrollEvent(
        delta, granularity, scrollable_area_element_id, injected_type);
  }

  WebPagePopupImpl* popup_;
};

class PagePopupFeaturesClient : public ContextFeaturesClient {
  bool IsEnabled(Document*, ContextFeatures::FeatureType, bool) override;
};

bool PagePopupFeaturesClient::IsEnabled(Document*,
                                        ContextFeatures::FeatureType type,
                                        bool default_value) {
  if (type == ContextFeatures::kPagePopup)
    return true;
  return default_value;
}

// WebPagePopupImpl ----------------------------------------------------------

WebPagePopupImpl::WebPagePopupImpl(WebPagePopupClient* client)
    : web_page_popup_client_(client) {
  DCHECK(client);
}

WebPagePopupImpl::~WebPagePopupImpl() {
  DCHECK(!page_);
}

void WebPagePopupImpl::Initialize(WebViewImpl* web_view,
                                  PagePopupClient* popup_client) {
  DCHECK(web_view);
  DCHECK(popup_client);
  web_view_ = web_view;
  popup_client_ = popup_client;

  Page::PageClients page_clients;
  FillWithEmptyClients(page_clients);
  chrome_client_ = MakeGarbageCollected<PagePopupChromeClient>(this);
  page_clients.chrome_client = chrome_client_.Get();

  Settings& main_settings = web_view_->GetPage()->GetSettings();
  page_ = Page::CreateNonOrdinary(page_clients);
  page_->GetSettings().SetAcceleratedCompositingEnabled(true);
  page_->GetSettings().SetScriptEnabled(true);
  page_->GetSettings().SetAllowScriptsToCloseWindows(true);
  page_->GetSettings().SetMinimumFontSize(main_settings.GetMinimumFontSize());
  page_->GetSettings().SetMinimumLogicalFontSize(
      main_settings.GetMinimumLogicalFontSize());
  page_->GetSettings().SetScrollAnimatorEnabled(
      main_settings.GetScrollAnimatorEnabled());
  page_->GetSettings().SetAvailablePointerTypes(
      main_settings.GetAvailablePointerTypes());
  page_->GetSettings().SetPrimaryPointerType(
      main_settings.GetPrimaryPointerType());

  ProvideContextFeaturesTo(*page_, std::make_unique<PagePopupFeaturesClient>());
  DEFINE_STATIC_LOCAL(Persistent<LocalFrameClient>, empty_local_frame_client,
                      (MakeGarbageCollected<EmptyLocalFrameClient>()));
  LocalFrame* frame =
      LocalFrame::Create(empty_local_frame_client, *page_, nullptr);
  frame->SetPagePopupOwner(popup_client_->OwnerElement());
  frame->SetView(LocalFrameView::Create(*frame));
  frame->Init();
  frame->View()->SetParentVisible(true);
  frame->View()->SetSelfVisible(true);

  DCHECK(frame->DomWindow());
  PagePopupSupplement::Install(*frame, *this, popup_client_);
  DCHECK_EQ(popup_client_->OwnerElement().GetDocument().ExistingAXObjectCache(),
            frame->GetDocument()->ExistingAXObjectCache());
  if (AXObjectCache* cache = frame->GetDocument()->ExistingAXObjectCache()) {
    cache->InitializePopup(frame->GetDocument());
    cache->ChildrenChanged(&popup_client_->OwnerElement());
  }

  page_->LayerTreeViewInitialized(*layer_tree_view_, *animation_host_, nullptr);

  scoped_refptr<SharedBuffer> data = SharedBuffer::Create();
  popup_client_->WriteDocument(data.get());
  frame->SetPageZoomFactor(popup_client_->ZoomFactor());
  frame->ForceSynchronousDocumentInstall("text/html", std::move(data));

  WidgetClient()->Show(WebNavigationPolicy());
  SetFocus(true);
}

void WebPagePopupImpl::SetLayerTreeView(WebLayerTreeView* layer_tree_view,
                                        cc::AnimationHost* animation_host) {
  // The WebWidgetClient is given |this| as its WebWidget but it is set up
  // before Initialize() is called on |this|. So we store the |layer_tree_view|
  // here, but finish setting it up in Initialize().
  layer_tree_view_ = layer_tree_view;
  animation_host_ = animation_host;
}

void WebPagePopupImpl::PostMessageToPopup(const String& message) {
  if (!page_)
    return;
  ScriptForbiddenScope::AllowUserAgentScript allow_script;
  MainFrame().DomWindow()->DispatchEvent(*MessageEvent::Create(message));
}

void WebPagePopupImpl::DestroyPage() {
  page_->WillCloseLayerTreeView(*layer_tree_view_, nullptr);
  page_->WillBeDestroyed();
  page_.Clear();
}

AXObject* WebPagePopupImpl::RootAXObject() {
  if (!page_)
    return nullptr;
  // If |page_| is non-null, the main frame must have a Document.
  Document* document = MainFrame().GetDocument();
  AXObjectCache* cache = document->ExistingAXObjectCache();
  // There should never be a circumstance when RootAXObject() is triggered
  // and the AXObjectCache doesn't already exist. It's called when trying
  // to attach the accessibility tree of the pop-up to the host page.
  DCHECK(cache);
  return To<AXObjectCacheBase>(cache)->GetOrCreate(document->GetLayoutView());
}

void WebPagePopupImpl::SetWindowRect(const IntRect& rect_in_screen) {
  WidgetClient()->SetWindowRect(rect_in_screen);
}

void WebPagePopupImpl::SetRootLayer(scoped_refptr<cc::Layer> layer) {
  is_accelerated_compositing_active_ = !!layer;
  root_layer_ = std::move(layer);
  WidgetClient()->SetRootLayer(root_layer_);
}

void WebPagePopupImpl::SetSuppressFrameRequestsWorkaroundFor704763Only(
    bool suppress_frame_requests) {
  if (!page_)
    return;
  page_->Animator().SetSuppressFrameRequestsWorkaroundFor704763Only(
      suppress_frame_requests);
}

void WebPagePopupImpl::BeginFrame(base::TimeTicks last_frame_time, bool) {
  if (!page_)
    return;
  // FIXME: This should use lastFrameTimeMonotonic but doing so
  // breaks tests.
  PageWidgetDelegate::Animate(*page_, CurrentTimeTicks());
}

void WebPagePopupImpl::UpdateLifecycle(LifecycleUpdate requested_update,
                                       LifecycleUpdateReason reason) {
  if (!page_)
    return;
  // Popups always update their lifecycle in the context of the containing
  // document's lifecycle, so explicitly override the reason.
  PageWidgetDelegate::UpdateLifecycle(*page_, MainFrame(), requested_update,
                                      WebWidget::LifecycleUpdateReason::kOther);
}

void WebPagePopupImpl::Resize(const WebSize& new_size_in_viewport) {
  WebRect new_size(0, 0, new_size_in_viewport.width,
                   new_size_in_viewport.height);
  WidgetClient()->ConvertViewportToWindow(&new_size);

  WebRect window_rect = WindowRectInScreen();

  // TODO(bokan): We should only call into this if the bounds actually changed
  // but this reveals a bug in Aura. crbug.com/633140.
  window_rect.width = new_size.width;
  window_rect.height = new_size.height;
  SetWindowRect(window_rect);

  if (page_) {
    MainFrame().View()->Resize(new_size_in_viewport);
    page_->GetVisualViewport().SetSize(new_size_in_viewport);
  }
}

WebInputEventResult WebPagePopupImpl::HandleKeyEvent(
    const WebKeyboardEvent& event) {
  if (closing_)
    return WebInputEventResult::kNotHandled;
  return MainFrame().GetEventHandler().KeyEvent(event);
}

WebInputEventResult WebPagePopupImpl::HandleCharEvent(
    const WebKeyboardEvent& event) {
  return HandleKeyEvent(event);
}

WebInputEventResult WebPagePopupImpl::HandleGestureEvent(
    const WebGestureEvent& event) {
  if (closing_)
    return WebInputEventResult::kNotHandled;
  if ((event.GetType() == WebInputEvent::kGestureTap ||
       event.GetType() == WebInputEvent::kGestureTapDown) &&
      !IsViewportPointInWindow(event.PositionInWidget().x,
                               event.PositionInWidget().y)) {
    Cancel();
    return WebInputEventResult::kNotHandled;
  }
  WebGestureEvent scaled_event =
      TransformWebGestureEvent(MainFrame().View(), event);
  return MainFrame().GetEventHandler().HandleGestureEvent(scaled_event);
}

void WebPagePopupImpl::HandleMouseDown(LocalFrame& main_frame,
                                       const WebMouseEvent& event) {
  if (IsViewportPointInWindow(event.PositionInWidget().x,
                              event.PositionInWidget().y))
    PageWidgetEventHandler::HandleMouseDown(main_frame, event);
  else
    Cancel();
}

WebInputEventResult WebPagePopupImpl::HandleMouseWheel(
    LocalFrame& main_frame,
    const WebMouseWheelEvent& event) {
  if (IsViewportPointInWindow(event.PositionInWidget().x,
                              event.PositionInWidget().y))
    return PageWidgetEventHandler::HandleMouseWheel(main_frame, event);
  Cancel();
  return WebInputEventResult::kNotHandled;
}

LocalFrame& WebPagePopupImpl::MainFrame() const {
  DCHECK(page_);
  // The main frame for a popup will never be out-of-process.
  return *To<LocalFrame>(page_->MainFrame());
}

bool WebPagePopupImpl::IsViewportPointInWindow(int x, int y) {
  WebRect point_in_window(x, y, 0, 0);
  WidgetClient()->ConvertViewportToWindow(&point_in_window);
  WebRect window_rect = WindowRectInScreen();
  return IntRect(0, 0, window_rect.width, window_rect.height)
      .Contains(IntPoint(point_in_window.x, point_in_window.y));
}

WebInputEventResult WebPagePopupImpl::DispatchBufferedTouchEvents() {
  if (closing_)
    return WebInputEventResult::kNotHandled;
  return MainFrame().GetEventHandler().DispatchBufferedTouchEvents();
}

WebInputEventResult WebPagePopupImpl::HandleInputEvent(
    const WebCoalescedInputEvent& event) {
  if (closing_)
    return WebInputEventResult::kNotHandled;
  DCHECK(!WebInputEvent::IsTouchEventType(event.Event().GetType()));
  return PageWidgetDelegate::HandleInputEvent(*this, event, &MainFrame());
}

void WebPagePopupImpl::SetFocus(bool enable) {
  if (!page_)
    return;
  if (enable)
    page_->GetFocusController().SetActive(true);
  page_->GetFocusController().SetFocused(enable);
}

WebURL WebPagePopupImpl::GetURLForDebugTrace() {
  WebFrame* main_frame = web_view_->MainFrame();
  if (main_frame->IsWebLocalFrame())
    return main_frame->ToWebLocalFrame()->GetDocument().Url();
  return {};
}

void WebPagePopupImpl::Close() {
  // If the popup is closed from the renderer via Cancel(), then ClosePopup()
  // has already run on another stack, and destroyed |page_|. If the popup is
  // closed from the browser via IPC to RenderWidget, then we come here first
  // and want to synchronously Cancel() immediately.
  if (page_) {
    // We set |closing_| here to inform ClosePopup() that it is being run
    // synchronously from inside Close().
    closing_ = true;
    // This should end up running ClosePopup() though the PopupClient.
    Cancel();
  }

  is_accelerated_compositing_active_ = false;
  layer_tree_view_ = nullptr;
  animation_host_ = nullptr;
  web_page_popup_client_ = nullptr;

  // Self-delete on Close().
  Release();
}

void WebPagePopupImpl::ClosePopup() {
  // There's always a |page_| when we get here because if we Close() this object
  // due to ClosePopupWidgetSoon(), it will see the |page_| destroyed and not
  // run this method again. And the renderer does not close the same popup more
  // than once.
  DCHECK(page_);

  // If the popup is closed from the renderer via Cancel(), then we want to
  // initiate closing immediately here, but send a request for completing the
  // close process through the browser via ClosePopupWidgetSoon(), which will
  // close the RenderWidget and come back to this class to Close().
  // If |closing_| is already true, then the browser initiated the close on its
  // own, via IPC to the RenderWidget, which means ClosePopup() is being run
  // inside the same stack, and does not need to request the browser to close
  // the RenderWidget.
  const bool running_inside_close = closing_;
  if (!running_inside_close) {
    // Bounce through the browser to get it to close the RenderWidget, which
    // will Close() this object too. Only if we're not currently already
    // responding to the browser closing us though.
    web_page_popup_client_->ClosePopupWidgetSoon();
  }

  closing_ = true;

  if (AXObjectCache* cache = MainFrame().GetDocument()->ExistingAXObjectCache())
    cache->DisposePopup(MainFrame().GetDocument());

  {
    // This function can be called in EventDispatchForbiddenScope for the main
    // document, and the following operations dispatch some events.  It's safe
    // because web authors can't listen the events.
    EventDispatchForbiddenScope::AllowUserAgentEvents allow_events;

    MainFrame().Loader().StopAllLoaders();
    PagePopupSupplement::Uninstall(MainFrame());
    DestroyPage();
  }

  // Informs the client to drop any references to this popup as it will be
  // destroyed.
  popup_client_->DidClosePopup();

  // Drops the reference to the popup from WebViewImpl, making |this| the only
  // owner of itself. Note however that WebViewImpl may briefly extend the
  // lifetime of this object since it owns a reference, but it should only be
  // to call HasSamePopupClient().
  web_view_->CleanupPagePopup();
}

LocalDOMWindow* WebPagePopupImpl::Window() {
  return MainFrame().DomWindow();
}

WebPoint WebPagePopupImpl::PositionRelativeToOwner() {
  WebRect root_window_rect = WindowRectInScreen();
  WebRect window_rect = WindowRectInScreen();
  return WebPoint(window_rect.x - root_window_rect.x,
                  window_rect.y - root_window_rect.y);
}

WebPagePopupClient* WebPagePopupImpl::GetClientForTesting() const {
  return web_page_popup_client_;
}

void WebPagePopupImpl::Cancel() {
  if (popup_client_)
    popup_client_->CancelPopup();
}

WebRect WebPagePopupImpl::WindowRectInScreen() const {
  return WidgetClient()->WindowRect();
}

// WebPagePopup ----------------------------------------------------------------

WebPagePopup* WebPagePopup::Create(WebPagePopupClient* client) {
  CHECK(client);
  // A WebPagePopupImpl instance usually has two references.
  //  - One owned by the instance itself. It represents the visible widget.
  //  - One owned by a WebViewImpl. It's released when the WebViewImpl ask the
  //    WebPagePopupImpl to close.
  // We need them because the closing operation is asynchronous and the widget
  // can be closed while the WebViewImpl is unaware of it.
  auto popup = base::AdoptRef(new WebPagePopupImpl(client));
  popup->AddRef();
  return popup.get();
}

}  // namespace blink
