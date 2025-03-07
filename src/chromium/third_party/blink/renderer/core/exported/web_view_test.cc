/*
 * Copyright (C) 2011, 2012 Google Inc. All rights reserved.
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

#include "third_party/blink/public/web/web_view.h"

#include <limits>
#include <memory>
#include <string>

#include "base/stl_util.h"
#include "base/time/time.h"
#include "build/build_config.h"
#include "cc/trees/layer_tree_host.h"
#include "gin/handle.h"
#include "gin/object_template_builder.h"
#include "gin/wrappable.h"
#include "testing/gtest/include/gtest/gtest.h"
#include "third_party/blink/public/common/frame/frame_owner_element_type.h"
#include "third_party/blink/public/common/manifest/web_display_mode.h"
#include "third_party/blink/public/mojom/frame/document_interface_broker.mojom-blink.h"
#include "third_party/blink/public/platform/platform.h"
#include "third_party/blink/public/platform/web_coalesced_input_event.h"
#include "third_party/blink/public/platform/web_cursor_info.h"
#include "third_party/blink/public/platform/web_drag_data.h"
#include "third_party/blink/public/platform/web_drag_operation.h"
#include "third_party/blink/public/platform/web_float_point.h"
#include "third_party/blink/public/platform/web_input_event.h"
#include "third_party/blink/public/platform/web_keyboard_event.h"
#include "third_party/blink/public/platform/web_layer_tree_view.h"
#include "third_party/blink/public/platform/web_size.h"
#include "third_party/blink/public/platform/web_url_loader_mock_factory.h"
#include "third_party/blink/public/public_buildflags.h"
#include "third_party/blink/public/web/web_autofill_client.h"
#include "third_party/blink/public/web/web_console_message.h"
#include "third_party/blink/public/web/web_device_emulation_params.h"
#include "third_party/blink/public/web/web_document.h"
#include "third_party/blink/public/web/web_element.h"
#include "third_party/blink/public/web/web_frame.h"
#include "third_party/blink/public/web/web_frame_content_dumper.h"
#include "third_party/blink/public/web/web_frame_widget.h"
#include "third_party/blink/public/web/web_hit_test_result.h"
#include "third_party/blink/public/web/web_input_method_controller.h"
#include "third_party/blink/public/web/web_local_frame_client.h"
#include "third_party/blink/public/web/web_print_params.h"
#include "third_party/blink/public/web/web_script_source.h"
#include "third_party/blink/public/web/web_settings.h"
#include "third_party/blink/public/web/web_tree_scope_type.h"
#include "third_party/blink/public/web/web_view_client.h"
#include "third_party/blink/public/web/web_widget.h"
#include "third_party/blink/public/web/web_widget_client.h"
#include "third_party/blink/renderer/bindings/core/v8/v8_document.h"
#include "third_party/blink/renderer/core/dom/document.h"
#include "third_party/blink/renderer/core/dom/element.h"
#include "third_party/blink/renderer/core/dom/node_computed_style.h"
#include "third_party/blink/renderer/core/dom/user_gesture_indicator.h"
#include "third_party/blink/renderer/core/editing/frame_selection.h"
#include "third_party/blink/renderer/core/editing/ime/input_method_controller.h"
#include "third_party/blink/renderer/core/editing/markers/document_marker_controller.h"
#include "third_party/blink/renderer/core/exported/web_settings_impl.h"
#include "third_party/blink/renderer/core/exported/web_view_impl.h"
#include "third_party/blink/renderer/core/frame/event_handler_registry.h"
#include "third_party/blink/renderer/core/frame/frame_test_helpers.h"
#include "third_party/blink/renderer/core/frame/local_dom_window.h"
#include "third_party/blink/renderer/core/frame/local_frame.h"
#include "third_party/blink/renderer/core/frame/local_frame_view.h"
#include "third_party/blink/renderer/core/frame/settings.h"
#include "third_party/blink/renderer/core/frame/visual_viewport.h"
#include "third_party/blink/renderer/core/frame/web_local_frame_impl.h"
#include "third_party/blink/renderer/core/fullscreen/fullscreen.h"
#include "third_party/blink/renderer/core/html/forms/external_date_time_chooser.h"
#include "third_party/blink/renderer/core/html/forms/html_input_element.h"
#include "third_party/blink/renderer/core/html/forms/html_text_area_element.h"
#include "third_party/blink/renderer/core/html/html_iframe_element.h"
#include "third_party/blink/renderer/core/html/html_object_element.h"
#include "third_party/blink/renderer/core/inspector/dev_tools_emulator.h"
#include "third_party/blink/renderer/core/layout/layout_view.h"
#include "third_party/blink/renderer/core/loader/document_loader.h"
#include "third_party/blink/renderer/core/loader/frame_load_request.h"
#include "third_party/blink/renderer/core/loader/interactive_detector.h"
#include "third_party/blink/renderer/core/page/chrome_client.h"
#include "third_party/blink/renderer/core/page/focus_controller.h"
#include "third_party/blink/renderer/core/page/page.h"
#include "third_party/blink/renderer/core/page/page_hidden_state.h"
#include "third_party/blink/renderer/core/page/print_context.h"
#include "third_party/blink/renderer/core/page/scoped_page_pauser.h"
#include "third_party/blink/renderer/core/paint/paint_layer.h"
#include "third_party/blink/renderer/core/paint/paint_layer_painter.h"
#include "third_party/blink/renderer/core/paint/paint_layer_scrollable_area.h"
#include "third_party/blink/renderer/core/scroll/scroll_types.h"
#include "third_party/blink/renderer/core/testing/fake_web_plugin.h"
#include "third_party/blink/renderer/core/timing/dom_window_performance.h"
#include "third_party/blink/renderer/core/timing/window_performance.h"
#include "third_party/blink/renderer/platform/geometry/int_rect.h"
#include "third_party/blink/renderer/platform/geometry/int_size.h"
#include "third_party/blink/renderer/platform/graphics/color.h"
#include "third_party/blink/renderer/platform/graphics/graphics_context.h"
#include "third_party/blink/renderer/platform/graphics/graphics_layer.h"
#include "third_party/blink/renderer/platform/graphics/paint/paint_record_builder.h"
#include "third_party/blink/renderer/platform/keyboard_codes.h"
#include "third_party/blink/renderer/platform/scheduler/public/thread.h"
#include "third_party/blink/renderer/platform/testing/histogram_tester.h"
#include "third_party/blink/renderer/platform/testing/unit_test_helpers.h"
#include "third_party/blink/renderer/platform/testing/url_test_helpers.h"
#include "third_party/blink/renderer/platform/testing/wtf/scoped_mock_clock.h"
#include "third_party/skia/include/core/SkBitmap.h"
#include "third_party/skia/include/core/SkCanvas.h"
#include "ui/events/keycodes/dom/dom_key.h"
#include "v8/include/v8.h"

#if defined(OS_MACOSX)
#include "third_party/blink/public/web/mac/web_substring_util.h"
#endif

#if BUILDFLAG(ENABLE_UNHANDLED_TAP)
#include "mojo/public/cpp/bindings/strong_binding.h"
#include "third_party/blink/public/mojom/unhandled_tap_notifier/unhandled_tap_notifier.mojom-blink.h"
#include "third_party/blink/renderer/platform/testing/testing_platform_support.h"
#endif  // BUILDFLAG(ENABLE_UNHANDLED_TAP)

using blink::frame_test_helpers::LoadFrame;
using blink::url_test_helpers::ToKURL;
using blink::url_test_helpers::RegisterMockedURLLoad;
using blink::test::RunPendingTasks;

namespace blink {

enum HorizontalScrollbarState {
  kNoHorizontalScrollbar,
  kVisibleHorizontalScrollbar,
};

enum VerticalScrollbarState {
  kNoVerticalScrollbar,
  kVisibleVerticalScrollbar,
};

class TestData {
 public:
  void SetWebView(WebView* web_view) {
    web_view_ = static_cast<WebViewImpl*>(web_view);
  }
  void SetSize(const WebSize& new_size) { size_ = new_size; }
  HorizontalScrollbarState GetHorizontalScrollbarState() const {
    return web_view_->HasHorizontalScrollbar() ? kVisibleHorizontalScrollbar
                                               : kNoHorizontalScrollbar;
  }
  VerticalScrollbarState GetVerticalScrollbarState() const {
    return web_view_->HasVerticalScrollbar() ? kVisibleVerticalScrollbar
                                             : kNoVerticalScrollbar;
  }
  int Width() const { return size_.width; }
  int Height() const { return size_.height; }

 private:
  WebSize size_;
  WebViewImpl* web_view_;
};

class AutoResizeWebViewClient : public frame_test_helpers::TestWebViewClient {
 public:
  // WebViewClient methods
  void DidAutoResize(const WebSize& new_size) override {
    test_data_.SetSize(new_size);
  }

  // Local methods
  TestData& GetTestData() { return test_data_; }

 private:
  TestData test_data_;
};

class TapHandlingWebWidgetClient
    : public frame_test_helpers::TestWebWidgetClient {
 public:
  // WebWidgetClient overrides.
  void DidHandleGestureEvent(const WebGestureEvent& event,
                             bool event_cancelled) override {
    if (event.GetType() == WebInputEvent::kGestureTap) {
      tap_x_ = event.PositionInWidget().x;
      tap_y_ = event.PositionInWidget().y;
    } else if (event.GetType() == WebInputEvent::kGestureLongPress) {
      longpress_x_ = event.PositionInWidget().x;
      longpress_y_ = event.PositionInWidget().y;
    }
  }

  // Local methods
  void Reset() {
    tap_x_ = -1;
    tap_y_ = -1;
    longpress_x_ = -1;
    longpress_y_ = -1;
  }
  int TapX() { return tap_x_; }
  int TapY() { return tap_y_; }
  int LongpressX() { return longpress_x_; }
  int LongpressY() { return longpress_y_; }

 private:
  int tap_x_;
  int tap_y_;
  int longpress_x_;
  int longpress_y_;
};

class WebViewTest : public testing::Test {
 public:
  WebViewTest() : base_url_("http://www.test.com/") {}

  void TearDown() override {
    Platform::Current()
        ->GetURLLoaderMockFactory()
        ->UnregisterAllURLsAndClearMemoryCache();
  }

 protected:
  void SetViewportSize(const WebSize& size) {
    content::LayerTreeView* layer_tree_view =
        web_view_helper_.GetLayerTreeView();
    layer_tree_view->SetViewportSizeAndScale(
        static_cast<gfx::Size>(size), /*device_scale_factor=*/1.f,
        layer_tree_view->layer_tree_host()
            ->local_surface_id_allocation_from_parent());
  }

  std::string RegisterMockedHttpURLLoad(const std::string& file_name) {
    return url_test_helpers::RegisterMockedURLLoadFromBase(
               WebString::FromUTF8(base_url_), test::CoreTestDataPath(),
               WebString::FromUTF8(file_name))
        .GetString()
        .Utf8();
  }

  void TestAutoResize(const WebSize& min_auto_resize,
                      const WebSize& max_auto_resize,
                      const std::string& page_width,
                      const std::string& page_height,
                      int expected_width,
                      int expected_height,
                      HorizontalScrollbarState expected_horizontal_state,
                      VerticalScrollbarState expected_vertical_state);

  void TestTextInputType(WebTextInputType expected_type,
                         const std::string& html_file);
  void TestInputMode(WebTextInputMode expected_input_mode,
                     const std::string& html_file);
  bool TapElement(WebInputEvent::Type, Element*);
  bool TapElementById(WebInputEvent::Type, const WebString& id);
  IntSize PrintICBSizeFromPageSize(const FloatSize& page_size);

  ExternalDateTimeChooser* GetExternalDateTimeChooser(
      WebViewImpl* web_view_impl);

  void UpdateAllLifecyclePhases() {
    web_view_helper_.GetWebView()->MainFrameWidget()->UpdateAllLifecyclePhases(
        WebWidget::LifecycleUpdateReason::kTest);
  }

  std::string base_url_;
  frame_test_helpers::WebViewHelper web_view_helper_;
};

static bool HitTestIsContentEditable(WebView* view, int x, int y) {
  gfx::Point hit_point(x, y);
  WebHitTestResult hit_test_result =
      view->MainFrameWidget()->HitTestResultAt(hit_point);
  return hit_test_result.IsContentEditable();
}

static std::string HitTestElementId(WebView* view, int x, int y) {
  gfx::Point hit_point(x, y);
  WebHitTestResult hit_test_result =
      view->MainFrameWidget()->HitTestResultAt(hit_point);
  return hit_test_result.GetNode().To<WebElement>().GetAttribute("id").Utf8();
}

TEST_F(WebViewTest, HitTestVideo) {
  // Test that hit tests on parts of a video element result in hits on the video
  // element itself as opposed to its child elements.
  std::string url = RegisterMockedHttpURLLoad("video_200x200.html");
  WebView* web_view = web_view_helper_.InitializeAndLoad(url);
  web_view->MainFrameWidget()->Resize(WebSize(200, 200));

  // Center of video.
  EXPECT_EQ("video", HitTestElementId(web_view, 100, 100));

  // Play button.
  EXPECT_EQ("video", HitTestElementId(web_view, 10, 195));

  // Timeline bar.
  EXPECT_EQ("video", HitTestElementId(web_view, 100, 195));
}

TEST_F(WebViewTest, HitTestContentEditableImageMaps) {
  std::string url =
      RegisterMockedHttpURLLoad("content-editable-image-maps.html");
  WebView* web_view = web_view_helper_.InitializeAndLoad(url);
  web_view->MainFrameWidget()->Resize(WebSize(500, 500));

  EXPECT_EQ("areaANotEditable", HitTestElementId(web_view, 25, 25));
  EXPECT_FALSE(HitTestIsContentEditable(web_view, 25, 25));
  EXPECT_EQ("imageANotEditable", HitTestElementId(web_view, 75, 25));
  EXPECT_FALSE(HitTestIsContentEditable(web_view, 75, 25));

  EXPECT_EQ("areaBNotEditable", HitTestElementId(web_view, 25, 125));
  EXPECT_FALSE(HitTestIsContentEditable(web_view, 25, 125));
  EXPECT_EQ("imageBEditable", HitTestElementId(web_view, 75, 125));
  EXPECT_TRUE(HitTestIsContentEditable(web_view, 75, 125));

  EXPECT_EQ("areaCNotEditable", HitTestElementId(web_view, 25, 225));
  EXPECT_FALSE(HitTestIsContentEditable(web_view, 25, 225));
  EXPECT_EQ("imageCNotEditable", HitTestElementId(web_view, 75, 225));
  EXPECT_FALSE(HitTestIsContentEditable(web_view, 75, 225));

  EXPECT_EQ("areaDEditable", HitTestElementId(web_view, 25, 325));
  EXPECT_TRUE(HitTestIsContentEditable(web_view, 25, 325));
  EXPECT_EQ("imageDNotEditable", HitTestElementId(web_view, 75, 325));
  EXPECT_FALSE(HitTestIsContentEditable(web_view, 75, 325));
}

static std::string HitTestAbsoluteUrl(WebView* view, int x, int y) {
  gfx::Point hit_point(x, y);
  WebHitTestResult hit_test_result =
      view->MainFrameWidget()->HitTestResultAt(hit_point);
  return hit_test_result.AbsoluteImageURL().GetString().Utf8();
}

static WebElement HitTestUrlElement(WebView* view, int x, int y) {
  gfx::Point hit_point(x, y);
  WebHitTestResult hit_test_result =
      view->MainFrameWidget()->HitTestResultAt(hit_point);
  return hit_test_result.UrlElement();
}

TEST_F(WebViewTest, ImageMapUrls) {
  std::string url = RegisterMockedHttpURLLoad("image-map.html");
  WebView* web_view = web_view_helper_.InitializeAndLoad(url);
  web_view->MainFrameWidget()->Resize(WebSize(400, 400));

  std::string image_url =
      "data:image/gif;base64,R0lGODlhAQABAIAAAAUEBAAAACwAAAAAAQABAAACAkQBADs=";

  EXPECT_EQ("area", HitTestElementId(web_view, 25, 25));
  EXPECT_EQ("area",
            HitTestUrlElement(web_view, 25, 25).GetAttribute("id").Utf8());
  EXPECT_EQ(image_url, HitTestAbsoluteUrl(web_view, 25, 25));

  EXPECT_EQ("image", HitTestElementId(web_view, 75, 25));
  EXPECT_TRUE(HitTestUrlElement(web_view, 75, 25).IsNull());
  EXPECT_EQ(image_url, HitTestAbsoluteUrl(web_view, 75, 25));
}

TEST_F(WebViewTest, BrokenImage) {
  url_test_helpers::RegisterMockedErrorURLLoad(
      KURL(ToKURL(base_url_), "non_existent.png"));
  std::string url = RegisterMockedHttpURLLoad("image-broken.html");

  WebViewImpl* web_view = web_view_helper_.Initialize();
  web_view->GetSettings()->SetLoadsImagesAutomatically(true);
  LoadFrame(web_view->MainFrameImpl(), url);
  web_view->MainFrameWidget()->Resize(WebSize(400, 400));

  std::string image_url = "http://www.test.com/non_existent.png";

  EXPECT_EQ("image", HitTestElementId(web_view, 25, 25));
  EXPECT_TRUE(HitTestUrlElement(web_view, 25, 25).IsNull());
  EXPECT_EQ(image_url, HitTestAbsoluteUrl(web_view, 25, 25));
}

TEST_F(WebViewTest, BrokenInputImage) {
  url_test_helpers::RegisterMockedErrorURLLoad(
      KURL(ToKURL(base_url_), "non_existent.png"));
  std::string url = RegisterMockedHttpURLLoad("input-image-broken.html");

  WebViewImpl* web_view = web_view_helper_.Initialize();
  web_view->GetSettings()->SetLoadsImagesAutomatically(true);
  LoadFrame(web_view->MainFrameImpl(), url);
  web_view->MainFrameWidget()->Resize(WebSize(400, 400));

  std::string image_url = "http://www.test.com/non_existent.png";

  EXPECT_EQ("image", HitTestElementId(web_view, 25, 25));
  EXPECT_TRUE(HitTestUrlElement(web_view, 25, 25).IsNull());
  EXPECT_EQ(image_url, HitTestAbsoluteUrl(web_view, 25, 25));
}

TEST_F(WebViewTest, SetBaseBackgroundColor) {
  const SkColor kDarkCyan = SkColorSetARGB(0xFF, 0x22, 0x77, 0x88);
  const SkColor kTranslucentPutty = SkColorSetARGB(0x80, 0xBF, 0xB1, 0x96);

  WebViewImpl* web_view = web_view_helper_.Initialize();
  EXPECT_EQ(SK_ColorWHITE, web_view->BackgroundColor());

  web_view->SetBaseBackgroundColor(SK_ColorBLUE);
  EXPECT_EQ(SK_ColorBLUE, web_view->BackgroundColor());

  WebURL base_url = url_test_helpers::ToKURL("http://example.com/");
  frame_test_helpers::LoadHTMLString(
      web_view->MainFrameImpl(),
      "<html><head><style>body "
      "{background-color:#227788}</style></head></"
      "html>",
      base_url);
  EXPECT_EQ(kDarkCyan, web_view->BackgroundColor());

  frame_test_helpers::LoadHTMLString(web_view->MainFrameImpl(),
                                     "<html><head><style>body "
                                     "{background-color:rgba(255,0,0,0.5)}</"
                                     "style></head></html>",
                                     base_url);
  // Expected: red (50% alpha) blended atop base of SK_ColorBLUE.
  EXPECT_EQ(0xFF80007F, web_view->BackgroundColor());

  web_view->SetBaseBackgroundColor(kTranslucentPutty);
  // Expected: red (50% alpha) blended atop kTranslucentPutty. Note the alpha.
  EXPECT_EQ(0xBFE93A31, web_view->BackgroundColor());

  web_view->SetBaseBackgroundColor(SK_ColorTRANSPARENT);
  frame_test_helpers::LoadHTMLString(web_view->MainFrameImpl(),
                                     "<html><head><style>body "
                                     "{background-color:transparent}</style></"
                                     "head></html>",
                                     base_url);
  // Expected: transparent on top of transparent will still be transparent.
  EXPECT_EQ(SK_ColorTRANSPARENT, web_view->BackgroundColor());

  LocalFrame* frame = web_view->MainFrameImpl()->GetFrame();
  // The shutdown() calls are a hack to prevent this test
  // from violating invariants about frame state during navigation/detach.
  frame->GetDocument()->Shutdown();

  // Creating a new frame view with the background color having 0 alpha.
  frame->CreateView(IntSize(1024, 768), Color::kTransparent);
  EXPECT_EQ(SK_ColorTRANSPARENT, frame->View()->BaseBackgroundColor());
  frame->View()->Dispose();

  const Color transparent_red(100, 0, 0, 0);
  frame->CreateView(IntSize(1024, 768), transparent_red);
  EXPECT_EQ(transparent_red, frame->View()->BaseBackgroundColor());
  frame->View()->Dispose();
}

TEST_F(WebViewTest, SetBaseBackgroundColorBeforeMainFrame) {
  // Note: this test doesn't use WebViewHelper since it intentionally runs
  // initialization code between WebView and WebLocalFrame creation.
  frame_test_helpers::TestWebViewClient web_view_client;
  frame_test_helpers::TestWebWidgetClient web_widget_client;
  WebViewImpl* web_view = static_cast<WebViewImpl*>(
      WebView::Create(&web_view_client,
                      /*is_hidden=*/false,
                      /*compositing_enabled=*/true, nullptr));
  EXPECT_NE(SK_ColorBLUE, web_view->BackgroundColor());
  // WebView does not have a frame yet, but we should still be able to set the
  // background color.
  web_view->SetBaseBackgroundColor(SK_ColorBLUE);
  EXPECT_EQ(SK_ColorBLUE, web_view->BackgroundColor());

  frame_test_helpers::TestWebFrameClient web_frame_client;
  mojom::blink::DocumentInterfaceBrokerPtrInfo document_interface_broker;
  WebLocalFrame* frame = WebLocalFrame::CreateMainFrame(
      web_view, &web_frame_client, nullptr,
      mojo::MakeRequest(&document_interface_broker).PassMessagePipe(), nullptr);
  web_frame_client.Bind(frame);

  // We inform the WebView when it has a local main frame attached once the
  // WebFrame it fully set up and the WebWidgetClient is initialized (which is
  // the case by this point).
  web_view->DidAttachLocalMainFrame(&web_widget_client);

  // The color should be passed to the compositor.
  cc::LayerTreeHost* host = web_widget_client.layer_tree_host();
  EXPECT_EQ(SK_ColorBLUE, web_view->BackgroundColor());
  EXPECT_EQ(SK_ColorBLUE, host->background_color());

  // This closes the WebView also.
  web_view->MainFrameWidget()->Close();
}

TEST_F(WebViewTest, SetBaseBackgroundColorAndBlendWithExistingContent) {
  const SkColor kAlphaRed = SkColorSetARGB(0x80, 0xFF, 0x00, 0x00);
  const SkColor kAlphaGreen = SkColorSetARGB(0x80, 0x00, 0xFF, 0x00);
  const int kWidth = 100;
  const int kHeight = 100;

  WebViewImpl* web_view = web_view_helper_.Initialize();

  // Set WebView background to green with alpha.
  web_view->SetBaseBackgroundColor(kAlphaGreen);
  web_view->GetSettings()->SetShouldClearDocumentBackground(false);
  web_view->MainFrameWidget()->Resize(WebSize(kWidth, kHeight));
  UpdateAllLifecyclePhases();

  // Set canvas background to red with alpha.
  SkBitmap bitmap;
  bitmap.allocN32Pixels(kWidth, kHeight);
  SkCanvas canvas(bitmap);
  canvas.clear(kAlphaRed);

  PaintRecordBuilder builder;

  // Paint the root of the main frame in the way that CompositedLayerMapping
  // would.
  LocalFrameView* view = web_view_helper_.LocalMainFrame()->GetFrameView();
  PaintLayer* root_layer = view->GetLayoutView()->Layer();
  CullRect paint_rect(IntRect(0, 0, kWidth, kHeight));
  PaintLayerPaintingInfo painting_info(root_layer, paint_rect,
                                       kGlobalPaintNormalPhase, LayoutSize());

  view->GetLayoutView()->GetDocument().Lifecycle().AdvanceTo(
      DocumentLifecycle::kInPaint);
  PaintLayerPainter(*root_layer)
      .PaintLayerContents(builder.Context(), painting_info,
                          kPaintLayerPaintingCompositingAllPhases);
  view->GetLayoutView()->GetDocument().Lifecycle().AdvanceTo(
      DocumentLifecycle::kPaintClean);
  builder.EndRecording()->Playback(&canvas);

  // The result should be a blend of red and green.
  SkColor color = bitmap.getColor(kWidth / 2, kHeight / 2);
  EXPECT_TRUE(RedChannel(color));
  EXPECT_TRUE(GreenChannel(color));
}

TEST_F(WebViewTest, FocusIsInactive) {
  RegisterMockedHttpURLLoad("visible_iframe.html");
  WebViewImpl* web_view =
      web_view_helper_.InitializeAndLoad(base_url_ + "visible_iframe.html");

  web_view->MainFrameWidget()->SetFocus(true);
  web_view->SetIsActive(true);
  WebLocalFrameImpl* frame = web_view->MainFrameImpl();
  EXPECT_TRUE(frame->GetFrame()->GetDocument()->IsHTMLDocument());

  Document* document = frame->GetFrame()->GetDocument();
  EXPECT_TRUE(document->hasFocus());
  web_view->MainFrameWidget()->SetFocus(false);
  web_view->SetIsActive(false);
  EXPECT_FALSE(document->hasFocus());
  web_view->MainFrameWidget()->SetFocus(true);
  web_view->SetIsActive(true);
  EXPECT_TRUE(document->hasFocus());
  web_view->MainFrameWidget()->SetFocus(true);
  web_view->SetIsActive(false);
  EXPECT_FALSE(document->hasFocus());
  web_view->MainFrameWidget()->SetFocus(false);
  web_view->SetIsActive(true);
  EXPECT_FALSE(document->hasFocus());
  web_view->SetIsActive(false);
  web_view->MainFrameWidget()->SetFocus(true);
  EXPECT_TRUE(document->hasFocus());
  web_view->SetIsActive(true);
  web_view->MainFrameWidget()->SetFocus(false);
  EXPECT_FALSE(document->hasFocus());
}

TEST_F(WebViewTest, DocumentHasFocus) {
  WebViewImpl* web_view = web_view_helper_.Initialize();
  web_view->MainFrameWidget()->SetFocus(true);

  WebURL base_url = url_test_helpers::ToKURL("http://example.com/");
  frame_test_helpers::LoadHTMLString(
      web_view->MainFrameImpl(),
      "<input id=input></input>"
      "<div id=log></div>"
      "<script>"
      "  document.getElementById('input').addEventListener('focus', () => {"
      "    document.getElementById('log').textContent = 'document.hasFocus(): "
      "' + document.hasFocus();"
      "  });"
      "  document.getElementById('input').addEventListener('blur', () => {"
      "    document.getElementById('log').textContent = '';"
      "  });"
      "  document.getElementById('input').focus();"
      "</script>",
      base_url);

  WebLocalFrameImpl* frame = web_view->MainFrameImpl();
  Document* document = frame->GetFrame()->GetDocument();
  WebElement log_element = frame->GetDocument().GetElementById("log");
  EXPECT_TRUE(document->hasFocus());
  EXPECT_STREQ("document.hasFocus(): true",
               log_element.TextContent().Utf8().data());

  web_view->SetIsActive(false);
  web_view->MainFrameWidget()->SetFocus(false);
  EXPECT_FALSE(document->hasFocus());
  EXPECT_TRUE(log_element.TextContent().IsEmpty());

  web_view->MainFrameWidget()->SetFocus(true);
  EXPECT_TRUE(document->hasFocus());
  EXPECT_STREQ("document.hasFocus(): true",
               log_element.TextContent().Utf8().data());
}

TEST_F(WebViewTest, ActiveState) {
  RegisterMockedHttpURLLoad("visible_iframe.html");
  WebView* web_view =
      web_view_helper_.InitializeAndLoad(base_url_ + "visible_iframe.html");

  ASSERT_TRUE(web_view);

  web_view->SetIsActive(true);
  EXPECT_TRUE(web_view->IsActive());

  web_view->SetIsActive(false);
  EXPECT_FALSE(web_view->IsActive());

  web_view->SetIsActive(true);
  EXPECT_TRUE(web_view->IsActive());
}

TEST_F(WebViewTest, HitTestResultAtWithPageScale) {
  std::string url = base_url_ + "specify_size.html?" + "50px" + ":" + "50px";
  url_test_helpers::RegisterMockedURLLoad(
      ToKURL(url), test::CoreTestDataPath("specify_size.html"));
  WebView* web_view = web_view_helper_.InitializeAndLoad(url);
  web_view->MainFrameWidget()->Resize(WebSize(100, 100));
  gfx::Point hit_point(75, 75);

  // Image is at top left quandrant, so should not hit it.
  WebHitTestResult negative_result =
      web_view->MainFrameWidget()->HitTestResultAt(hit_point);
  EXPECT_FALSE(
      negative_result.GetNode().To<WebElement>().HasHTMLTagName("img"));
  negative_result.Reset();

  // Scale page up 2x so image should occupy the whole viewport.
  web_view->SetPageScaleFactor(2.0f);
  WebHitTestResult positive_result =
      web_view->MainFrameWidget()->HitTestResultAt(hit_point);
  EXPECT_TRUE(positive_result.GetNode().To<WebElement>().HasHTMLTagName("img"));
  positive_result.Reset();
}

TEST_F(WebViewTest, HitTestResultAtWithPageScaleAndPan) {
  std::string url = base_url_ + "specify_size.html?" + "50px" + ":" + "50px";
  url_test_helpers::RegisterMockedURLLoad(
      ToKURL(url), test::CoreTestDataPath("specify_size.html"));
  WebViewImpl* web_view = web_view_helper_.Initialize();
  LoadFrame(web_view->MainFrameImpl(), url);
  web_view->MainFrameWidget()->Resize(WebSize(100, 100));
  gfx::Point hit_point(75, 75);

  // Image is at top left quandrant, so should not hit it.
  WebHitTestResult negative_result = web_view->HitTestResultAt(hit_point);
  EXPECT_FALSE(
      negative_result.GetNode().To<WebElement>().HasHTMLTagName("img"));
  negative_result.Reset();

  // Scale page up 2x so image should occupy the whole viewport.
  web_view->SetPageScaleFactor(2.0f);
  WebHitTestResult positive_result = web_view->HitTestResultAt(hit_point);
  EXPECT_TRUE(positive_result.GetNode().To<WebElement>().HasHTMLTagName("img"));
  positive_result.Reset();

  // Pan around the zoomed in page so the image is not visible in viewport.
  web_view->SetVisualViewportOffset(WebFloatPoint(100, 100));
  WebHitTestResult negative_result2 = web_view->HitTestResultAt(hit_point);
  EXPECT_FALSE(
      negative_result2.GetNode().To<WebElement>().HasHTMLTagName("img"));
  negative_result2.Reset();
}

TEST_F(WebViewTest, HitTestResultForTapWithTapArea) {
  std::string url = RegisterMockedHttpURLLoad("hit_test.html");
  WebView* web_view = web_view_helper_.InitializeAndLoad(url);
  web_view->MainFrameWidget()->Resize(WebSize(100, 100));
  gfx::Point hit_point(55, 55);

  // Image is at top left quandrant, so should not hit it.
  WebHitTestResult negative_result =
      web_view->MainFrameWidget()->HitTestResultAt(hit_point);
  EXPECT_FALSE(
      negative_result.GetNode().To<WebElement>().HasHTMLTagName("img"));
  negative_result.Reset();

  // The tap area is 20 by 20 square, centered at 55, 55.
  WebSize tap_area(20, 20);
  WebHitTestResult positive_result =
      web_view->HitTestResultForTap(hit_point, tap_area);
  EXPECT_TRUE(positive_result.GetNode().To<WebElement>().HasHTMLTagName("img"));
  positive_result.Reset();

  // Move the hit point the image is just outside the tapped area now.
  hit_point = gfx::Point(61, 61);
  WebHitTestResult negative_result2 =
      web_view->HitTestResultForTap(hit_point, tap_area);
  EXPECT_FALSE(
      negative_result2.GetNode().To<WebElement>().HasHTMLTagName("img"));
  negative_result2.Reset();
}

TEST_F(WebViewTest, HitTestResultForTapWithTapAreaPageScaleAndPan) {
  std::string url = RegisterMockedHttpURLLoad("hit_test.html");
  WebViewImpl* web_view = web_view_helper_.Initialize();
  LoadFrame(web_view->MainFrameImpl(), url);
  web_view->MainFrameWidget()->Resize(WebSize(100, 100));
  gfx::Point hit_point(55, 55);

  // Image is at top left quandrant, so should not hit it.
  WebHitTestResult negative_result = web_view->HitTestResultAt(hit_point);
  EXPECT_FALSE(
      negative_result.GetNode().To<WebElement>().HasHTMLTagName("img"));
  negative_result.Reset();

  // The tap area is 20 by 20 square, centered at 55, 55.
  WebSize tap_area(20, 20);
  WebHitTestResult positive_result =
      web_view->HitTestResultForTap(hit_point, tap_area);
  EXPECT_TRUE(positive_result.GetNode().To<WebElement>().HasHTMLTagName("img"));
  positive_result.Reset();

  // Zoom in and pan around the page so the image is not visible in viewport.
  web_view->SetPageScaleFactor(2.0f);
  web_view->SetVisualViewportOffset(WebFloatPoint(100, 100));
  WebHitTestResult negative_result2 =
      web_view->HitTestResultForTap(hit_point, tap_area);
  EXPECT_FALSE(
      negative_result2.GetNode().To<WebElement>().HasHTMLTagName("img"));
  negative_result2.Reset();
}

void WebViewTest::TestAutoResize(
    const WebSize& min_auto_resize,
    const WebSize& max_auto_resize,
    const std::string& page_width,
    const std::string& page_height,
    int expected_width,
    int expected_height,
    HorizontalScrollbarState expected_horizontal_state,
    VerticalScrollbarState expected_vertical_state) {
  AutoResizeWebViewClient client;
  std::string url =
      base_url_ + "specify_size.html?" + page_width + ":" + page_height;
  url_test_helpers::RegisterMockedURLLoad(
      ToKURL(url), test::CoreTestDataPath("specify_size.html"));
  WebViewImpl* web_view =
      web_view_helper_.InitializeAndLoad(url, nullptr, &client);
  client.GetTestData().SetWebView(web_view);

  WebLocalFrameImpl* frame = web_view->MainFrameImpl();
  LocalFrameView* frame_view = frame->GetFrame()->View();
  frame_view->UpdateLayout();
  EXPECT_FALSE(frame_view->LayoutPending());
  EXPECT_FALSE(frame_view->NeedsLayout());

  web_view->EnableAutoResizeMode(min_auto_resize, max_auto_resize);
  EXPECT_TRUE(frame_view->LayoutPending());
  EXPECT_TRUE(frame_view->NeedsLayout());
  frame_view->UpdateLayout();

  EXPECT_TRUE(frame->GetFrame()->GetDocument()->IsHTMLDocument());

  EXPECT_EQ(expected_width, client.GetTestData().Width());
  EXPECT_EQ(expected_height, client.GetTestData().Height());

// Android disables main frame scrollbars.
#if !defined(OS_ANDROID)
  EXPECT_EQ(expected_horizontal_state,
            client.GetTestData().GetHorizontalScrollbarState());
  EXPECT_EQ(expected_vertical_state,
            client.GetTestData().GetVerticalScrollbarState());
#endif

  // Explicitly reset to break dependency on locally scoped client.
  web_view_helper_.Reset();
}

TEST_F(WebViewTest, AutoResizeMinimumSize) {
  WebSize min_auto_resize(91, 56);
  WebSize max_auto_resize(403, 302);
  std::string page_width = "91px";
  std::string page_height = "56px";
  int expected_width = 91;
  int expected_height = 56;
  TestAutoResize(min_auto_resize, max_auto_resize, page_width, page_height,
                 expected_width, expected_height, kNoHorizontalScrollbar,
                 kNoVerticalScrollbar);
}

TEST_F(WebViewTest, AutoResizeHeightOverflowAndFixedWidth) {
  WebSize min_auto_resize(90, 95);
  WebSize max_auto_resize(90, 100);
  std::string page_width = "60px";
  std::string page_height = "200px";
  int expected_width = 90;
  int expected_height = 100;
  TestAutoResize(min_auto_resize, max_auto_resize, page_width, page_height,
                 expected_width, expected_height, kNoHorizontalScrollbar,
                 kVisibleVerticalScrollbar);
}

TEST_F(WebViewTest, AutoResizeFixedHeightAndWidthOverflow) {
  WebSize min_auto_resize(90, 100);
  WebSize max_auto_resize(200, 100);
  std::string page_width = "300px";
  std::string page_height = "80px";
  int expected_width = 200;
  int expected_height = 100;
  TestAutoResize(min_auto_resize, max_auto_resize, page_width, page_height,
                 expected_width, expected_height, kVisibleHorizontalScrollbar,
                 kNoVerticalScrollbar);
}

// Next three tests disabled for https://bugs.webkit.org/show_bug.cgi?id=92318 .
// It seems we can run three AutoResize tests, then the next one breaks.
TEST_F(WebViewTest, AutoResizeInBetweenSizes) {
  WebSize min_auto_resize(90, 95);
  WebSize max_auto_resize(200, 300);
  std::string page_width = "100px";
  std::string page_height = "200px";
  int expected_width = 100;
  int expected_height = 200;
  TestAutoResize(min_auto_resize, max_auto_resize, page_width, page_height,
                 expected_width, expected_height, kNoHorizontalScrollbar,
                 kNoVerticalScrollbar);
}

TEST_F(WebViewTest, AutoResizeOverflowSizes) {
  WebSize min_auto_resize(90, 95);
  WebSize max_auto_resize(200, 300);
  std::string page_width = "300px";
  std::string page_height = "400px";
  int expected_width = 200;
  int expected_height = 300;
  TestAutoResize(min_auto_resize, max_auto_resize, page_width, page_height,
                 expected_width, expected_height, kVisibleHorizontalScrollbar,
                 kVisibleVerticalScrollbar);
}

TEST_F(WebViewTest, AutoResizeMaxSize) {
  WebSize min_auto_resize(90, 95);
  WebSize max_auto_resize(200, 300);
  std::string page_width = "200px";
  std::string page_height = "300px";
  int expected_width = 200;
  int expected_height = 300;
  TestAutoResize(min_auto_resize, max_auto_resize, page_width, page_height,
                 expected_width, expected_height, kNoHorizontalScrollbar,
                 kNoVerticalScrollbar);
}

void WebViewTest::TestTextInputType(WebTextInputType expected_type,
                                    const std::string& html_file) {
  RegisterMockedHttpURLLoad(html_file);
  WebViewImpl* web_view =
      web_view_helper_.InitializeAndLoad(base_url_ + html_file);
  WebInputMethodController* controller =
      web_view->MainFrameImpl()->GetInputMethodController();
  EXPECT_EQ(kWebTextInputTypeNone, controller->TextInputType());
  EXPECT_EQ(kWebTextInputTypeNone, controller->TextInputInfo().type);
  web_view->SetInitialFocus(false);
  EXPECT_EQ(expected_type, controller->TextInputType());
  EXPECT_EQ(expected_type, controller->TextInputInfo().type);
  web_view->ClearFocusedElement();
  EXPECT_EQ(kWebTextInputTypeNone, controller->TextInputType());
  EXPECT_EQ(kWebTextInputTypeNone, controller->TextInputInfo().type);
}

TEST_F(WebViewTest, TextInputType) {
  TestTextInputType(kWebTextInputTypeText, "input_field_default.html");
  TestTextInputType(kWebTextInputTypePassword, "input_field_password.html");
  TestTextInputType(kWebTextInputTypeEmail, "input_field_email.html");
  TestTextInputType(kWebTextInputTypeSearch, "input_field_search.html");
  TestTextInputType(kWebTextInputTypeNumber, "input_field_number.html");
  TestTextInputType(kWebTextInputTypeTelephone, "input_field_tel.html");
  TestTextInputType(kWebTextInputTypeURL, "input_field_url.html");
}

TEST_F(WebViewTest, TextInputInfoUpdateStyleAndLayout) {
  frame_test_helpers::WebViewHelper web_view_helper;
  WebViewImpl* web_view_impl = web_view_helper.Initialize();

  WebURL base_url = url_test_helpers::ToKURL("http://example.com/");
  // Here, we need to construct a document that has a special property:
  // Adding id="foo" to the <path> element will trigger creation of an SVG
  // instance tree for the use <use> element.
  // This is significant, because SVG instance trees are actually created lazily
  // during Document::updateStyleAndLayout code, thus incrementing the DOM tree
  // version and freaking out the EphemeralRange (invalidating it).
  frame_test_helpers::LoadHTMLString(
      web_view_impl->MainFrameImpl(),
      "<svg height='100%' version='1.1' viewBox='0 0 14 14' width='100%'>"
      "<use xmlns:xlink='http://www.w3.org/1999/xlink' xlink:href='#foo'></use>"
      "<path d='M 100 100 L 300 100 L 200 300 z' fill='#000'></path>"
      "</svg>"
      "<input>",
      base_url);
  web_view_impl->SetInitialFocus(false);

  // Add id="foo" to <path>, thus triggering the condition described above.
  Document* document =
      web_view_impl->MainFrameImpl()->GetFrame()->GetDocument();
  document->body()
      ->QuerySelector("path", ASSERT_NO_EXCEPTION)
      ->SetIdAttribute("foo");

  // This should not DCHECK.
  EXPECT_EQ(kWebTextInputTypeText, web_view_impl->MainFrameImpl()
                                       ->GetInputMethodController()
                                       ->TextInputInfo()
                                       .type);
}

void WebViewTest::TestInputMode(WebTextInputMode expected_input_mode,
                                const std::string& html_file) {
  RegisterMockedHttpURLLoad(html_file);
  WebViewImpl* web_view_impl =
      web_view_helper_.InitializeAndLoad(base_url_ + html_file);
  web_view_impl->SetInitialFocus(false);
  EXPECT_EQ(expected_input_mode, web_view_impl->MainFrameImpl()
                                     ->GetInputMethodController()
                                     ->TextInputInfo()
                                     .input_mode);
}

TEST_F(WebViewTest, InputMode) {
  TestInputMode(WebTextInputMode::kWebTextInputModeDefault,
                "input_mode_default.html");
  TestInputMode(WebTextInputMode::kWebTextInputModeDefault,
                "input_mode_default_unknown.html");
  TestInputMode(WebTextInputMode::kWebTextInputModeNone,
                "input_mode_type_none.html");
  TestInputMode(WebTextInputMode::kWebTextInputModeText,
                "input_mode_type_text.html");
  TestInputMode(WebTextInputMode::kWebTextInputModeTel,
                "input_mode_type_tel.html");
  TestInputMode(WebTextInputMode::kWebTextInputModeUrl,
                "input_mode_type_url.html");
  TestInputMode(WebTextInputMode::kWebTextInputModeEmail,
                "input_mode_type_email.html");
  TestInputMode(WebTextInputMode::kWebTextInputModeNumeric,
                "input_mode_type_numeric.html");
  TestInputMode(WebTextInputMode::kWebTextInputModeDecimal,
                "input_mode_type_decimal.html");
  TestInputMode(WebTextInputMode::kWebTextInputModeSearch,
                "input_mode_type_search.html");
}

TEST_F(WebViewTest, TextInputInfoWithReplacedElements) {
  std::string url = RegisterMockedHttpURLLoad("div_with_image.html");
  url_test_helpers::RegisterMockedURLLoad(
      ToKURL("http://www.test.com/foo.png"),
      test::CoreTestDataPath("white-1x1.png"));
  WebViewImpl* web_view_impl = web_view_helper_.InitializeAndLoad(url);
  web_view_impl->SetInitialFocus(false);
  WebTextInputInfo info = web_view_impl->MainFrameImpl()
                              ->GetInputMethodController()
                              ->TextInputInfo();

  EXPECT_EQ("foo\xef\xbf\xbc", info.value.Utf8());
}

TEST_F(WebViewTest, SetEditableSelectionOffsetsAndTextInputInfo) {
  RegisterMockedHttpURLLoad("input_field_populated.html");
  WebViewImpl* web_view = web_view_helper_.InitializeAndLoad(
      base_url_ + "input_field_populated.html");
  web_view->SetInitialFocus(false);
  WebLocalFrameImpl* frame = web_view->MainFrameImpl();
  WebInputMethodController* active_input_method_controller =
      frame->GetInputMethodController();
  frame->SetEditableSelectionOffsets(5, 13);
  EXPECT_EQ("56789abc", frame->SelectionAsText());
  WebTextInputInfo info = active_input_method_controller->TextInputInfo();
  EXPECT_EQ("0123456789abcdefghijklmnopqrstuvwxyz", info.value);
  EXPECT_EQ(5, info.selection_start);
  EXPECT_EQ(13, info.selection_end);
  EXPECT_EQ(-1, info.composition_start);
  EXPECT_EQ(-1, info.composition_end);

  RegisterMockedHttpURLLoad("content_editable_populated.html");
  web_view = web_view_helper_.InitializeAndLoad(
      base_url_ + "content_editable_populated.html");
  web_view->SetInitialFocus(false);
  frame = web_view->MainFrameImpl();
  active_input_method_controller = frame->GetInputMethodController();
  frame->SetEditableSelectionOffsets(8, 19);
  EXPECT_EQ("89abcdefghi", frame->SelectionAsText());
  info = active_input_method_controller->TextInputInfo();
  EXPECT_EQ("0123456789abcdefghijklmnopqrstuvwxyz", info.value);
  EXPECT_EQ(8, info.selection_start);
  EXPECT_EQ(19, info.selection_end);
  EXPECT_EQ(-1, info.composition_start);
  EXPECT_EQ(-1, info.composition_end);
}

// Regression test for crbug.com/663645
TEST_F(WebViewTest, FinishComposingTextDoesNotAssert) {
  RegisterMockedHttpURLLoad("input_field_default.html");
  WebViewImpl* web_view = web_view_helper_.InitializeAndLoad(
      base_url_ + "input_field_default.html");
  web_view->SetInitialFocus(false);

  WebInputMethodController* active_input_method_controller =
      web_view->MainFrameImpl()
          ->FrameWidget()
          ->GetActiveWebInputMethodController();

  // The test requires non-empty composition.
  std::string composition_text("hello");
  WebVector<WebImeTextSpan> empty_ime_text_spans;
  active_input_method_controller->SetComposition(
      WebString::FromUTF8(composition_text.c_str()), empty_ime_text_spans,
      WebRange(), 5, 5);

  // Do arbitrary change to make layout dirty.
  Document& document = *web_view->MainFrameImpl()->GetFrame()->GetDocument();
  Element* br = document.CreateRawElement(html_names::kBrTag);
  document.body()->AppendChild(br);

  // Should not hit assertion when calling
  // WebInputMethodController::finishComposingText with non-empty composition
  // and dirty layout.
  active_input_method_controller->FinishComposingText(
      WebInputMethodController::kKeepSelection);
}

// Regression test for https://crbug.com/873999
TEST_F(WebViewTest, LongPressOutsideInputShouldNotSelectPlaceholderText) {
  RegisterMockedHttpURLLoad("input_placeholder.html");
  WebViewImpl* web_view =
      web_view_helper_.InitializeAndLoad(base_url_ + "input_placeholder.html");
  web_view->SetInitialFocus(false);
  web_view->MainFrameWidget()->Resize(WebSize(500, 300));
  UpdateAllLifecyclePhases();
  RunPendingTasks();

  WebString input_id = WebString::FromUTF8("input");

  // Focus in input.
  EXPECT_TRUE(TapElementById(WebInputEvent::kGestureTap, input_id));

  // Long press below input.
  WebGestureEvent event(WebInputEvent::kGestureLongPress,
                        WebInputEvent::kNoModifiers,
                        WebInputEvent::GetStaticTimeStampForTests(),
                        WebGestureDevice::kTouchscreen);
  event.SetPositionInWidget(WebFloatPoint(100, 150));
  EXPECT_EQ(WebInputEventResult::kHandledSystem,
            web_view->MainFrameWidget()->HandleInputEvent(
                WebCoalescedInputEvent(event)));
  EXPECT_TRUE(web_view->MainFrameImpl()->SelectionAsText().IsEmpty());
}

TEST_F(WebViewTest, FinishComposingTextCursorPositionChange) {
  RegisterMockedHttpURLLoad("input_field_populated.html");
  WebViewImpl* web_view = web_view_helper_.InitializeAndLoad(
      base_url_ + "input_field_populated.html");
  web_view->SetInitialFocus(false);

  // Set up a composition that needs to be committed.
  std::string composition_text("hello");

  WebInputMethodController* active_input_method_controller =
      web_view->MainFrameImpl()
          ->FrameWidget()
          ->GetActiveWebInputMethodController();
  WebVector<WebImeTextSpan> empty_ime_text_spans;
  active_input_method_controller->SetComposition(
      WebString::FromUTF8(composition_text.c_str()), empty_ime_text_spans,
      WebRange(), 3, 3);

  WebTextInputInfo info = active_input_method_controller->TextInputInfo();
  EXPECT_EQ("hello", std::string(info.value.Utf8().data()));
  EXPECT_EQ(3, info.selection_start);
  EXPECT_EQ(3, info.selection_end);
  EXPECT_EQ(0, info.composition_start);
  EXPECT_EQ(5, info.composition_end);

  active_input_method_controller->FinishComposingText(
      WebInputMethodController::kKeepSelection);
  info = active_input_method_controller->TextInputInfo();
  EXPECT_EQ(3, info.selection_start);
  EXPECT_EQ(3, info.selection_end);
  EXPECT_EQ(-1, info.composition_start);
  EXPECT_EQ(-1, info.composition_end);

  active_input_method_controller->SetComposition(
      WebString::FromUTF8(composition_text.c_str()), empty_ime_text_spans,
      WebRange(), 3, 3);
  info = active_input_method_controller->TextInputInfo();
  EXPECT_EQ("helhellolo", std::string(info.value.Utf8().data()));
  EXPECT_EQ(6, info.selection_start);
  EXPECT_EQ(6, info.selection_end);
  EXPECT_EQ(3, info.composition_start);
  EXPECT_EQ(8, info.composition_end);

  active_input_method_controller->FinishComposingText(
      WebInputMethodController::kDoNotKeepSelection);
  info = active_input_method_controller->TextInputInfo();
  EXPECT_EQ(8, info.selection_start);
  EXPECT_EQ(8, info.selection_end);
  EXPECT_EQ(-1, info.composition_start);
  EXPECT_EQ(-1, info.composition_end);
}

TEST_F(WebViewTest, SetCompositionForNewCaretPositions) {
  RegisterMockedHttpURLLoad("input_field_populated.html");
  WebViewImpl* web_view = web_view_helper_.InitializeAndLoad(
      base_url_ + "input_field_populated.html");
  web_view->SetInitialFocus(false);
  WebInputMethodController* active_input_method_controller =
      web_view->MainFrameImpl()
          ->FrameWidget()
          ->GetActiveWebInputMethodController();

  WebVector<WebImeTextSpan> empty_ime_text_spans;

  active_input_method_controller->CommitText("hello", empty_ime_text_spans,
                                             WebRange(), 0);
  active_input_method_controller->CommitText("world", empty_ime_text_spans,
                                             WebRange(), -5);
  WebTextInputInfo info = active_input_method_controller->TextInputInfo();

  EXPECT_EQ("helloworld", std::string(info.value.Utf8().data()));
  EXPECT_EQ(5, info.selection_start);
  EXPECT_EQ(5, info.selection_end);
  EXPECT_EQ(-1, info.composition_start);
  EXPECT_EQ(-1, info.composition_end);

  // Set up a composition that needs to be committed.
  std::string composition_text("ABC");

  // Caret is on the left of composing text.
  active_input_method_controller->SetComposition(
      WebString::FromUTF8(composition_text.c_str()), empty_ime_text_spans,
      WebRange(), 0, 0);
  info = active_input_method_controller->TextInputInfo();
  EXPECT_EQ("helloABCworld", std::string(info.value.Utf8().data()));
  EXPECT_EQ(5, info.selection_start);
  EXPECT_EQ(5, info.selection_end);
  EXPECT_EQ(5, info.composition_start);
  EXPECT_EQ(8, info.composition_end);

  // Caret is on the right of composing text.
  active_input_method_controller->SetComposition(
      WebString::FromUTF8(composition_text.c_str()), empty_ime_text_spans,
      WebRange(), 3, 3);
  info = active_input_method_controller->TextInputInfo();
  EXPECT_EQ("helloABCworld", std::string(info.value.Utf8().data()));
  EXPECT_EQ(8, info.selection_start);
  EXPECT_EQ(8, info.selection_end);
  EXPECT_EQ(5, info.composition_start);
  EXPECT_EQ(8, info.composition_end);

  // Caret is between composing text and left boundary.
  active_input_method_controller->SetComposition(
      WebString::FromUTF8(composition_text.c_str()), empty_ime_text_spans,
      WebRange(), -2, -2);
  info = active_input_method_controller->TextInputInfo();
  EXPECT_EQ("helloABCworld", std::string(info.value.Utf8().data()));
  EXPECT_EQ(3, info.selection_start);
  EXPECT_EQ(3, info.selection_end);
  EXPECT_EQ(5, info.composition_start);
  EXPECT_EQ(8, info.composition_end);

  // Caret is between composing text and right boundary.
  active_input_method_controller->SetComposition(
      WebString::FromUTF8(composition_text.c_str()), empty_ime_text_spans,
      WebRange(), 5, 5);
  info = active_input_method_controller->TextInputInfo();
  EXPECT_EQ("helloABCworld", std::string(info.value.Utf8().data()));
  EXPECT_EQ(10, info.selection_start);
  EXPECT_EQ(10, info.selection_end);
  EXPECT_EQ(5, info.composition_start);
  EXPECT_EQ(8, info.composition_end);

  // Caret is on the left boundary.
  active_input_method_controller->SetComposition(
      WebString::FromUTF8(composition_text.c_str()), empty_ime_text_spans,
      WebRange(), -5, -5);
  info = active_input_method_controller->TextInputInfo();
  EXPECT_EQ("helloABCworld", std::string(info.value.Utf8().data()));
  EXPECT_EQ(0, info.selection_start);
  EXPECT_EQ(0, info.selection_end);
  EXPECT_EQ(5, info.composition_start);
  EXPECT_EQ(8, info.composition_end);

  // Caret is on the right boundary.
  active_input_method_controller->SetComposition(
      WebString::FromUTF8(composition_text.c_str()), empty_ime_text_spans,
      WebRange(), 8, 8);
  info = active_input_method_controller->TextInputInfo();
  EXPECT_EQ("helloABCworld", std::string(info.value.Utf8().data()));
  EXPECT_EQ(13, info.selection_start);
  EXPECT_EQ(13, info.selection_end);
  EXPECT_EQ(5, info.composition_start);
  EXPECT_EQ(8, info.composition_end);

  // Caret exceeds the left boundary.
  active_input_method_controller->SetComposition(
      WebString::FromUTF8(composition_text.c_str()), empty_ime_text_spans,
      WebRange(), -100, -100);
  info = active_input_method_controller->TextInputInfo();
  EXPECT_EQ("helloABCworld", std::string(info.value.Utf8().data()));
  EXPECT_EQ(0, info.selection_start);
  EXPECT_EQ(0, info.selection_end);
  EXPECT_EQ(5, info.composition_start);
  EXPECT_EQ(8, info.composition_end);

  // Caret exceeds the right boundary.
  active_input_method_controller->SetComposition(
      WebString::FromUTF8(composition_text.c_str()), empty_ime_text_spans,
      WebRange(), 100, 100);
  info = active_input_method_controller->TextInputInfo();
  EXPECT_EQ("helloABCworld", std::string(info.value.Utf8().data()));
  EXPECT_EQ(13, info.selection_start);
  EXPECT_EQ(13, info.selection_end);
  EXPECT_EQ(5, info.composition_start);
  EXPECT_EQ(8, info.composition_end);
}

TEST_F(WebViewTest, SetCompositionWithEmptyText) {
  RegisterMockedHttpURLLoad("input_field_populated.html");
  WebViewImpl* web_view = web_view_helper_.InitializeAndLoad(
      base_url_ + "input_field_populated.html");
  web_view->SetInitialFocus(false);
  WebInputMethodController* active_input_method_controller =
      web_view->MainFrameImpl()
          ->FrameWidget()
          ->GetActiveWebInputMethodController();

  WebVector<WebImeTextSpan> empty_ime_text_spans;

  active_input_method_controller->CommitText("hello", empty_ime_text_spans,
                                             WebRange(), 0);
  WebTextInputInfo info = active_input_method_controller->TextInputInfo();

  EXPECT_EQ("hello", std::string(info.value.Utf8().data()));
  EXPECT_EQ(5, info.selection_start);
  EXPECT_EQ(5, info.selection_end);
  EXPECT_EQ(-1, info.composition_start);
  EXPECT_EQ(-1, info.composition_end);

  active_input_method_controller->SetComposition(
      WebString::FromUTF8(""), empty_ime_text_spans, WebRange(), 0, 0);
  info = active_input_method_controller->TextInputInfo();
  EXPECT_EQ("hello", std::string(info.value.Utf8().data()));
  EXPECT_EQ(5, info.selection_start);
  EXPECT_EQ(5, info.selection_end);
  EXPECT_EQ(-1, info.composition_start);
  EXPECT_EQ(-1, info.composition_end);

  active_input_method_controller->SetComposition(
      WebString::FromUTF8(""), empty_ime_text_spans, WebRange(), -2, -2);
  info = active_input_method_controller->TextInputInfo();
  EXPECT_EQ("hello", std::string(info.value.Utf8().data()));
  EXPECT_EQ(3, info.selection_start);
  EXPECT_EQ(3, info.selection_end);
  EXPECT_EQ(-1, info.composition_start);
  EXPECT_EQ(-1, info.composition_end);
}

TEST_F(WebViewTest, CommitTextForNewCaretPositions) {
  RegisterMockedHttpURLLoad("input_field_populated.html");
  WebViewImpl* web_view = web_view_helper_.InitializeAndLoad(
      base_url_ + "input_field_populated.html");
  web_view->SetInitialFocus(false);
  WebInputMethodController* active_input_method_controller =
      web_view->MainFrameImpl()
          ->FrameWidget()
          ->GetActiveWebInputMethodController();

  WebVector<WebImeTextSpan> empty_ime_text_spans;

  // Caret is on the left of composing text.
  active_input_method_controller->CommitText("ab", empty_ime_text_spans,
                                             WebRange(), -2);
  WebTextInputInfo info = active_input_method_controller->TextInputInfo();
  EXPECT_EQ("ab", std::string(info.value.Utf8().data()));
  EXPECT_EQ(0, info.selection_start);
  EXPECT_EQ(0, info.selection_end);
  EXPECT_EQ(-1, info.composition_start);
  EXPECT_EQ(-1, info.composition_end);

  // Caret is on the right of composing text.
  active_input_method_controller->CommitText("c", empty_ime_text_spans,
                                             WebRange(), 1);
  info = active_input_method_controller->TextInputInfo();
  EXPECT_EQ("cab", std::string(info.value.Utf8().data()));
  EXPECT_EQ(2, info.selection_start);
  EXPECT_EQ(2, info.selection_end);
  EXPECT_EQ(-1, info.composition_start);
  EXPECT_EQ(-1, info.composition_end);

  // Caret is on the left boundary.
  active_input_method_controller->CommitText("def", empty_ime_text_spans,
                                             WebRange(), -5);
  info = active_input_method_controller->TextInputInfo();
  EXPECT_EQ("cadefb", std::string(info.value.Utf8().data()));
  EXPECT_EQ(0, info.selection_start);
  EXPECT_EQ(0, info.selection_end);
  EXPECT_EQ(-1, info.composition_start);
  EXPECT_EQ(-1, info.composition_end);

  // Caret is on the right boundary.
  active_input_method_controller->CommitText("g", empty_ime_text_spans,
                                             WebRange(), 6);
  info = active_input_method_controller->TextInputInfo();
  EXPECT_EQ("gcadefb", std::string(info.value.Utf8().data()));
  EXPECT_EQ(7, info.selection_start);
  EXPECT_EQ(7, info.selection_end);
  EXPECT_EQ(-1, info.composition_start);
  EXPECT_EQ(-1, info.composition_end);

  // Caret exceeds the left boundary.
  active_input_method_controller->CommitText("hi", empty_ime_text_spans,
                                             WebRange(), -100);
  info = active_input_method_controller->TextInputInfo();
  EXPECT_EQ("gcadefbhi", std::string(info.value.Utf8().data()));
  EXPECT_EQ(0, info.selection_start);
  EXPECT_EQ(0, info.selection_end);
  EXPECT_EQ(-1, info.composition_start);
  EXPECT_EQ(-1, info.composition_end);

  // Caret exceeds the right boundary.
  active_input_method_controller->CommitText("jk", empty_ime_text_spans,
                                             WebRange(), 100);
  info = active_input_method_controller->TextInputInfo();
  EXPECT_EQ("jkgcadefbhi", std::string(info.value.Utf8().data()));
  EXPECT_EQ(11, info.selection_start);
  EXPECT_EQ(11, info.selection_end);
  EXPECT_EQ(-1, info.composition_start);
  EXPECT_EQ(-1, info.composition_end);
}

TEST_F(WebViewTest, CommitTextWhileComposing) {
  RegisterMockedHttpURLLoad("input_field_populated.html");
  WebViewImpl* web_view = web_view_helper_.InitializeAndLoad(
      base_url_ + "input_field_populated.html");
  web_view->SetInitialFocus(false);
  WebInputMethodController* active_input_method_controller =
      web_view->MainFrameImpl()
          ->FrameWidget()
          ->GetActiveWebInputMethodController();

  WebVector<WebImeTextSpan> empty_ime_text_spans;
  active_input_method_controller->SetComposition(
      WebString::FromUTF8("abc"), empty_ime_text_spans, WebRange(), 0, 0);
  WebTextInputInfo info = active_input_method_controller->TextInputInfo();
  EXPECT_EQ("abc", std::string(info.value.Utf8().data()));
  EXPECT_EQ(0, info.selection_start);
  EXPECT_EQ(0, info.selection_end);
  EXPECT_EQ(0, info.composition_start);
  EXPECT_EQ(3, info.composition_end);

  // Deletes ongoing composition, inserts the specified text and moves the
  // caret.
  active_input_method_controller->CommitText("hello", empty_ime_text_spans,
                                             WebRange(), -2);
  info = active_input_method_controller->TextInputInfo();
  EXPECT_EQ("hello", std::string(info.value.Utf8().data()));
  EXPECT_EQ(3, info.selection_start);
  EXPECT_EQ(3, info.selection_end);
  EXPECT_EQ(-1, info.composition_start);
  EXPECT_EQ(-1, info.composition_end);

  active_input_method_controller->SetComposition(
      WebString::FromUTF8("abc"), empty_ime_text_spans, WebRange(), 0, 0);
  info = active_input_method_controller->TextInputInfo();
  EXPECT_EQ("helabclo", std::string(info.value.Utf8().data()));
  EXPECT_EQ(3, info.selection_start);
  EXPECT_EQ(3, info.selection_end);
  EXPECT_EQ(3, info.composition_start);
  EXPECT_EQ(6, info.composition_end);

  // Deletes ongoing composition and moves the caret.
  active_input_method_controller->CommitText("", empty_ime_text_spans,
                                             WebRange(), 2);
  info = active_input_method_controller->TextInputInfo();
  EXPECT_EQ("hello", std::string(info.value.Utf8().data()));
  EXPECT_EQ(5, info.selection_start);
  EXPECT_EQ(5, info.selection_end);
  EXPECT_EQ(-1, info.composition_start);
  EXPECT_EQ(-1, info.composition_end);

  // Inserts the specified text and moves the caret.
  active_input_method_controller->CommitText("world", empty_ime_text_spans,
                                             WebRange(), -5);
  info = active_input_method_controller->TextInputInfo();
  EXPECT_EQ("helloworld", std::string(info.value.Utf8().data()));
  EXPECT_EQ(5, info.selection_start);
  EXPECT_EQ(5, info.selection_end);
  EXPECT_EQ(-1, info.composition_start);
  EXPECT_EQ(-1, info.composition_end);

  // Only moves the caret.
  active_input_method_controller->CommitText("", empty_ime_text_spans,
                                             WebRange(), 5);
  info = active_input_method_controller->TextInputInfo();
  EXPECT_EQ("helloworld", std::string(info.value.Utf8().data()));
  EXPECT_EQ(10, info.selection_start);
  EXPECT_EQ(10, info.selection_end);
  EXPECT_EQ(-1, info.composition_start);
  EXPECT_EQ(-1, info.composition_end);
}

TEST_F(WebViewTest, FinishCompositionDoesNotRevealSelection) {
  RegisterMockedHttpURLLoad("form_with_input.html");
  WebViewImpl* web_view =
      web_view_helper_.InitializeAndLoad(base_url_ + "form_with_input.html");
  web_view->MainFrameWidget()->Resize(WebSize(800, 600));
  web_view->SetInitialFocus(false);
  EXPECT_EQ(0, web_view->MainFrameImpl()->GetScrollOffset().width);
  EXPECT_EQ(0, web_view->MainFrameImpl()->GetScrollOffset().height);

  // Set up a composition from existing text that needs to be committed.
  Vector<ImeTextSpan> empty_ime_text_spans;
  WebLocalFrameImpl* frame = web_view->MainFrameImpl();
  frame->GetFrame()->GetInputMethodController().SetCompositionFromExistingText(
      empty_ime_text_spans, 0, 3);

  // Scroll the input field out of the viewport.
  Element* element = static_cast<Element*>(
      web_view->MainFrameImpl()->GetDocument().GetElementById("btn"));
  element->scrollIntoView();
  float offset_height = web_view->MainFrameImpl()->GetScrollOffset().height;
  EXPECT_EQ(0, web_view->MainFrameImpl()->GetScrollOffset().width);
  EXPECT_LT(0, offset_height);

  WebTextInputInfo info = frame->GetInputMethodController()->TextInputInfo();
  EXPECT_EQ("hello", std::string(info.value.Utf8().data()));

  // Verify that the input field is not scrolled back into the viewport.
  frame->FrameWidget()
      ->GetActiveWebInputMethodController()
      ->FinishComposingText(WebInputMethodController::kDoNotKeepSelection);
  EXPECT_EQ(0, web_view->MainFrameImpl()->GetScrollOffset().width);
  EXPECT_EQ(offset_height, web_view->MainFrameImpl()->GetScrollOffset().height);
}

TEST_F(WebViewTest, InsertNewLinePlacementAfterFinishComposingText) {
  RegisterMockedHttpURLLoad("text_area_populated.html");
  WebViewImpl* web_view = web_view_helper_.InitializeAndLoad(
      base_url_ + "text_area_populated.html");
  web_view->SetInitialFocus(false);

  WebVector<WebImeTextSpan> empty_ime_text_spans;

  WebLocalFrameImpl* frame = web_view->MainFrameImpl();
  WebInputMethodController* active_input_method_controller =
      frame->GetInputMethodController();
  frame->SetEditableSelectionOffsets(4, 4);
  frame->SetCompositionFromExistingText(8, 12, empty_ime_text_spans);

  WebTextInputInfo info = active_input_method_controller->TextInputInfo();
  EXPECT_EQ("0123456789abcdefghijklmnopqrstuvwxyz",
            std::string(info.value.Utf8().data()));
  EXPECT_EQ(4, info.selection_start);
  EXPECT_EQ(4, info.selection_end);
  EXPECT_EQ(8, info.composition_start);
  EXPECT_EQ(12, info.composition_end);

  active_input_method_controller->FinishComposingText(
      WebInputMethodController::kKeepSelection);
  info = active_input_method_controller->TextInputInfo();
  EXPECT_EQ(4, info.selection_start);
  EXPECT_EQ(4, info.selection_end);
  EXPECT_EQ(-1, info.composition_start);
  EXPECT_EQ(-1, info.composition_end);

  std::string composition_text("\n");
  active_input_method_controller->CommitText(
      WebString::FromUTF8(composition_text.c_str()), empty_ime_text_spans,
      WebRange(), 0);
  info = active_input_method_controller->TextInputInfo();
  EXPECT_EQ(5, info.selection_start);
  EXPECT_EQ(5, info.selection_end);
  EXPECT_EQ(-1, info.composition_start);
  EXPECT_EQ(-1, info.composition_end);
  EXPECT_EQ("0123\n456789abcdefghijklmnopqrstuvwxyz",
            std::string(info.value.Utf8().data()));
}

TEST_F(WebViewTest, ExtendSelectionAndDelete) {
  RegisterMockedHttpURLLoad("input_field_populated.html");
  WebViewImpl* web_view = web_view_helper_.InitializeAndLoad(
      base_url_ + "input_field_populated.html");
  WebLocalFrameImpl* frame = web_view->MainFrameImpl();
  web_view->SetInitialFocus(false);
  frame->SetEditableSelectionOffsets(10, 10);
  frame->ExtendSelectionAndDelete(5, 8);
  WebInputMethodController* active_input_method_controller =
      frame->GetInputMethodController();
  WebTextInputInfo info = active_input_method_controller->TextInputInfo();
  EXPECT_EQ("01234ijklmnopqrstuvwxyz", std::string(info.value.Utf8().data()));
  EXPECT_EQ(5, info.selection_start);
  EXPECT_EQ(5, info.selection_end);
  frame->ExtendSelectionAndDelete(10, 0);
  info = active_input_method_controller->TextInputInfo();
  EXPECT_EQ("ijklmnopqrstuvwxyz", std::string(info.value.Utf8().data()));
}

TEST_F(WebViewTest, DeleteSurroundingText) {
  RegisterMockedHttpURLLoad("input_field_populated.html");
  WebView* web_view = web_view_helper_.InitializeAndLoad(
      base_url_ + "input_field_populated.html");
  auto* frame = To<WebLocalFrameImpl>(web_view->MainFrame());
  WebInputMethodController* active_input_method_controller =
      frame->GetInputMethodController();
  web_view->SetInitialFocus(false);

  frame->SetEditableSelectionOffsets(10, 10);
  frame->DeleteSurroundingText(5, 8);
  WebTextInputInfo info = active_input_method_controller->TextInputInfo();
  EXPECT_EQ("01234ijklmnopqrstuvwxyz", std::string(info.value.Utf8().data()));
  EXPECT_EQ(5, info.selection_start);
  EXPECT_EQ(5, info.selection_end);

  frame->SetEditableSelectionOffsets(5, 10);
  frame->DeleteSurroundingText(3, 5);
  info = active_input_method_controller->TextInputInfo();
  EXPECT_EQ("01ijklmstuvwxyz", std::string(info.value.Utf8().data()));
  EXPECT_EQ(2, info.selection_start);
  EXPECT_EQ(7, info.selection_end);

  frame->SetEditableSelectionOffsets(5, 5);
  frame->DeleteSurroundingText(10, 0);
  info = active_input_method_controller->TextInputInfo();
  EXPECT_EQ("lmstuvwxyz", std::string(info.value.Utf8().data()));
  EXPECT_EQ(0, info.selection_start);
  EXPECT_EQ(0, info.selection_end);

  frame->DeleteSurroundingText(0, 20);
  info = active_input_method_controller->TextInputInfo();
  EXPECT_EQ("", std::string(info.value.Utf8().data()));
  EXPECT_EQ(0, info.selection_start);
  EXPECT_EQ(0, info.selection_end);

  frame->DeleteSurroundingText(10, 10);
  info = active_input_method_controller->TextInputInfo();
  EXPECT_EQ("", std::string(info.value.Utf8().data()));
  EXPECT_EQ(0, info.selection_start);
  EXPECT_EQ(0, info.selection_end);
}

TEST_F(WebViewTest, SetCompositionFromExistingText) {
  RegisterMockedHttpURLLoad("input_field_populated.html");
  WebViewImpl* web_view = web_view_helper_.InitializeAndLoad(
      base_url_ + "input_field_populated.html");
  web_view->SetInitialFocus(false);
  WebVector<WebImeTextSpan> ime_text_spans(static_cast<size_t>(1));
  ime_text_spans[0] = WebImeTextSpan(WebImeTextSpan::Type::kComposition, 0, 4,
                                     ui::mojom::ImeTextSpanThickness::kThin, 0);
  WebLocalFrameImpl* frame = web_view->MainFrameImpl();
  WebInputMethodController* active_input_method_controller =
      frame->GetInputMethodController();
  frame->SetEditableSelectionOffsets(4, 10);
  frame->SetCompositionFromExistingText(8, 12, ime_text_spans);
  WebTextInputInfo info = active_input_method_controller->TextInputInfo();
  EXPECT_EQ(4, info.selection_start);
  EXPECT_EQ(10, info.selection_end);
  EXPECT_EQ(8, info.composition_start);
  EXPECT_EQ(12, info.composition_end);
  WebVector<WebImeTextSpan> empty_ime_text_spans;
  frame->SetCompositionFromExistingText(0, 0, empty_ime_text_spans);
  info = active_input_method_controller->TextInputInfo();
  EXPECT_EQ(4, info.selection_start);
  EXPECT_EQ(10, info.selection_end);
  EXPECT_EQ(-1, info.composition_start);
  EXPECT_EQ(-1, info.composition_end);
}

TEST_F(WebViewTest, SetCompositionFromExistingTextInTextArea) {
  RegisterMockedHttpURLLoad("text_area_populated.html");
  WebViewImpl* web_view = web_view_helper_.InitializeAndLoad(
      base_url_ + "text_area_populated.html");
  web_view->SetInitialFocus(false);
  WebVector<WebImeTextSpan> ime_text_spans(static_cast<size_t>(1));
  ime_text_spans[0] = WebImeTextSpan(WebImeTextSpan::Type::kComposition, 0, 4,
                                     ui::mojom::ImeTextSpanThickness::kThin, 0);
  WebLocalFrameImpl* frame = web_view->MainFrameImpl();
  WebInputMethodController* active_input_method_controller =
      frame->FrameWidget()->GetActiveWebInputMethodController();
  frame->SetEditableSelectionOffsets(27, 27);
  std::string new_line_text("\n");
  WebVector<WebImeTextSpan> empty_ime_text_spans;
  active_input_method_controller->CommitText(
      WebString::FromUTF8(new_line_text.c_str()), empty_ime_text_spans,
      WebRange(), 0);
  WebTextInputInfo info = active_input_method_controller->TextInputInfo();
  EXPECT_EQ("0123456789abcdefghijklmnopq\nrstuvwxyz",
            std::string(info.value.Utf8().data()));

  frame->SetEditableSelectionOffsets(31, 31);
  frame->SetCompositionFromExistingText(30, 34, ime_text_spans);
  info = active_input_method_controller->TextInputInfo();
  EXPECT_EQ("0123456789abcdefghijklmnopq\nrstuvwxyz",
            std::string(info.value.Utf8().data()));
  EXPECT_EQ(31, info.selection_start);
  EXPECT_EQ(31, info.selection_end);
  EXPECT_EQ(30, info.composition_start);
  EXPECT_EQ(34, info.composition_end);

  std::string composition_text("yolo");
  active_input_method_controller->CommitText(
      WebString::FromUTF8(composition_text.c_str()), empty_ime_text_spans,
      WebRange(), 0);
  info = active_input_method_controller->TextInputInfo();
  EXPECT_EQ("0123456789abcdefghijklmnopq\nrsyoloxyz",
            std::string(info.value.Utf8().data()));
  EXPECT_EQ(34, info.selection_start);
  EXPECT_EQ(34, info.selection_end);
  EXPECT_EQ(-1, info.composition_start);
  EXPECT_EQ(-1, info.composition_end);
}

TEST_F(WebViewTest, SetCompositionFromExistingTextInRichText) {
  RegisterMockedHttpURLLoad("content_editable_rich_text.html");
  WebViewImpl* web_view = web_view_helper_.InitializeAndLoad(
      base_url_ + "content_editable_rich_text.html");
  web_view->SetInitialFocus(false);
  WebVector<WebImeTextSpan> ime_text_spans(static_cast<size_t>(1));
  ime_text_spans[0] = WebImeTextSpan(WebImeTextSpan::Type::kComposition, 0, 4,
                                     ui::mojom::ImeTextSpanThickness::kThin, 0);
  WebLocalFrameImpl* frame = web_view->MainFrameImpl();
  frame->SetEditableSelectionOffsets(1, 1);
  WebDocument document = web_view->MainFrameImpl()->GetDocument();
  EXPECT_FALSE(document.GetElementById("bold").IsNull());
  frame->SetCompositionFromExistingText(0, 4, ime_text_spans);
  EXPECT_FALSE(document.GetElementById("bold").IsNull());
}

TEST_F(WebViewTest, SetEditableSelectionOffsetsKeepsComposition) {
  RegisterMockedHttpURLLoad("input_field_populated.html");
  WebViewImpl* web_view = web_view_helper_.InitializeAndLoad(
      base_url_ + "input_field_populated.html");
  web_view->SetInitialFocus(false);

  std::string composition_text_first("hello ");
  std::string composition_text_second("world");
  WebVector<WebImeTextSpan> empty_ime_text_spans;
  WebInputMethodController* active_input_method_controller =
      web_view->MainFrameImpl()
          ->FrameWidget()
          ->GetActiveWebInputMethodController();
  active_input_method_controller->CommitText(
      WebString::FromUTF8(composition_text_first.c_str()), empty_ime_text_spans,
      WebRange(), 0);
  active_input_method_controller->SetComposition(
      WebString::FromUTF8(composition_text_second.c_str()),
      empty_ime_text_spans, WebRange(), 5, 5);

  WebTextInputInfo info = active_input_method_controller->TextInputInfo();
  EXPECT_EQ("hello world", std::string(info.value.Utf8().data()));
  EXPECT_EQ(11, info.selection_start);
  EXPECT_EQ(11, info.selection_end);
  EXPECT_EQ(6, info.composition_start);
  EXPECT_EQ(11, info.composition_end);

  WebLocalFrameImpl* frame = web_view->MainFrameImpl();
  frame->SetEditableSelectionOffsets(6, 6);
  info = active_input_method_controller->TextInputInfo();
  EXPECT_EQ("hello world", std::string(info.value.Utf8().data()));
  EXPECT_EQ(6, info.selection_start);
  EXPECT_EQ(6, info.selection_end);
  EXPECT_EQ(6, info.composition_start);
  EXPECT_EQ(11, info.composition_end);

  frame->SetEditableSelectionOffsets(8, 8);
  info = active_input_method_controller->TextInputInfo();
  EXPECT_EQ("hello world", std::string(info.value.Utf8().data()));
  EXPECT_EQ(8, info.selection_start);
  EXPECT_EQ(8, info.selection_end);
  EXPECT_EQ(6, info.composition_start);
  EXPECT_EQ(11, info.composition_end);

  frame->SetEditableSelectionOffsets(11, 11);
  info = active_input_method_controller->TextInputInfo();
  EXPECT_EQ("hello world", std::string(info.value.Utf8().data()));
  EXPECT_EQ(11, info.selection_start);
  EXPECT_EQ(11, info.selection_end);
  EXPECT_EQ(6, info.composition_start);
  EXPECT_EQ(11, info.composition_end);

  frame->SetEditableSelectionOffsets(6, 11);
  info = active_input_method_controller->TextInputInfo();
  EXPECT_EQ("hello world", std::string(info.value.Utf8().data()));
  EXPECT_EQ(6, info.selection_start);
  EXPECT_EQ(11, info.selection_end);
  EXPECT_EQ(6, info.composition_start);
  EXPECT_EQ(11, info.composition_end);

  frame->SetEditableSelectionOffsets(2, 2);
  info = active_input_method_controller->TextInputInfo();
  EXPECT_EQ("hello world", std::string(info.value.Utf8().data()));
  EXPECT_EQ(2, info.selection_start);
  EXPECT_EQ(2, info.selection_end);
  // Composition range should be reset by browser process or keyboard apps.
  EXPECT_EQ(6, info.composition_start);
  EXPECT_EQ(11, info.composition_end);
}

TEST_F(WebViewTest, IsSelectionAnchorFirst) {
  RegisterMockedHttpURLLoad("input_field_populated.html");
  WebViewImpl* web_view = web_view_helper_.InitializeAndLoad(
      base_url_ + "input_field_populated.html");
  WebLocalFrame* frame = web_view->MainFrameImpl();

  web_view->SetInitialFocus(false);
  frame->SetEditableSelectionOffsets(4, 10);
  EXPECT_TRUE(frame->IsSelectionAnchorFirst());
  WebRect anchor;
  WebRect focus;
  web_view->MainFrameWidget()->SelectionBounds(anchor, focus);
  frame->SelectRange(WebPoint(focus.x, focus.y), WebPoint(anchor.x, anchor.y));
  EXPECT_FALSE(frame->IsSelectionAnchorFirst());
}

TEST_F(
    WebViewTest,
    MoveFocusToNextFocusableElementInFormWithKeyEventListenersAndNonEditableElements) {
  const std::string test_file =
      "advance_focus_in_form_with_key_event_listeners.html";
  RegisterMockedHttpURLLoad(test_file);
  WebViewImpl* web_view =
      web_view_helper_.InitializeAndLoad(base_url_ + test_file);
  web_view->SetInitialFocus(false);
  Document* document = web_view->MainFrameImpl()->GetFrame()->GetDocument();
  WebInputMethodController* active_input_method_controller =
      web_view->MainFrameImpl()
          ->FrameWidget()
          ->GetActiveWebInputMethodController();
  const int default_text_input_flags = kWebTextInputFlagNone;

  struct FocusedElement {
    AtomicString element_id;
    int next_previous_flags;
  } focused_elements[] = {
      {"input1",
       default_text_input_flags | kWebTextInputFlagHaveNextFocusableElement},
      {"contenteditable1", kWebTextInputFlagHaveNextFocusableElement |
                               kWebTextInputFlagHavePreviousFocusableElement},
      {"input2", default_text_input_flags |
                     kWebTextInputFlagHaveNextFocusableElement |
                     kWebTextInputFlagHavePreviousFocusableElement},
      {"textarea1", default_text_input_flags |
                        kWebTextInputFlagHaveNextFocusableElement |
                        kWebTextInputFlagHavePreviousFocusableElement},
      {"input3", default_text_input_flags |
                     kWebTextInputFlagHaveNextFocusableElement |
                     kWebTextInputFlagHavePreviousFocusableElement},
      {"textarea2", default_text_input_flags |
                        kWebTextInputFlagHavePreviousFocusableElement},
  };

  // Forward Navigation in form1 with NEXT
  Element* input1 = document->getElementById("input1");
  input1->focus();
  Element* current_focus = nullptr;
  Element* next_focus = nullptr;
  int next_previous_flags;
  for (size_t i = 0; i < base::size(focused_elements); ++i) {
    current_focus = document->getElementById(focused_elements[i].element_id);
    EXPECT_EQ(current_focus, document->FocusedElement());
    next_previous_flags =
        active_input_method_controller->ComputeWebTextInputNextPreviousFlags();
    EXPECT_EQ(focused_elements[i].next_previous_flags, next_previous_flags);
    next_focus =
        document->GetPage()->GetFocusController().NextFocusableElementInForm(
            current_focus, kWebFocusTypeForward);
    if (next_focus) {
      EXPECT_EQ(next_focus->GetIdAttribute(),
                focused_elements[i + 1].element_id);
    }
    web_view->MainFrameImpl()->AdvanceFocusInForm(kWebFocusTypeForward);
  }
  // Now focus will stay on previous focus itself, because it has no next
  // element.
  EXPECT_EQ(current_focus, document->FocusedElement());

  // Backward Navigation in form1 with PREVIOUS
  for (size_t i = base::size(focused_elements); i-- > 0;) {
    current_focus = document->getElementById(focused_elements[i].element_id);
    EXPECT_EQ(current_focus, document->FocusedElement());
    next_previous_flags =
        active_input_method_controller->ComputeWebTextInputNextPreviousFlags();
    EXPECT_EQ(focused_elements[i].next_previous_flags, next_previous_flags);
    next_focus =
        document->GetPage()->GetFocusController().NextFocusableElementInForm(
            current_focus, kWebFocusTypeBackward);
    if (next_focus) {
      EXPECT_EQ(next_focus->GetIdAttribute(),
                focused_elements[i - 1].element_id);
    }
    web_view->MainFrameImpl()->AdvanceFocusInForm(kWebFocusTypeBackward);
  }
  // Now focus will stay on previous focus itself, because it has no previous
  // element.
  EXPECT_EQ(current_focus, document->FocusedElement());

  // Setting a non editable element as focus in form1, and ensuring editable
  // navigation is fine in forward and backward.
  Element* button1 = document->getElementById("button1");
  button1->focus();
  next_previous_flags =
      active_input_method_controller->ComputeWebTextInputNextPreviousFlags();
  EXPECT_EQ(kWebTextInputFlagHaveNextFocusableElement |
                kWebTextInputFlagHavePreviousFocusableElement,
            next_previous_flags);
  next_focus =
      document->GetPage()->GetFocusController().NextFocusableElementInForm(
          button1, kWebFocusTypeForward);
  EXPECT_EQ(next_focus->GetIdAttribute(), "contenteditable1");
  web_view->MainFrameImpl()->AdvanceFocusInForm(kWebFocusTypeForward);
  Element* content_editable1 = document->getElementById("contenteditable1");
  EXPECT_EQ(content_editable1, document->FocusedElement());
  button1->focus();
  next_focus =
      document->GetPage()->GetFocusController().NextFocusableElementInForm(
          button1, kWebFocusTypeBackward);
  EXPECT_EQ(next_focus->GetIdAttribute(), "input1");
  web_view->MainFrameImpl()->AdvanceFocusInForm(kWebFocusTypeBackward);
  EXPECT_EQ(input1, document->FocusedElement());

  Element* anchor1 = document->getElementById("anchor1");
  anchor1->focus();
  next_previous_flags =
      active_input_method_controller->ComputeWebTextInputNextPreviousFlags();
  // No Next/Previous element for elements outside form.
  EXPECT_EQ(0, next_previous_flags);
  next_focus =
      document->GetPage()->GetFocusController().NextFocusableElementInForm(
          anchor1, kWebFocusTypeForward);
  EXPECT_EQ(next_focus, nullptr);
  web_view->MainFrameImpl()->AdvanceFocusInForm(kWebFocusTypeForward);
  // Since anchor is not a form control element, next/previous element will
  // be null, hence focus will stay same as it is.
  EXPECT_EQ(anchor1, document->FocusedElement());

  next_focus =
      document->GetPage()->GetFocusController().NextFocusableElementInForm(
          anchor1, kWebFocusTypeBackward);
  EXPECT_EQ(next_focus, nullptr);
  web_view->MainFrameImpl()->AdvanceFocusInForm(kWebFocusTypeBackward);
  EXPECT_EQ(anchor1, document->FocusedElement());

  // Navigation of elements which is not part of any forms.
  Element* text_area3 = document->getElementById("textarea3");
  text_area3->focus();
  next_previous_flags =
      active_input_method_controller->ComputeWebTextInputNextPreviousFlags();
  // No Next/Previous element for elements outside form.
  EXPECT_EQ(default_text_input_flags, next_previous_flags);
  next_focus =
      document->GetPage()->GetFocusController().NextFocusableElementInForm(
          text_area3, kWebFocusTypeForward);
  EXPECT_EQ(next_focus, nullptr);
  web_view->MainFrameImpl()->AdvanceFocusInForm(kWebFocusTypeForward);
  // No Next/Previous element to this element because it's not part of any
  // form. Hence focus won't change wrt NEXT/PREVIOUS.
  EXPECT_EQ(text_area3, document->FocusedElement());
  next_focus =
      document->GetPage()->GetFocusController().NextFocusableElementInForm(
          text_area3, kWebFocusTypeBackward);
  EXPECT_EQ(next_focus, nullptr);
  web_view->MainFrameImpl()->AdvanceFocusInForm(kWebFocusTypeBackward);
  EXPECT_EQ(text_area3, document->FocusedElement());

  // Navigation from an element which is part of a form but not an editable
  // element.
  Element* button2 = document->getElementById("button2");
  button2->focus();
  next_previous_flags =
      active_input_method_controller->ComputeWebTextInputNextPreviousFlags();
  // No Next element for this element, due to last element outside the form.
  EXPECT_EQ(kWebTextInputFlagHavePreviousFocusableElement, next_previous_flags);
  next_focus =
      document->GetPage()->GetFocusController().NextFocusableElementInForm(
          button2, kWebFocusTypeForward);
  EXPECT_EQ(next_focus, nullptr);
  web_view->MainFrameImpl()->AdvanceFocusInForm(kWebFocusTypeForward);
  // No Next element to this element because it's not part of any form.
  // Hence focus won't change wrt NEXT.
  EXPECT_EQ(button2, document->FocusedElement());
  Element* text_area2 = document->getElementById("textarea2");
  next_focus =
      document->GetPage()->GetFocusController().NextFocusableElementInForm(
          button2, kWebFocusTypeBackward);
  EXPECT_EQ(next_focus, text_area2);
  web_view->MainFrameImpl()->AdvanceFocusInForm(kWebFocusTypeBackward);
  // Since button is a form control element from form1, ensuring focus is set
  // at correct position.
  EXPECT_EQ(text_area2, document->FocusedElement());

  Element* content_editable2 = document->getElementById("contenteditable2");
  document->SetFocusedElement(
      content_editable2,
      FocusParams(SelectionBehaviorOnFocus::kNone, kWebFocusTypeNone, nullptr));
  next_previous_flags =
      active_input_method_controller->ComputeWebTextInputNextPreviousFlags();
  // No Next/Previous element for elements outside form.
  EXPECT_EQ(0, next_previous_flags);
  next_focus =
      document->GetPage()->GetFocusController().NextFocusableElementInForm(
          content_editable2, kWebFocusTypeForward);
  EXPECT_EQ(next_focus, nullptr);
  web_view->MainFrameImpl()->AdvanceFocusInForm(kWebFocusTypeForward);
  // No Next/Previous element to this element because it's not part of any
  // form. Hence focus won't change wrt NEXT/PREVIOUS.
  EXPECT_EQ(content_editable2, document->FocusedElement());
  next_focus =
      document->GetPage()->GetFocusController().NextFocusableElementInForm(
          content_editable2, kWebFocusTypeBackward);
  EXPECT_EQ(next_focus, nullptr);
  web_view->MainFrameImpl()->AdvanceFocusInForm(kWebFocusTypeBackward);
  EXPECT_EQ(content_editable2, document->FocusedElement());

  // Navigation of elements which is having invalid form attribute and hence
  // not part of any forms.
  Element* text_area4 = document->getElementById("textarea4");
  text_area4->focus();
  next_previous_flags =
      active_input_method_controller->ComputeWebTextInputNextPreviousFlags();
  // No Next/Previous element for elements which is having invalid form
  // attribute.
  EXPECT_EQ(default_text_input_flags, next_previous_flags);
  next_focus =
      document->GetPage()->GetFocusController().NextFocusableElementInForm(
          text_area4, kWebFocusTypeForward);
  EXPECT_EQ(next_focus, nullptr);
  web_view->MainFrameImpl()->AdvanceFocusInForm(kWebFocusTypeForward);
  // No Next/Previous element to this element because it's not part of any
  // form. Hence focus won't change wrt NEXT/PREVIOUS.
  EXPECT_EQ(text_area4, document->FocusedElement());
  next_focus =
      document->GetPage()->GetFocusController().NextFocusableElementInForm(
          text_area4, kWebFocusTypeBackward);
  EXPECT_EQ(next_focus, nullptr);
  web_view->MainFrameImpl()->AdvanceFocusInForm(kWebFocusTypeBackward);
  EXPECT_EQ(text_area4, document->FocusedElement());

  web_view_helper_.Reset();
}

TEST_F(
    WebViewTest,
    MoveFocusToNextFocusableElementInFormWithNonEditableNonFormControlElements) {
  const std::string test_file =
      "advance_focus_in_form_with_key_event_listeners.html";
  RegisterMockedHttpURLLoad(test_file);
  WebViewImpl* web_view =
      web_view_helper_.InitializeAndLoad(base_url_ + test_file);
  web_view->SetInitialFocus(false);
  Document* document = web_view->MainFrameImpl()->GetFrame()->GetDocument();
  WebInputMethodController* active_input_method_controller =
      web_view->MainFrameImpl()
          ->FrameWidget()
          ->GetActiveWebInputMethodController();
  const int default_text_input_flags = kWebTextInputFlagNone;

  struct FocusedElement {
    const char* element_id;
    int next_previous_flags;
  } focused_elements[] = {
      {"textarea5",
       default_text_input_flags | kWebTextInputFlagHaveNextFocusableElement},
      {"input4", default_text_input_flags |
                     kWebTextInputFlagHaveNextFocusableElement |
                     kWebTextInputFlagHavePreviousFocusableElement},
      {"contenteditable3", kWebTextInputFlagHaveNextFocusableElement |
                               kWebTextInputFlagHavePreviousFocusableElement},
      {"input5", kWebTextInputFlagHavePreviousFocusableElement},
  };

  // Forward Navigation in form2 with NEXT
  Element* text_area5 = document->getElementById("textarea5");
  text_area5->focus();
  Element* current_focus = nullptr;
  Element* next_focus = nullptr;
  int next_previous_flags;
  for (size_t i = 0; i < base::size(focused_elements); ++i) {
    current_focus = document->getElementById(focused_elements[i].element_id);
    EXPECT_EQ(current_focus, document->FocusedElement());
    next_previous_flags =
        active_input_method_controller->ComputeWebTextInputNextPreviousFlags();
    EXPECT_EQ(focused_elements[i].next_previous_flags, next_previous_flags);
    next_focus =
        document->GetPage()->GetFocusController().NextFocusableElementInForm(
            current_focus, kWebFocusTypeForward);
    if (next_focus) {
      EXPECT_EQ(next_focus->GetIdAttribute(),
                focused_elements[i + 1].element_id);
    }
    web_view->MainFrameImpl()->AdvanceFocusInForm(kWebFocusTypeForward);
  }
  // Now focus will stay on previous focus itself, because it has no next
  // element.
  EXPECT_EQ(current_focus, document->FocusedElement());

  // Backward Navigation in form1 with PREVIOUS
  for (size_t i = base::size(focused_elements); i-- > 0;) {
    current_focus = document->getElementById(focused_elements[i].element_id);
    EXPECT_EQ(current_focus, document->FocusedElement());
    next_previous_flags =
        active_input_method_controller->ComputeWebTextInputNextPreviousFlags();
    EXPECT_EQ(focused_elements[i].next_previous_flags, next_previous_flags);
    next_focus =
        document->GetPage()->GetFocusController().NextFocusableElementInForm(
            current_focus, kWebFocusTypeBackward);
    if (next_focus) {
      EXPECT_EQ(next_focus->GetIdAttribute(),
                focused_elements[i - 1].element_id);
    }
    web_view->MainFrameImpl()->AdvanceFocusInForm(kWebFocusTypeBackward);
  }
  // Now focus will stay on previous focus itself, because it has no previous
  // element.
  EXPECT_EQ(current_focus, document->FocusedElement());

  // Setting a non editable element as focus in form1, and ensuring editable
  // navigation is fine in forward and backward.
  Element* anchor2 = document->getElementById("anchor2");
  anchor2->focus();
  next_previous_flags =
      active_input_method_controller->ComputeWebTextInputNextPreviousFlags();
  // No Next/Previous element for non-form control elements inside form.
  EXPECT_EQ(0, next_previous_flags);
  next_focus =
      document->GetPage()->GetFocusController().NextFocusableElementInForm(
          anchor2, kWebFocusTypeForward);
  EXPECT_EQ(next_focus, nullptr);
  web_view->MainFrameImpl()->AdvanceFocusInForm(kWebFocusTypeForward);
  // Since anchor is not a form control element, next/previous element will
  // be null, hence focus will stay same as it is.
  EXPECT_EQ(anchor2, document->FocusedElement());
  next_focus =
      document->GetPage()->GetFocusController().NextFocusableElementInForm(
          anchor2, kWebFocusTypeBackward);
  EXPECT_EQ(next_focus, nullptr);
  web_view->MainFrameImpl()->AdvanceFocusInForm(kWebFocusTypeBackward);
  EXPECT_EQ(anchor2, document->FocusedElement());

  web_view_helper_.Reset();
}

TEST_F(WebViewTest, MoveFocusToNextFocusableElementInFormWithTabIndexElements) {
  const std::string test_file =
      "advance_focus_in_form_with_tabindex_elements.html";
  RegisterMockedHttpURLLoad(test_file);
  WebViewImpl* web_view =
      web_view_helper_.InitializeAndLoad(base_url_ + test_file);
  web_view->SetInitialFocus(false);
  Document* document = web_view->MainFrameImpl()->GetFrame()->GetDocument();
  WebInputMethodController* active_input_method_controller =
      web_view->MainFrameImpl()
          ->FrameWidget()
          ->GetActiveWebInputMethodController();
  const int default_text_input_flags = kWebTextInputFlagNone;

  struct FocusedElement {
    const char* element_id;
    int next_previous_flags;
  } focused_elements[] = {
      {"textarea6",
       default_text_input_flags | kWebTextInputFlagHaveNextFocusableElement},
      {"input5", default_text_input_flags |
                     kWebTextInputFlagHaveNextFocusableElement |
                     kWebTextInputFlagHavePreviousFocusableElement},
      {"contenteditable4", kWebTextInputFlagHaveNextFocusableElement |
                               kWebTextInputFlagHavePreviousFocusableElement},
      {"input6", default_text_input_flags |
                     kWebTextInputFlagHavePreviousFocusableElement},
  };

  // Forward Navigation in form with NEXT which has tabindex attribute
  // which differs visual order.
  Element* text_area6 = document->getElementById("textarea6");
  text_area6->focus();
  Element* current_focus = nullptr;
  Element* next_focus = nullptr;
  int next_previous_flags;
  for (size_t i = 0; i < base::size(focused_elements); ++i) {
    current_focus = document->getElementById(focused_elements[i].element_id);
    EXPECT_EQ(current_focus, document->FocusedElement());
    next_previous_flags =
        active_input_method_controller->ComputeWebTextInputNextPreviousFlags();
    EXPECT_EQ(focused_elements[i].next_previous_flags, next_previous_flags);
    next_focus =
        document->GetPage()->GetFocusController().NextFocusableElementInForm(
            current_focus, kWebFocusTypeForward);
    if (next_focus) {
      EXPECT_EQ(next_focus->GetIdAttribute(),
                focused_elements[i + 1].element_id);
    }
    web_view->MainFrameImpl()->AdvanceFocusInForm(kWebFocusTypeForward);
  }
  // No next editable element which is focusable with proper tab index, hence
  // staying on previous focus.
  EXPECT_EQ(current_focus, document->FocusedElement());

  // Backward Navigation in form with PREVIOUS which has tabindex attribute
  // which differs visual order.
  for (size_t i = base::size(focused_elements); i-- > 0;) {
    current_focus = document->getElementById(focused_elements[i].element_id);
    EXPECT_EQ(current_focus, document->FocusedElement());
    next_previous_flags =
        active_input_method_controller->ComputeWebTextInputNextPreviousFlags();
    EXPECT_EQ(focused_elements[i].next_previous_flags, next_previous_flags);
    next_focus =
        document->GetPage()->GetFocusController().NextFocusableElementInForm(
            current_focus, kWebFocusTypeBackward);
    if (next_focus) {
      EXPECT_EQ(next_focus->GetIdAttribute(),
                focused_elements[i - 1].element_id);
    }
    web_view->MainFrameImpl()->AdvanceFocusInForm(kWebFocusTypeBackward);
  }
  // Now focus will stay on previous focus itself, because it has no previous
  // element.
  EXPECT_EQ(current_focus, document->FocusedElement());

  // Setting an element which has invalid tabindex and ensuring it is not
  // modifying further navigation.
  Element* content_editable5 = document->getElementById("contenteditable5");
  content_editable5->focus();
  Element* input6 = document->getElementById("input6");
  next_focus =
      document->GetPage()->GetFocusController().NextFocusableElementInForm(
          content_editable5, kWebFocusTypeForward);
  EXPECT_EQ(next_focus, input6);
  web_view->MainFrameImpl()->AdvanceFocusInForm(kWebFocusTypeForward);
  EXPECT_EQ(input6, document->FocusedElement());
  content_editable5->focus();
  next_focus =
      document->GetPage()->GetFocusController().NextFocusableElementInForm(
          content_editable5, kWebFocusTypeBackward);
  EXPECT_EQ(next_focus, text_area6);
  web_view->MainFrameImpl()->AdvanceFocusInForm(kWebFocusTypeBackward);
  EXPECT_EQ(text_area6, document->FocusedElement());

  web_view_helper_.Reset();
}

TEST_F(WebViewTest,
       MoveFocusToNextFocusableElementInFormWithDisabledAndReadonlyElements) {
  const std::string test_file =
      "advance_focus_in_form_with_disabled_and_readonly_elements.html";
  RegisterMockedHttpURLLoad(test_file);
  WebViewImpl* web_view =
      web_view_helper_.InitializeAndLoad(base_url_ + test_file);
  web_view->SetInitialFocus(false);
  Document* document = web_view->MainFrameImpl()->GetFrame()->GetDocument();
  WebInputMethodController* active_input_method_controller =
      web_view->MainFrameImpl()
          ->FrameWidget()
          ->GetActiveWebInputMethodController();

  struct FocusedElement {
    const char* element_id;
    int next_previous_flags;
  } focused_elements[] = {
      {"contenteditable6", kWebTextInputFlagHaveNextFocusableElement},
      {"contenteditable7", kWebTextInputFlagHavePreviousFocusableElement},
  };
  // Forward Navigation in form with NEXT which has has disabled/enabled
  // elements which will gets skipped during navigation.
  Element* content_editable6 = document->getElementById("contenteditable6");
  content_editable6->focus();
  Element* current_focus = nullptr;
  Element* next_focus = nullptr;
  int next_previous_flags;
  for (size_t i = 0; i < base::size(focused_elements); ++i) {
    current_focus = document->getElementById(focused_elements[i].element_id);
    EXPECT_EQ(current_focus, document->FocusedElement());
    next_previous_flags =
        active_input_method_controller->ComputeWebTextInputNextPreviousFlags();
    EXPECT_EQ(focused_elements[i].next_previous_flags, next_previous_flags);
    next_focus =
        document->GetPage()->GetFocusController().NextFocusableElementInForm(
            current_focus, kWebFocusTypeForward);
    if (next_focus) {
      EXPECT_EQ(next_focus->GetIdAttribute(),
                focused_elements[i + 1].element_id);
    }
    web_view->MainFrameImpl()->AdvanceFocusInForm(kWebFocusTypeForward);
  }
  // No next editable element which is focusable, hence staying on previous
  // focus.
  EXPECT_EQ(current_focus, document->FocusedElement());

  // Backward Navigation in form with PREVIOUS which has has
  // disabled/enabled elements which will gets skipped during navigation.
  for (size_t i = base::size(focused_elements); i-- > 0;) {
    current_focus = document->getElementById(focused_elements[i].element_id);
    EXPECT_EQ(current_focus, document->FocusedElement());
    next_previous_flags =
        active_input_method_controller->ComputeWebTextInputNextPreviousFlags();
    EXPECT_EQ(focused_elements[i].next_previous_flags, next_previous_flags);
    next_focus =
        document->GetPage()->GetFocusController().NextFocusableElementInForm(
            current_focus, kWebFocusTypeBackward);
    if (next_focus) {
      EXPECT_EQ(next_focus->GetIdAttribute(),
                focused_elements[i - 1].element_id);
    }
    web_view->MainFrameImpl()->AdvanceFocusInForm(kWebFocusTypeBackward);
  }
  // Now focus will stay on previous focus itself, because it has no previous
  // element.
  EXPECT_EQ(current_focus, document->FocusedElement());

  web_view_helper_.Reset();
}

TEST_F(WebViewTest, ExitingDeviceEmulationResetsPageScale) {
  RegisterMockedHttpURLLoad("200-by-300.html");
  WebViewImpl* web_view_impl =
      web_view_helper_.InitializeAndLoad(base_url_ + "200-by-300.html");
  web_view_impl->MainFrameWidget()->Resize(WebSize(200, 300));

  float page_scale_expected = web_view_impl->PageScaleFactor();

  WebDeviceEmulationParams params;
  params.screen_position = WebDeviceEmulationParams::kDesktop;
  params.device_scale_factor = 0;
  params.scale = 1;

  web_view_impl->EnableDeviceEmulation(params);

  web_view_impl->SetPageScaleFactor(2);

  web_view_impl->DisableDeviceEmulation();

  EXPECT_EQ(page_scale_expected, web_view_impl->PageScaleFactor());
}

TEST_F(WebViewTest, HistoryResetScrollAndScaleState) {
  RegisterMockedHttpURLLoad("200-by-300.html");
  WebViewImpl* web_view_impl =
      web_view_helper_.InitializeAndLoad(base_url_ + "200-by-300.html");
  web_view_impl->MainFrameWidget()->Resize(WebSize(100, 150));
  UpdateAllLifecyclePhases();
  EXPECT_EQ(0, web_view_impl->MainFrameImpl()->GetScrollOffset().width);
  EXPECT_EQ(0, web_view_impl->MainFrameImpl()->GetScrollOffset().height);

  // Make the page scale and scroll with the given paremeters.
  web_view_impl->SetPageScaleFactor(2.0f);
  web_view_impl->MainFrameImpl()->SetScrollOffset(WebSize(94, 111));
  EXPECT_EQ(2.0f, web_view_impl->PageScaleFactor());
  EXPECT_EQ(94, web_view_impl->MainFrameImpl()->GetScrollOffset().width);
  EXPECT_EQ(111, web_view_impl->MainFrameImpl()->GetScrollOffset().height);
  auto* main_frame_local =
      To<LocalFrame>(web_view_impl->GetPage()->MainFrame());
  main_frame_local->Loader().SaveScrollState();
  EXPECT_EQ(2.0f, main_frame_local->Loader()
                      .GetDocumentLoader()
                      ->GetHistoryItem()
                      ->GetViewState()
                      ->page_scale_factor_);
  EXPECT_EQ(94, main_frame_local->Loader()
                    .GetDocumentLoader()
                    ->GetHistoryItem()
                    ->GetViewState()
                    ->scroll_offset_.Width());
  EXPECT_EQ(111, main_frame_local->Loader()
                     .GetDocumentLoader()
                     ->GetHistoryItem()
                     ->GetViewState()
                     ->scroll_offset_.Height());

  // Confirm that resetting the page state resets the saved scroll position.
  web_view_impl->ResetScrollAndScaleState();
  EXPECT_EQ(1.0f, web_view_impl->PageScaleFactor());
  EXPECT_EQ(0, web_view_impl->MainFrameImpl()->GetScrollOffset().width);
  EXPECT_EQ(0, web_view_impl->MainFrameImpl()->GetScrollOffset().height);
  EXPECT_FALSE(main_frame_local->Loader()
                   .GetDocumentLoader()
                   ->GetHistoryItem()
                   ->GetViewState()
                   .has_value());
}

TEST_F(WebViewTest, BackForwardRestoreScroll) {
  RegisterMockedHttpURLLoad("back_forward_restore_scroll.html");
  WebViewImpl* web_view_impl = web_view_helper_.InitializeAndLoad(
      base_url_ + "back_forward_restore_scroll.html");
  web_view_impl->MainFrameWidget()->Resize(WebSize(640, 480));
  web_view_impl->MainFrameWidget()->UpdateAllLifecyclePhases(
      WebWidget::LifecycleUpdateReason::kTest);

  // Emulate a user scroll
  web_view_impl->MainFrameImpl()->SetScrollOffset(WebSize(0, 900));
  auto* main_frame_local =
      To<LocalFrame>(web_view_impl->GetPage()->MainFrame());
  Persistent<HistoryItem> item1 =
      main_frame_local->Loader().GetDocumentLoader()->GetHistoryItem();

  // Click an anchor
  main_frame_local->Loader().StartNavigation(FrameLoadRequest(
      main_frame_local->GetDocument(),
      ResourceRequest(main_frame_local->GetDocument()->CompleteURL("#a"))));
  Persistent<HistoryItem> item2 =
      main_frame_local->Loader().GetDocumentLoader()->GetHistoryItem();

  // Go back, then forward, then back again.
  main_frame_local->Loader().GetDocumentLoader()->CommitSameDocumentNavigation(
      item1->Url(), WebFrameLoadType::kBackForward, item1.Get(),
      ClientRedirectPolicy::kNotClientRedirect, nullptr, false, nullptr);
  main_frame_local->Loader().GetDocumentLoader()->CommitSameDocumentNavigation(
      item2->Url(), WebFrameLoadType::kBackForward, item2.Get(),
      ClientRedirectPolicy::kNotClientRedirect, nullptr, false, nullptr);
  main_frame_local->Loader().GetDocumentLoader()->CommitSameDocumentNavigation(
      item1->Url(), WebFrameLoadType::kBackForward, item1.Get(),
      ClientRedirectPolicy::kNotClientRedirect, nullptr, false, nullptr);

  // Click a different anchor
  main_frame_local->Loader().StartNavigation(FrameLoadRequest(
      main_frame_local->GetDocument(),
      ResourceRequest(main_frame_local->GetDocument()->CompleteURL("#b"))));
  Persistent<HistoryItem> item3 =
      main_frame_local->Loader().GetDocumentLoader()->GetHistoryItem();

  // Go back, then forward. The scroll position should be properly set on the
  // forward navigation.
  main_frame_local->Loader().GetDocumentLoader()->CommitSameDocumentNavigation(
      item1->Url(), WebFrameLoadType::kBackForward, item1.Get(),
      ClientRedirectPolicy::kNotClientRedirect, nullptr, false, nullptr);
  main_frame_local->Loader().GetDocumentLoader()->CommitSameDocumentNavigation(
      item3->Url(), WebFrameLoadType::kBackForward, item3.Get(),
      ClientRedirectPolicy::kNotClientRedirect, nullptr, false, nullptr);
  EXPECT_EQ(0, web_view_impl->MainFrameImpl()->GetScrollOffset().width);
  EXPECT_GT(web_view_impl->MainFrameImpl()->GetScrollOffset().height, 2000);
}

// Tests that scroll offset modified during fullscreen is preserved when
// exiting fullscreen.
TEST_F(WebViewTest, FullscreenNoResetScroll) {
  RegisterMockedHttpURLLoad("fullscreen_style.html");
  WebViewImpl* web_view_impl =
      web_view_helper_.InitializeAndLoad(base_url_ + "fullscreen_style.html");
  web_view_impl->MainFrameWidget()->Resize(WebSize(800, 600));
  UpdateAllLifecyclePhases();

  // Scroll the page down.
  web_view_impl->MainFrameImpl()->SetScrollOffset(WebSize(0, 2000));
  ASSERT_EQ(2000, web_view_impl->MainFrameImpl()->GetScrollOffset().height);

  // Enter fullscreen.
  LocalFrame* frame = web_view_impl->MainFrameImpl()->GetFrame();
  Element* element = frame->GetDocument()->documentElement();
  std::unique_ptr<UserGestureIndicator> gesture =
      LocalFrame::NotifyUserActivation(frame);
  Fullscreen::RequestFullscreen(*element);
  web_view_impl->MainFrameWidget()->DidEnterFullscreen();
  UpdateAllLifecyclePhases();

  // Assert the scroll position on the document element doesn't change.
  ASSERT_EQ(2000, web_view_impl->MainFrameImpl()->GetScrollOffset().height);

  web_view_impl->MainFrameImpl()->SetScrollOffset(WebSize(0, 2100));

  web_view_impl->MainFrameWidget()->DidExitFullscreen();
  UpdateAllLifecyclePhases();

  EXPECT_EQ(2100, web_view_impl->MainFrameImpl()->GetScrollOffset().height);
}

// Tests that background color is read from the backdrop on fullscreen.
TEST_F(WebViewTest, FullscreenBackgroundColor) {
  RegisterMockedHttpURLLoad("fullscreen_style.html");
  WebViewImpl* web_view_impl =
      web_view_helper_.InitializeAndLoad(base_url_ + "fullscreen_style.html");
  web_view_impl->MainFrameWidget()->Resize(WebSize(800, 600));
  UpdateAllLifecyclePhases();
  EXPECT_EQ(SK_ColorWHITE, web_view_impl->BackgroundColor());

  // Enter fullscreen.
  LocalFrame* frame = web_view_impl->MainFrameImpl()->GetFrame();
  Element* element = frame->GetDocument()->getElementById("fullscreenElement");
  ASSERT_TRUE(element);
  std::unique_ptr<UserGestureIndicator> gesture =
      LocalFrame::NotifyUserActivation(frame);
  Fullscreen::RequestFullscreen(*element);
  web_view_impl->MainFrameWidget()->DidEnterFullscreen();
  UpdateAllLifecyclePhases();

  EXPECT_EQ(SK_ColorYELLOW, web_view_impl->BackgroundColor());
}

class PrintWebViewClient : public frame_test_helpers::TestWebViewClient {
 public:
  PrintWebViewClient() : print_called_(false) {}

  // WebViewClient methods
  void PrintPage(WebLocalFrame*) override { print_called_ = true; }

  bool PrintCalled() const { return print_called_; }

 private:
  bool print_called_;
};

TEST_F(WebViewTest, PrintWithXHRInFlight) {
  PrintWebViewClient client;
  RegisterMockedHttpURLLoad("print_with_xhr_inflight.html");
  WebViewImpl* web_view_impl = web_view_helper_.InitializeAndLoad(
      base_url_ + "print_with_xhr_inflight.html", nullptr, &client);

  ASSERT_TRUE(To<LocalFrame>(web_view_impl->GetPage()->MainFrame())
                  ->GetDocument()
                  ->LoadEventFinished());
  EXPECT_TRUE(client.PrintCalled());
  web_view_helper_.Reset();
}

static void DragAndDropURL(WebViewImpl* web_view, const std::string& url) {
  WebDragData drag_data;
  drag_data.Initialize();

  WebDragData::Item item;
  item.storage_type = WebDragData::Item::kStorageTypeString;
  item.string_type = "text/uri-list";
  item.string_data = WebString::FromUTF8(url);
  drag_data.AddItem(item);

  const WebFloatPoint client_point(0, 0);
  const WebFloatPoint screen_point(0, 0);
  WebFrameWidget* widget = web_view->MainFrameImpl()->FrameWidget();
  widget->DragTargetDragEnter(drag_data, client_point, screen_point,
                              kWebDragOperationCopy, 0);
  widget->DragTargetDrop(drag_data, client_point, screen_point, 0);
  frame_test_helpers::PumpPendingRequestsForFrameToLoad(
      web_view->MainFrameImpl());
}

TEST_F(WebViewTest, DragDropURL) {
  RegisterMockedHttpURLLoad("foo.html");
  RegisterMockedHttpURLLoad("bar.html");

  const std::string foo_url = base_url_ + "foo.html";
  const std::string bar_url = base_url_ + "bar.html";

  WebViewImpl* web_view = web_view_helper_.InitializeAndLoad(foo_url);

  ASSERT_TRUE(web_view);

  // Drag and drop barUrl and verify that we've navigated to it.
  DragAndDropURL(web_view, bar_url);
  EXPECT_EQ(bar_url,
            web_view->MainFrameImpl()->GetDocument().Url().GetString().Utf8());

  // Drag and drop fooUrl and verify that we've navigated back to it.
  DragAndDropURL(web_view, foo_url);
  EXPECT_EQ(foo_url,
            web_view->MainFrameImpl()->GetDocument().Url().GetString().Utf8());

  // Disable navigation on drag-and-drop.
  web_view->SettingsImpl()->SetNavigateOnDragDrop(false);

  // Attempt to drag and drop to barUrl and verify that no navigation has
  // occurred.
  DragAndDropURL(web_view, bar_url);
  EXPECT_EQ(foo_url,
            web_view->MainFrameImpl()->GetDocument().Url().GetString().Utf8());
}

bool WebViewTest::TapElement(WebInputEvent::Type type, Element* element) {
  if (!element || !element->GetLayoutObject())
    return false;

  DCHECK(web_view_helper_.GetWebView());
  element->scrollIntoViewIfNeeded();

  // TODO(bokan): Technically incorrect, event positions should be in viewport
  // space. crbug.com/371902.
  FloatPoint center(
      web_view_helper_.GetWebView()
          ->MainFrameImpl()
          ->GetFrameView()
          ->FrameToScreen(element->GetLayoutObject()->AbsoluteBoundingBoxRect())
          .Center());

  WebGestureEvent event(type, WebInputEvent::kNoModifiers,
                        WebInputEvent::GetStaticTimeStampForTests(),
                        WebGestureDevice::kTouchscreen);
  event.SetPositionInWidget(center);

  web_view_helper_.GetWebView()->MainFrameWidget()->HandleInputEvent(
      WebCoalescedInputEvent(event));
  RunPendingTasks();
  return true;
}

bool WebViewTest::TapElementById(WebInputEvent::Type type,
                                 const WebString& id) {
  DCHECK(web_view_helper_.GetWebView());
  Element* element = static_cast<Element*>(
      web_view_helper_.LocalMainFrame()->GetDocument().GetElementById(id));
  return TapElement(type, element);
}

IntSize WebViewTest::PrintICBSizeFromPageSize(const FloatSize& page_size) {
  // The expected layout size comes from the calculation done in
  // ResizePageRectsKeepingRatio() which is used from PrintContext::begin() to
  // scale the page size.
  const float ratio = page_size.Height() / (float)page_size.Width();
  const int icb_width =
      floor(page_size.Width() * PrintContext::kPrintingMinimumShrinkFactor);
  const int icb_height = floor(icb_width * ratio);
  return IntSize(icb_width, icb_height);
}

ExternalDateTimeChooser* WebViewTest::GetExternalDateTimeChooser(
    WebViewImpl* web_view_impl) {
  return web_view_impl->GetChromeClient()
      .GetExternalDateTimeChooserForTesting();
}

TEST_F(WebViewTest, ClientTapHandling) {
  TapHandlingWebWidgetClient client;
  WebView* web_view = web_view_helper_.InitializeAndLoad("about:blank", nullptr,
                                                         nullptr, &client);
  WebGestureEvent event(WebInputEvent::kGestureTap, WebInputEvent::kNoModifiers,
                        WebInputEvent::GetStaticTimeStampForTests(),
                        WebGestureDevice::kTouchscreen);
  event.SetPositionInWidget(WebFloatPoint(3, 8));
  web_view->MainFrameWidget()->HandleInputEvent(WebCoalescedInputEvent(event));
  RunPendingTasks();
  EXPECT_EQ(3, client.TapX());
  EXPECT_EQ(8, client.TapY());
  client.Reset();
  event.SetType(WebInputEvent::kGestureLongPress);
  event.SetPositionInWidget(WebFloatPoint(25, 7));
  web_view->MainFrameWidget()->HandleInputEvent(WebCoalescedInputEvent(event));
  RunPendingTasks();
  EXPECT_EQ(25, client.LongpressX());
  EXPECT_EQ(7, client.LongpressY());

  // Explicitly reset to break dependency on locally scoped client.
  web_view_helper_.Reset();
}

TEST_F(WebViewTest, ClientTapHandlingNullWebViewClient) {
  // Note: this test doesn't use WebViewHelper since WebViewHelper creates an
  // internal WebViewClient on demand if the supplied WebViewClient is null.
  WebViewImpl* web_view = static_cast<WebViewImpl*>(
      WebView::Create(nullptr, /*is_hidden=*/false,
                      /*compositing_enabled=*/false, nullptr));
  frame_test_helpers::TestWebFrameClient web_frame_client;
  frame_test_helpers::TestWebWidgetClient web_widget_client;
  mojom::blink::DocumentInterfaceBrokerPtrInfo document_interface_broker;
  WebLocalFrame* local_frame = WebLocalFrame::CreateMainFrame(
      web_view, &web_frame_client, nullptr,
      mojo::MakeRequest(&document_interface_broker).PassMessagePipe(), nullptr);
  web_frame_client.Bind(local_frame);
  blink::WebFrameWidget::CreateForMainFrame(&web_widget_client, local_frame);

  WebGestureEvent event(WebInputEvent::kGestureTap, WebInputEvent::kNoModifiers,
                        WebInputEvent::GetStaticTimeStampForTests(),
                        WebGestureDevice::kTouchscreen);
  event.SetPositionInWidget(WebFloatPoint(3, 8));
  EXPECT_EQ(WebInputEventResult::kNotHandled,
            web_view->MainFrameWidget()->HandleInputEvent(
                WebCoalescedInputEvent(event)));
  // This also closes the WebView.
  web_view->MainFrameWidget()->Close();
}

TEST_F(WebViewTest, LongPressEmptyDiv) {
  RegisterMockedHttpURLLoad("long_press_empty_div.html");

  WebViewImpl* web_view = web_view_helper_.InitializeAndLoad(
      base_url_ + "long_press_empty_div.html");
  web_view->SettingsImpl()->SetAlwaysShowContextMenuOnTouch(false);
  web_view->MainFrameWidget()->Resize(WebSize(500, 300));
  UpdateAllLifecyclePhases();
  RunPendingTasks();

  WebGestureEvent event(WebInputEvent::kGestureLongPress,
                        WebInputEvent::kNoModifiers,
                        WebInputEvent::GetStaticTimeStampForTests(),
                        WebGestureDevice::kTouchscreen);
  event.SetPositionInWidget(WebFloatPoint(250, 150));

  EXPECT_EQ(WebInputEventResult::kNotHandled,
            web_view->MainFrameWidget()->HandleInputEvent(
                WebCoalescedInputEvent(event)));
}

TEST_F(WebViewTest, LongPressEmptyDivAlwaysShow) {
  RegisterMockedHttpURLLoad("long_press_empty_div.html");

  WebViewImpl* web_view = web_view_helper_.InitializeAndLoad(
      base_url_ + "long_press_empty_div.html");
  web_view->SettingsImpl()->SetAlwaysShowContextMenuOnTouch(true);
  web_view->MainFrameWidget()->Resize(WebSize(500, 300));
  UpdateAllLifecyclePhases();
  RunPendingTasks();

  WebGestureEvent event(WebInputEvent::kGestureLongPress,
                        WebInputEvent::kNoModifiers,
                        WebInputEvent::GetStaticTimeStampForTests(),
                        WebGestureDevice::kTouchscreen);
  event.SetPositionInWidget(WebFloatPoint(250, 150));

  EXPECT_EQ(WebInputEventResult::kHandledSystem,
            web_view->MainFrameWidget()->HandleInputEvent(
                WebCoalescedInputEvent(event)));
}

TEST_F(WebViewTest, LongPressObject) {
  RegisterMockedHttpURLLoad("long_press_object.html");

  WebViewImpl* web_view =
      web_view_helper_.InitializeAndLoad(base_url_ + "long_press_object.html");
  web_view->SettingsImpl()->SetAlwaysShowContextMenuOnTouch(true);
  web_view->MainFrameWidget()->Resize(WebSize(500, 300));
  UpdateAllLifecyclePhases();
  RunPendingTasks();

  WebGestureEvent event(WebInputEvent::kGestureLongPress,
                        WebInputEvent::kNoModifiers,
                        WebInputEvent::GetStaticTimeStampForTests(),
                        WebGestureDevice::kTouchscreen);
  event.SetPositionInWidget(WebFloatPoint(10, 10));

  EXPECT_NE(WebInputEventResult::kHandledSystem,
            web_view->MainFrameWidget()->HandleInputEvent(
                WebCoalescedInputEvent(event)));

  auto* element = To<HTMLElement>(static_cast<Node*>(
      web_view->MainFrameImpl()->GetDocument().GetElementById("obj")));
  EXPECT_FALSE(element->CanStartSelection());
}

TEST_F(WebViewTest, LongPressObjectFallback) {
  RegisterMockedHttpURLLoad("long_press_object_fallback.html");

  WebViewImpl* web_view = web_view_helper_.InitializeAndLoad(
      base_url_ + "long_press_object_fallback.html");
  web_view->SettingsImpl()->SetAlwaysShowContextMenuOnTouch(true);
  web_view->MainFrameWidget()->Resize(WebSize(500, 300));
  UpdateAllLifecyclePhases();
  RunPendingTasks();

  WebGestureEvent event(WebInputEvent::kGestureLongPress,
                        WebInputEvent::kNoModifiers,
                        WebInputEvent::GetStaticTimeStampForTests(),
                        WebGestureDevice::kTouchscreen);
  event.SetPositionInWidget(WebFloatPoint(10, 10));

  EXPECT_EQ(WebInputEventResult::kHandledSystem,
            web_view->MainFrameWidget()->HandleInputEvent(
                WebCoalescedInputEvent(event)));

  auto* element = To<HTMLElement>(static_cast<Node*>(
      web_view->MainFrameImpl()->GetDocument().GetElementById("obj")));
  EXPECT_TRUE(element->CanStartSelection());
}

TEST_F(WebViewTest, LongPressImage) {
  RegisterMockedHttpURLLoad("long_press_image.html");

  WebViewImpl* web_view =
      web_view_helper_.InitializeAndLoad(base_url_ + "long_press_image.html");
  web_view->SettingsImpl()->SetAlwaysShowContextMenuOnTouch(false);
  web_view->MainFrameWidget()->Resize(WebSize(500, 300));
  UpdateAllLifecyclePhases();
  RunPendingTasks();

  WebGestureEvent event(WebInputEvent::kGestureLongPress,
                        WebInputEvent::kNoModifiers,
                        WebInputEvent::GetStaticTimeStampForTests(),
                        WebGestureDevice::kTouchscreen);
  event.SetPositionInWidget(WebFloatPoint(10, 10));

  EXPECT_EQ(WebInputEventResult::kHandledSystem,
            web_view->MainFrameWidget()->HandleInputEvent(
                WebCoalescedInputEvent(event)));
}

TEST_F(WebViewTest, LongPressVideo) {
  RegisterMockedHttpURLLoad("long_press_video.html");

  WebViewImpl* web_view =
      web_view_helper_.InitializeAndLoad(base_url_ + "long_press_video.html");
  web_view->SettingsImpl()->SetAlwaysShowContextMenuOnTouch(false);
  web_view->MainFrameWidget()->Resize(WebSize(500, 300));
  UpdateAllLifecyclePhases();
  RunPendingTasks();

  WebGestureEvent event(WebInputEvent::kGestureLongPress,
                        WebInputEvent::kNoModifiers,
                        WebInputEvent::GetStaticTimeStampForTests(),
                        WebGestureDevice::kTouchscreen);
  event.SetPositionInWidget(WebFloatPoint(10, 10));

  EXPECT_EQ(WebInputEventResult::kHandledSystem,
            web_view->MainFrameWidget()->HandleInputEvent(
                WebCoalescedInputEvent(event)));
}

TEST_F(WebViewTest, LongPressLink) {
  RegisterMockedHttpURLLoad("long_press_link.html");

  WebViewImpl* web_view =
      web_view_helper_.InitializeAndLoad(base_url_ + "long_press_link.html");
  web_view->SettingsImpl()->SetAlwaysShowContextMenuOnTouch(false);
  web_view->MainFrameWidget()->Resize(WebSize(500, 300));
  UpdateAllLifecyclePhases();
  RunPendingTasks();

  WebGestureEvent event(WebInputEvent::kGestureLongPress,
                        WebInputEvent::kNoModifiers,
                        WebInputEvent::GetStaticTimeStampForTests(),
                        WebGestureDevice::kTouchscreen);
  event.SetPositionInWidget(WebFloatPoint(500, 300));

  EXPECT_EQ(WebInputEventResult::kHandledSystem,
            web_view->MainFrameWidget()->HandleInputEvent(
                WebCoalescedInputEvent(event)));
}

// Tests that we send touchcancel when drag start by long press.
TEST_F(WebViewTest, TouchCancelOnStartDragging) {
  RegisterMockedHttpURLLoad("long_press_draggable_div.html");

  url_test_helpers::RegisterMockedURLLoad(
      ToKURL("http://www.test.com/foo.png"),
      test::CoreTestDataPath("white-1x1.png"));
  WebViewImpl* web_view = web_view_helper_.InitializeAndLoad(
      base_url_ + "long_press_draggable_div.html");

  web_view->SettingsImpl()->SetTouchDragDropEnabled(true);
  web_view->MainFrameWidget()->Resize(WebSize(500, 300));
  UpdateAllLifecyclePhases();
  RunPendingTasks();

  WebPointerEvent pointer_down(
      WebInputEvent::kPointerDown,
      WebPointerProperties(1, WebPointerProperties::PointerType::kTouch), 5, 5);
  pointer_down.SetPositionInWidget(250, 8);
  web_view->MainFrameWidget()->HandleInputEvent(
      WebCoalescedInputEvent(pointer_down));
  web_view->MainFrameWidget()->DispatchBufferedTouchEvents();

  WebString target_id = WebString::FromUTF8("target");

  // Send long press to start dragging
  EXPECT_TRUE(TapElementById(WebInputEvent::kGestureLongPress, target_id));
  EXPECT_STREQ("dragstart",
               web_view->MainFrameImpl()->GetDocument().Title().Utf8().data());

  // Check pointer cancel is sent to dom.
  WebPointerEvent pointer_cancel(
      WebInputEvent::kPointerCancel,
      WebPointerProperties(1, WebPointerProperties::PointerType::kTouch), 5, 5);
  pointer_cancel.SetPositionInWidget(250, 8);
  EXPECT_NE(WebInputEventResult::kHandledSuppressed,
            web_view->MainFrameWidget()->HandleInputEvent(
                WebCoalescedInputEvent(pointer_cancel)));
  web_view->MainFrameWidget()->DispatchBufferedTouchEvents();
  EXPECT_STREQ("touchcancel",
               web_view->MainFrameImpl()->GetDocument().Title().Utf8().data());
}

TEST_F(WebViewTest, showContextMenuOnLongPressingLinks) {
  RegisterMockedHttpURLLoad("long_press_links_and_images.html");

  url_test_helpers::RegisterMockedURLLoad(
      ToKURL("http://www.test.com/foo.png"),
      test::CoreTestDataPath("white-1x1.png"));
  WebViewImpl* web_view = web_view_helper_.InitializeAndLoad(
      base_url_ + "long_press_links_and_images.html");

  web_view->SettingsImpl()->SetTouchDragDropEnabled(true);
  web_view->MainFrameWidget()->Resize(WebSize(500, 300));
  UpdateAllLifecyclePhases();
  RunPendingTasks();

  WebString anchor_tag_id = WebString::FromUTF8("anchorTag");
  WebString image_tag_id = WebString::FromUTF8("imageTag");

  EXPECT_TRUE(TapElementById(WebInputEvent::kGestureLongPress, anchor_tag_id));
  EXPECT_STREQ("anchor contextmenu",
               web_view->MainFrameImpl()->GetDocument().Title().Utf8().data());

  EXPECT_TRUE(TapElementById(WebInputEvent::kGestureLongPress, image_tag_id));
  EXPECT_STREQ("image contextmenu",
               web_view->MainFrameImpl()->GetDocument().Title().Utf8().data());
}

TEST_F(WebViewTest, LongPressEmptyEditableSelection) {
  RegisterMockedHttpURLLoad("long_press_empty_editable_selection.html");

  WebViewImpl* web_view = web_view_helper_.InitializeAndLoad(
      base_url_ + "long_press_empty_editable_selection.html");
  web_view->SettingsImpl()->SetAlwaysShowContextMenuOnTouch(false);
  web_view->MainFrameWidget()->Resize(WebSize(500, 300));
  UpdateAllLifecyclePhases();
  RunPendingTasks();

  WebGestureEvent event(WebInputEvent::kGestureLongPress,
                        WebInputEvent::kNoModifiers,
                        WebInputEvent::GetStaticTimeStampForTests(),
                        WebGestureDevice::kTouchscreen);
  event.SetPositionInWidget(WebFloatPoint(10, 10));

  EXPECT_EQ(WebInputEventResult::kHandledSystem,
            web_view->MainFrameWidget()->HandleInputEvent(
                WebCoalescedInputEvent(event)));
}

TEST_F(WebViewTest, LongPressEmptyNonEditableSelection) {
  RegisterMockedHttpURLLoad("long_press_image.html");

  WebViewImpl* web_view =
      web_view_helper_.InitializeAndLoad(base_url_ + "long_press_image.html");
  web_view->MainFrameWidget()->Resize(WebSize(500, 500));
  UpdateAllLifecyclePhases();
  RunPendingTasks();

  WebGestureEvent event(WebInputEvent::kGestureLongPress,
                        WebInputEvent::kNoModifiers,
                        WebInputEvent::GetStaticTimeStampForTests(),
                        WebGestureDevice::kTouchscreen);
  event.SetPositionInWidget(WebFloatPoint(300, 300));
  WebLocalFrameImpl* frame = web_view->MainFrameImpl();

  EXPECT_EQ(WebInputEventResult::kHandledSystem,
            web_view->MainFrameWidget()->HandleInputEvent(
                WebCoalescedInputEvent(event)));
  EXPECT_TRUE(frame->SelectionAsText().IsEmpty());
}

TEST_F(WebViewTest, LongPressSelection) {
  RegisterMockedHttpURLLoad("longpress_selection.html");

  WebViewImpl* web_view = web_view_helper_.InitializeAndLoad(
      base_url_ + "longpress_selection.html");
  web_view->MainFrameWidget()->Resize(WebSize(500, 300));
  UpdateAllLifecyclePhases();
  RunPendingTasks();

  WebString target = WebString::FromUTF8("target");
  WebString onselectstartfalse = WebString::FromUTF8("onselectstartfalse");
  WebLocalFrameImpl* frame = web_view->MainFrameImpl();

  EXPECT_TRUE(
      TapElementById(WebInputEvent::kGestureLongPress, onselectstartfalse));
  EXPECT_EQ("", std::string(frame->SelectionAsText().Utf8().data()));
  EXPECT_TRUE(TapElementById(WebInputEvent::kGestureLongPress, target));
  EXPECT_EQ("testword", std::string(frame->SelectionAsText().Utf8().data()));
}

TEST_F(WebViewTest, FinishComposingTextDoesNotDismissHandles) {
  RegisterMockedHttpURLLoad("longpress_selection.html");

  WebViewImpl* web_view = web_view_helper_.InitializeAndLoad(
      base_url_ + "longpress_selection.html");
  web_view->MainFrameWidget()->Resize(WebSize(500, 300));
  UpdateAllLifecyclePhases();
  RunPendingTasks();

  WebString target = WebString::FromUTF8("target");
  WebLocalFrameImpl* frame = web_view->MainFrameImpl();
  WebInputMethodController* active_input_method_controller =
      frame->FrameWidget()->GetActiveWebInputMethodController();
  EXPECT_TRUE(TapElementById(WebInputEvent::kGestureTap, target));
  WebVector<WebImeTextSpan> empty_ime_text_spans;
  frame->SetEditableSelectionOffsets(8, 8);
  EXPECT_TRUE(active_input_method_controller->SetComposition(
      "12345", empty_ime_text_spans, WebRange(), 8, 13));
  EXPECT_TRUE(frame->GetFrame()->GetInputMethodController().HasComposition());
  EXPECT_EQ("", std::string(frame->SelectionAsText().Utf8().data()));
  EXPECT_FALSE(frame->GetFrame()->Selection().IsHandleVisible());
  EXPECT_TRUE(frame->GetFrame()->GetInputMethodController().HasComposition());

  EXPECT_TRUE(TapElementById(WebInputEvent::kGestureLongPress, target));
  EXPECT_EQ("testword12345",
            std::string(frame->SelectionAsText().Utf8().data()));
  EXPECT_TRUE(frame->GetFrame()->Selection().IsHandleVisible());
  EXPECT_TRUE(frame->GetFrame()->GetInputMethodController().HasComposition());

  // Check that finishComposingText(KeepSelection) does not dismiss handles.
  active_input_method_controller->FinishComposingText(
      WebInputMethodController::kKeepSelection);
  EXPECT_TRUE(frame->GetFrame()->Selection().IsHandleVisible());
}

#if !defined(OS_MACOSX)
TEST_F(WebViewTest, TouchDoesntSelectEmptyTextarea) {
  RegisterMockedHttpURLLoad("longpress_textarea.html");

  WebViewImpl* web_view =
      web_view_helper_.InitializeAndLoad(base_url_ + "longpress_textarea.html");
  web_view->MainFrameWidget()->Resize(WebSize(500, 300));
  UpdateAllLifecyclePhases();
  RunPendingTasks();

  WebString blanklinestextbox = WebString::FromUTF8("blanklinestextbox");
  WebLocalFrameImpl* frame = web_view->MainFrameImpl();

  // Long-press on carriage returns.
  EXPECT_TRUE(
      TapElementById(WebInputEvent::kGestureLongPress, blanklinestextbox));
  EXPECT_TRUE(frame->SelectionAsText().IsEmpty());

  // Double-tap on carriage returns.
  WebGestureEvent event(WebInputEvent::kGestureTap, WebInputEvent::kNoModifiers,
                        WebInputEvent::GetStaticTimeStampForTests(),
                        WebGestureDevice::kTouchscreen);
  event.SetPositionInWidget(WebFloatPoint(100, 25));
  event.data.tap.tap_count = 2;

  web_view->MainFrameWidget()->HandleInputEvent(WebCoalescedInputEvent(event));
  EXPECT_TRUE(frame->SelectionAsText().IsEmpty());

  HTMLTextAreaElement* text_area_element = ToHTMLTextAreaElement(
      web_view->MainFrameImpl()->GetDocument().GetElementById(
          blanklinestextbox));
  text_area_element->setValue("hello");

  // Long-press past last word of textbox.
  EXPECT_TRUE(
      TapElementById(WebInputEvent::kGestureLongPress, blanklinestextbox));
  EXPECT_TRUE(frame->SelectionAsText().IsEmpty());

  // Double-tap past last word of textbox.
  web_view->MainFrameWidget()->HandleInputEvent(WebCoalescedInputEvent(event));
  EXPECT_TRUE(frame->SelectionAsText().IsEmpty());
}
#endif

TEST_F(WebViewTest, LongPressImageTextarea) {
  RegisterMockedHttpURLLoad("longpress_image_contenteditable.html");

  WebViewImpl* web_view = web_view_helper_.InitializeAndLoad(
      base_url_ + "longpress_image_contenteditable.html");
  web_view->MainFrameWidget()->Resize(WebSize(500, 300));
  UpdateAllLifecyclePhases();
  RunPendingTasks();

  WebString image = WebString::FromUTF8("purpleimage");

  EXPECT_TRUE(TapElementById(WebInputEvent::kGestureLongPress, image));
  WebRange range = web_view->MainFrameImpl()
                       ->GetInputMethodController()
                       ->GetSelectionOffsets();
  EXPECT_FALSE(range.IsNull());
  EXPECT_EQ(0, range.StartOffset());
  EXPECT_EQ(1, range.length());
}

TEST_F(WebViewTest, BlinkCaretAfterLongPress) {
  RegisterMockedHttpURLLoad("blink_caret_on_typing_after_long_press.html");

  WebViewImpl* web_view = web_view_helper_.InitializeAndLoad(
      base_url_ + "blink_caret_on_typing_after_long_press.html");
  web_view->MainFrameWidget()->Resize(WebSize(640, 480));
  UpdateAllLifecyclePhases();
  RunPendingTasks();

  WebString target = WebString::FromUTF8("target");
  WebLocalFrameImpl* main_frame = web_view->MainFrameImpl();

  EXPECT_TRUE(TapElementById(WebInputEvent::kGestureLongPress, target));
  EXPECT_FALSE(main_frame->GetFrame()->Selection().IsCaretBlinkingSuspended());
}

TEST_F(WebViewTest, BlinkCaretOnClosingContextMenu) {
  RegisterMockedHttpURLLoad("form.html");
  WebViewImpl* web_view =
      web_view_helper_.InitializeAndLoad(base_url_ + "form.html");

  web_view->SetInitialFocus(false);
  RunPendingTasks();

  // We suspend caret blinking when pressing with mouse right button.
  // Note that we do not send MouseUp event here since it will be consumed
  // by the context menu once it shows up.
  WebMouseEvent mouse_event(WebInputEvent::kMouseDown,
                            WebInputEvent::kNoModifiers,
                            WebInputEvent::GetStaticTimeStampForTests());

  mouse_event.button = WebMouseEvent::Button::kRight;
  mouse_event.SetPositionInWidget(1, 1);
  mouse_event.click_count = 1;
  web_view->MainFrameWidget()->HandleInputEvent(
      WebCoalescedInputEvent(mouse_event));
  RunPendingTasks();

  WebLocalFrameImpl* main_frame = web_view->MainFrameImpl();
  EXPECT_TRUE(main_frame->GetFrame()->Selection().IsCaretBlinkingSuspended());

  // Caret blinking is still suspended after showing context menu.
  web_view->MainFrameWidget()->ShowContextMenu(kMenuSourceMouse);
  EXPECT_TRUE(main_frame->GetFrame()->Selection().IsCaretBlinkingSuspended());

  // Caret blinking will be resumed only after context menu is closed.
  web_view->DidCloseContextMenu();

  EXPECT_FALSE(main_frame->GetFrame()->Selection().IsCaretBlinkingSuspended());
}

TEST_F(WebViewTest, SelectionOnReadOnlyInput) {
  RegisterMockedHttpURLLoad("selection_readonly.html");
  WebViewImpl* web_view =
      web_view_helper_.InitializeAndLoad(base_url_ + "selection_readonly.html");
  web_view->MainFrameWidget()->Resize(WebSize(640, 480));
  UpdateAllLifecyclePhases();
  RunPendingTasks();

  std::string test_word = "This text should be selected.";

  WebLocalFrameImpl* frame = web_view->MainFrameImpl();
  EXPECT_EQ(test_word, std::string(frame->SelectionAsText().Utf8().data()));

  WebRange range = web_view->MainFrameImpl()
                       ->GetInputMethodController()
                       ->GetSelectionOffsets();
  EXPECT_FALSE(range.IsNull());
  EXPECT_EQ(0, range.StartOffset());
  EXPECT_EQ(static_cast<int>(test_word.length()), range.length());
}

TEST_F(WebViewTest, KeyDownScrollsHandled) {
  RegisterMockedHttpURLLoad("content-width-1000.html");

  WebViewImpl* web_view =
      web_view_helper_.InitializeAndLoad(base_url_ + "content-width-1000.html");
  web_view->MainFrameWidget()->Resize(WebSize(100, 100));
  UpdateAllLifecyclePhases();
  RunPendingTasks();

  WebKeyboardEvent key_event(WebInputEvent::kRawKeyDown,
                             WebInputEvent::kNoModifiers,
                             WebInputEvent::GetStaticTimeStampForTests());

  // RawKeyDown pagedown should be handled.
  key_event.windows_key_code = VKEY_NEXT;
  EXPECT_EQ(WebInputEventResult::kHandledSystem,
            web_view->MainFrameWidget()->HandleInputEvent(
                WebCoalescedInputEvent(key_event)));
  key_event.SetType(WebInputEvent::kKeyUp);
  web_view->MainFrameWidget()->HandleInputEvent(
      WebCoalescedInputEvent(key_event));

  // Coalesced KeyDown arrow-down should be handled.
  key_event.windows_key_code = VKEY_DOWN;
  key_event.SetType(WebInputEvent::kKeyDown);
  EXPECT_EQ(WebInputEventResult::kHandledSystem,
            web_view->MainFrameWidget()->HandleInputEvent(
                WebCoalescedInputEvent(key_event)));
  key_event.SetType(WebInputEvent::kKeyUp);
  web_view->MainFrameWidget()->HandleInputEvent(
      WebCoalescedInputEvent(key_event));

  // Ctrl-Home should be handled...
  key_event.windows_key_code = VKEY_HOME;
  key_event.SetModifiers(WebInputEvent::kControlKey);
  key_event.SetType(WebInputEvent::kRawKeyDown);
  EXPECT_EQ(WebInputEventResult::kNotHandled,
            web_view->MainFrameWidget()->HandleInputEvent(
                WebCoalescedInputEvent(key_event)));
  key_event.SetType(WebInputEvent::kKeyUp);
  web_view->MainFrameWidget()->HandleInputEvent(
      WebCoalescedInputEvent(key_event));

  // But Ctrl-Down should not.
  key_event.windows_key_code = VKEY_DOWN;
  key_event.SetModifiers(WebInputEvent::kControlKey);
  key_event.SetType(WebInputEvent::kRawKeyDown);
  EXPECT_EQ(WebInputEventResult::kNotHandled,
            web_view->MainFrameWidget()->HandleInputEvent(
                WebCoalescedInputEvent(key_event)));
  key_event.SetType(WebInputEvent::kKeyUp);
  web_view->MainFrameWidget()->HandleInputEvent(
      WebCoalescedInputEvent(key_event));

  // Shift, meta, and alt should not be handled.
  key_event.windows_key_code = VKEY_NEXT;
  key_event.SetModifiers(WebInputEvent::kShiftKey);
  key_event.SetType(WebInputEvent::kRawKeyDown);
  EXPECT_EQ(WebInputEventResult::kNotHandled,
            web_view->MainFrameWidget()->HandleInputEvent(
                WebCoalescedInputEvent(key_event)));
  key_event.SetType(WebInputEvent::kKeyUp);
  web_view->MainFrameWidget()->HandleInputEvent(
      WebCoalescedInputEvent(key_event));

  key_event.windows_key_code = VKEY_NEXT;
  key_event.SetModifiers(WebInputEvent::kMetaKey);
  key_event.SetType(WebInputEvent::kRawKeyDown);
  EXPECT_EQ(WebInputEventResult::kNotHandled,
            web_view->MainFrameWidget()->HandleInputEvent(
                WebCoalescedInputEvent(key_event)));
  key_event.SetType(WebInputEvent::kKeyUp);
  web_view->MainFrameWidget()->HandleInputEvent(
      WebCoalescedInputEvent(key_event));

  key_event.windows_key_code = VKEY_NEXT;
  key_event.SetModifiers(WebInputEvent::kAltKey);
  key_event.SetType(WebInputEvent::kRawKeyDown);
  EXPECT_EQ(WebInputEventResult::kNotHandled,
            web_view->MainFrameWidget()->HandleInputEvent(
                WebCoalescedInputEvent(key_event)));
  key_event.SetType(WebInputEvent::kKeyUp);
  web_view->MainFrameWidget()->HandleInputEvent(
      WebCoalescedInputEvent(key_event));

  // System-key labeled Alt-Down (as in Windows) should do nothing,
  // but non-system-key labeled Alt-Down (as in Mac) should be handled
  // as a page-down.
  key_event.windows_key_code = VKEY_DOWN;
  key_event.SetModifiers(WebInputEvent::kAltKey);
  key_event.is_system_key = true;
  key_event.SetType(WebInputEvent::kRawKeyDown);
  EXPECT_EQ(WebInputEventResult::kNotHandled,
            web_view->MainFrameWidget()->HandleInputEvent(
                WebCoalescedInputEvent(key_event)));
  key_event.SetType(WebInputEvent::kKeyUp);
  web_view->MainFrameWidget()->HandleInputEvent(
      WebCoalescedInputEvent(key_event));

  key_event.windows_key_code = VKEY_DOWN;
  key_event.SetModifiers(WebInputEvent::kAltKey);
  key_event.is_system_key = false;
  key_event.SetType(WebInputEvent::kRawKeyDown);
  EXPECT_EQ(WebInputEventResult::kHandledSystem,
            web_view->MainFrameWidget()->HandleInputEvent(
                WebCoalescedInputEvent(key_event)));
  key_event.SetType(WebInputEvent::kKeyUp);
  web_view->MainFrameWidget()->HandleInputEvent(
      WebCoalescedInputEvent(key_event));
}

class MiddleClickAutoscrollWebWidgetClient
    : public frame_test_helpers::TestWebWidgetClient {
 public:
  // WebWidgetClient methods

  void DidChangeCursor(const WebCursorInfo& cursor) override {
    last_cursor_type_ = cursor.type;
  }

  int GetLastCursorType() const { return last_cursor_type_; }

 private:
  int last_cursor_type_ = 0;
};

TEST_F(WebViewTest, MiddleClickAutoscrollCursor) {
  MiddleClickAutoscrollWebWidgetClient client;
  ScopedMiddleClickAutoscrollForTest middle_click_autoscroll(true);
  RegisterMockedHttpURLLoad("content-width-1000.html");

  WebViewImpl* web_view = web_view_helper_.InitializeAndLoad(
      base_url_ + "content-width-1000.html", nullptr, nullptr, &client);
  web_view->MainFrameWidget()->Resize(WebSize(100, 100));
  UpdateAllLifecyclePhases();
  RunPendingTasks();

  WebMouseEvent mouse_event(WebInputEvent::kMouseDown,
                            WebInputEvent::kNoModifiers,
                            WebInputEvent::GetStaticTimeStampForTests());
  mouse_event.button = WebMouseEvent::Button::kMiddle;
  mouse_event.SetPositionInWidget(1, 1);
  mouse_event.click_count = 1;

  // Start middle-click autoscroll.
  web_view->MainFrameWidget()->HandleInputEvent(
      WebCoalescedInputEvent(mouse_event));
  mouse_event.SetType(WebInputEvent::kMouseUp);
  web_view->MainFrameWidget()->HandleInputEvent(
      WebCoalescedInputEvent(mouse_event));

  EXPECT_EQ(MiddlePanningCursor().GetType(), client.GetLastCursorType());

  LocalFrame* local_frame =
      To<WebLocalFrameImpl>(web_view->MainFrame())->GetFrame();

  // Even if a plugin tries to change the cursor type, that should be ignored
  // during middle-click autoscroll.
  web_view->GetChromeClient().SetCursorForPlugin(WebCursorInfo(PointerCursor()),
                                                 local_frame);
  EXPECT_EQ(MiddlePanningCursor().GetType(), client.GetLastCursorType());

  // End middle-click autoscroll.
  mouse_event.SetType(WebInputEvent::kMouseDown);
  web_view->MainFrameWidget()->HandleInputEvent(
      WebCoalescedInputEvent(mouse_event));
  mouse_event.SetType(WebInputEvent::kMouseUp);
  web_view->MainFrameWidget()->HandleInputEvent(
      WebCoalescedInputEvent(mouse_event));

  web_view->GetChromeClient().SetCursorForPlugin(WebCursorInfo(IBeamCursor()),
                                                 local_frame);
  EXPECT_EQ(IBeamCursor().GetType(), client.GetLastCursorType());

  // Explicitly reset to break dependency on locally scoped client.
  web_view_helper_.Reset();
}

static void ConfigueCompositingWebView(WebSettings* settings) {
  settings->SetPreferCompositingToLCDTextEnabled(true);
}

TEST_F(WebViewTest, ShowPressOnTransformedLink) {
  frame_test_helpers::WebViewHelper web_view_helper;
  WebViewImpl* web_view_impl =
      web_view_helper.InitializeWithSettings(&ConfigueCompositingWebView);

  int page_width = 640;
  int page_height = 480;
  web_view_impl->MainFrameWidget()->Resize(WebSize(page_width, page_height));

  WebURL base_url = url_test_helpers::ToKURL("http://example.com/");
  frame_test_helpers::LoadHTMLString(
      web_view_impl->MainFrameImpl(),
      "<a href='http://www.test.com' style='position: absolute; left: 20px; "
      "top: 20px; width: 200px; transform:translateZ(0);'>A link to "
      "highlight</a>",
      base_url);

  WebGestureEvent event(WebInputEvent::kGestureShowPress,
                        WebInputEvent::kNoModifiers,
                        WebInputEvent::GetStaticTimeStampForTests(),
                        WebGestureDevice::kTouchscreen);
  event.SetPositionInWidget(WebFloatPoint(20, 20));

  // Just make sure we don't hit any asserts.
  web_view_impl->MainFrameWidget()->HandleInputEvent(
      WebCoalescedInputEvent(event));
}

class MockAutofillClient : public WebAutofillClient {
 public:
  MockAutofillClient() = default;

  ~MockAutofillClient() override = default;

  void TextFieldDidChange(const WebFormControlElement&) override {
    ++text_changes_;

    if (UserGestureIndicator::ProcessingUserGesture())
      ++text_changes_from_user_gesture_;
  }
  void UserGestureObserved() override { ++user_gesture_notifications_count_; }

  bool HasFillData(const WebFormControlElement&) const override {
    return has_fill_data_;
  }

  void SetFillData(bool has_fill_data) { has_fill_data_ = has_fill_data; }

  void ClearChangeCounts() { text_changes_ = 0; }

  int TextChanges() { return text_changes_; }
  int TextChangesFromUserGesture() { return text_changes_from_user_gesture_; }
  int GetUserGestureNotificationsCount() {
    return user_gesture_notifications_count_;
  }

 private:
  int text_changes_ = 0;
  int text_changes_from_user_gesture_ = 0;
  int user_gesture_notifications_count_ = 0;
  bool has_fill_data_ = false;
};

TEST_F(WebViewTest, LosingFocusDoesNotTriggerAutofillTextChange) {
  RegisterMockedHttpURLLoad("input_field_populated.html");
  MockAutofillClient client;
  WebViewImpl* web_view = web_view_helper_.InitializeAndLoad(
      base_url_ + "input_field_populated.html");
  WebLocalFrameImpl* frame = web_view->MainFrameImpl();
  frame->SetAutofillClient(&client);
  web_view->SetInitialFocus(false);

  // Set up a composition that needs to be committed.
  WebVector<WebImeTextSpan> empty_ime_text_spans;
  frame->SetEditableSelectionOffsets(4, 10);
  frame->SetCompositionFromExistingText(8, 12, empty_ime_text_spans);
  WebTextInputInfo info = frame->GetInputMethodController()->TextInputInfo();
  EXPECT_EQ(4, info.selection_start);
  EXPECT_EQ(10, info.selection_end);
  EXPECT_EQ(8, info.composition_start);
  EXPECT_EQ(12, info.composition_end);

  // Clear the focus and track that the subsequent composition commit does not
  // trigger a text changed notification for autofill.
  client.ClearChangeCounts();
  web_view->MainFrameWidget()->SetFocus(false);
  EXPECT_EQ(0, client.TextChanges());

  frame->SetAutofillClient(nullptr);
}

static void VerifySelectionAndComposition(WebViewImpl* web_view,
                                          int selection_start,
                                          int selection_end,
                                          int composition_start,
                                          int composition_end,
                                          const char* fail_message) {
  WebTextInputInfo info =
      web_view->MainFrameImpl()->GetInputMethodController()->TextInputInfo();
  EXPECT_EQ(selection_start, info.selection_start) << fail_message;
  EXPECT_EQ(selection_end, info.selection_end) << fail_message;
  EXPECT_EQ(composition_start, info.composition_start) << fail_message;
  EXPECT_EQ(composition_end, info.composition_end) << fail_message;
}

TEST_F(WebViewTest, CompositionNotCancelledByBackspace) {
  RegisterMockedHttpURLLoad("composition_not_cancelled_by_backspace.html");
  MockAutofillClient client;
  WebViewImpl* web_view = web_view_helper_.InitializeAndLoad(
      base_url_ + "composition_not_cancelled_by_backspace.html");
  WebLocalFrameImpl* frame = web_view->MainFrameImpl();
  frame->SetAutofillClient(&client);
  web_view->SetInitialFocus(false);

  // Test both input elements.
  for (int i = 0; i < 2; ++i) {
    // Select composition and do sanity check.
    WebVector<WebImeTextSpan> empty_ime_text_spans;
    frame->SetEditableSelectionOffsets(6, 6);
    WebInputMethodController* active_input_method_controller =
        frame->FrameWidget()->GetActiveWebInputMethodController();
    EXPECT_TRUE(active_input_method_controller->SetComposition(
        "fghij", empty_ime_text_spans, WebRange(), 0, 5));
    frame->SetEditableSelectionOffsets(11, 11);
    VerifySelectionAndComposition(web_view, 11, 11, 6, 11, "initial case");

    // Press Backspace and verify composition didn't get cancelled. This is to
    // verify the fix for crbug.com/429916.
    WebKeyboardEvent key_event(WebInputEvent::kRawKeyDown,
                               WebInputEvent::kNoModifiers,
                               WebInputEvent::GetStaticTimeStampForTests());
    key_event.dom_key = ui::DomKey::BACKSPACE;
    key_event.windows_key_code = VKEY_BACK;
    web_view->MainFrameWidget()->HandleInputEvent(
        WebCoalescedInputEvent(key_event));

    frame->SetEditableSelectionOffsets(6, 6);
    EXPECT_TRUE(active_input_method_controller->SetComposition(
        "fghi", empty_ime_text_spans, WebRange(), 0, 4));
    frame->SetEditableSelectionOffsets(10, 10);
    VerifySelectionAndComposition(web_view, 10, 10, 6, 10,
                                  "after pressing Backspace");

    key_event.SetType(WebInputEvent::kKeyUp);
    web_view->MainFrameWidget()->HandleInputEvent(
        WebCoalescedInputEvent(key_event));

    web_view->AdvanceFocus(false);
  }

  frame->SetAutofillClient(nullptr);
}

TEST_F(WebViewTest, FinishComposingTextDoesntTriggerAutofillTextChange) {
  RegisterMockedHttpURLLoad("input_field_populated.html");
  MockAutofillClient client;
  WebViewImpl* web_view = web_view_helper_.InitializeAndLoad(
      base_url_ + "input_field_populated.html");
  WebLocalFrameImpl* frame = web_view->MainFrameImpl();
  frame->SetAutofillClient(&client);
  web_view->SetInitialFocus(false);

  WebDocument document = web_view->MainFrameImpl()->GetDocument();
  HTMLFormControlElement* form =
      ToHTMLFormControlElement(document.GetElementById("sample"));

  WebInputMethodController* active_input_method_controller =
      frame->FrameWidget()->GetActiveWebInputMethodController();
  // Set up a composition that needs to be committed.
  std::string composition_text("testingtext");

  WebVector<WebImeTextSpan> empty_ime_text_spans;
  active_input_method_controller->SetComposition(
      WebString::FromUTF8(composition_text.c_str()), empty_ime_text_spans,
      WebRange(), 0, composition_text.length());

  WebTextInputInfo info = active_input_method_controller->TextInputInfo();
  EXPECT_EQ(0, info.selection_start);
  EXPECT_EQ((int)composition_text.length(), info.selection_end);
  EXPECT_EQ(0, info.composition_start);
  EXPECT_EQ((int)composition_text.length(), info.composition_end);

  form->SetAutofillState(blink::WebAutofillState::kAutofilled);
  client.ClearChangeCounts();

  active_input_method_controller->FinishComposingText(
      WebInputMethodController::kKeepSelection);
  EXPECT_EQ(0, client.TextChanges());

  EXPECT_TRUE(form->IsAutofilled());

  frame->SetAutofillClient(nullptr);
}

TEST_F(WebViewTest,
       SetCompositionFromExistingTextDoesntTriggerAutofillTextChange) {
  RegisterMockedHttpURLLoad("input_field_populated.html");
  MockAutofillClient client;
  WebViewImpl* web_view = web_view_helper_.InitializeAndLoad(
      base_url_ + "input_field_populated.html");
  WebLocalFrameImpl* frame = web_view->MainFrameImpl();
  frame->SetAutofillClient(&client);
  web_view->SetInitialFocus(false);

  WebVector<WebImeTextSpan> empty_ime_text_spans;

  client.ClearChangeCounts();
  frame->SetCompositionFromExistingText(8, 12, empty_ime_text_spans);

  WebTextInputInfo info = frame->GetInputMethodController()->TextInputInfo();
  EXPECT_EQ("0123456789abcdefghijklmnopqrstuvwxyz",
            std::string(info.value.Utf8().data()));
  EXPECT_EQ(8, info.composition_start);
  EXPECT_EQ(12, info.composition_end);

  EXPECT_EQ(0, client.TextChanges());

  WebDocument document = web_view->MainFrameImpl()->GetDocument();
  EXPECT_EQ(WebString::FromUTF8("none"),
            document.GetElementById("inputEvent").FirstChild().NodeValue());

  frame->SetAutofillClient(nullptr);
}

class ViewCreatingWebViewClient : public frame_test_helpers::TestWebViewClient {
 public:
  ViewCreatingWebViewClient() : did_focus_called_(false) {}

  // WebViewClient methods
  WebView* CreateView(WebLocalFrame* opener,
                      const WebURLRequest&,
                      const WebWindowFeatures&,
                      const WebString& name,
                      WebNavigationPolicy,
                      WebSandboxFlags,
                      const FeaturePolicy::FeatureState&,
                      const SessionStorageNamespaceId&) override {
    return web_view_helper_.InitializeWithOpener(opener);
  }

  // WebWidgetClient methods
  void DidFocus(WebLocalFrame*) override { did_focus_called_ = true; }

  bool DidFocusCalled() const { return did_focus_called_; }
  WebView* CreatedWebView() const { return web_view_helper_.GetWebView(); }

 private:
  frame_test_helpers::WebViewHelper web_view_helper_;
  bool did_focus_called_;
};

TEST_F(WebViewTest, DoNotFocusCurrentFrameOnNavigateFromLocalFrame) {
  ViewCreatingWebViewClient client;
  frame_test_helpers::WebViewHelper web_view_helper;
  WebViewImpl* web_view_impl = web_view_helper.Initialize(nullptr, &client);

  WebURL base_url = url_test_helpers::ToKURL("http://example.com/");
  frame_test_helpers::LoadHTMLString(
      web_view_impl->MainFrameImpl(),
      "<html><body><iframe src=\"about:blank\"></iframe></body></html>",
      base_url);

  // Make a request from a local frame.
  WebURLRequest web_url_request_with_target_start(KURL("about:blank"));
  LocalFrame* local_frame =
      To<WebLocalFrameImpl>(web_view_impl->MainFrame()->FirstChild())
          ->GetFrame();
  FrameLoadRequest request_with_target_start(
      local_frame->GetDocument(),
      web_url_request_with_target_start.ToResourceRequest(), "_top");
  local_frame->Tree().FindOrCreateFrameForNavigation(request_with_target_start);
  EXPECT_FALSE(client.DidFocusCalled());

  web_view_helper.Reset();  // Remove dependency on locally scoped client.
}

TEST_F(WebViewTest, FocusExistingFrameOnNavigate) {
  ViewCreatingWebViewClient client;
  frame_test_helpers::WebViewHelper web_view_helper;
  WebViewImpl* web_view_impl = web_view_helper.Initialize(nullptr, &client);
  WebLocalFrameImpl* frame = web_view_impl->MainFrameImpl();
  frame->SetName("_start");

  // Make a request that will open a new window
  WebURLRequest web_url_request(KURL("about:blank"));
  FrameLoadRequest request(nullptr, web_url_request.ToResourceRequest(),
                           "_blank");
  To<LocalFrame>(web_view_impl->GetPage()->MainFrame())
      ->Tree()
      .FindOrCreateFrameForNavigation(request);
  ASSERT_TRUE(client.CreatedWebView());
  EXPECT_FALSE(client.DidFocusCalled());

  // Make a request from the new window that will navigate the original window.
  // The original window should be focused.
  WebURLRequest web_url_request_with_target_start(KURL("about:blank"));
  FrameLoadRequest request_with_target_start(
      nullptr, web_url_request_with_target_start.ToResourceRequest(), "_start");
  To<LocalFrame>(static_cast<WebViewImpl*>(client.CreatedWebView())
                     ->GetPage()
                     ->MainFrame())
      ->Tree()
      .FindOrCreateFrameForNavigation(request_with_target_start);
  EXPECT_TRUE(client.DidFocusCalled());

  web_view_helper.Reset();  // Remove dependency on locally scoped client.
}

class ViewReusingWebViewClient : public frame_test_helpers::TestWebViewClient {
 public:
  ViewReusingWebViewClient() = default;

  // WebViewClient methods
  WebView* CreateView(WebLocalFrame*,
                      const WebURLRequest&,
                      const WebWindowFeatures&,
                      const WebString& name,
                      WebNavigationPolicy,
                      WebSandboxFlags,
                      const FeaturePolicy::FeatureState&,
                      const SessionStorageNamespaceId&) override {
    return web_view_;
  }

  void SetWebView(WebView* view) { web_view_ = view; }

 private:
  WebView* web_view_ = nullptr;
};

TEST_F(WebViewTest,
       ReuseExistingWindowOnCreateViewUsesCorrectNavigationPolicy) {
  ViewReusingWebViewClient view_client;
  frame_test_helpers::WebViewHelper web_view_helper;
  WebViewImpl* web_view_impl =
      web_view_helper.Initialize(nullptr, &view_client);
  view_client.SetWebView(web_view_impl);
  LocalFrame* frame = To<LocalFrame>(web_view_impl->GetPage()->MainFrame());

  // Request a new window, but the WebViewClient will decline to and instead
  // return the current window.
  WebURLRequest web_url_request(KURL("about:blank"));
  FrameLoadRequest request(frame->GetDocument(),
                           web_url_request.ToResourceRequest(), "_blank");
  FrameTree::FindResult result =
      frame->Tree().FindOrCreateFrameForNavigation(request);
  EXPECT_EQ(frame, result.frame);
  EXPECT_EQ(kNavigationPolicyCurrentTab, request.GetNavigationPolicy());
}

TEST_F(WebViewTest, DispatchesFocusOutFocusInOnViewToggleFocus) {
  RegisterMockedHttpURLLoad("focusout_focusin_events.html");
  WebViewImpl* web_view = web_view_helper_.InitializeAndLoad(
      base_url_ + "focusout_focusin_events.html");

  web_view->MainFrameWidget()->SetFocus(true);
  web_view->MainFrameWidget()->SetFocus(false);
  web_view->MainFrameWidget()->SetFocus(true);

  WebElement element =
      web_view->MainFrameImpl()->GetDocument().GetElementById("message");
  EXPECT_STREQ("focusoutfocusin", element.TextContent().Utf8().data());
}

TEST_F(WebViewTest, DispatchesDomFocusOutDomFocusInOnViewToggleFocus) {
  RegisterMockedHttpURLLoad("domfocusout_domfocusin_events.html");
  WebViewImpl* web_view = web_view_helper_.InitializeAndLoad(
      base_url_ + "domfocusout_domfocusin_events.html");

  web_view->MainFrameWidget()->SetFocus(true);
  web_view->MainFrameWidget()->SetFocus(false);
  web_view->MainFrameWidget()->SetFocus(true);

  WebElement element =
      web_view->MainFrameImpl()->GetDocument().GetElementById("message");
  EXPECT_STREQ("DOMFocusOutDOMFocusIn", element.TextContent().Utf8().data());
}

static void OpenDateTimeChooser(WebView* web_view,
                                HTMLInputElement* input_element) {
  input_element->focus();

  WebKeyboardEvent key_event(WebInputEvent::kRawKeyDown,
                             WebInputEvent::kNoModifiers,
                             WebInputEvent::GetStaticTimeStampForTests());
  key_event.dom_key = ui::DomKey::FromCharacter(' ');
  key_event.windows_key_code = VKEY_SPACE;
  web_view->MainFrameWidget()->HandleInputEvent(
      WebCoalescedInputEvent(key_event));

  key_event.SetType(WebInputEvent::kKeyUp);
  web_view->MainFrameWidget()->HandleInputEvent(
      WebCoalescedInputEvent(key_event));
}

TEST_F(WebViewTest, ChooseValueFromDateTimeChooser) {
  ScopedInputMultipleFieldsUIForTest input_multiple_fields_ui(false);
  std::string url = RegisterMockedHttpURLLoad("date_time_chooser.html");
  WebViewImpl* web_view_impl =
      web_view_helper_.InitializeAndLoad(url, nullptr, nullptr);

  Document* document =
      web_view_impl->MainFrameImpl()->GetFrame()->GetDocument();

  HTMLInputElement* input_element;

  input_element = ToHTMLInputElement(document->getElementById("date"));
  OpenDateTimeChooser(web_view_impl, input_element);
  GetExternalDateTimeChooser(web_view_impl)->ResponseHandler(true, 0);
  EXPECT_STREQ("1970-01-01", input_element->value().Utf8().data());

  OpenDateTimeChooser(web_view_impl, input_element);
  GetExternalDateTimeChooser(web_view_impl)
      ->ResponseHandler(true, std::numeric_limits<double>::quiet_NaN());
  EXPECT_STREQ("", input_element->value().Utf8().data());

  input_element = ToHTMLInputElement(document->getElementById("datetimelocal"));
  OpenDateTimeChooser(web_view_impl, input_element);
  GetExternalDateTimeChooser(web_view_impl)->ResponseHandler(true, 0);
  EXPECT_STREQ("1970-01-01T00:00", input_element->value().Utf8().data());

  OpenDateTimeChooser(web_view_impl, input_element);
  GetExternalDateTimeChooser(web_view_impl)
      ->ResponseHandler(true, std::numeric_limits<double>::quiet_NaN());
  EXPECT_STREQ("", input_element->value().Utf8().data());

  input_element = ToHTMLInputElement(document->getElementById("month"));
  OpenDateTimeChooser(web_view_impl, input_element);
  GetExternalDateTimeChooser(web_view_impl)->ResponseHandler(true, 0);
  EXPECT_STREQ("1970-01", input_element->value().Utf8().data());

  OpenDateTimeChooser(web_view_impl, input_element);
  GetExternalDateTimeChooser(web_view_impl)
      ->ResponseHandler(true, std::numeric_limits<double>::quiet_NaN());
  EXPECT_STREQ("", input_element->value().Utf8().data());

  input_element = ToHTMLInputElement(document->getElementById("time"));
  OpenDateTimeChooser(web_view_impl, input_element);
  GetExternalDateTimeChooser(web_view_impl)->ResponseHandler(true, 0);
  EXPECT_STREQ("00:00", input_element->value().Utf8().data());

  OpenDateTimeChooser(web_view_impl, input_element);
  GetExternalDateTimeChooser(web_view_impl)
      ->ResponseHandler(true, std::numeric_limits<double>::quiet_NaN());
  EXPECT_STREQ("", input_element->value().Utf8().data());

  input_element = ToHTMLInputElement(document->getElementById("week"));
  OpenDateTimeChooser(web_view_impl, input_element);
  GetExternalDateTimeChooser(web_view_impl)->ResponseHandler(true, 0);
  EXPECT_STREQ("1970-W01", input_element->value().Utf8().data());

  OpenDateTimeChooser(web_view_impl, input_element);
  GetExternalDateTimeChooser(web_view_impl)
      ->ResponseHandler(true, std::numeric_limits<double>::quiet_NaN());
  EXPECT_STREQ("", input_element->value().Utf8().data());

  // Clear the WebViewClient from the webViewHelper to avoid use-after-free in
  // the WebViewHelper destructor.
  web_view_helper_.Reset();
}

TEST_F(WebViewTest, DispatchesFocusBlurOnViewToggle) {
  RegisterMockedHttpURLLoad("focus_blur_events.html");
  WebViewImpl* web_view =
      web_view_helper_.InitializeAndLoad(base_url_ + "focus_blur_events.html");

  web_view->MainFrameWidget()->SetFocus(true);
  web_view->MainFrameWidget()->SetFocus(false);
  web_view->MainFrameWidget()->SetFocus(true);

  WebElement element =
      web_view->MainFrameImpl()->GetDocument().GetElementById("message");
  // Expect not to see duplication of events.
  EXPECT_STREQ("blurfocus", element.TextContent().Utf8().data());
}

class CreateChildCounterFrameClient
    : public frame_test_helpers::TestWebFrameClient {
 public:
  CreateChildCounterFrameClient() : count_(0) {}
  WebLocalFrame* CreateChildFrame(WebLocalFrame* parent,
                                  WebTreeScopeType,
                                  const WebString& name,
                                  const WebString& fallback_name,
                                  const FramePolicy&,
                                  const WebFrameOwnerProperties&,
                                  FrameOwnerElementType) override;

  int Count() const { return count_; }

 private:
  int count_;
};

WebLocalFrame* CreateChildCounterFrameClient::CreateChildFrame(
    WebLocalFrame* parent,
    WebTreeScopeType scope,
    const WebString& name,
    const WebString& fallback_name,
    const FramePolicy& frame_policy,
    const WebFrameOwnerProperties& frame_owner_properties,
    FrameOwnerElementType frame_owner_element_type) {
  ++count_;
  return TestWebFrameClient::CreateChildFrame(
      parent, scope, name, fallback_name, frame_policy, frame_owner_properties,
      frame_owner_element_type);
}

TEST_F(WebViewTest, ChangeDisplayMode) {
  RegisterMockedHttpURLLoad("display_mode.html");
  WebView* web_view =
      web_view_helper_.InitializeAndLoad(base_url_ + "display_mode.html");

  std::string content =
      WebFrameContentDumper::DumpWebViewAsText(web_view, 21).Utf8();
  EXPECT_EQ("regular-ui", content);

  web_view->SetDisplayMode(kWebDisplayModeMinimalUi);
  content = WebFrameContentDumper::DumpWebViewAsText(web_view, 21).Utf8();
  EXPECT_EQ("minimal-ui", content);
  web_view_helper_.Reset();
}

TEST_F(WebViewTest, AddFrameInCloseUnload) {
  CreateChildCounterFrameClient frame_client;
  RegisterMockedHttpURLLoad("add_frame_in_unload.html");
  web_view_helper_.InitializeAndLoad(base_url_ + "add_frame_in_unload.html",
                                     &frame_client);
  web_view_helper_.Reset();
  EXPECT_EQ(0, frame_client.Count());
}

TEST_F(WebViewTest, AddFrameInCloseURLUnload) {
  CreateChildCounterFrameClient frame_client;
  RegisterMockedHttpURLLoad("add_frame_in_unload.html");
  web_view_helper_.InitializeAndLoad(base_url_ + "add_frame_in_unload.html",
                                     &frame_client);
  web_view_helper_.LocalMainFrame()->DispatchUnloadEvent();
  EXPECT_EQ(0, frame_client.Count());
  web_view_helper_.Reset();
}

TEST_F(WebViewTest, AddFrameInNavigateUnload) {
  CreateChildCounterFrameClient frame_client;
  RegisterMockedHttpURLLoad("add_frame_in_unload.html");
  web_view_helper_.InitializeAndLoad(base_url_ + "add_frame_in_unload.html",
                                     &frame_client);
  frame_test_helpers::LoadFrame(web_view_helper_.GetWebView()->MainFrameImpl(),
                                "about:blank");
  EXPECT_EQ(0, frame_client.Count());
  web_view_helper_.Reset();
}

TEST_F(WebViewTest, AddFrameInChildInNavigateUnload) {
  CreateChildCounterFrameClient frame_client;
  RegisterMockedHttpURLLoad("add_frame_in_unload_wrapper.html");
  RegisterMockedHttpURLLoad("add_frame_in_unload.html");
  web_view_helper_.InitializeAndLoad(
      base_url_ + "add_frame_in_unload_wrapper.html", &frame_client);
  frame_test_helpers::LoadFrame(web_view_helper_.GetWebView()->MainFrameImpl(),
                                "about:blank");
  EXPECT_EQ(1, frame_client.Count());
  web_view_helper_.Reset();
}

class TouchEventHandlerWebWidgetClient
    : public frame_test_helpers::TestWebWidgetClient {
 public:
  // WebWidgetClient methods
  void HasTouchEventHandlers(bool state) override {
    // Only count the times the state changes.
    if (state != has_touch_event_handler_)
      has_touch_event_handler_count_[state]++;
    has_touch_event_handler_ = state;
  }

  // Local methods
  TouchEventHandlerWebWidgetClient()
      : has_touch_event_handler_count_(), has_touch_event_handler_(false) {}

  int GetAndResetHasTouchEventHandlerCallCount(bool state) {
    int value = has_touch_event_handler_count_[state];
    has_touch_event_handler_count_[state] = 0;
    return value;
  }

 private:
  int has_touch_event_handler_count_[2];
  bool has_touch_event_handler_;
};

// This test verifies that WebWidgetClient::hasTouchEventHandlers is called
// accordingly for various calls to EventHandlerRegistry::did{Add|Remove|
// RemoveAll}EventHandler(..., TouchEvent). Verifying that those calls are made
// correctly is the job of web_tests/fast/events/event-handler-count.html.
TEST_F(WebViewTest, HasTouchEventHandlers) {
  TouchEventHandlerWebWidgetClient client;
  std::string url = RegisterMockedHttpURLLoad("has_touch_event_handlers.html");
  WebViewImpl* web_view_impl =
      web_view_helper_.InitializeAndLoad(url, nullptr, nullptr, &client);
  const EventHandlerRegistry::EventHandlerClass kTouchEvent =
      EventHandlerRegistry::kTouchStartOrMoveEventBlocking;

  // The page is initialized with at least one no-handlers call.
  // In practice we get two such calls because WebViewHelper::initializeAndLoad
  // first initializes an empty frame, and then loads a document into it, so
  // there are two FrameLoader::commitProvisionalLoad calls.
  EXPECT_EQ(0, client.GetAndResetHasTouchEventHandlerCallCount(false));
  EXPECT_EQ(0, client.GetAndResetHasTouchEventHandlerCallCount(true));

  // Adding the first document handler results in a has-handlers call.
  Document* document =
      web_view_impl->MainFrameImpl()->GetFrame()->GetDocument();
  EventHandlerRegistry* registry =
      &document->GetFrame()->GetEventHandlerRegistry();
  registry->DidAddEventHandler(*document, kTouchEvent);
  EXPECT_EQ(0, client.GetAndResetHasTouchEventHandlerCallCount(false));
  EXPECT_EQ(1, client.GetAndResetHasTouchEventHandlerCallCount(true));

  // Adding another handler has no effect.
  registry->DidAddEventHandler(*document, kTouchEvent);
  EXPECT_EQ(0, client.GetAndResetHasTouchEventHandlerCallCount(false));
  EXPECT_EQ(0, client.GetAndResetHasTouchEventHandlerCallCount(true));

  // Removing the duplicate handler has no effect.
  registry->DidRemoveEventHandler(*document, kTouchEvent);
  EXPECT_EQ(0, client.GetAndResetHasTouchEventHandlerCallCount(false));
  EXPECT_EQ(0, client.GetAndResetHasTouchEventHandlerCallCount(true));

  // Removing the final handler results in a no-handlers call.
  registry->DidRemoveEventHandler(*document, kTouchEvent);
  EXPECT_EQ(1, client.GetAndResetHasTouchEventHandlerCallCount(false));
  EXPECT_EQ(0, client.GetAndResetHasTouchEventHandlerCallCount(true));

  // Adding a handler on a div results in a has-handlers call.
  Element* parent_div = document->getElementById("parentdiv");
  DCHECK(parent_div);
  registry->DidAddEventHandler(*parent_div, kTouchEvent);
  EXPECT_EQ(0, client.GetAndResetHasTouchEventHandlerCallCount(false));
  EXPECT_EQ(1, client.GetAndResetHasTouchEventHandlerCallCount(true));

  // Adding a duplicate handler on the div, clearing all document handlers
  // (of which there are none) and removing the extra handler on the div
  // all have no effect.
  registry->DidAddEventHandler(*parent_div, kTouchEvent);
  registry->DidRemoveAllEventHandlers(*document);
  registry->DidRemoveEventHandler(*parent_div, kTouchEvent);
  EXPECT_EQ(0, client.GetAndResetHasTouchEventHandlerCallCount(false));
  EXPECT_EQ(0, client.GetAndResetHasTouchEventHandlerCallCount(true));

  // Removing the final handler on the div results in a no-handlers call.
  registry->DidRemoveEventHandler(*parent_div, kTouchEvent);
  EXPECT_EQ(1, client.GetAndResetHasTouchEventHandlerCallCount(false));
  EXPECT_EQ(0, client.GetAndResetHasTouchEventHandlerCallCount(true));

  // Adding two handlers then clearing them in a single call results in a
  // has-handlers then no-handlers call.
  registry->DidAddEventHandler(*parent_div, kTouchEvent);
  EXPECT_EQ(0, client.GetAndResetHasTouchEventHandlerCallCount(false));
  EXPECT_EQ(1, client.GetAndResetHasTouchEventHandlerCallCount(true));
  registry->DidAddEventHandler(*parent_div, kTouchEvent);
  EXPECT_EQ(0, client.GetAndResetHasTouchEventHandlerCallCount(false));
  EXPECT_EQ(0, client.GetAndResetHasTouchEventHandlerCallCount(true));
  registry->DidRemoveAllEventHandlers(*parent_div);
  EXPECT_EQ(1, client.GetAndResetHasTouchEventHandlerCallCount(false));
  EXPECT_EQ(0, client.GetAndResetHasTouchEventHandlerCallCount(true));

  // Adding a handler inside of a child iframe results in a has-handlers call.
  Element* child_frame = document->getElementById("childframe");
  DCHECK(child_frame);
  Document* child_document =
      ToHTMLIFrameElement(child_frame)->contentDocument();
  Element* child_div = child_document->getElementById("childdiv");
  DCHECK(child_div);
  registry->DidAddEventHandler(*child_div, kTouchEvent);
  EXPECT_EQ(0, client.GetAndResetHasTouchEventHandlerCallCount(false));
  EXPECT_EQ(1, client.GetAndResetHasTouchEventHandlerCallCount(true));

  // Adding and clearing handlers in the parent doc or elsewhere in the child
  // doc has no impact.
  registry->DidAddEventHandler(*document, kTouchEvent);
  registry->DidAddEventHandler(*child_frame, kTouchEvent);
  registry->DidAddEventHandler(*child_document, kTouchEvent);
  registry->DidRemoveAllEventHandlers(*document);
  registry->DidRemoveAllEventHandlers(*child_frame);
  registry->DidRemoveAllEventHandlers(*child_document);
  EXPECT_EQ(0, client.GetAndResetHasTouchEventHandlerCallCount(false));
  EXPECT_EQ(0, client.GetAndResetHasTouchEventHandlerCallCount(true));

  // Removing the final handler inside the child frame results in a no-handlers
  // call.
  registry->DidRemoveAllEventHandlers(*child_div);
  EXPECT_EQ(1, client.GetAndResetHasTouchEventHandlerCallCount(false));
  EXPECT_EQ(0, client.GetAndResetHasTouchEventHandlerCallCount(true));

  // Adding a handler inside the child frame results in a has-handlers call.
  registry->DidAddEventHandler(*child_document, kTouchEvent);
  EXPECT_EQ(0, client.GetAndResetHasTouchEventHandlerCallCount(false));
  EXPECT_EQ(1, client.GetAndResetHasTouchEventHandlerCallCount(true));

  // Adding a handler in the parent document and removing the one in the frame
  // has no effect.
  registry->DidAddEventHandler(*child_frame, kTouchEvent);
  registry->DidRemoveEventHandler(*child_document, kTouchEvent);
  registry->DidRemoveAllEventHandlers(*child_document);
  registry->DidRemoveAllEventHandlers(*document);
  EXPECT_EQ(0, client.GetAndResetHasTouchEventHandlerCallCount(false));
  EXPECT_EQ(0, client.GetAndResetHasTouchEventHandlerCallCount(true));

  // Now removing the handler in the parent document results in a no-handlers
  // call.
  registry->DidRemoveEventHandler(*child_frame, kTouchEvent);
  EXPECT_EQ(1, client.GetAndResetHasTouchEventHandlerCallCount(false));
  EXPECT_EQ(0, client.GetAndResetHasTouchEventHandlerCallCount(true));

  // Free the webView before the TouchEventHandlerWebViewClient gets freed.
  web_view_helper_.Reset();
}

// This test checks that deleting nodes which have only non-JS-registered touch
// handlers also removes them from the event handler registry. Note that this
// is different from detaching and re-attaching the same node, which is covered
// by web tests under fast/events/.
TEST_F(WebViewTest, DeleteElementWithRegisteredHandler) {
  std::string url = RegisterMockedHttpURLLoad("simple_div.html");
  WebViewImpl* web_view_impl = web_view_helper_.InitializeAndLoad(url);

  Persistent<Document> document =
      web_view_impl->MainFrameImpl()->GetFrame()->GetDocument();
  Element* div = document->getElementById("div");
  EventHandlerRegistry& registry =
      document->GetFrame()->GetEventHandlerRegistry();

  registry.DidAddEventHandler(*div, EventHandlerRegistry::kScrollEvent);
  EXPECT_TRUE(registry.HasEventHandlers(EventHandlerRegistry::kScrollEvent));

  DummyExceptionStateForTesting exception_state;
  div->remove(exception_state);

  // For oilpan we have to force a GC to ensure the event handlers have been
  // removed when checking below. We do a precise GC (collectAllGarbage does not
  // scan the stack) to ensure the div element dies. This is also why the
  // Document is in a Persistent since we want that to stay around.
  ThreadState::Current()->CollectAllGarbageForTesting();

  EXPECT_FALSE(registry.HasEventHandlers(EventHandlerRegistry::kScrollEvent));
}

// This test verifies the text input flags are correctly exposed to script.
TEST_F(WebViewTest, TextInputFlags) {
  std::string url = RegisterMockedHttpURLLoad("text_input_flags.html");
  WebViewImpl* web_view_impl = web_view_helper_.InitializeAndLoad(url);
  web_view_impl->SetInitialFocus(false);

  WebLocalFrameImpl* frame = web_view_impl->MainFrameImpl();
  WebInputMethodController* active_input_method_controller =
      frame->GetInputMethodController();
  Document* document = frame->GetFrame()->GetDocument();

  // (A) <input>
  // (A.1) Verifies autocorrect/autocomplete/spellcheck flags are Off and
  // autocapitalize is set to none.
  HTMLInputElement* input_element =
      ToHTMLInputElement(document->getElementById("input"));
  document->SetFocusedElement(
      input_element,
      FocusParams(SelectionBehaviorOnFocus::kNone, kWebFocusTypeNone, nullptr));
  web_view_impl->MainFrameWidget()->SetFocus(true);
  WebTextInputInfo info1 = active_input_method_controller->TextInputInfo();
  EXPECT_EQ(kWebTextInputFlagAutocompleteOff | kWebTextInputFlagAutocorrectOff |
                kWebTextInputFlagSpellcheckOff |
                kWebTextInputFlagAutocapitalizeNone,
            info1.flags);

  // (A.2) Verifies autocorrect/autocomplete/spellcheck flags are On and
  // autocapitalize is set to sentences.
  input_element = ToHTMLInputElement(document->getElementById("input2"));
  document->SetFocusedElement(
      input_element,
      FocusParams(SelectionBehaviorOnFocus::kNone, kWebFocusTypeNone, nullptr));
  web_view_impl->MainFrameWidget()->SetFocus(true);
  WebTextInputInfo info2 = active_input_method_controller->TextInputInfo();
  EXPECT_EQ(kWebTextInputFlagAutocompleteOn | kWebTextInputFlagAutocorrectOn |
                kWebTextInputFlagSpellcheckOn |
                kWebTextInputFlagAutocapitalizeSentences,
            info2.flags);

  // (B) <textarea> Verifies the default text input flags are
  // WebTextInputFlagAutocapitalizeSentences.
  HTMLTextAreaElement* text_area_element =
      ToHTMLTextAreaElement(document->getElementById("textarea"));
  document->SetFocusedElement(
      text_area_element,
      FocusParams(SelectionBehaviorOnFocus::kNone, kWebFocusTypeNone, nullptr));
  web_view_impl->MainFrameWidget()->SetFocus(true);
  WebTextInputInfo info3 = active_input_method_controller->TextInputInfo();
  EXPECT_EQ(kWebTextInputFlagAutocapitalizeSentences, info3.flags);

  // (C) Verifies the WebTextInputInfo's don't equal.
  EXPECT_FALSE(info1.Equals(info2));
  EXPECT_FALSE(info2.Equals(info3));

  // Free the webView before freeing the NonUserInputTextUpdateWebViewClient.
  web_view_helper_.Reset();
}

// Check that the WebAutofillClient is correctly notified about first user
// gestures after load, following various input events.
TEST_F(WebViewTest, FirstUserGestureObservedKeyEvent) {
  RegisterMockedHttpURLLoad("form.html");
  MockAutofillClient client;
  WebViewImpl* web_view =
      web_view_helper_.InitializeAndLoad(base_url_ + "form.html");
  WebLocalFrameImpl* frame = web_view->MainFrameImpl();
  frame->SetAutofillClient(&client);
  web_view->SetInitialFocus(false);

  EXPECT_EQ(0, client.GetUserGestureNotificationsCount());

  WebKeyboardEvent key_event(WebInputEvent::kRawKeyDown,
                             WebInputEvent::kNoModifiers,
                             WebInputEvent::GetStaticTimeStampForTests());
  key_event.dom_key = ui::DomKey::FromCharacter(' ');
  key_event.windows_key_code = VKEY_SPACE;
  web_view->MainFrameWidget()->HandleInputEvent(
      WebCoalescedInputEvent(key_event));
  key_event.SetType(WebInputEvent::kKeyUp);
  web_view->MainFrameWidget()->HandleInputEvent(
      WebCoalescedInputEvent(key_event));

  EXPECT_EQ(2, client.GetUserGestureNotificationsCount());
  frame->SetAutofillClient(nullptr);
}

TEST_F(WebViewTest, FirstUserGestureObservedMouseEvent) {
  RegisterMockedHttpURLLoad("form.html");
  MockAutofillClient client;
  WebViewImpl* web_view =
      web_view_helper_.InitializeAndLoad(base_url_ + "form.html");
  WebLocalFrameImpl* frame = web_view->MainFrameImpl();
  frame->SetAutofillClient(&client);
  web_view->SetInitialFocus(false);

  EXPECT_EQ(0, client.GetUserGestureNotificationsCount());

  WebMouseEvent mouse_event(WebInputEvent::kMouseDown,
                            WebInputEvent::kNoModifiers,
                            WebInputEvent::GetStaticTimeStampForTests());
  mouse_event.button = WebMouseEvent::Button::kLeft;
  mouse_event.SetPositionInWidget(1, 1);
  mouse_event.click_count = 1;
  web_view->MainFrameWidget()->HandleInputEvent(
      WebCoalescedInputEvent(mouse_event));
  mouse_event.SetType(WebInputEvent::kMouseUp);
  web_view->MainFrameWidget()->HandleInputEvent(
      WebCoalescedInputEvent(mouse_event));

  EXPECT_EQ(1, client.GetUserGestureNotificationsCount());
  frame->SetAutofillClient(nullptr);
}

TEST_F(WebViewTest, CompositionIsUserGesture) {
  RegisterMockedHttpURLLoad("input_field_populated.html");
  WebViewImpl* web_view = web_view_helper_.InitializeAndLoad(
      base_url_ + "input_field_populated.html");
  WebLocalFrameImpl* frame = web_view->MainFrameImpl();
  MockAutofillClient client;
  frame->SetAutofillClient(&client);
  web_view->SetInitialFocus(false);

  EXPECT_TRUE(
      frame->FrameWidget()->GetActiveWebInputMethodController()->SetComposition(
          WebString::FromUTF8(std::string("hello").c_str()),
          WebVector<WebImeTextSpan>(), WebRange(), 3, 3));
  EXPECT_EQ(1, client.TextChangesFromUserGesture());
  EXPECT_FALSE(UserGestureIndicator::ProcessingUserGesture());
  EXPECT_TRUE(frame->HasMarkedText());

  frame->SetAutofillClient(nullptr);
}

// Currently, SelectionAsText() is built upon TextIterator, but
// WebFrameContentDumper is built upon TextDumperForTests. Their results can
// be different, making the test fail.
// TODO(crbug.com/781434): Build a selection serializer upon TextDumperForTests.
TEST_F(WebViewTest, DISABLED_CompareSelectAllToContentAsText) {
  RegisterMockedHttpURLLoad("longpress_selection.html");
  WebViewImpl* web_view = web_view_helper_.InitializeAndLoad(
      base_url_ + "longpress_selection.html");

  WebLocalFrameImpl* frame = web_view->MainFrameImpl();
  frame->ExecuteScript(WebScriptSource(
      WebString::FromUTF8("document.execCommand('SelectAll', false, null)")));
  std::string actual = frame->SelectionAsText().Utf8();

  const int kMaxOutputCharacters = 1024;
  std::string expected =
      WebFrameContentDumper::DumpWebViewAsText(web_view, kMaxOutputCharacters)
          .Utf8();
  EXPECT_EQ(expected, actual);
}

TEST_F(WebViewTest, AutoResizeSubtreeLayout) {
  std::string url = RegisterMockedHttpURLLoad("subtree-layout.html");
  WebViewImpl* web_view = web_view_helper_.Initialize();

  web_view->EnableAutoResizeMode(WebSize(200, 200), WebSize(200, 200));
  LoadFrame(web_view->MainFrameImpl(), url);

  LocalFrameView* frame_view =
      web_view_helper_.LocalMainFrame()->GetFrameView();

  // Auto-resizing used to DCHECK(needsLayout()) in LayoutBlockFlow::layout.
  // This EXPECT is merely a dummy. The real test is that we don't trigger
  // asserts in debug builds.
  EXPECT_FALSE(frame_view->NeedsLayout());
}

TEST_F(WebViewTest, PreferredSize) {
  std::string url = base_url_ + "specify_size.html?100px:100px";
  url_test_helpers::RegisterMockedURLLoad(
      ToKURL(url), test::CoreTestDataPath("specify_size.html"));
  WebView* web_view = web_view_helper_.InitializeAndLoad(url);

  WebSize size = web_view->ContentsPreferredMinimumSize();
  EXPECT_EQ(100, size.width);
  EXPECT_EQ(100, size.height);

  web_view->SetZoomLevel(WebView::ZoomFactorToZoomLevel(2.0));
  size = web_view->ContentsPreferredMinimumSize();
  EXPECT_EQ(200, size.width);
  EXPECT_EQ(200, size.height);

  // Verify that both width and height are rounded (in this case up)
  web_view->SetZoomLevel(WebView::ZoomFactorToZoomLevel(0.9995));
  size = web_view->ContentsPreferredMinimumSize();
  EXPECT_EQ(100, size.width);
  EXPECT_EQ(100, size.height);

  // Verify that both width and height are rounded (in this case down)
  web_view->SetZoomLevel(WebView::ZoomFactorToZoomLevel(1.0005));
  size = web_view->ContentsPreferredMinimumSize();
  EXPECT_EQ(100, size.width);
  EXPECT_EQ(100, size.height);

  url = base_url_ + "specify_size.html?1.5px:1.5px";
  url_test_helpers::RegisterMockedURLLoad(
      ToKURL(url), test::CoreTestDataPath("specify_size.html"));
  web_view = web_view_helper_.InitializeAndLoad(url);

  web_view->SetZoomLevel(WebView::ZoomFactorToZoomLevel(1));
  size = web_view->ContentsPreferredMinimumSize();
  EXPECT_EQ(2, size.width);
  EXPECT_EQ(2, size.height);
}

TEST_F(WebViewTest, PreferredMinimumSizeQuirksMode) {
  WebViewImpl* web_view = web_view_helper_.Initialize();
  web_view->MainFrameWidget()->Resize(WebSize(800, 600));
  frame_test_helpers::LoadHTMLString(
      web_view->MainFrameImpl(),
      R"HTML(<html>
        <body style="margin: 0px;">
          <div style="width: 99px; height: 100px; display: inline-block;"></div>
        </body>
      </html>)HTML",
      url_test_helpers::ToKURL("http://example.com/"));

  WebSize size = web_view->ContentsPreferredMinimumSize();
  EXPECT_EQ(99, size.width);
  // When in quirks mode the preferred height stretches to fill the viewport.
  EXPECT_EQ(600, size.height);
}

TEST_F(WebViewTest, PreferredSizeWithGrid) {
  WebViewImpl* web_view = web_view_helper_.Initialize();
  WebURL base_url = url_test_helpers::ToKURL("http://example.com/");
  frame_test_helpers::LoadHTMLString(web_view->MainFrameImpl(),
                                     R"HTML(<!DOCTYPE html>
    <style>
      html { writing-mode: vertical-rl; }
      body { margin: 0px; }
    </style>
    <div style="width: 100px;">
      <div style="display: grid; width: 100%;">
        <div style="writing-mode: horizontal-tb; height: 100px;"></div>
      </div>
    </div>
                                   )HTML",
                                     base_url);

  WebSize size = web_view->ContentsPreferredMinimumSize();
  EXPECT_EQ(100, size.width);
  EXPECT_EQ(100, size.height);
}

TEST_F(WebViewTest, PreferredSizeWithGridMinWidth) {
  WebViewImpl* web_view = web_view_helper_.Initialize();
  WebURL base_url = url_test_helpers::ToKURL("http://example.com/");
  frame_test_helpers::LoadHTMLString(web_view->MainFrameImpl(),
                                     R"HTML(<!DOCTYPE html>
    <body style="margin: 0px;">
      <div style="display: inline-grid; min-width: 200px;">
        <div>item</div>
      </div>
    </body>
                                   )HTML",
                                     base_url);

  WebSize size = web_view->ContentsPreferredMinimumSize();
  EXPECT_EQ(200, size.width);
}

TEST_F(WebViewTest, PreferredSizeWithGridMinWidthFlexibleTracks) {
  WebViewImpl* web_view = web_view_helper_.Initialize();
  WebURL base_url = url_test_helpers::ToKURL("http://example.com/");
  frame_test_helpers::LoadHTMLString(web_view->MainFrameImpl(),
                                     R"HTML(<!DOCTYPE html>
    <body style="margin: 0px;">
      <div style="display: inline-grid; min-width: 200px; grid-template-columns: 1fr;">
        <div>item</div>
      </div>
    </body>
                                   )HTML",
                                     base_url);

  WebSize size = web_view->ContentsPreferredMinimumSize();
  EXPECT_EQ(200, size.width);
}

#if BUILDFLAG(ENABLE_UNHANDLED_TAP)

// Helps set up any test that uses a mock Mojo implementation.
class MojoTestHelper {
 public:
  MojoTestHelper(const String& test_file,
                 frame_test_helpers::WebViewHelper& web_view_helper)
      : web_view_helper_(web_view_helper) {
    web_view_ = web_view_helper.InitializeAndLoad(WebString(test_file).Utf8(),
                                                  &web_frame_client_);
  }

  ~MojoTestHelper() {
    web_view_helper_.Reset();  // Remove dependency on locally scoped client.
  }

  // Bind the test API to a service with the given |name| and repeating Bind
  // method given by |callback|.
  void BindTestApi(
      const String& name,
      base::RepeatingCallback<void(mojo::ScopedMessagePipeHandle)> callback) {
    // Set up our Mock Mojo API.
    test_api_.reset(new service_manager::InterfaceProvider::TestApi(
        web_frame_client_.GetInterfaceProvider()));
    test_api_->SetBinderForName(WebString(name).Utf8(), callback);
  }
  WebViewImpl* WebView() const { return web_view_; }

 private:
  WebViewImpl* web_view_;
  frame_test_helpers::WebViewHelper& web_view_helper_;
  frame_test_helpers::TestWebFrameClient web_frame_client_;
  std::unique_ptr<service_manager::InterfaceProvider::TestApi> test_api_;
};

// Mock implementation of the UnhandledTapNotifier Mojo receiver, for testing
// the ShowUnhandledTapUIIfNeeded notification.
class MockUnhandledTapNotifierImpl : public mojom::blink::UnhandledTapNotifier {
 public:
  MockUnhandledTapNotifierImpl() : binding_(this) {}
  ~MockUnhandledTapNotifierImpl() override = default;

  void Bind(mojo::ScopedMessagePipeHandle handle) {
    binding_.Bind(mojom::blink::UnhandledTapNotifierRequest(std::move(handle)));
  }

  void ShowUnhandledTapUIIfNeeded(
      mojom::blink::UnhandledTapInfoPtr unhandled_tap_info) override {
    was_unhandled_tap_ = true;
    tapped_position_ = unhandled_tap_info->tapped_position_in_viewport;
    element_text_run_length_ = unhandled_tap_info->element_text_run_length;
    font_size_ = unhandled_tap_info->font_size_in_pixels;
  }
  bool WasUnhandledTap() const { return was_unhandled_tap_; }
  int GetTappedXPos() const { return tapped_position_.X(); }
  int GetTappedYPos() const { return tapped_position_.Y(); }
  int GetFontSize() const { return font_size_; }
  int GetElementTextRunLength() const { return element_text_run_length_; }
  void Reset() {
    was_unhandled_tap_ = false;
    tapped_position_ = IntPoint();
    element_text_run_length_ = 0;
    font_size_ = 0;
    binding_.Close();
  }

 private:
  bool was_unhandled_tap_ = false;
  IntPoint tapped_position_;
  int element_text_run_length_ = 0;
  int font_size_ = 0;

  mojo::Binding<mojom::blink::UnhandledTapNotifier> binding_;
};

// A Test Fixture for testing ShowUnhandledTapUIIfNeeded usages.
class ShowUnhandledTapTest : public WebViewTest {
 public:
  void SetUp() override {
    WebViewTest::SetUp();
    std::string test_file = "show_unhandled_tap.html";
    RegisterMockedHttpURLLoad("Ahem.ttf");
    RegisterMockedHttpURLLoad(test_file);

    mojo_test_helper_.reset(new MojoTestHelper(
        WebString::FromUTF8(base_url_ + test_file), web_view_helper_));

    web_view_ = mojo_test_helper_->WebView();
    web_view_->MainFrameWidget()->Resize(WebSize(500, 300));
    web_view_->MainFrameWidget()->UpdateAllLifecyclePhases(
        WebWidget::LifecycleUpdateReason::kTest);
    RunPendingTasks();

    mojo_test_helper_->BindTestApi(
        mojom::blink::UnhandledTapNotifier::Name_,
        WTF::BindRepeating(&MockUnhandledTapNotifierImpl::Bind,
                           WTF::Unretained(&mock_notifier_)));
  }

 protected:
  // Tap on the given element by ID.
  void Tap(const String& element_id) {
    mock_notifier_.Reset();
    EXPECT_TRUE(TapElementById(WebInputEvent::kGestureTap, element_id));
  }

  // Set up a test script for the given |operation| with the given |handler|.
  void SetTestScript(const String& operation, const String& handler) {
    String test_key = operation + "-" + handler;
    web_view_->MainFrameImpl()->ExecuteScript(
        WebScriptSource(String("setTest('" + test_key + "');")));
  }

  // Test each mouse event combination with the given |handler|, and verify the
  // |expected| outcome.
  void TestEachMouseEvent(const String& handler, bool expected) {
    SetTestScript("mousedown", handler);
    Tap("target");
    EXPECT_EQ(expected, mock_notifier_.WasUnhandledTap());

    SetTestScript("mouseup", handler);
    Tap("target");
    EXPECT_EQ(expected, mock_notifier_.WasUnhandledTap());

    SetTestScript("click", handler);
    Tap("target");
    EXPECT_EQ(expected, mock_notifier_.WasUnhandledTap());
  }

  WebViewImpl* web_view_;
  MockUnhandledTapNotifierImpl mock_notifier_;

 private:
  std::unique_ptr<MojoTestHelper> mojo_test_helper_;
};

TEST_F(ShowUnhandledTapTest, ShowUnhandledTapUIIfNeeded) {
  // Scroll the bottom into view so we can distinguish window coordinates from
  // document coordinates.
  Tap("bottom");
  EXPECT_TRUE(mock_notifier_.WasUnhandledTap());
  EXPECT_EQ(64, mock_notifier_.GetTappedXPos());
  EXPECT_EQ(278, mock_notifier_.GetTappedYPos());
  EXPECT_EQ(16, mock_notifier_.GetFontSize());
  EXPECT_EQ(7, mock_notifier_.GetElementTextRunLength());

  // Test basic tap handling and notification.
  Tap("target");
  EXPECT_TRUE(mock_notifier_.WasUnhandledTap());
  EXPECT_EQ(144, mock_notifier_.GetTappedXPos());
  EXPECT_EQ(82, mock_notifier_.GetTappedYPos());

  // Test correct conversion of coordinates to viewport space under pinch-zoom.
  web_view_->SetPageScaleFactor(2);
  web_view_->SetVisualViewportOffset(WebFloatPoint(50, 20));

  Tap("target");
  EXPECT_TRUE(mock_notifier_.WasUnhandledTap());
  EXPECT_EQ(188, mock_notifier_.GetTappedXPos());
  EXPECT_EQ(124, mock_notifier_.GetTappedYPos());
  EXPECT_EQ(16, mock_notifier_.GetFontSize());
  EXPECT_EQ(28, mock_notifier_.GetElementTextRunLength());
}

TEST_F(ShowUnhandledTapTest, ShowUnhandledTapUIIfNeededWithMutateDom) {
  // Test dom mutation.
  TestEachMouseEvent("mutateDom", FALSE);

  // Test without any DOM mutation.
  TestEachMouseEvent("none", TRUE);
}

TEST_F(ShowUnhandledTapTest, ShowUnhandledTapUIIfNeededWithMutateStyle) {
  // Test style mutation.
  TestEachMouseEvent("mutateStyle", FALSE);

  // Test checkbox:indeterminate style mutation.
  TestEachMouseEvent("mutateIndeterminate", FALSE);

  // Test click div with :active style.
  Tap("style_active");
  EXPECT_FALSE(mock_notifier_.WasUnhandledTap());
}

TEST_F(ShowUnhandledTapTest, ShowUnhandledTapUIIfNeededWithPreventDefault) {
  // Test swallowing.
  TestEachMouseEvent("preventDefault", FALSE);

  // Test without any preventDefault.
  TestEachMouseEvent("none", TRUE);
}

TEST_F(ShowUnhandledTapTest, ShowUnhandledTapUIIfNeededWithNonTriggeringNodes) {
  Tap("image");
  EXPECT_FALSE(mock_notifier_.WasUnhandledTap());

  Tap("editable");
  EXPECT_FALSE(mock_notifier_.WasUnhandledTap());

  Tap("focusable");
  EXPECT_FALSE(mock_notifier_.WasUnhandledTap());
}

TEST_F(ShowUnhandledTapTest, ShowUnhandledTapUIIfNeededWithTextSizes) {
  Tap("large");
  EXPECT_TRUE(mock_notifier_.WasUnhandledTap());
  EXPECT_EQ(20, mock_notifier_.GetFontSize());

  Tap("small");
  EXPECT_TRUE(mock_notifier_.WasUnhandledTap());
  EXPECT_EQ(10, mock_notifier_.GetFontSize());
}

#endif  // BUILDFLAG(ENABLE_UNHANDLED_TAP)

#if defined(OS_MACOSX)
TEST_F(WebViewTest, WebSubstringUtil) {
  RegisterMockedHttpURLLoad("content_editable_populated.html");
  WebViewImpl* web_view = web_view_helper_.InitializeAndLoad(
      base_url_ + "content_editable_populated.html");
  web_view->GetSettings()->SetDefaultFontSize(12);
  web_view->MainFrameWidget()->Resize(WebSize(400, 400));
  WebLocalFrameImpl* frame = web_view->MainFrameImpl();

  WebPoint baseline_point;
  NSAttributedString* result = WebSubstringUtil::AttributedSubstringInRange(
      frame, 10, 3, &baseline_point);
  ASSERT_TRUE(!!result);

  WebPoint point(baseline_point.x, baseline_point.y);
  result = WebSubstringUtil::AttributedWordAtPoint(frame->FrameWidget(), point,
                                                   baseline_point);
  ASSERT_TRUE(!!result);

  web_view->SetZoomLevel(3);

  result = WebSubstringUtil::AttributedSubstringInRange(frame, 5, 5,
                                                        &baseline_point);
  ASSERT_TRUE(!!result);

  point = WebPoint(baseline_point.x, baseline_point.y);
  result = WebSubstringUtil::AttributedWordAtPoint(frame->FrameWidget(), point,
                                                   baseline_point);
  ASSERT_TRUE(!!result);
}

TEST_F(WebViewTest, WebSubstringUtilBaselinePoint) {
  RegisterMockedHttpURLLoad("content_editable_multiline.html");
  WebViewImpl* web_view = web_view_helper_.InitializeAndLoad(
      base_url_ + "content_editable_multiline.html");
  web_view->GetSettings()->SetDefaultFontSize(12);
  web_view->MainFrameWidget()->Resize(WebSize(400, 400));
  WebLocalFrameImpl* frame = web_view->MainFrameImpl();

  WebPoint old_point;
  WebSubstringUtil::AttributedSubstringInRange(frame, 3, 1, &old_point);

  WebPoint new_point;
  WebSubstringUtil::AttributedSubstringInRange(frame, 3, 20, &new_point);

  EXPECT_EQ(old_point.x, new_point.x);
  EXPECT_EQ(old_point.y, new_point.y);
}

TEST_F(WebViewTest, WebSubstringUtilPinchZoom) {
  RegisterMockedHttpURLLoad("content_editable_populated.html");
  WebViewImpl* web_view = web_view_helper_.InitializeAndLoad(
      base_url_ + "content_editable_populated.html");
  web_view->GetSettings()->SetDefaultFontSize(12);
  web_view->MainFrameWidget()->Resize(WebSize(400, 400));
  WebLocalFrameImpl* frame = web_view->MainFrameImpl();
  NSAttributedString* result = nil;

  WebPoint baseline_point;
  result = WebSubstringUtil::AttributedSubstringInRange(frame, 10, 3,
                                                        &baseline_point);
  ASSERT_TRUE(!!result);

  web_view->SetPageScaleFactor(3);

  WebPoint point_after_zoom;
  result = WebSubstringUtil::AttributedSubstringInRange(frame, 10, 3,
                                                        &point_after_zoom);
  ASSERT_TRUE(!!result);

  // We won't have moved by a full factor of 3 because of the translations, but
  // we should move by a factor of >2.
  EXPECT_LT(2 * baseline_point.x, point_after_zoom.x);
  EXPECT_LT(2 * baseline_point.y, point_after_zoom.y);
}

TEST_F(WebViewTest, WebSubstringUtilIframe) {
  RegisterMockedHttpURLLoad("single_iframe.html");
  RegisterMockedHttpURLLoad("visible_iframe.html");
  WebViewImpl* web_view =
      web_view_helper_.InitializeAndLoad(base_url_ + "single_iframe.html");
  web_view->GetSettings()->SetDefaultFontSize(12);
  web_view->GetSettings()->SetJavaScriptEnabled(true);
  web_view->MainFrameWidget()->Resize(WebSize(400, 400));
  WebLocalFrameImpl* main_frame = web_view->MainFrameImpl();
  WebLocalFrameImpl* child_frame = WebLocalFrameImpl::FromFrame(
      To<LocalFrame>(main_frame->GetFrame()->Tree().FirstChild()));

  WebPoint baseline_point;
  NSAttributedString* result = WebSubstringUtil::AttributedSubstringInRange(
      child_frame, 11, 7, &baseline_point);
  ASSERT_NE(result, nullptr);

  WebPoint point(baseline_point.x, baseline_point.y);
  result = WebSubstringUtil::AttributedWordAtPoint(main_frame->FrameWidget(),
                                                   point, baseline_point);
  ASSERT_NE(result, nullptr);

  int y_before_change = baseline_point.y;

  // Now move the <iframe> down by 100px.
  main_frame->ExecuteScript(WebScriptSource(
      "document.querySelector('iframe').style.marginTop = '100px';"));

  point = WebPoint(point.x, point.y + 100);
  result = WebSubstringUtil::AttributedWordAtPoint(main_frame->FrameWidget(),
                                                   point, baseline_point);
  ASSERT_NE(result, nullptr);

  EXPECT_EQ(y_before_change, baseline_point.y - 100);
}

#endif

TEST_F(WebViewTest, PasswordFieldCanBeAutofilled) {
  RegisterMockedHttpURLLoad("input_field_password.html");
  // Pretend client has fill data for all fields it's queried.
  MockAutofillClient client;
  client.SetFillData(true);
  WebViewImpl* web_view = web_view_helper_.InitializeAndLoad(
      base_url_ + "input_field_password.html");
  WebLocalFrameImpl* frame = web_view->MainFrameImpl();
  frame->SetAutofillClient(&client);
  // No field is focused.
  EXPECT_FALSE(frame->CanFocusedFieldBeAutofilled());

  // Focusing a field should result in treating it autofillable.
  web_view->SetInitialFocus(false);
  EXPECT_TRUE(frame->CanFocusedFieldBeAutofilled());

  // Pretend that |client| no longer has autofill data available.
  client.SetFillData(false);
  EXPECT_FALSE(frame->CanFocusedFieldBeAutofilled());
  frame->SetAutofillClient(nullptr);
}

TEST_F(WebViewTest, PasswordFieldEditingIsUserGesture) {
  RegisterMockedHttpURLLoad("input_field_password.html");
  MockAutofillClient client;
  WebViewImpl* web_view = web_view_helper_.InitializeAndLoad(
      base_url_ + "input_field_password.html");
  WebLocalFrameImpl* frame = web_view->MainFrameImpl();
  frame->SetAutofillClient(&client);
  web_view->SetInitialFocus(false);

  WebVector<WebImeTextSpan> empty_ime_text_spans;

  EXPECT_TRUE(
      frame->FrameWidget()->GetActiveWebInputMethodController()->CommitText(
          WebString::FromUTF8(std::string("hello").c_str()),
          empty_ime_text_spans, WebRange(), 0));
  EXPECT_EQ(1, client.TextChangesFromUserGesture());
  EXPECT_FALSE(UserGestureIndicator::ProcessingUserGesture());
  frame->SetAutofillClient(nullptr);
}

// Verify that a WebView created with a ScopedPagePauser already on the
// stack defers its loads.
TEST_F(WebViewTest, CreatedDuringPagePause) {
  {
    WebViewImpl* web_view = web_view_helper_.Initialize();
    EXPECT_FALSE(web_view->GetPage()->Paused());
  }

  {
    ScopedPagePauser pauser;
    WebViewImpl* web_view = web_view_helper_.Initialize();
    EXPECT_TRUE(web_view->GetPage()->Paused());
  }
}

// Make sure the SubframeBeforeUnloadUseCounter is only incremented on subframe
// unloads. crbug.com/635029.
TEST_F(WebViewTest, SubframeBeforeUnloadUseCounter) {
  RegisterMockedHttpURLLoad("visible_iframe.html");
  RegisterMockedHttpURLLoad("single_iframe.html");
  WebViewImpl* web_view =
      web_view_helper_.InitializeAndLoad(base_url_ + "single_iframe.html");

  WebLocalFrame* frame = web_view_helper_.LocalMainFrame();
  Document* document =
      To<LocalFrame>(web_view_helper_.GetWebView()->GetPage()->MainFrame())
          ->GetDocument();

  // Add a beforeunload handler in the main frame. Make sure firing
  // beforeunload doesn't increment the subframe use counter.
  {
    frame->ExecuteScript(
        WebScriptSource("addEventListener('beforeunload', function() {});"));
    web_view->MainFrameImpl()->DispatchBeforeUnloadEvent(false);
    EXPECT_FALSE(UseCounter::IsCounted(*document,
                                       WebFeature::kSubFrameBeforeUnloadFired));
  }

  // Add a beforeunload handler in the iframe and dispatch. Make sure we do
  // increment the use counter for subframe beforeunloads.
  {
    frame->ExecuteScript(WebScriptSource(
        "document.getElementsByTagName('iframe')[0].contentWindow."
        "addEventListener('beforeunload', function() {});"));
    To<WebLocalFrameImpl>(
        web_view->MainFrame()->FirstChild()->ToWebLocalFrame())
        ->DispatchBeforeUnloadEvent(false);

    Document* child_document = To<LocalFrame>(web_view_helper_.GetWebView()
                                                  ->GetPage()
                                                  ->MainFrame()
                                                  ->Tree()
                                                  .FirstChild())
                                   ->GetDocument();
    EXPECT_TRUE(UseCounter::IsCounted(*child_document,
                                      WebFeature::kSubFrameBeforeUnloadFired));
  }
}

// Verify that page loads are deferred until all ScopedPageLoadDeferrers are
// destroyed.
TEST_F(WebViewTest, NestedPagePauses) {
  WebViewImpl* web_view = web_view_helper_.Initialize();
  EXPECT_FALSE(web_view->GetPage()->Paused());

  {
    ScopedPagePauser pauser;
    EXPECT_TRUE(web_view->GetPage()->Paused());

    {
      ScopedPagePauser pauser2;
      EXPECT_TRUE(web_view->GetPage()->Paused());
    }

    EXPECT_TRUE(web_view->GetPage()->Paused());
  }

  EXPECT_FALSE(web_view->GetPage()->Paused());
}

TEST_F(WebViewTest, ClosingPageIsPaused) {
  WebViewImpl* web_view = web_view_helper_.Initialize();
  Page* page = web_view_helper_.GetWebView()->GetPage();
  EXPECT_FALSE(page->Paused());

  web_view->SetOpenedByDOM();

  auto* main_frame = To<LocalFrame>(page->MainFrame());
  EXPECT_FALSE(main_frame->DomWindow()->closed());

  ScriptState* script_state = ToScriptStateForMainWorld(main_frame);
  ScriptState::Scope entered_context_scope(script_state);
  v8::Context::BackupIncumbentScope incumbent_context_scope(
      script_state->GetContext());

  main_frame->DomWindow()->close(script_state->GetIsolate());
  // The window should be marked closed...
  EXPECT_TRUE(main_frame->DomWindow()->closed());
  // EXPECT_TRUE(page->isClosing());
  // ...but not yet detached.
  EXPECT_TRUE(main_frame->GetPage());

  {
    ScopedPagePauser pauser;
    EXPECT_TRUE(page->Paused());
  }
}

TEST_F(WebViewTest, ForceAndResetViewport) {
  RegisterMockedHttpURLLoad("200-by-300.html");
  WebViewImpl* web_view_impl =
      web_view_helper_.InitializeAndLoad(base_url_ + "200-by-300.html");
  web_view_impl->MainFrameWidget()->Resize(WebSize(100, 150));
  SetViewportSize(WebSize(100, 150));
  VisualViewport* visual_viewport =
      &web_view_impl->GetPage()->GetVisualViewport();
  DevToolsEmulator* dev_tools_emulator = web_view_impl->GetDevToolsEmulator();

  TransformationMatrix expected_matrix;
  expected_matrix.MakeIdentity();
  EXPECT_EQ(expected_matrix, web_view_impl->GetDeviceEmulationTransform());
  {
    IntRect visible_rect(1, 2, 3, 4);
    dev_tools_emulator->OverrideVisibleRect(IntSize(), &visible_rect);
    EXPECT_EQ(IntRect(1, 2, 3, 4), visible_rect);  // Was modified.
  }
  EXPECT_TRUE(visual_viewport->ContainerLayer()->MasksToBounds());

  // Override applies transform, sets visible rect, and disables
  // visual viewport clipping.
  dev_tools_emulator->ForceViewport(WebFloatPoint(50, 55), 2.f);
  expected_matrix.MakeIdentity().Scale(2.f).Translate(-50, -55);
  EXPECT_EQ(expected_matrix, web_view_impl->GetDeviceEmulationTransform());
  {
    IntRect visible_rect(1, 2, 3, 4);
    dev_tools_emulator->OverrideVisibleRect(IntSize(100, 150), &visible_rect);
    EXPECT_EQ(IntRect(50, 55, 50, 75), visible_rect);
  }
  EXPECT_FALSE(visual_viewport->ContainerLayer()->MasksToBounds());

  // Setting new override discards previous one.
  dev_tools_emulator->ForceViewport(WebFloatPoint(5.4f, 10.5f), 1.5f);
  expected_matrix.MakeIdentity().Scale(1.5f).Translate(-5.4f, -10.5f);
  EXPECT_EQ(expected_matrix, web_view_impl->GetDeviceEmulationTransform());
  {
    IntRect visible_rect(1, 2, 3, 4);
    dev_tools_emulator->OverrideVisibleRect(IntSize(100, 150), &visible_rect);
    EXPECT_EQ(IntRect(5, 10, 68, 101), visible_rect);  // Was modified.
  }
  EXPECT_FALSE(visual_viewport->ContainerLayer()->MasksToBounds());

  // Clearing override restores original transform, visible rect and
  // visual viewport clipping.
  dev_tools_emulator->ResetViewport();
  expected_matrix.MakeIdentity();
  EXPECT_EQ(expected_matrix, web_view_impl->GetDeviceEmulationTransform());
  {
    IntRect visible_rect(1, 2, 3, 4);
    dev_tools_emulator->OverrideVisibleRect(IntSize(), &visible_rect);
    EXPECT_EQ(IntRect(1, 2, 3, 4), visible_rect);  // Not modified.
  }
  EXPECT_TRUE(visual_viewport->ContainerLayer()->MasksToBounds());
}

TEST_F(WebViewTest, ViewportOverrideIntegratesDeviceMetricsOffsetAndScale) {
  RegisterMockedHttpURLLoad("200-by-300.html");
  WebViewImpl* web_view_impl =
      web_view_helper_.InitializeAndLoad(base_url_ + "200-by-300.html");
  web_view_impl->MainFrameWidget()->Resize(WebSize(100, 150));

  TransformationMatrix expected_matrix;
  expected_matrix.MakeIdentity();
  EXPECT_EQ(expected_matrix, web_view_impl->GetDeviceEmulationTransform());

  WebDeviceEmulationParams emulation_params;
  emulation_params.scale = 2.f;
  web_view_impl->EnableDeviceEmulation(emulation_params);
  expected_matrix.MakeIdentity().Scale(2.f);
  EXPECT_EQ(expected_matrix, web_view_impl->GetDeviceEmulationTransform());

  // Device metrics offset and scale are applied before viewport override.
  web_view_impl->GetDevToolsEmulator()->ForceViewport(WebFloatPoint(5, 10),
                                                      1.5f);
  expected_matrix.MakeIdentity()
      .Scale(1.5f)
      .Translate(-5, -10)
      .Scale(2.f);
  EXPECT_EQ(expected_matrix, web_view_impl->GetDeviceEmulationTransform());
}

TEST_F(WebViewTest, ViewportOverrideAdaptsToScaleAndScroll) {
  RegisterMockedHttpURLLoad("200-by-300.html");
  WebViewImpl* web_view_impl =
      web_view_helper_.InitializeAndLoad(base_url_ + "200-by-300.html");
  web_view_impl->MainFrameWidget()->Resize(WebSize(100, 150));
  SetViewportSize(WebSize(100, 150));
  LocalFrameView* frame_view =
      web_view_impl->MainFrameImpl()->GetFrame()->View();
  DevToolsEmulator* dev_tools_emulator = web_view_impl->GetDevToolsEmulator();

  TransformationMatrix expected_matrix;
  expected_matrix.MakeIdentity();
  EXPECT_EQ(expected_matrix, web_view_impl->GetDeviceEmulationTransform());

  // Initial transform takes current page scale and scroll position into
  // account.
  web_view_impl->SetPageScaleFactor(1.5f);
  frame_view->LayoutViewport()->SetScrollOffset(
      ScrollOffset(100, 150), kProgrammaticScroll, kScrollBehaviorInstant);
  dev_tools_emulator->ForceViewport(WebFloatPoint(50, 55), 2.f);
  expected_matrix.MakeIdentity()
      .Scale(2.f)
      .Translate(-50, -55)
      .Translate(100, 150)
      .Scale(1. / 1.5f);
  EXPECT_EQ(expected_matrix, web_view_impl->GetDeviceEmulationTransform());
  // Page scroll and scale are irrelevant for visible rect.
  {
    IntRect visible_rect(1, 2, 3, 4);
    dev_tools_emulator->OverrideVisibleRect(IntSize(100, 150), &visible_rect);
    EXPECT_EQ(IntRect(50, 55, 50, 75), visible_rect);  // Was modified.
  }

  // Transform adapts to scroll changes.
  frame_view->LayoutViewport()->SetScrollOffset(
      ScrollOffset(50, 55), kProgrammaticScroll, kScrollBehaviorInstant);
  expected_matrix.MakeIdentity()
      .Scale(2.f)
      .Translate(-50, -55)
      .Translate(50, 55)
      .Scale(1. / 1.5f);
  EXPECT_EQ(expected_matrix, web_view_impl->GetDeviceEmulationTransform());
  // Visible rect doesn't change.
  {
    IntRect visible_rect(1, 2, 3, 4);
    dev_tools_emulator->OverrideVisibleRect(IntSize(100, 150), &visible_rect);
    EXPECT_EQ(IntRect(50, 55, 50, 75), visible_rect);  // Was modified.
  }

  // Transform adapts to page scale changes.
  web_view_impl->SetPageScaleFactor(2.f);
  expected_matrix.MakeIdentity()
      .Scale(2.f)
      .Translate(-50, -55)
      .Translate(50, 55)
      .Scale(1. / 2.f);
  EXPECT_EQ(expected_matrix, web_view_impl->GetDeviceEmulationTransform());
  // Visible rect doesn't change.
  {
    IntRect visible_rect(1, 2, 3, 4);
    dev_tools_emulator->OverrideVisibleRect(IntSize(100, 150), &visible_rect);
    EXPECT_EQ(IntRect(50, 55, 50, 75), visible_rect);  // Was modified.
  }
}

TEST_F(WebViewTest, ResizeForPrintingViewportUnits) {
  WebViewImpl* web_view = web_view_helper_.Initialize();
  web_view->MainFrameWidget()->Resize(WebSize(800, 600));

  WebURL base_url = url_test_helpers::ToKURL("http://example.com/");
  frame_test_helpers::LoadHTMLString(web_view->MainFrameImpl(),
                                     "<style>"
                                     "  body { margin: 0px; }"
                                     "  #vw { width: 100vw; height: 100vh; }"
                                     "</style>"
                                     "<div id=vw></div>",
                                     base_url);

  WebLocalFrameImpl* frame = web_view->MainFrameImpl();
  Document* document = frame->GetFrame()->GetDocument();
  Element* vw_element = document->getElementById("vw");

  EXPECT_EQ(800, vw_element->OffsetWidth());

  FloatSize page_size(300, 360);

  WebPrintParams print_params;
  print_params.print_content_area.width = page_size.Width();
  print_params.print_content_area.height = page_size.Height();

  IntSize expected_size = PrintICBSizeFromPageSize(page_size);

  frame->PrintBegin(print_params, WebNode());

  EXPECT_EQ(expected_size.Width(), vw_element->OffsetWidth());
  EXPECT_EQ(expected_size.Height(), vw_element->OffsetHeight());

  web_view->MainFrameWidget()->Resize(FlooredIntSize(page_size));

  EXPECT_EQ(expected_size.Width(), vw_element->OffsetWidth());
  EXPECT_EQ(expected_size.Height(), vw_element->OffsetHeight());

  web_view->MainFrameWidget()->Resize(WebSize(800, 600));
  frame->PrintEnd();

  EXPECT_EQ(800, vw_element->OffsetWidth());
}

TEST_F(WebViewTest, WidthMediaQueryWithPageZoomAfterPrinting) {
  WebViewImpl* web_view = web_view_helper_.Initialize();
  web_view->MainFrameWidget()->Resize(WebSize(800, 600));
  web_view->SetZoomLevel(WebView::ZoomFactorToZoomLevel(2.0));

  WebURL base_url = url_test_helpers::ToKURL("http://example.com/");
  frame_test_helpers::LoadHTMLString(web_view->MainFrameImpl(),
                                     "<style>"
                                     "  @media (max-width: 600px) {"
                                     "    div { color: green }"
                                     "  }"
                                     "</style>"
                                     "<div id=d></div>",
                                     base_url);

  WebLocalFrameImpl* frame = web_view->MainFrameImpl();
  Document* document = frame->GetFrame()->GetDocument();
  Element* div = document->getElementById("d");

  EXPECT_EQ(MakeRGB(0, 128, 0), div->GetComputedStyle()->VisitedDependentColor(
                                    GetCSSPropertyColor()));

  FloatSize page_size(300, 360);

  WebPrintParams print_params;
  print_params.print_content_area.width = page_size.Width();
  print_params.print_content_area.height = page_size.Height();

  frame->PrintBegin(print_params, WebNode());
  frame->PrintEnd();

  EXPECT_EQ(MakeRGB(0, 128, 0), div->GetComputedStyle()->VisitedDependentColor(
                                    GetCSSPropertyColor()));
}

TEST_F(WebViewTest, ViewportUnitsPrintingWithPageZoom) {
  WebViewImpl* web_view = web_view_helper_.Initialize();
  web_view->MainFrameWidget()->Resize(WebSize(800, 600));
  web_view->SetZoomLevel(WebView::ZoomFactorToZoomLevel(2.0));

  WebURL base_url = url_test_helpers::ToKURL("http://example.com/");
  frame_test_helpers::LoadHTMLString(web_view->MainFrameImpl(),
                                     "<style>"
                                     "  body { margin: 0 }"
                                     "  #t1 { width: 100% }"
                                     "  #t2 { width: 100vw }"
                                     "</style>"
                                     "<div id=t1></div>"
                                     "<div id=t2></div>",
                                     base_url);

  WebLocalFrameImpl* frame = web_view->MainFrameImpl();
  Document* document = frame->GetFrame()->GetDocument();
  Element* t1 = document->getElementById("t1");
  Element* t2 = document->getElementById("t2");

  EXPECT_EQ(400, t1->OffsetWidth());
  EXPECT_EQ(400, t2->OffsetWidth());

  FloatSize page_size(600, 720);
  int expected_width = PrintICBSizeFromPageSize(page_size).Width();

  WebPrintParams print_params;
  print_params.print_content_area.width = page_size.Width();
  print_params.print_content_area.height = page_size.Height();

  frame->PrintBegin(print_params, WebNode());

  EXPECT_EQ(expected_width, t1->OffsetWidth());
  EXPECT_EQ(expected_width, t2->OffsetWidth());

  frame->PrintEnd();
}

TEST_F(WebViewTest, DeviceEmulationResetScrollbars) {
  WebViewImpl* web_view = web_view_helper_.Initialize();
  web_view->MainFrameWidget()->Resize(WebSize(800, 600));

  WebURL base_url = url_test_helpers::ToKURL("http://example.com/");
  frame_test_helpers::LoadHTMLString(web_view->MainFrameImpl(),
                                     "<!doctype html>"
                                     "<meta name='viewport'"
                                     "    content='width=device-width'>"
                                     "<style>"
                                     "  body {margin: 0px; height:3000px;}"
                                     "</style>",
                                     base_url);
  UpdateAllLifecyclePhases();

  WebLocalFrameImpl* frame = web_view->MainFrameImpl();
  auto* frame_view = frame->GetFrameView();
  EXPECT_FALSE(frame_view->VisualViewportSuppliesScrollbars());
  EXPECT_NE(nullptr, frame_view->LayoutViewport()->VerticalScrollbar());

  WebDeviceEmulationParams params;
  params.screen_position = WebDeviceEmulationParams::kMobile;
  params.device_scale_factor = 0;
  params.scale = 1;

  web_view->EnableDeviceEmulation(params);

  // The visual viewport should now proivde the scrollbars instead of the view.
  EXPECT_TRUE(frame_view->VisualViewportSuppliesScrollbars());
  EXPECT_EQ(nullptr, frame_view->LayoutViewport()->VerticalScrollbar());

  web_view->DisableDeviceEmulation();

  // The view should once again provide the scrollbars.
  EXPECT_FALSE(frame_view->VisualViewportSuppliesScrollbars());
  EXPECT_NE(nullptr, frame_view->LayoutViewport()->VerticalScrollbar());
}

TEST_F(WebViewTest, SetZoomLevelWhilePluginFocused) {
  class PluginCreatingWebFrameClient
      : public frame_test_helpers::TestWebFrameClient {
   public:
    // WebLocalFrameClient overrides:
    WebPlugin* CreatePlugin(const WebPluginParams& params) override {
      return new FakeWebPlugin(params);
    }
  };
  PluginCreatingWebFrameClient frame_client;
  WebViewImpl* web_view = web_view_helper_.Initialize(&frame_client);
  WebURL base_url = url_test_helpers::ToKURL("https://example.com/");
  frame_test_helpers::LoadHTMLString(
      web_view->MainFrameImpl(),
      "<!DOCTYPE html><html><body>"
      "<object type='application/x-webkit-test-plugin'></object>"
      "</body></html>",
      base_url);
  // Verify the plugin is loaded.
  LocalFrame* main_frame = web_view->MainFrameImpl()->GetFrame();
  HTMLObjectElement* plugin_element =
      ToHTMLObjectElement(main_frame->GetDocument()->body()->firstChild());
  EXPECT_TRUE(plugin_element->OwnedPlugin());
  // Focus the plugin element, and then change the zoom level on the WebView.
  plugin_element->focus();
  EXPECT_FLOAT_EQ(1.0f, main_frame->PageZoomFactor());
  web_view->SetZoomLevel(-1.0);
  // Even though the plugin is focused, the entire frame's zoom factor should
  // still be updated.
  EXPECT_FLOAT_EQ(5.0f / 6.0f, main_frame->PageZoomFactor());
  web_view_helper_.Reset();  // Remove dependency on locally scoped client.
}

// Tests that a layout update that detaches a plugin doesn't crash if the
// plugin tries to execute script while being destroyed.
TEST_F(WebViewTest, DetachPluginInLayout) {
  class ScriptInDestroyPlugin : public FakeWebPlugin {
   public:
    ScriptInDestroyPlugin(WebLocalFrame* frame, const WebPluginParams& params)
        : FakeWebPlugin(params), frame_(frame) {}

    // WebPlugin overrides:
    void Destroy() override {
      frame_->ExecuteScript(WebScriptSource("console.log('done')"));
      // Deletes this.
      FakeWebPlugin::Destroy();
    }

   private:
    WebLocalFrame* frame_;  // Unowned
  };

  class PluginCreatingWebFrameClient
      : public frame_test_helpers::TestWebFrameClient {
   public:
    // WebLocalFrameClient overrides:
    WebPlugin* CreatePlugin(const WebPluginParams& params) override {
      return new ScriptInDestroyPlugin(Frame(), params);
    }

    void DidAddMessageToConsole(const WebConsoleMessage& message,
                                const WebString& source_name,
                                unsigned source_line,
                                const WebString& stack_trace) override {
      message_ = message.text;
    }

    const String& Message() const { return message_; }

   private:
    String message_;
  };

  PluginCreatingWebFrameClient frame_client;
  WebViewImpl* web_view = web_view_helper_.Initialize(&frame_client);
  WebURL base_url = url_test_helpers::ToKURL("https://example.com/");
  frame_test_helpers::LoadHTMLString(
      web_view->MainFrameImpl(),
      "<!DOCTYPE html><html><body>"
      "<object type='application/x-webkit-test-plugin'></object>"
      "</body></html>",
      base_url);
  // Verify the plugin is loaded.
  LocalFrame* main_frame = web_view->MainFrameImpl()->GetFrame();
  HTMLObjectElement* plugin_element =
      ToHTMLObjectElement(main_frame->GetDocument()->body()->firstChild());
  EXPECT_TRUE(plugin_element->OwnedPlugin());

  plugin_element->style()->setCSSText(main_frame->GetDocument(),
                                      "display: none", ASSERT_NO_EXCEPTION);
  EXPECT_TRUE(plugin_element->OwnedPlugin());
  UpdateAllLifecyclePhases();
  EXPECT_FALSE(plugin_element->OwnedPlugin());
  EXPECT_EQ("done", frame_client.Message());
  web_view_helper_.Reset();  // Remove dependency on locally scoped client.
}

// Check that first input delay is correctly reported to the document.
TEST_F(WebViewTest, FirstInputDelayReported) {
  WebViewImpl* web_view = web_view_helper_.Initialize();
  WebURL base_url = url_test_helpers::ToKURL("http://example.com/");
  frame_test_helpers::LoadHTMLString(web_view->MainFrameImpl(),
                                     "<html><body></body></html>", base_url);

  LocalFrame* main_frame = web_view->MainFrameImpl()->GetFrame();
  ASSERT_NE(nullptr, main_frame);

  Document* document = main_frame->GetDocument();
  ASSERT_NE(nullptr, document);

  WTF::ScopedMockClock clock;
  clock.Advance(TimeDelta::FromMilliseconds(70));

  InteractiveDetector* interactive_detector(
      InteractiveDetector::From(*document));
  ASSERT_NE(nullptr, interactive_detector);

  EXPECT_TRUE(interactive_detector->GetFirstInputDelay().is_zero());

  WebKeyboardEvent key_event1(WebInputEvent::kRawKeyDown,
                              WebInputEvent::kNoModifiers,
                              WebInputEvent::GetStaticTimeStampForTests());
  key_event1.dom_key = ui::DomKey::FromCharacter(' ');
  key_event1.windows_key_code = VKEY_SPACE;
  key_event1.SetTimeStamp(CurrentTimeTicks());
  clock.Advance(TimeDelta::FromMilliseconds(50));
  web_view->MainFrameWidget()->HandleInputEvent(
      WebCoalescedInputEvent(key_event1));

  EXPECT_NEAR(50, interactive_detector->GetFirstInputDelay().InMillisecondsF(),
              0.01);
  EXPECT_EQ(70, interactive_detector->GetFirstInputTimestamp()
                    .since_origin()
                    .InMillisecondsF());

  // Sending a second event won't change the FirstInputDelay.
  WebKeyboardEvent key_event2(WebInputEvent::kRawKeyDown,
                              WebInputEvent::kNoModifiers,
                              WebInputEvent::GetStaticTimeStampForTests());
  key_event2.dom_key = ui::DomKey::FromCharacter(' ');
  key_event2.windows_key_code = VKEY_SPACE;
  clock.Advance(TimeDelta::FromMilliseconds(60));
  key_event2.SetTimeStamp(CurrentTimeTicks());
  web_view->MainFrameWidget()->HandleInputEvent(
      WebCoalescedInputEvent(key_event2));

  EXPECT_NEAR(50, interactive_detector->GetFirstInputDelay().InMillisecondsF(),
              0.01);
  EXPECT_EQ(70, interactive_detector->GetFirstInputTimestamp()
                    .since_origin()
                    .InMillisecondsF());
}

// Check that longest input delay is correctly reported to the document.
TEST_F(WebViewTest, LongestInputDelayReported) {
  WebViewImpl* web_view = web_view_helper_.Initialize();
  WebURL base_url = url_test_helpers::ToKURL("http://example.com/");
  frame_test_helpers::LoadHTMLString(web_view->MainFrameImpl(),
                                     "<html><body></body></html>", base_url);

  LocalFrame* main_frame = web_view->MainFrameImpl()->GetFrame();
  ASSERT_NE(nullptr, main_frame);

  Document* document = main_frame->GetDocument();
  ASSERT_NE(nullptr, document);

  WTF::ScopedMockClock clock;
  clock.Advance(TimeDelta::FromMilliseconds(70));

  InteractiveDetector* interactive_detector(
      InteractiveDetector::From(*document));
  ASSERT_NE(nullptr, interactive_detector);

  EXPECT_TRUE(interactive_detector->GetLongestInputDelay().is_zero());

  WebKeyboardEvent key_event1(WebInputEvent::kRawKeyDown,
                              WebInputEvent::kNoModifiers,
                              WebInputEvent::GetStaticTimeStampForTests());
  key_event1.dom_key = ui::DomKey::FromCharacter(' ');
  key_event1.windows_key_code = VKEY_SPACE;
  key_event1.SetTimeStamp(CurrentTimeTicks());
  clock.Advance(TimeDelta::FromMilliseconds(50));
  web_view->MainFrameWidget()->HandleInputEvent(
      WebCoalescedInputEvent(key_event1));

  TimeTicks longest_input_timestamp = CurrentTimeTicks();

  WebKeyboardEvent key_event2(WebInputEvent::kRawKeyDown,
                              WebInputEvent::kNoModifiers,
                              WebInputEvent::GetStaticTimeStampForTests());
  key_event2.dom_key = ui::DomKey::FromCharacter(' ');
  key_event2.windows_key_code = VKEY_SPACE;
  key_event2.SetTimeStamp(longest_input_timestamp);
  clock.Advance(TimeDelta::FromMilliseconds(100));
  web_view->MainFrameWidget()->HandleInputEvent(
      WebCoalescedInputEvent(key_event2));

  WebKeyboardEvent key_event3(WebInputEvent::kRawKeyDown,
                              WebInputEvent::kNoModifiers,
                              WebInputEvent::GetStaticTimeStampForTests());
  key_event3.dom_key = ui::DomKey::FromCharacter(' ');
  key_event3.windows_key_code = VKEY_SPACE;
  key_event3.SetTimeStamp(CurrentTimeTicks());
  clock.Advance(TimeDelta::FromMilliseconds(70));
  web_view->MainFrameWidget()->HandleInputEvent(
      WebCoalescedInputEvent(key_event3));

  EXPECT_NEAR(100,
              interactive_detector->GetLongestInputDelay().InMillisecondsF(),
              0.01);
  EXPECT_EQ(longest_input_timestamp,
            interactive_detector->GetLongestInputTimestamp());
}

TEST_F(WebViewTest, InputDelayReported) {
  WTF::ScopedMockClock clock;
  clock.Advance(TimeDelta::FromMilliseconds(50));

  WebViewImpl* web_view = web_view_helper_.Initialize();

  WebURL base_url = url_test_helpers::ToKURL("http://example.com/");
  frame_test_helpers::LoadHTMLString(web_view->MainFrameImpl(),
                                     "<html><body></body></html>", base_url);

  clock.Advance(TimeDelta::FromMilliseconds(70));

  HistogramTester histogram_tester;
  WebKeyboardEvent key_event1(WebInputEvent::kRawKeyDown,
                              WebInputEvent::kNoModifiers,
                              WebInputEvent::GetStaticTimeStampForTests());
  key_event1.dom_key = ui::DomKey::FromCharacter(' ');
  key_event1.windows_key_code = VKEY_SPACE;
  key_event1.SetTimeStamp(CurrentTimeTicks());
  clock.Advance(TimeDelta::FromMilliseconds(50));
  web_view->MainFrameWidget()->HandleInputEvent(
      WebCoalescedInputEvent(key_event1));

  WebKeyboardEvent key_event2(WebInputEvent::kRawKeyDown,
                              WebInputEvent::kNoModifiers,
                              WebInputEvent::GetStaticTimeStampForTests());
  key_event2.dom_key = ui::DomKey::FromCharacter(' ');
  key_event2.windows_key_code = VKEY_SPACE;
  key_event2.SetTimeStamp(CurrentTimeTicks());
  clock.Advance(TimeDelta::FromMilliseconds(50));
  web_view->MainFrameWidget()->HandleInputEvent(
      WebCoalescedInputEvent(key_event2));

  WebKeyboardEvent key_event3(WebInputEvent::kRawKeyDown,
                              WebInputEvent::kNoModifiers,
                              WebInputEvent::GetStaticTimeStampForTests());
  key_event3.dom_key = ui::DomKey::FromCharacter(' ');
  key_event3.windows_key_code = VKEY_SPACE;
  key_event3.SetTimeStamp(CurrentTimeTicks());
  clock.Advance(TimeDelta::FromMilliseconds(70));
  web_view->MainFrameWidget()->HandleInputEvent(
      WebCoalescedInputEvent(key_event3));

  histogram_tester.ExpectTotalCount("PageLoad.InteractiveTiming.InputDelay3",
                                    3);
  histogram_tester.ExpectBucketCount("PageLoad.InteractiveTiming.InputDelay3",
                                     50, 2);
  histogram_tester.ExpectBucketCount("PageLoad.InteractiveTiming.InputDelay3",
                                     70, 1);

  histogram_tester.ExpectTotalCount(
      "PageLoad.InteractiveTiming.InputTimestamp3", 3);
  histogram_tester.ExpectBucketCount(
      "PageLoad.InteractiveTiming.InputTimestamp3", 70, 1);
  histogram_tester.ExpectBucketCount(
      "PageLoad.InteractiveTiming.InputTimestamp3", 120, 1);
  histogram_tester.ExpectBucketCount(
      "PageLoad.InteractiveTiming.InputTimestamp3", 170, 1);
}

// Tests that if the page was backgrounded while an input event was queued,
// we do not count its delay to calculate longest input delay.
TEST_F(WebViewTest, LongestInputDelayPageBackgroundedDuringQueuing) {
  WebViewImpl* web_view = web_view_helper_.Initialize();
  WebURL base_url = url_test_helpers::ToKURL("http://example.com/");
  frame_test_helpers::LoadHTMLString(web_view->MainFrameImpl(),
                                     "<html><body></body></html>", base_url);

  LocalFrame* main_frame = web_view->MainFrameImpl()->GetFrame();
  ASSERT_NE(nullptr, main_frame);

  Document* document = main_frame->GetDocument();
  ASSERT_NE(nullptr, document);

  WTF::ScopedMockClock clock;
  clock.Advance(TimeDelta::FromMilliseconds(70));

  InteractiveDetector* interactive_detector(
      InteractiveDetector::From(*document));
  ASSERT_NE(nullptr, interactive_detector);

  EXPECT_TRUE(interactive_detector->GetLongestInputDelay().is_zero());

  WebKeyboardEvent key_event1(WebInputEvent::kRawKeyDown,
                              WebInputEvent::kNoModifiers,
                              WebInputEvent::GetStaticTimeStampForTests());
  key_event1.dom_key = ui::DomKey::FromCharacter(' ');
  key_event1.windows_key_code = VKEY_SPACE;
  TimeTicks key_event1_time = CurrentTimeTicks();
  key_event1.SetTimeStamp(key_event1_time);
  clock.Advance(TimeDelta::FromMilliseconds(50));
  web_view->MainFrameWidget()->HandleInputEvent(
      WebCoalescedInputEvent(key_event1));

  WebKeyboardEvent key_event2(WebInputEvent::kRawKeyDown,
                              WebInputEvent::kNoModifiers,
                              WebInputEvent::GetStaticTimeStampForTests());
  key_event2.dom_key = ui::DomKey::FromCharacter(' ');
  key_event2.windows_key_code = VKEY_SPACE;
  key_event2.SetTimeStamp(CurrentTimeTicks());
  clock.Advance(TimeDelta::FromMilliseconds(100));
  web_view->SetIsHidden(/*is_hidden=*/true, /*initial_state=*/false);
  clock.Advance(TimeDelta::FromMilliseconds(100));
  web_view->SetIsHidden(/*is_hidden=*/false, /*initial_state=*/false);
  clock.Advance(TimeDelta::FromMilliseconds(100));
  // Total input delay is >300ms.
  web_view->MainFrameWidget()->HandleInputEvent(
      WebCoalescedInputEvent(key_event2));

  EXPECT_NEAR(
      50, interactive_detector->GetLongestInputDelay().InMillisecondsF(), 0.01);
  EXPECT_EQ(key_event1_time, interactive_detector->GetLongestInputTimestamp());
}

// Tests that if the page was backgrounded at navigation start and an input
// event was queued before it was foregrounded, we do not count its delay to
// calculate longest input delay.
TEST_F(WebViewTest, LongestInputDelayPageBackgroundedAtNavStart) {
  WebViewImpl* web_view = web_view_helper_.Initialize();
  web_view->SetIsHidden(/*is_hidden=*/true, /*initial_state=*/false);
  WebURL base_url = url_test_helpers::ToKURL("http://example.com/");
  frame_test_helpers::LoadHTMLString(web_view->MainFrameImpl(),
                                     "<html><body></body></html>", base_url);

  LocalFrame* main_frame = web_view->MainFrameImpl()->GetFrame();
  ASSERT_NE(nullptr, main_frame);

  Document* document = main_frame->GetDocument();
  ASSERT_NE(nullptr, document);

  WTF::ScopedMockClock clock;
  clock.Advance(TimeDelta::FromMilliseconds(70));

  InteractiveDetector* interactive_detector(
      InteractiveDetector::From(*document));
  ASSERT_NE(nullptr, interactive_detector);

  WebKeyboardEvent key_event(WebInputEvent::kRawKeyDown,
                             WebInputEvent::kNoModifiers,
                             WebInputEvent::GetStaticTimeStampForTests());
  key_event.dom_key = ui::DomKey::FromCharacter(' ');
  key_event.windows_key_code = VKEY_SPACE;
  key_event.SetTimeStamp(CurrentTimeTicks());
  clock.Advance(TimeDelta::FromMilliseconds(100));
  web_view->SetIsHidden(/*is_hidden=*/false, /*initial_state=*/false);
  web_view->MainFrameWidget()->HandleInputEvent(
      WebCoalescedInputEvent(key_event));

  EXPECT_TRUE(interactive_detector->GetLongestInputDelay().is_zero());
}

// Tests page backgrounding outside of input queuing time does not affect
// longest input delay.
TEST_F(WebViewTest, LongestInputDelayPageBackgroundedNotDuringQueuing) {
  WebViewImpl* web_view = web_view_helper_.Initialize();
  WebURL base_url = url_test_helpers::ToKURL("http://example.com/");
  frame_test_helpers::LoadHTMLString(web_view->MainFrameImpl(),
                                     "<html><body></body></html>", base_url);

  LocalFrame* main_frame = web_view->MainFrameImpl()->GetFrame();
  ASSERT_NE(nullptr, main_frame);

  Document* document = main_frame->GetDocument();
  ASSERT_NE(nullptr, document);

  WTF::ScopedMockClock clock;
  clock.Advance(TimeDelta::FromMilliseconds(70));

  InteractiveDetector* interactive_detector(
      InteractiveDetector::From(*document));
  ASSERT_NE(nullptr, interactive_detector);

  EXPECT_TRUE(interactive_detector->GetLongestInputDelay().is_zero());

  web_view->SetIsHidden(/*is_hidden=*/true, /*initial_state=*/false);
  clock.Advance(TimeDelta::FromMilliseconds(100));
  web_view->SetIsHidden(/*is_hidden=*/false, /*initial_state=*/false);
  clock.Advance(TimeDelta::FromMilliseconds(1));

  WebKeyboardEvent key_event(WebInputEvent::kRawKeyDown,
                             WebInputEvent::kNoModifiers,
                             WebInputEvent::GetStaticTimeStampForTests());
  key_event.dom_key = ui::DomKey::FromCharacter(' ');
  key_event.windows_key_code = VKEY_SPACE;
  TimeTicks key_event_time = CurrentTimeTicks();
  key_event.SetTimeStamp(key_event_time);
  clock.Advance(TimeDelta::FromMilliseconds(50));
  web_view->MainFrameWidget()->HandleInputEvent(
      WebCoalescedInputEvent(key_event));

  EXPECT_NEAR(
      50, interactive_detector->GetLongestInputDelay().InMillisecondsF(), 0.01);
  EXPECT_EQ(key_event_time, interactive_detector->GetLongestInputTimestamp());
}

// TODO(npm): Improve this test to receive real input sequences and avoid hacks.
// Check that first input delay is correctly reported to the document when the
// first input is a pointer down event, and we receive a pointer up event.
TEST_F(WebViewTest, PointerDownUpFirstInputDelay) {
  WebViewImpl* web_view = web_view_helper_.Initialize();
  WebURL base_url = url_test_helpers::ToKURL("http://example.com/");
  frame_test_helpers::LoadHTMLString(web_view->MainFrameImpl(),
                                     "<html><body></body></html>", base_url);
  // Add an event listener for pointerdown to ensure it is not optimized out
  // before reaching the EventDispatcher.
  WebLocalFrame* frame = web_view_helper_.LocalMainFrame();
  frame->ExecuteScript(
      WebScriptSource("addEventListener('pointerdown', function() {});"));

  LocalFrame* main_frame = web_view->MainFrameImpl()->GetFrame();
  ASSERT_NE(nullptr, main_frame);

  Document* document = main_frame->GetDocument();
  ASSERT_NE(nullptr, document);

  WTF::ScopedMockClock clock;
  clock.Advance(TimeDelta::FromMilliseconds(70));

  InteractiveDetector* interactive_detector(
      InteractiveDetector::From(*document));
  ASSERT_NE(nullptr, interactive_detector);

  WebPointerEvent pointer_down(
      WebInputEvent::kPointerDown,
      WebPointerProperties(1, WebPointerProperties::PointerType::kTouch), 5, 5);
  pointer_down.SetTimeStamp(CurrentTimeTicks());
  // Set this to the left button, needed for testing to behave properly.
  pointer_down.SetModifiers(WebInputEvent::kLeftButtonDown);
  pointer_down.button = WebPointerProperties::Button::kLeft;
  clock.Advance(TimeDelta::FromMilliseconds(50));
  web_view->MainFrameWidget()->HandleInputEvent(
      WebCoalescedInputEvent(pointer_down));

  // We don't know if this pointer event will result in a scroll or not, so we
  // can't report its delay. We don't consider a scroll to be meaningful input.
  EXPECT_TRUE(interactive_detector->GetFirstInputDelay().is_zero());

  // When we receive a pointer up, we report the delay of the pointer down.
  WebPointerEvent pointer_up(
      WebInputEvent::kPointerUp,
      WebPointerProperties(1, WebPointerProperties::PointerType::kTouch), 5, 5);
  clock.Advance(TimeDelta::FromMilliseconds(60));
  pointer_up.SetTimeStamp(CurrentTimeTicks());
  web_view->MainFrameWidget()->HandleInputEvent(
      WebCoalescedInputEvent(pointer_up));

  EXPECT_NEAR(50, interactive_detector->GetFirstInputDelay().InMillisecondsF(),
              0.01);
  EXPECT_EQ(70, interactive_detector->GetFirstInputTimestamp()
                    .since_origin()
                    .InMillisecondsF());
}

// We need a way for JS to advance the mock clock. Hook into console.log, so
// that logging advances the clock by |event_handling_delay| seconds.
class MockClockAdvancingWebFrameClient
    : public frame_test_helpers::TestWebFrameClient {
 public:
  MockClockAdvancingWebFrameClient(WTF::ScopedMockClock* mock_clock,
                                   TimeDelta event_handling_delay)
      : mock_clock_(mock_clock), event_handling_delay_(event_handling_delay) {}
  // WebLocalFrameClient overrides:
  void DidAddMessageToConsole(const WebConsoleMessage& message,
                              const WebString& source_name,
                              unsigned source_line,
                              const WebString& stack_trace) override {
    mock_clock_->Advance(event_handling_delay_);
  }

 private:
  WTF::ScopedMockClock* mock_clock_;
  TimeDelta event_handling_delay_;
};

// Check that the input delay is correctly reported to the document.
TEST_F(WebViewTest, FirstInputDelayExcludesProcessingTime) {
  WTF::ScopedMockClock clock;
  // Page load timing logic depends on the time not being zero.
  clock.Advance(TimeDelta::FromMilliseconds(1));
  MockClockAdvancingWebFrameClient frame_client(
      &clock, TimeDelta::FromMilliseconds(6000));
  WebViewImpl* web_view = web_view_helper_.Initialize(&frame_client);
  WebURL base_url = url_test_helpers::ToKURL("http://example.com/");
  frame_test_helpers::LoadHTMLString(web_view->MainFrameImpl(),
                                     "<html><body></body></html>", base_url);

  LocalFrame* main_frame = web_view->MainFrameImpl()->GetFrame();
  ASSERT_NE(nullptr, main_frame);

  Document* document = main_frame->GetDocument();
  ASSERT_NE(nullptr, document);

  WebLocalFrame* frame = web_view_helper_.LocalMainFrame();
  // console.log will advance the mock clock.
  frame->ExecuteScript(
      WebScriptSource("document.addEventListener('keydown', "
                      "() => {console.log('advancing timer');})"));

  InteractiveDetector* interactive_detector(
      InteractiveDetector::From(*document));
  ASSERT_NE(nullptr, interactive_detector);

  WebKeyboardEvent key_event(WebInputEvent::kRawKeyDown,
                             WebInputEvent::kNoModifiers,
                             WebInputEvent::GetStaticTimeStampForTests());
  key_event.dom_key = ui::DomKey::FromCharacter(' ');
  key_event.windows_key_code = VKEY_SPACE;
  key_event.SetTimeStamp(CurrentTimeTicks());

  clock.Advance(TimeDelta::FromMilliseconds(5000));

  web_view->MainFrameWidget()->HandleInputEvent(
      WebCoalescedInputEvent(key_event));

  TimeDelta first_input_delay = interactive_detector->GetFirstInputDelay();
  EXPECT_EQ(5000, first_input_delay.InMillisecondsF());

  web_view_helper_.Reset();  // Remove dependency on locally scoped client.
}

// Check that the longest input delay is correctly reported to the document.
TEST_F(WebViewTest, LongestInputDelayExcludesProcessingTime) {
  WTF::ScopedMockClock clock;
  // Page load timing logic depends on the time not being zero.
  clock.Advance(TimeDelta::FromMilliseconds(1));
  MockClockAdvancingWebFrameClient frame_client(
      &clock, TimeDelta::FromMilliseconds(6000));
  WebViewImpl* web_view = web_view_helper_.Initialize(&frame_client);
  WebURL base_url = url_test_helpers::ToKURL("http://example.com/");
  frame_test_helpers::LoadHTMLString(web_view->MainFrameImpl(),
                                     "<html><body></body></html>", base_url);

  LocalFrame* main_frame = web_view->MainFrameImpl()->GetFrame();
  ASSERT_NE(nullptr, main_frame);

  Document* document = main_frame->GetDocument();
  ASSERT_NE(nullptr, document);

  WebLocalFrame* frame = web_view_helper_.LocalMainFrame();
  // console.log will advance the mock clock.
  frame->ExecuteScript(
      WebScriptSource("document.addEventListener('keydown', "
                      "() => {console.log('advancing timer');})"));

  InteractiveDetector* interactive_detector(
      InteractiveDetector::From(*document));
  ASSERT_NE(nullptr, interactive_detector);

  WebKeyboardEvent key_event(WebInputEvent::kRawKeyDown,
                             WebInputEvent::kNoModifiers,
                             WebInputEvent::GetStaticTimeStampForTests());
  key_event.dom_key = ui::DomKey::FromCharacter(' ');
  key_event.windows_key_code = VKEY_SPACE;
  key_event.SetTimeStamp(CurrentTimeTicks());

  clock.Advance(TimeDelta::FromMilliseconds(5000));

  web_view->MainFrameWidget()->HandleInputEvent(
      WebCoalescedInputEvent(key_event));

  TimeDelta longest_input_delay = interactive_detector->GetLongestInputDelay();
  EXPECT_EQ(5000, longest_input_delay.InMillisecondsF());

  web_view_helper_.Reset();  // Remove dependency on locally scoped client.
}

TEST_F(WebViewTest, RootLayerAttachment) {
  WebView* web_view = web_view_helper_.InitializeAndLoad("about:blank");

  // Do a lifecycle update that includes compositing but not paint. Hit test
  // events are an example of a real case where this occurs
  // (see: WebViewTest::ClientTapHandling).
  web_view->MainFrameWidget()->UpdateLifecycle(
      WebFrameWidget::LifecycleUpdate::kPrePaint,
      WebWidget::LifecycleUpdateReason::kTest);

  // With BlinkGenPropertyTrees, layers (including the root layer) should not be
  // attached until the paint lifecycle phase.
  auto* layer_tree_view = web_view_helper_.GetLayerTreeView();
  EXPECT_FALSE(layer_tree_view->GetRootLayer());

  // Do a full lifecycle update and ensure that the root layer has been added.
  web_view->MainFrameWidget()->UpdateLifecycle(
      WebFrameWidget::LifecycleUpdate::kAll,
      WebWidget::LifecycleUpdateReason::kTest);
  EXPECT_TRUE(layer_tree_view->GetRootLayer());
}

// Verifies that we emit Blink.UseCounter.FeaturePolicy.PotentialAnimation for
// CSS and JS animations in a document.
TEST_F(WebViewTest, PotentialViolationReportsForLayoutAnimations) {
  const char* kHistogramName =
      "Blink.UseCounter.FeaturePolicy.PotentialViolation";
  WebViewImpl* web_view = web_view_helper_.Initialize();
  // A page with non-violating animation does not generate report.
  WebURL base_url_no_violation =
      url_test_helpers::ToKURL("http://good-css.example.com/");
  frame_test_helpers::LoadHTMLString(
      web_view->MainFrameImpl(),
      "<html><head><style>@keyframes foo {from "
      "{color: blue;} to {color: red}}</style></head></html>",
      base_url_no_violation);
  HistogramTester histogram_tester;
  histogram_tester.ExpectTotalCount(kHistogramName, 0);
  // Page with 2 potential (CSS) layout-animation violations.
  WebURL base_url_css_violations =
      url_test_helpers::ToKURL("http://bad-css.example.com/");
  frame_test_helpers::LoadHTMLString(
      web_view->MainFrameImpl(),
      "<html><head><style>@keyframes bar {"
      "from{height: 100px;} to {height: 200px;}}"
      "@keyframes baz {from{top: 100px;} to {top: 200px;}}"
      "</style></head></html>",
      base_url_css_violations);
  histogram_tester.ExpectTotalCount(kHistogramName, 1);
  // Page with a JS layout-animations violation.
  WebURL base_url_js_violations =
      url_test_helpers::ToKURL("http://js.example.com/");
  frame_test_helpers::LoadHTMLString(
      web_view->MainFrameImpl(),
      "<html><body><div></div><script>document.body.firstChild.animate("
      "{top: '100px'});</script></body></html>",
      base_url_js_violations);
  histogram_tester.ExpectTotalCount(kHistogramName, 2);
}

TEST_F(WebViewTest, ForceDarkModeInvalidatesPaint) {
  WebViewImpl* web_view = web_view_helper_.Initialize();
  web_view->MainFrameWidget()->Resize(WebSize(500, 500));
  UpdateAllLifecyclePhases();

  Document* document = web_view->MainFrameImpl()->GetFrame()->GetDocument();
  ASSERT_TRUE(document);
  web_view->GetSettings()->SetForceDarkModeEnabled(true);
  EXPECT_TRUE(document->GetLayoutView()->ShouldDoFullPaintInvalidation());
}

}  // namespace blink
