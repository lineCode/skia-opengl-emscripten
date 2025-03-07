﻿#include "skia_ui_demo.h"

#if defined(ENABLE_SKIA)

#include "gl_helpers.h"

#include "utils.h"

#ifdef ENABLE_COBALT
#include "cobalt/base/polymorphic_downcast.h"
#endif ENABLE_COBALT

#ifdef ENABLE_BASE

#include "base/memory/ptr_util.h"
#include "base/macros.h"
#include "base/strings/stringprintf.h"
#include "base/stl_util.h"
#include "base/strings/utf_string_conversions.h"
//#include "base/task/sequence_manager/thread_controller_with_message_pump_impl.h"
#include "base/bind.h"
#include "base/bind_helpers.h"
#include "base/memory/scoped_refptr.h"
#include "base/single_thread_task_runner.h"
#include "base/threading/thread_task_runner_handle.h"

#include "base/numerics/checked_math.h"
#include "base/numerics/clamped_math.h"
#include "base/numerics/safe_conversions.h"

#include "base/i18n/icu_string_conversions.h"
#include "base/i18n/string_compare.h"

#include "base/stl_util.h"
#include "base/strings/string_number_conversions.h"
#include "base/strings/sys_string_conversions.h"
#include "base/base_switches.h"
#include "base/command_line.h"
#include "base/containers/small_map.h"
#include "base/i18n/icu_util.h"
#include "base/i18n/rtl.h"
#include "base/allocator/partition_allocator/page_allocator.h"

#include "base/memory/scoped_refptr.h"

#include "base/i18n/rtl.h"
#include "base/stl_util.h"

#include "base/memory/ref_counted_memory.h"
#include "base/memory/read_only_shared_memory_region.h"
#include "base/stl_util.h"
#include "base/bind.h"
#include "base/memory/weak_ptr.h"
//#include "base/test/gtest_util.h"
#include "base/threading/thread.h"
#include "base/logging.h"
#include "base/system/sys_info.h"

#include "base/synchronization/waitable_event.h"

//#include "base/task/sequence_manager/sequence_manager.h"

#include "base/observer_list.h"
#include "base/synchronization/lock.h"
#include "base/synchronization/waitable_event.h"
#include "base/threading/thread.h"
#include "base/timer/timer.h"

#include "base/callback.h"
//#include "base/containers/hash_tables.h"
#include "base/bind.h"
#include "base/callback.h"
#include "base/command_line.h"
#include "base/logging.h"
#include "base/memory/weak_ptr.h"
#include "base/message_loop/message_loop.h"
#include "base/optional.h"
#include "base/strings/stringprintf.h"
#include "base/synchronization/waitable_event.h"
//#include "base/trace_event/trace_event.h"
#include "base/bind.h"
#include "base/strings/stringprintf.h"
#include "base/trace_event/trace_event.h"
#include "base/callback.h"
////#include "base/message_loop.h"

#include "base/files/file_path.h"
#include "base/message_loop/message_loop.h"
#include "base/synchronization/waitable_event.h"
#include "base/threading/thread.h"
#include "base/threading/thread_checker.h"

#include "base/feature_list.h"

// Create a TYPE_DEFAULT message-loop.

#if defined(__EMSCRIPTEN__)

/// \note defined by CMAKE
//#define ENABLE_EMSCRIPTEN_INPUT 1

//static base::MessageLoop browser_loop;
//base::RunLoop run_loop;
//static scoped_refptr<base::SingleThreadTaskRunner> task_runner =
#endif // __EMSCRIPTEN__

#endif // ENABLE_BASE

#include "blink_demo.h"

#ifdef ENABLE_BLINK_UI
#include "ui/display/display.h"
#include "ui/display/display_switches.h"
#include "ui/display/display_list.h"
#include "ui/display/display_observer.h"
#include "ui/gfx/geometry/insets.h"
#include "ui/gfx/geometry/rect.h"
#include "ui/gfx/geometry/size.h"
#endif // ENABLE_UI

#ifdef ENABLE_BLINK_UI_NATIVE_THEME
#include "ui/native_theme/native_theme.h"
#include "ui/native_theme/native_theme_aura.h"
#include "third_party/skia/include/core/SkPath.h"

#include "ui/gfx/geometry/size.h"
#include "ui/gfx/skia_util.h"
#include "ui/gfx/canvas.h"
#include "ui/gfx/color_utils.h"
#include "ui/gfx/geometry/rect.h"
#include "ui/gfx/geometry/insets.h"
#include "ui/gfx/geometry/vector2d.h"
//#include "ui/gfx/native_widget_types.h"
#endif // ENABLE_BLINK_UI_NATIVE_THEME

#ifdef ENABLE_BLINK_UI
static ::std::unique_ptr<gfx::ImageSkia> gfxImageSkia;
static sk_sp<SkImage> skImageSp;
#endif // ENABLE_UI

#if defined(ENABLE_BLINK_UI_VIEWS)
#include "ui/display/manager/default_touch_transform_setter.h"
#include "ui/display/manager/display_manager.h"
#include "ui/display/manager/test/touch_device_manager_test_api.h"
#include "ui/display/manager/touch_device_manager.h"
#include "ui/display/screen_base.h"
#include "ui/events/devices/device_data_manager.h"
#include "base/strings/string_number_conversions.h"

#include "ui/compositor/compositor.h"
//#include "ui/compositor/test/test_context_factories.h"

#include "cc/layers/layer.h"
#include "cc/trees/layer_tree_host.h"

#include "base/macros.h"
#include "ui/views/controls/button/button.h"
#include "ui/views/controls/button/label_button.h"
#include "ui/views/controls/button/radio_button.h"
#include "ui/views/controls/combobox/combobox_listener.h"
#include "ui/views/controls/textfield/textfield_controller.h"
#include "ui/views/controls/button/image_button.h"
#include "ui/views/controls/native/native_view_host.h"
#include "ui/views/controls/scroll_view.h"
#include "ui/views/controls/textfield/textfield.h"
#include "ui/views/controls/textfield/textfield_controller.h"
#include "ui/views/controls/button/checkbox.h"
#include "ui/views/controls/combobox/combobox.h"
#include "ui/views/controls/label.h"
#include "ui/views/controls/textfield/textfield.h"
#include "ui/views/controls/combobox/combobox.h"
#include "ui/views/controls/label.h"
#include "ui/views/controls/button/checkbox.h"
#include "ui/views/controls/scroll_view.h"
#include "ui/views/controls/throbber.h"
#include "ui/views/controls/tabbed_pane/tabbed_pane.h"
#include "ui/views/controls/tabbed_pane/tabbed_pane_listener.h"
#include "ui/views/examples/example_base.h"
#include "base/memory/ptr_util.h"
#include "base/stl_util.h"
#include "base/strings/utf_string_conversions.h"
#include "ui/views/background.h"
#include "ui/views/border.h"
#include "ui/views/paint_info.h"
#include "ui/views/background.h"
#include "ui/views/metadata/metadata_types.h"
#include "ui/views/view_observer.h"
#include "ui/views/widget/native_widget.h"
#include "ui/views/widget/root_view.h"
#include "ui/base/ime/text_input_client.h"
#include "ui/base/ime/input_method.h"
#include "ui/base/l10n/l10n_font_util.h"
#include "ui/events/event.h"
#include "ui/events/event_utils.h"
#include "ui/views/focus/focus_manager_factory.h"
#include "ui/views/focus/widget_focus_manager.h"
#include "ui/views/window/dialog_client_view.h"
#include "ui/views/window/dialog_delegate.h"
#include "ui/views/examples/example_combobox_model.h"
#include "ui/views/layout/box_layout.h"
#include "ui/views/layout/grid_layout.h"
#include "ui/views/layout/fill_layout.h"
#include "ui/views/view.h"
#include "ui/views/layout/layout_provider.h"
#include "ui/views/views_delegate.h"
#include "ui/views/widget/widget.h"
#include "ui/views/widget/widget_delegate.h"
#include "base/macros.h"
#include "base/run_loop.h"
#include "base/strings/utf_string_conversions.h"
#include "ui/base/l10n/l10n_util.h"
#include "ui/base/models/combobox_model.h"
#include "ui/base/ui_base_paths.h"
#include "ui/views/background.h"
#include "ui/views/view_class_properties.h"

#include "ui/base/resource/data_pack.h"
#include "ui/base/resource/resource_bundle.h"

#include "ui/compositor/paint_recorder.h"
#include "ui/compositor/canvas_painter.h"

static gfx::FontList GetTextFontList();

static void recursivePaintHelper(views::View* view, gfx::Canvas* gfx_canvas) {
  views::View::Views children = view->GetChildrenInZOrder();
  // TODO:
  gfx_canvas->Translate(gfx::Vector2d(view->origin().x(), view->origin().y()));
  view->OnPaint(gfx_canvas);
  for (auto* child : children) {
    if (!child->layer())
      recursivePaintHelper(child, gfx_canvas);
  }
}

class TestLabel : public views::Label {
 public:
  TestLabel(const base::string16& text,
            const views::Label::CustomFont& font)
                      : Label(text, font) {
    SizeToPreferredSize();
  }

  int schedule_paint_count() const { return schedule_paint_count_; }

  void SimulatePaint() {
    SkColor color = SK_ColorTRANSPARENT;
    Paint(views::PaintInfo::CreateRootPaintInfo(
        ui::CanvasPainter(&bitmap, bounds().size(), 1.f, color, false)
            .context(),
        bounds().size()));
  }

  // View:
  void SchedulePaintInRect(const gfx::Rect& r) override {
    ++schedule_paint_count_;
    Label::SchedulePaintInRect(r);
  }

 //private:
  int schedule_paint_count_ = 0;
  SkBitmap bitmap;

  DISALLOW_COPY_AND_ASSIGN(TestLabel);
};

// TODO: free mem
static TestLabel* test_label = nullptr;

static int testScrollY = 0;

class ScrollableView :
  public views::View
 {
 public:
  ScrollableView() {
    SetColor(SK_ColorRED, SK_ColorCYAN);
    auto lb1 =
      new views::LabelButton(nullptr,
        base::ASCIIToUTF16("Button"),
        views::style::CONTEXT_BUTTON);
    AddChildView(lb1);
    lb1->SetBounds(0, 0, 100, 100);
    lb1->SetPreferredSize(gfx::Size(100, 100));
    lb1->SetSize(gfx::Size(100, 100));
    lb1->SetBorder(views::CreateSolidBorder(2, SK_ColorGRAY));
    //lb1->SetVisible(true);
    lb1->SetEnabled(true);

    auto lb2 =
      new views::RadioButton(base::ASCIIToUTF16("Radio Button"), 0);
    AddChildView(lb2);
    lb2->SetBounds(0, 50, 100, 100);
    lb2->SetPreferredSize(gfx::Size(100, 100));
    lb2->SetSize(gfx::Size(100, 100));
    lb2->SetBorder(views::CreateSolidBorder(2, SK_ColorGRAY));
    //lb2->SetVisible(true);
    lb2->SetEnabled(true);

    InvalidateLayout();
    SizeToPreferredSize();
    Layout();
  }

  void SetColor(SkColor from, SkColor to) {
    from_color_ = from;
    to_color_ = to;
  }

  /*void PlaceChildY(size_t index, int y) {
    View* view = children()[index];
    DCHECK(view);
    gfx::Size size = view->GetPreferredSize();
    view->SetBounds(0, y, size.width(), size.height());
  }

  // View
  void Layout() override {
    DCHECK(children().size() > 1);
    PlaceChildY(0, 0);
    PlaceChildY(1, height() / 2);
    SizeToPreferredSize();
  }*/

  void OnPaintBackground(gfx::Canvas* canvas) override {
    cc::PaintFlags flags;
    flags.setShader(
        gfx::CreateGradientShader(0, height(), from_color_, to_color_));
    flags.setStyle(cc::PaintFlags::kFill_Style);
    canvas->DrawRect(GetLocalBounds(), flags);
  }

  /*gfx::Size CalculatePreferredSize() const override {
    return gfx::Size(200, 200);//gfx::Size(width(), height());
  }*/

 private:
  SkColor from_color_;
  SkColor to_color_;

  DISALLOW_COPY_AND_ASSIGN(ScrollableView);
};

// TODO: OnPaintLayer
class ContainerView :
  public views::View,
  public views::TextfieldController,
  public views::ButtonListener,
  public views::TabbedPaneListener {
 public:
  std::unique_ptr<views::LayoutProvider> layout_provider_ =
      std::make_unique<views::LayoutProvider>();

  explicit ContainerView(/*ExampleBase* base,views::GridLayout* layout*/)
      : example_view_created_(false)/*,
        example_base_(base)*/ {
  }


  // Overridden from views::TextfieldController:
  /*void ContentsChanged(views::Textfield* sender,
                       const base::string16& new_contents) override;
  bool HandleMouseEvent(views::Textfield* sender,
                        const ui::MouseEvent& mouse_event) override;
  bool HandleGestureEvent(views::Textfield* sender,
                          const ui::GestureEvent& gesture_event) override;
*/

void ContentsChanged(views::Textfield* sender,
                                        const base::string16& new_contents) override {
  printf("ContentsChanged %s\n", new_contents.c_str());
  // Set search box focused when query changes.
  ///search_box_->RequestFocus();
  //UpdateModel(true);
  ///NotifyQueryChanged();
  ///if (!new_contents.empty())
  ///  SetSearchBoxActive(true, ui::ET_KEY_PRESSED);
  ///UpdateButtonsVisisbility();

  SchedulePaint();
}

bool HandleMouseEvent(views::Textfield* sender,
                                         const ui::MouseEvent& mouse_event) override {
  //return OnTextfieldEvent(mouse_event.type());
  SchedulePaint();
  return true;
}

bool HandleGestureEvent(
    views::Textfield* sender,
    const ui::GestureEvent& gesture_event) override {
  //return OnTextfieldEvent(gesture_event.type());

  SchedulePaint();
  return true;
}

  std::unique_ptr<views::PaintInfo> last_paint_info_;

  void PaintChildren(const views::PaintInfo& info) override {
    last_paint_info_ = std::make_unique<views::PaintInfo>(info);
    View::PaintChildren(info);
  }

  std::unique_ptr<views::PaintInfo> GetLastPaintInfo() {
    return std::move(last_paint_info_);
  }

  void TabSelectedAt(int index) override {
    // Just print the status when selection changes.
    DCHECK(tabbed_pane_);
    printf("TabSelectedAt %d "
           "(GetTabCount %d GetSelectedTabIndex %d)\n",
           index,
           tabbed_pane_->GetTabCount(),
           tabbed_pane_->GetSelectedTabIndex());
  }

  /*void OnPaintBackground(gfx::Canvas* canvas) override {
    cc::PaintFlags flags;
    flags.setShader(
        gfx::CreateGradientShader(0, height(), from_color_, to_color_));
    flags.setStyle(cc::PaintFlags::kFill_Style);
    canvas->DrawRect(GetLocalBounds(), flags);
  }*/

  void ButtonPressed(views::Button* sender, const ui::Event& event) override {
    printf("ButtonPressed\n");
    DCHECK(scroll_to_);
    if (sender == scroll_to_) {
      DCHECK(scroll_view_);
      scroll_view_->contents()->ScrollRectToVisible(
          gfx::Rect(20, 10, 10, 50));
      scroll_view_->InvalidateLayout();
    }
  }

  void addChildren(views::GridLayout* layout) {
    DCHECK(layout);

    const gfx::FontList& font_list = GetTextFontList();

    title_ = new views::Label();
    title_->SetFontList(font_list);
    title_->SetHorizontalAlignment(gfx::ALIGN_LEFT);
    title_->SetEnabledColor(
      blink::Color(1.0f, 0.5f, 0.5f, 0.5f).Rgb());
    title_->SelectRange({0, 4});
    title_->SetBackgroundColor(
      blink::Color(1.0f, 1.0f, 1.0f, 1.0f).Rgb());
    title_->SetBorder(views::CreateSolidBorder(2, SK_ColorWHITE));
    set_title(base::UTF8ToUTF16("title_ ! title_ ! title_ !"));
    //AddChildView(title_);

    message_ = new views::Label();
    message_->SetFontList(font_list);
    message_->SetHorizontalAlignment(gfx::ALIGN_RIGHT);
    message_->SetObscured(false);
    message_->SetSelectable(true);
    message_->SetSelectionBackgroundColor(
      blink::Color(0.1f, 0.2f, 0.0f, 0.5f).Rgb());
    message_->SetSelectionTextColor(
      blink::Color(0.4f, 0.4f, 0.9f, 0.5f).Rgb());
    message_->SetBackgroundColor(
      blink::Color(0.9f, 0.0f, 0.9f, 0.5f).Rgb());
    message_->SetElideBehavior(gfx::ELIDE_TAIL);
    message_->SetEnabledColor(
      blink::Color(1.0f, 0.0f, 1.0f, 0.5f).Rgb());
    set_message(base::UTF8ToUTF16("message_ ! message_ ! message_ !"));
    message_->SelectRange({0, 4});
    //message_->RecalculateFont();
    message_->SetBorder(views::CreateSolidBorder(2, SK_ColorCYAN));
    message_->SetHorizontalAlignment(gfx::ALIGN_TO_HEAD);
    //AddChildView(message_);

    DCHECK(!textfield_);
    textfield_ = new views::Textfield();
    const int size_delta = std::abs(font_list.GetFontSize() - 15); // size in pixels to add
    DCHECK(size_delta > 0);
    textfield_->SetFontList(font_list.Derive(
        -size_delta, gfx::Font::UNDERLINE, gfx::Font::Weight::BOLD));
    //textfield_->SetSize(gfx::Size(100, 100));
    textfield_->SetBorder(
      views::CreateSolidBorder(2, SK_ColorGRAY));
    textfield_->SetColor(
      blink::Color(1.0f, 0.0f, 1.0f, 0.5f).Rgb());
    textfield_->SetTextInputType(ui::TEXT_INPUT_TYPE_TEXT);
    textfield_->set_placeholder_text(
      base::ASCIIToUTF16("TEXT_1"));
    textfield_->set_controller(this);
    //AddChildView(textfield_);

    DCHECK(!textfield2_);
    textfield2_ = new views::Textfield();
    textfield2_->SetFontList(font_list);
    //textfield2_->SetSize(gfx::Size(100, 100));
    textfield2_->SetBorder(
      views::CreateSolidBorder(2, SK_ColorGRAY));
    textfield2_->SetColor(
      blink::Color(1.0f, 0.0f, 1.0f, 0.5f).Rgb());
    textfield2_->SetTextInputType(ui::TEXT_INPUT_TYPE_TEXT);
    textfield2_->set_placeholder_text(
      base::ASCIIToUTF16("TEXT_2"));
    textfield2_->set_controller(this);
    //

    checkbox_ = new views::Checkbox(
      base::ASCIIToUTF16("Checkbox label"));
    //checkbox_->SetFocusntList(font_list);
    checkbox_->SetBorder(
      views::CreateSolidBorder(2, SK_ColorBLACK));
    checkbox_->SetSize(gfx::Size(100, 100));
    checkbox_->SetBackground(views::CreateSolidBackground(
        blink::Color(1.0f, 0.0f, 1.0f, 0.5f).Rgb()));
    checkbox_->SetChecked(true);
    checkbox_->SetBounds(0,0,100,100);
    //AddChildView(checkbox_);

    scroll_view_ = new views::ScrollView();
    DCHECK(scroll_view_);

    scrollable_ptr_ = std::make_unique<ScrollableView>();
    scrollable_ = scrollable_ptr_.get();

    DCHECK(scrollable_);
    scrollable_->SetColor(SK_ColorYELLOW, SK_ColorCYAN);
    scrollable_->SetBounds(0, 0, 300, 300);
    scrollable_->SetPreferredSize(gfx::Size(300, 300));
    scrollable_->SetSize(gfx::Size(300, 300));
    scrollable_->SetBorder(views::CreateSolidBorder(2, SK_ColorGRAY));
    //scrollable_->SetVisible(true);
    scrollable_->SetEnabled(true);
    scrollable_->InvalidateLayout();
    scrollable_->SizeToPreferredSize();
    scrollable_->Layout();
    /*scrollable_->SetBounds(0, 0, 300, 300);
    scrollable_->SetPreferredSize(gfx::Size(300, 300));
    scrollable_->SetSize(gfx::Size(300, 300));
    scrollable_->SetBorder(views::CreateSolidBorder(2, SK_ColorGRAY));*/

    DCHECK(scrollable_ptr_);
    auto scrollable_contents = scroll_view_->
      SetContents(std::make_unique<View>());

    /*auto scrollable_contents =
      scroll_view_->SetContents(std::move(scrollable_ptr_));
    DCHECK(scroll_view_->contents());*/

    DCHECK(scrollable_contents);
    DCHECK(scroll_view_->contents());
    scrollable_contents->AddChildView(scrollable_);

    DCHECK(!scrollable_->GetLocalBounds().IsEmpty());
    scrollable_contents->SetBoundsRect(scrollable_->GetLocalBounds());
    scrollable_contents->SetPreferredSize(scrollable_->size());
    scrollable_contents->SetSize(scrollable_->size());
    scrollable_contents->SetBorder(views::CreateSolidBorder(2, SK_ColorGRAY));
    scrollable_contents->SetBackground(views::CreateSolidBackground(
          blink::Color(0.9f, 0.1f, 0.3f, 0.2f).Rgb()));
    scrollable_contents->SetEnabled(true);
    scrollable_contents->InvalidateLayout();
    scrollable_contents->SizeToPreferredSize();
    scrollable_contents->Layout();

    //scroll_view_->contents_viewport_ = scrollable_contents;
      //->AddChildView(scrollable_contents);

#if 0
    auto scrollable_contents = //new views::Textfield();
      scroll_view_->SetContents(std::make_unique<views::Textfield>());

    scrollable_contents->SetFontList(font_list);
    //scrollable_contents->SetSize(gfx::Size(300, 300));
    scrollable_contents->SetBounds(0, 0, 300, 300);
    scrollable_contents->SetPreferredSize(gfx::Size(300, 300));
    scrollable_contents->SetBorder(
      views::CreateSolidBorder(2, SK_ColorGRAY));
    scrollable_contents->SetColor(
      blink::Color(1.0f, 0.0f, 1.0f, 0.5f).Rgb());
    scrollable_contents->SetTextInputType(ui::TEXT_INPUT_TYPE_TEXT);
    scrollable_contents->set_placeholder_text(
      base::ASCIIToUTF16("TEXT_scrollable_contents"));
    scrollable_contents->SetBackground(views::CreateSolidBackground(
          blink::Color(1.0f, 0.5f, 1.0f, 0.5f).Rgb()));
    scrollable_contents->set_controller(this);
#endif // 0

    //scroll_view_->EnableViewPortLayer();
    //scroll_view_->SetControlVisibility(scroll_view_, true);
    //scroll_view_->SetControlVisibility(scrollable_, true);
    //scroll_view_->SetHeaderOrContents(scrollable_, true);
    DCHECK(!scroll_view_->ScrollsWithLayers());

    // TODO
    //scroll_view_->AddChildView(scrollable_);

    // TODO
    DCHECK(!scrollable_->layer());
    //DCHECK(!scroll_view_->contents_viewport_->layer());

    // TODO
    //DCHECK(!scroll_view_->DoesViewportOrScrollViewHaveLayer());

    DCHECK(scroll_view_->contents_viewport_);

    // TODO
    if(!scroll_view_->contents_viewport_->Contains(scrollable_)) {
      scroll_view_->contents_viewport_->AddChildView(scrollable_);
    }

    DCHECK(scroll_view_->contents_viewport_->Contains(scrollable_));

    //scrollable_->SetVisible(true);
    scrollable_->SetEnabled(true);
    scrollable_->InvalidateLayout();
    scrollable_->SizeToPreferredSize();
    scrollable_->Layout();
    DCHECK(!scrollable_->GetLocalBounds().IsEmpty());

    // TODO
    //scroll_view_->ClipHeightTo(/*min_height*/ 0, /*max_height*/ 250);

    scroll_view_->SetBounds(0, 0, 250, 250);
    scroll_view_->SetPreferredSize(gfx::Size(250, 250));
    scroll_view_->SetSize(gfx::Size(250, 250));
    scroll_view_->SetBorder(views::CreateSolidBorder(2, SK_ColorGRAY));
    scroll_view_->SetBackground(views::CreateSolidBackground(
          blink::Color(0.9f, 0.1f, 0.3f, 0.2f).Rgb()));
    //scroll_view_->SetVisible(true);
    scroll_view_->SetEnabled(true);
    scroll_view_->InvalidateLayout();
    scroll_view_->SizeToPreferredSize();
    scroll_view_->Layout();


    tabbed_pane_ = new views::TabbedPane();
    tabbed_pane_->set_listener(this);
    const base::string16 tabbed_label = base::ASCIIToUTF16("Added at 1");
    tabbed_pane_->AddTab(tabbed_label,
      new views::LabelButton(nullptr, tabbed_label));
    const base::string16 tabbed_label_2 = base::ASCIIToUTF16("Added at 2");
    tabbed_pane_->AddTab(tabbed_label_2,
      new views::LabelButton(nullptr, tabbed_label_2));

    scroll_to_ = new views::LabelButton(
      this,
      base::ASCIIToUTF16("Scroll to"),
      views::style::CONTEXT_BUTTON);
    //scroll_to_->set_controller(this);

    auto MakeRow = [layout](int column_set_id, View* view1, View* view2) {
      //DCHECK(view1);
      //DCHECK(view2);
      layout->StartRowWithPadding(0, column_set_id, 0, 0);
      if (view1)
        layout->AddView(view1);
      if (view2)
        layout->AddView(view2);
    };

    // id 1
    DCHECK(scroll_view_);
    MakeRow(1, scroll_view_, nullptr);
    MakeRow(1, checkbox_, nullptr);
    //DCHECK(scrollable_);
    //MakeRow(1, scrollable_, nullptr);
    MakeRow(1, tabbed_pane_, nullptr);

    // id 0
    //MakeRow(0, title_, textfield_);
    MakeRow(0, textfield_, textfield2_);
    //DCHECK(scroll_view_);
    //MakeRow(0, scroll_view_, nullptr);
    //MakeRow(0, checkbox_, message_);
    //MakeRow(0, nullptr, nullptr);
    //MakeRow(0, nullptr, textfield2_);
    MakeRow(0, title_, message_);
    DCHECK(scroll_to_);
    MakeRow(0, scroll_to_, nullptr);
    //DCHECK(scrollable_);
    //MakeRow(0, scrollable_, nullptr);

    DCHECK(scrollable_);
    DCHECK(!scrollable_->size().IsEmpty());
    DCHECK(!scrollable_->GetPreferredSize().IsEmpty());
    DCHECK(!scrollable_->GetBoundsInScreen().IsEmpty());
    DCHECK(!scrollable_->GetLocalBounds().IsEmpty());

    //scrollable_->SetVisible(true);
    scrollable_->SetEnabled(true);
    scrollable_->SchedulePaint();
    scrollable_->SetFocusBehavior(views::View::FocusBehavior::ALWAYS);
    //scrollable_->SetPaintToLayer(ui::LAYER_TEXTURED);

    //scroll_view_->SetVisible(true);
    scroll_view_->SetEnabled(true);
    scroll_view_->SchedulePaint();
    scroll_view_->SetFocusBehavior(views::View::FocusBehavior::ALWAYS);
    //scroll_view_->SetPaintToLayer(ui::LAYER_TEXTURED);

    DCHECK(scroll_view_);
    DCHECK(!scroll_view_->GetLocalBounds().IsEmpty());
    /*scroll_view_->contents()->ScrollRectToVisible(
        gfx::Rect(20, 20, 20, 20));*/
    scroll_view_->InvalidateLayout();
    scroll_view_->Layout();
    scroll_view_->SizeToPreferredSize();

    /*views::View::OnFocus();
    InvalidateLayout();
    SchedulePaint();*/

    // TODO:
    // https://github.com/blockspacer/skia-opengl-emscripten/blob/master/src/chromium/ui/message_center/views/notification_view_md.cc
    // AddPreTargetHandler(click_activator_.get());
  }

  void ForcePaint(gfx::Canvas* gfx_canvas) {
    DCHECK(gfx_canvas);

    /*gfx::RenderText* msg_renderText = message_->GetRenderText();
    DCHECK(msg_renderText);
    msg_renderText->SetMultiline(true);
    msg_renderText->SetWordWrapBehavior(gfx::WRAP_LONG_WORDS);
    msg_renderText->set_clip_to_display_rect(false);
    msg_renderText->set_focused(true);
    const gfx::FontList& font_list = GetTextFontList();
    msg_renderText->SetFontList(font_list);*/

    // TODO: recursive
    //OnPaint(gfx_canvas);
    recursivePaintHelper(this, gfx_canvas);

    //message_->OnPaint(gfx_canvas);
  }
 // TODO:
 //private:
 public:

  // View:
  void ViewHierarchyChanged(
      const views::ViewHierarchyChangedDetails& details) override {
    views::View::ViewHierarchyChanged(details);
    // We're not using child == this because a Widget may not be
    // available when this is added to the hierarchy.
    if (details.is_add
        && views::View::GetWidget()
        && !example_view_created_) {
      example_view_created_ = true;
      printf("OK example_base_->CreateExampleView\n");
      /// example_base_->CreateExampleView(this);
    } else {
      printf("FAILED example_base_->CreateExampleView\n");
    }
  }

  //const char* GetClassName() const override;

#if 0
  bool OnMousePressed(const ui::MouseEvent& event) override {
    printf("ContainerView OnMousePressed!\n");
    return false;
  }

  gfx::Size CalculatePreferredSize() const override {
    gfx::Size title_size = title_->GetPreferredSize();
    gfx::Size message_size = message_->GetPreferredSize();
    return gfx::Size(title_size.width() + message_size.width() +
                         /*kCompactTitleMessageViewSpacing*/ 12,
                     std::max(title_size.height(), message_size.height()));
  }

  void Layout() override {
    // Elides title and message.
    // * If the message is too long, the message occupies at most
    //   kProgressNotificationMessageRatio of the width.
    // * If the title is too long, the full content of the message is shown,
    //   kCompactTitleMessageViewSpacing is added between them, and the elided
    //   title is shown.
    // * If they are short enough, the title is left-aligned and the message is
    //   right-aligned.
    const int message_width = std::min(
        message_->GetPreferredSize().width(),
        title_->GetPreferredSize().width() > 0
            ? static_cast<int>(/*kProgressNotificationMessageRatio*/ 0.7 * width())
            : width());
    const int title_width =
        std::max(0, width() - message_width - 12 /*kCompactTitleMessageViewSpacing*/);

    title_->SetBounds(0, 0, title_width, height());
    message_->SetBounds(width() - message_width, 0, message_width, height());
    std::cout << "message_->GetBoundsInScreen: " <<
      message_->GetBoundsInScreen().ToString().c_str() << std::endl;
  }
#endif // 0

  void set_title(const base::string16& title) {
    DCHECK(title_);
    title_->SetText(title);
  }
  void set_message(const base::string16& message) {
    DCHECK(message_);
    message_->SetText(message);
  }

  // True if the example view has already been created, or false otherwise.
  bool example_view_created_;

  //ExampleBase* example_base_;

  views::Label* title_ = nullptr;
  views::Label* message_ = nullptr;
  views::Textfield* textfield_ = nullptr;
  views::Textfield* textfield2_ = nullptr;
  views::Checkbox* checkbox_ = nullptr;
  views::ScrollView* scroll_view_;
  views::LabelButton* scroll_to_;
  views::TabbedPane* tabbed_pane_ = nullptr;

  // The content of the scroll view.
  ScrollableView* scrollable_;
  std::unique_ptr<ScrollableView> scrollable_ptr_; // TODO

  // TODO: AddStyleRange

  DISALLOW_COPY_AND_ASSIGN(ContainerView);
};

// TODO:: free memory
static ContainerView* container_ = nullptr;
static views::Widget* widget_ = nullptr;

//static std::unique_ptr<ui::TestContextFactories> context_factories_;
//static std::unique_ptr<ui::Compositor> compositor_;

/*static views::Textfield* textfield_ = nullptr;
static views::Combobox* alignment_ = nullptr;
static views::Combobox* elide_behavior_ = nullptr;
static views::Checkbox* multiline_ = nullptr;
static views::Checkbox* shadows_ = nullptr;
static views::Checkbox* selectable_ = nullptr;
static views::Label* custom_label_ = nullptr;*/
#endif // ENABLE_BLINK_UI_VIEWS

#if defined(ENABLE_BLINK_PLATFORM)
#if defined(ENABLE_IMAGES)
#include "third_party/blink/renderer/platform/graphics/accelerated_static_bitmap_image.h"
#include "third_party/blink/renderer/platform/graphics/static_bitmap_image.h"
//#include "third_party/blink/renderer/core/imagebitmap/image_bitmap.h"
#include "third_party/blink/renderer/platform/image-decoders/image_decoder.h"
//#include "third_party/blink/renderer/platform/image-decoders/image_decoder.h"
static scoped_refptr<blink::StaticBitmapImage> sStaticBitmapImage;
#endif // ENABLE_IMAGES

// TODO: free mem
static gfx::Font* default_font = nullptr;
static gfx::PlatformFont* customPlatformFont = nullptr;

// FontList for the texts except for the header.
static gfx::FontList GetTextFontList() {
  DCHECK(default_font);
  DCHECK(default_font->platform_font());
  DCHECK(default_font->platform_font()->GetFontSize());
  const int size_delta = 0; // size in pixels to add
  gfx::Font font = default_font->Derive(size_delta, gfx::Font::NORMAL,
                                       gfx::Font::Weight::NORMAL);
  return gfx::FontList(font);
}

static std::unique_ptr<blink::Platform> g_platform;
#endif // ENABLE_BLINK_PLATFORM

#include <skia/include/core/SkCanvas.h>
#include <skia/include/core/SkString.h>
#include <skia/include/core/SkFont.h>
#include <skia/include/core/SkGraphics.h>
#include <skia/include/core/SkPictureRecorder.h>
#include <skia/include/core/SkStream.h>
#include <skia/include/core/SkSurface.h>
#include <skia/include/core/SkPixmap.h>
#include <skia/include/core/SkBitmap.h>
#include <skia/include/core/SkCanvas.h>
#include <skia/include/core/SkShader.h>
#if defined(ENABLE_SKOTTIE)
#include <skia/modules/skottie/include/Skottie.h>
#include <skia/modules/skottie/utils/SkottieUtils.h>
#endif
#include <skia/src/core/SkMakeUnique.h>

#include <skia/include/effects/SkGradientShader.h>

#include <skia/src/core/SkOSFile.h>
#include <skia/src/utils/SkOSPath.h>

#include <skia/include/core/SkMaskFilter.h>
#include <skia/include/core/SkTextBlob.h>

#include <skia/include/core/SkTypeface.h>
#include <skia/include/core/SkTextBlob.h>
#include <skia/include/core/SkStream.h>
#include <skia/include/core/SkDocument.h>

//#include <SkCanvas.h>
//#include <SkColorFilter.h>
//#include <SkColorMatrixFilter.h>
//#include <SkColorPriv.h>
//#include <SkData.h>
//#include <SkDocument.h>
//#include <SkGraphics.h>
//#include <SkImage.h>
//#include <SkImageEncoder.h>
//#include <SkMallocPixelRef.h>
//#include <SkMatrix.h>
//#include <SkPaint.h>
//#include <SkPath.h>
//#include <SkStream.h>
//#include <SkSurface.h>
//#include <SkTypeface.h>
//#include <SkFont.h>
//
//#include "SkBitmap.h"
//#include "SkPixelRef.h"
//#include "SkImageEncoder.h"
//#include "SkImageInfo.h"
//#include "SkColor.h"
//#include "SkColorPriv.h"
//#include "SkColorSpace.h"
//#include "SkHalf.h"
//#include "SkMatrix44.h"
//#include "SkUnPreMultiply.h"
//#include "SkStream.h"
//
#if SK_SUPPORT_GPU
#include <skia/include/gpu/GrContext.h>
//
#include <skia/include/gpu/GrTypes.h>
//#include <skia/include/gpu/GrTypesPriv.h>
#include <skia/src/gpu/GrGpu.h>
////#include <skia/src/gpu/GrDirectContext.h>
#include <skia/src/gpu/gl/GrGLGpu.h>
#include <skia/src/gpu/GrContextPriv.h>
#include <skia/src/gpu/GrContextThreadSafeProxyPriv.h>
//#include <skia/src/gpu/gl/GrGLDefines.h>
#include <skia/include/gpu/gl/GrGLAssembleHelpers.h>
#include <skia/include/gpu/gl/GrGLAssembleInterface.h>
#include <skia/src/gpu/gl/GrGLUtil.h>
#include <skia/include/gpu/gl/GrGLAssembleInterface.h>
#include <skia/include/gpu/gl/GrGLInterface.h>
#include <skia/src/gpu/gl/GrGLUtil.h>
#endif // SK_SUPPORT_GPU
//
#include "skia/include/core/SkRefCnt.h"
#include "skia/include/core/SkTime.h"
//
//#include "GrContextPriv.h"
//
//#include "GrAHardwareBufferUtils.h"
//#include "GrBackendSurface.h"
//#include "GrCaps.h"
//#include "GrRecordingContext.h"
//#include "GrRecordingContextPriv.h"
//#include "GrRenderTarget.h"
//#include "GrRenderTargetContextPriv.h"
//#include "GrRenderTargetProxyPriv.h"
//#include "GrTexture.h"
//#include "SkCanvas.h"
//#include "SkDeferredDisplayList.h"
//#include "SkGpuDevice.h"
//#include "SkImagePriv.h"
//#include "SkImage_Base.h"
//#include "SkImage_Gpu.h"
//#include "SkSurfaceCharacterization.h"
//#include "SkSurface_Base.h"
//#include "SkSurface_Gpu.h"
//
//#if SK_SUPPORT_GPU
//#include "gl/GrGLConfig.h"
//#include "gl/GrGLInterface.h"
//#include "gl/GrGLUtil.h"
//#endif // SK_SUPPORT_GPU
//
//
//#include "GrBackendSurface.h"
//#include "GrContext.h"
//#include "GrRenderTargetContext.h"
//#include "SkCanvas.h"
//#include "SkGraphics.h"
//#include "SkRandom.h"
//#include "SkSurface.h"
//#include "SkSurface_Gpu.h"

#ifdef ENABLE_HARFBUZZ
#include <hb-ot.h>
#endif // ENABLE_HARFBUZZ

#include "blink_demo.h"

#if defined(ENABLE_IMAGES)
//#if defined(ENABLE_SKIA) && (defined(USE_LIBJPEG) || defined(USE_LIBJPEG_TURBO))
//static SkString fImagePath = SkString("./resources/images/JPEG_example.jpg");
//#elif defined(ENABLE_SKIA)
//static SkString fImagePath = SkString("./resources/images/image.png");
//#endif

static SkString fImagePath = SkString("./resources/images/image.png");
#endif // ENABLE_IMAGES

#ifdef ENABLE_HARFBUZZ
// see https://github.com/sam8dec/skia/blob/master/tools/using_skia_and_harfbuzz.cpp#L140
// see https://github.com/chromium/chromium/blob/master/third_party/blink/renderer/platform/fonts/shaping/harfbuzz_face.cc#L68
struct HBFDel { void operator()(hb_face_t* f) { hb_face_destroy(f); } };
static ::std::unique_ptr<hb_face_t, HBFDel> fHarfBuzzFace;
struct HBFontDel {
    void operator()(hb_font_t* f) { hb_font_destroy(f); }
};
static ::std::unique_ptr<hb_font_t, HBFontDel> fHarfBuzzFont;
static const int FONT_SIZE_SCALE = 512;
//static SkPaint glyph_paint;
#endif // ENABLE_HARFBUZZ

//static TTF_Font* ttfFont = nullptr;
static sk_sp<SkSurface> sRasterSurface;

class SkPainter;

static SkPainter* myView = nullptr;

static GLuint skia_texture = 0;

// TODO: https://github.com/flutter/engine/blob/master/shell/gpu/gpu_surface_gl.cc
// TODO: https://github.com/vsrinivas/fuchsia/blob/master/garnet/lib/vulkan/tests/vklatency/skia_gpu_painter.cc#L123
#ifdef SKIA_GR_CONTEXT
/// \note In OpenGL mode skia assumes that the correct OpenGL context
/// has been made current to the current thread when Skia calls are made.
/// \see https://skia.org/user/api/skcanvas_creation
// static sk_sp<GrContext> sContext = nullptr;
static GrContext* grContext = nullptr;
// static sk_sp<SkSurface> sSurface = nullptr;
static SkSurface* sSurface = nullptr;
#endif // SKIA_GR_CONTEXT

#if defined(ENABLE_SKOTTIE)
static SkString fAnimPath = SkString("./resources/animations/data.json");
// static SkString fPath = SkString("./resources/fonts/FreeSans.ttf");
static sk_sp<skottie::Animation> fAnimation;
static skottie::Animation::Builder::Stats fAnimationStats;
static SkSize fWinSize = SkSize::Make(
  /*width*/ DRAW_SURFACE_WIDTH, /*height*/ DRAW_SURFACE_HEIGHT); // TODO
static SkMSec fTimeBase = 0;
static bool fShowAnimationInval = false;
//static bool fShowAnimationStats = false;
#endif // ENABLE_SKOTTIE

static inline bool WithinEpsilon(const double a, const double b) {
  return std::abs(a - b) < std::numeric_limits<double>::epsilon();
}

#if defined(ENABLE_BLINK_UI)
static std::unique_ptr<display::ScreenBase> screen_;
#endif // ENABLE_BLINK_UI

void SkiaUiDemo::SetCursorScreenPoint(int x, int y) {
#if defined(ENABLE_BLINK_UI)
  DCHECK(screen_);
  screen_->SetCursorScreenPoint(gfx::Point(x, y));
#endif // ENABLE_BLINK_UI
}

#if defined(ENABLE_BLINK_UI)
void SkiaUiDemo::initBlinkPlatform() {
  g_platform = std::make_unique<blink::Platform>();
  ///mojo::core::Init();
  blink::Platform::CreateMainThreadAndInitialize(g_platform.get());

  // TODO >>>
  //ui::DeviceDataManager::CreateInstance();

  screen_ = std::make_unique<display::ScreenBase>();
  display::Screen::SetScreenInstance(screen_.get());
  DCHECK(screen_);

  // TODO >>>
  //  display::Screen::SetScreenInstance(nullptr);
  //  ui::DeviceDataManager::DeleteInstance();
}
#endif // ENABLE_BLINK_UI

#if defined(ENABLE_SKIA) && defined(ENABLE_CUSTOM_FONTS)
//static SkFont* skFont1 = nullptr;
//static SkFont* skFont2 = nullptr;
static const float FONT_SIZE_F = 22.0f;
//static sk_sp<SkTypeface> sktp;
static sk_sp<SkTypeface> sktpForUI;
static bool sktpForUICreated = false;

//static const char* fontPath = "./resources/fonts/Ubuntu-Regular.ttf";
static const char* fontPath = "./resources/fonts/FreeSans.ttf";
//static const char* fontPath = "./resources/fonts/arialuni.ttf";

#if defined(ENABLE_CUSTOM_FONTS) && defined(ENABLE_HARFBUZZ)
/// \note see SkShaper_harfbuzz.cpp if you want shaping in rectangle
/// \see https://github.com/skui-org/skia/blob/f577133e703ea6a81602426aea879857cfd0b2e1/experimental/canvaskit/canvaskit_bindings.cpp#L477
/// \see https://github.com/skui-org/skia/blob/f577133e703ea6a81602426aea879857cfd0b2e1/modules/skshaper/src/SkShaper_harfbuzz.cpp#L888
static bool DrawGlyphs(double current_x, double current_y,
                       SkPaint& glyph_paint, const SkFont& sfont,
                       SkCanvas* canvas, hb_buffer_t *hb_buffer) {
    SkTextBlobBuilder textBlobBuilder;
    unsigned len = hb_buffer_get_length (hb_buffer);
    if (len == 0) {
        printf("empty hb_buffer_get_length \n");
        return true;
    }
    hb_glyph_info_t *info = hb_buffer_get_glyph_infos (hb_buffer, nullptr);
    hb_glyph_position_t *pos = hb_buffer_get_glyph_positions (hb_buffer, nullptr);
    const SkTextBlobBuilder::RunBuffer& runBuffer =
      textBlobBuilder.allocRunPos(sfont, static_cast<int>(len));

    double x = 0;
    double y = 0;
    DCHECK(len < 99999);
    for (unsigned int i = 0; i < len; i++)
    {
        runBuffer.glyphs[i] =
          static_cast<SkGlyphID>(info[i].codepoint);
        reinterpret_cast<SkPoint*>(runBuffer.pos)[i] =
          SkPoint::Make(
            static_cast<SkScalar>(x + pos[i].x_offset / FONT_SIZE_SCALE),
            static_cast<SkScalar>(y - pos[i].y_offset / FONT_SIZE_SCALE));
        x += pos[i].x_advance / FONT_SIZE_SCALE;
        y += pos[i].y_advance / FONT_SIZE_SCALE;
    }
//#if 0
    canvas->drawTextBlob(textBlobBuilder.make(),
      static_cast<SkScalar>(current_x),
      static_cast<SkScalar>(current_y),
      glyph_paint);
//#endif // 0
    return true;
} // end of DrawGlyphs

/*static SkScalar shape(SkTextBlobBuilder* builder,
                         const SkPaint& srcPaint,
                         const char* utf8text,
                         size_t textBytes,
                         SkPoint point) const {
    SkPaint paint(srcPaint);
    //paint.setTextEncoding(SkPaint::kGlyphID_TextEncoding);
    //paint.setTypeface(fTypeface);
    SkASSERT(builder);
    hb_buffer_t* buffer = fBuffer.get();
    hb_buffer_add_utf8(buffer, utf8text, -1, 0, -1);
    hb_buffer_guess_segment_properties(buffer);
    hb_shape(fHarfBuzzFont.get(), buffer, nullptr, 0);
    unsigned len = hb_buffer_get_length(buffer);
    if (len == 0) {
        hb_buffer_clear_contents(buffer);
        return 0;
    }
    hb_glyph_info_t* info = hb_buffer_get_glyph_infos(buffer, nullptr);
    hb_glyph_position_t* pos =
        hb_buffer_get_glyph_positions(buffer, nullptr);
    auto runBuffer = builder->allocRunTextPos(
        paint, SkToInt(len), SkToInt(textBytes), SkString());
    memcpy(runBuffer.utf8text, utf8text, textBytes);
    double x = point.x();
    double y = point.y();
    double textSizeY = paint.getTextSize() / (double)FONT_SIZE_SCALE;
    double textSizeX = textSizeY * paint.getTextScaleX();
    for (unsigned i = 0; i < len; i++) {
        runBuffer.glyphs[i] = info[i].codepoint;
        runBuffer.clusters[i] = info[i].cluster;
        reinterpret_cast<SkPoint*>(runBuffer.pos)[i] =
            SkPoint::Make(SkDoubleToScalar(x + pos[i].x_offset * textSizeX),
                          SkDoubleToScalar(y - pos[i].y_offset * textSizeY));
        x += pos[i].x_advance * textSizeX;
        y += pos[i].y_advance * textSizeY;
    }
    hb_buffer_clear_contents(buffer);
    return (SkScalar)x;
}*/
#endif // ENABLE_CUSTOM_FONTS && ENABLE_HARFBUZZ

void SkiaUiDemo::handleTestEvent(const char* text) {
  printf("handleTestEvent for %s\n", text);
#if defined(ENABLE_BLINK_UI_VIEWS)
  if(std::string(text) == "A") {
      DCHECK(container_->textfield2_->IsFocusable());
      DCHECK(container_->textfield2_->GetEnabled());
      DCHECK(container_->textfield2_->IsDrawn());
      container_->textfield2_->SetCursorEnabled(true);
      container_->textfield2_->RequestFocus();
      container_->textfield2_->SetReadOnly(false);

      testScrollY+=10;
      DCHECK(container_->scroll_view_);
      DCHECK(container_->scroll_view_->contents());
      /*DCHECK(!container_->scroll_view_->contents()->
        GetVisibleBounds().IsEmpty());*/

      auto prev = container_->scroll_view_->CurrentOffset();
      std::cout << "ScrollOffset" << prev.x() << prev.y() << std::endl;

      /*container_->scroll_view_->contents()->ScrollRectToVisible(
        gfx::Rect(20, testScrollY, 20, 20));*/
      container_->scroll_view_->
        ScrollToOffset(gfx::ScrollOffset(0, testScrollY));
      container_->scroll_view_->InvalidateLayout();
      container_->scroll_view_->Layout();
      container_->scroll_view_->SizeToPreferredSize();
      container_->scroll_view_->SchedulePaint();

      DCHECK(container_->tabbed_pane_->GetTabCount() > 0);
      container_->tabbed_pane_->SelectTabAt(0);
  }

  if(std::string(text) == "B") {
      DCHECK(container_->textfield_->IsFocusable());
      DCHECK(container_->textfield_->GetEnabled());
      DCHECK(container_->textfield_->IsDrawn());
      container_->textfield_->SetCursorEnabled(true);
      container_->textfield_->RequestFocus();
      container_->textfield_->SetReadOnly(false);

      testScrollY-=10;
      DCHECK(container_->scroll_view_);
      DCHECK(container_->scroll_view_->contents());
      /*DCHECK(!container_->scroll_view_->contents()->
        GetVisibleBounds().IsEmpty());*/

      /*container_->scroll_view_->contents()->ScrollRectToVisible(
        gfx::Rect(20, testScrollY, 20, 20));*/

      auto prev = container_->scroll_view_->CurrentOffset();
      std::cout << "ScrollOffset" << prev.x() << prev.y() << std::endl;

      container_->scroll_view_->
        ScrollToOffset(gfx::ScrollOffset(0, testScrollY));
      container_->scroll_view_->InvalidateLayout();
      container_->scroll_view_->Layout();
      container_->scroll_view_->SizeToPreferredSize();
      container_->scroll_view_->SchedulePaint();

      DCHECK(container_->tabbed_pane_->GetTabCount() > 1);
      container_->tabbed_pane_->SelectTabAt(1);
  }

  if(std::string(text) == "C") {
      DCHECK(widget_);
      views::View* root_view = widget_->GetRootView();
      DCHECK(root_view->Contains(container_));

      const base::string16 inputText = base::UTF8ToUTF16("ф");
      int flags = ui::EF_NONE;

      for (base::string16::const_iterator i = inputText.begin();
         i != inputText.end(); ++i) {
        printf("sending OnKeyEvent(&press_event)\n");

        ui::KeyEvent press_event(ui::ET_KEY_PRESSED,
         ui::VKEY_PROCESSKEY, flags);
        DCHECK(!press_event.stopped_propagation());
        press_event.set_character(*i);
        press_event.set_source_device_id(0);
        DCHECK(!press_event.stopped_propagation());

        ui::InputMethod* im = widget_->GetInputMethod();
        im->DispatchKeyEvent(&press_event);
      }

      for (base::string16::const_iterator i = inputText.begin();
         i != inputText.end(); ++i) {
        printf("sending OnKeyEvent(&release_event)\n");
        ui::KeyEvent release_event(ui::ET_KEY_RELEASED,
          ui::VKEY_UNKNOWN, flags);
        DCHECK(!release_event.stopped_propagation());
        release_event.set_character(*i);
        release_event.set_source_device_id(0);

        DCHECK(!release_event.stopped_propagation());

        ui::InputMethod* im = widget_->GetInputMethod();
        im->DispatchKeyEvent(&release_event);
      }
  }

  if(std::string(text) == "D") {
    if(container_) {
      DCHECK(widget_);
      views::View* root_view = widget_->GetRootView();

      //views::internal::RootView* to_root_view =
      //    static_cast<views::internal::RootView*>(widget_->GetRootView());

      DCHECK(root_view);
      if(!root_view->Contains(container_)) {
        root_view->AddChildView(container_);
      }
      DCHECK(root_view->GetEffectiveViewTargeter());

      printf("handleTestEvent inject %s\n", text);
      container_->SetFocusBehavior(views::View::FocusBehavior::ALWAYS);
      DCHECK(container_->IsFocusable());
/*
      DCHECK(container_->GetWidget());
      DCHECK(container_->GetWidget()->GetTopLevelWidget() == container_->GetWidget());
      DCHECK(container_->GetWidget()->IsVisible());
      auto* focus_manager = container_->GetFocusManager();
      DCHECK(focus_manager);
      focus_manager->set_shortcut_handling_suspended(false);
      focus_manager->set_arrow_key_traversal_enabled_for_widget(false);
      container_->RequestFocus(); // requires FocusManager
      focus_manager->SetFocusedView(container_);
      DCHECK(focus_manager->ContainsView(container_));
      DCHECK(focus_manager->GetFocusedView() == container_);
      focus_manager->SetFocusedView(container_->textfield_);
      DCHECK(focus_manager->ContainsView(container_->textfield_));
      DCHECK(focus_manager->GetFocusedView() == container_->textfield_);
*/
      DCHECK(container_->textfield_->GetFocusManager());
      DCHECK(container_->GetFocusManager());
      container_->textfield_->SetFocusBehavior(views::View::FocusBehavior::ALWAYS);
      DCHECK(container_->textfield_->IsFocusable());
      DCHECK(container_->textfield_->GetEnabled());
      DCHECK(container_->textfield_->IsDrawn());
      container_->textfield_->SetCursorEnabled(true);
      container_->textfield_->RequestFocus();
      container_->textfield_->SetReadOnly(false);

      container_->textfield_->ChangeTextDirectionAndLayoutAlignment(base::i18n::RIGHT_TO_LEFT);

      container_->textfield_->SetEnabled(true);

      DCHECK(root_view->GetInputMethod());
      DCHECK(container_->GetInputMethod());
      container_->textfield_->SetTextInputType(ui::TEXT_INPUT_TYPE_TEXT);
      DCHECK(container_->textfield_->GetInputMethod()->GetTextInputType()
              == ui::TEXT_INPUT_TYPE_TEXT);
      DCHECK(container_->textfield_->GetInputMethod());

      DCHECK(container_->textfield_->HasFocus());
      //container_->textfield_->OnFocus();
      DCHECK(container_->textfield_->ShouldShowCursor());
      DCHECK(container_->textfield_->ShouldBlinkCursor());

      int flags = ui::EF_NONE;

      // Do not rewrite injected events. See crbug.com/136465.
      //flags |= ui::EF_FINAL;

      //pressed ? VKEY_PROCESSKEY : VKEY_UNKNOWN, EF_IME_FABRICATED_KEY

      //int flags = ui::EF_IME_FABRICATED_KEY;

      const base::string16 inputText = base::UTF8ToUTF16("y");

      //#include "ui/views/view_targeter.h"
      //views::ViewTargeter* view_targeter = new views::ViewTargeter(root_view);
      //container_->SetEventTargeter(base::WrapUnique(view_targeter));

      //ui::KeyEvent key_event(ui::ET_KEY_PRESSED, ui::VKEY_ESCAPE, ui::EF_NONE);

      for (base::string16::const_iterator i = inputText.begin();
         i != inputText.end(); ++i) {
        ui::KeyEvent press_event(ui::ET_KEY_PRESSED,
         /*ui::VKEY_UNKNOWN*/ui::VKEY_PROCESSKEY, flags);
        DCHECK(!press_event.stopped_propagation());
        press_event.set_character(*i);
        press_event.set_source_device_id(0);
        //press_event.set_is_char(true);
        ///focus_manager->OnKeyEvent(press_event);
        //container_->OnEvent(&press_event);
        //container_->textfield_->/*OnKeyEvent*/OnEvent(&press_event);

        //ui::EventDispatchDetails details =
        // to_root_view->OnEventFromSource(&press_event);

        //widget_->SendEventToSink(&press_event);
        DCHECK(!press_event.stopped_propagation());
        //widget_->OnKeyEvent(&press_event);

        printf("sending OnKeyEvent(&press_event)\n");

        //ui::KeyEvent event(ch, ui::VKEY_UNKNOWN, ui::DomCode::NONE, flags);
        ui::InputMethod* im = widget_->GetInputMethod();
        im->DispatchKeyEvent(&press_event);

        ui::TextInputClient* client = im->GetTextInputClient();
        DCHECK(client);
        //client->InsertText(inputText);
      }

      for (base::string16::const_iterator i = inputText.begin();
         i != inputText.end(); ++i) {
        ui::KeyEvent release_event(ui::ET_KEY_RELEASED,
        /*ui::VKEY_UNKNOWN*/ui::VKEY_UNKNOWN, flags);
        DCHECK(!release_event.stopped_propagation());
        release_event.set_character(*i);
        release_event.set_source_device_id(0);
        //release_event.set_is_char(true);
        //set_async_callback
        ///focus_manager->OnKeyEvent(release_event);
        //container_->OnEvent(&release_event);
        //container_->textfield_->OnEvent(&release_event);

        //ui::EventDispatchDetails details =
        // to_root_view->OnEventFromSource(&release_event);

        DCHECK(!release_event.stopped_propagation());
        //widget_->OnKeyEvent(&release_event);

        printf("sending OnKeyEvent(&release_event)\n");

        //ui::KeyEvent event(ch, ui::VKEY_UNKNOWN, ui::DomCode::NONE, flags);
        ui::InputMethod* im = widget_->GetInputMethod();
        im->DispatchKeyEvent(&release_event);

        ui::TextInputClient* client = im->GetTextInputClient();
        DCHECK(client);
        //client->InsertText(inputText);
      }

      //container_->textfield_->AppendText(base::UTF8ToUTF16("з"));
      //container_->textfield_->UpdateAfterChange(true, true);

      // TODO: OnMouseEvent OnScrollEvent
    }
  }
#endif // ENABLE_BLINK_UI_VIEWS
}

void SkiaUiDemo::prepareUIFonts() {
#if defined(OS_EMSCRIPTEN) && !defined(DISABLE_PTHREADS) && defined(ENABLE_COBALT)
  DCHECK(false);// << "can`t prepareUIFonts on WASM MT";
#endif

  printf("Creating ui fonts...\n");

  // SkFont font;//(nullptr, 24);//SkFont::kA8_MaskType, flags);
  if(!sktpForUI) {
    printf("creating sktpForUI\n");

    DCHECK(!sktpForUICreated);
    sktpForUICreated = true;

    sk_sp<SkData> data = SkData::MakeFromFileName(fontPath);
    if (!data) {
      printf("failed SkData::MakeFromMalloc for font %s\n", fontPath);
      NOTREACHED();
    }
    const int index = 0;
    sktpForUI = SkTypeface::MakeFromData(data, index);

    #ifdef ENABLE_HARFBUZZ
      printf("Creating harfbuzz fonts...\n");
      auto destroy = [](void *d) { static_cast<SkData*>(d)->unref(); };
      const char* bytes = static_cast<const char*>(data->data());
      unsigned int size = static_cast<unsigned int>(data->size());
      hb_blob_t* blob = hb_blob_create(bytes,
                                       size,
                                       HB_MEMORY_MODE_READONLY,
                                       data.release(),
                                       destroy);
      assert(blob);
      hb_blob_make_immutable(blob);
      // TODO: free mem
      hb_face_t* face = hb_face_create(blob, static_cast<unsigned>(index));
      hb_blob_destroy(blob);
      //assert(face);
      DCHECK(sktpForUI);
      if (!face) {
        printf("Can`t create harfbuzz face...\n");
        sktpForUI.reset();
        NOTREACHED();
      }
      hb_face_set_index(face, static_cast<unsigned>(index));
      hb_face_set_upem(face, static_cast<unsigned>(sktpForUI->getUnitsPerEm()));
      fHarfBuzzFace.reset(face);

      // see https://chromium.googlesource.com/skia/+/chrome/m56/tools/SkShaper_harfbuzz.cpp
      fHarfBuzzFont.reset(hb_font_create(fHarfBuzzFace.get()));
      if (!fHarfBuzzFont.get()) {
        printf("Can`t create harfbuzz font...\n");
        sktpForUI.reset();
        NOTREACHED();
      }

      // see https://github.com/aam/skiaex/blob/master/app/main.cpp#L177
      // see https://github.com/harfbuzz/harfbuzz-tutorial/blob/master/hello-harfbuzz-opentype.c#L31
      DCHECK(fHarfBuzzFont);
      hb_font_set_scale(fHarfBuzzFont.get(),
        static_cast<int>(FONT_SIZE_SCALE * FONT_SIZE_F),
        static_cast<int>(FONT_SIZE_SCALE * FONT_SIZE_F));
      hb_ot_font_set_funcs(fHarfBuzzFont.get());
    #endif // ENABLE_HARFBUZZ
  }
}
#endif // ENABLE_SKIA && ENABLE_CUSTOM_FONTS

class SkPainter {
public:
  SkPoint m_pos = SkPoint::Make(100, 100);
  SkPoint m_prev = SkPoint::Make(110, 110);
  SkColor m_color = SK_ColorDKGRAY;
  SkScalar m_size = 200;

  void onDraw(SkCanvas* sk_canvas) {
    DCHECK(sk_canvas);

    if (isDebugPeriodReached()) printf("onDraw() 1\n");

    if(!sk_canvas) return;

    // TODO: needs SKIA_GR_CONTEXT
    /*if (!canvas->getGrContext()) {
      return;
    }*/

    SkPaint paint;

    // paint.setAlpha(255);
#ifdef ENABLE_SKIA_HQ
    paint.setAntiAlias(true);
    paint.setFilterQuality( SkFilterQuality::kMedium_SkFilterQuality );
#else // ENABLE_SKIA_HQ
    paint.setAntiAlias(false);
    paint.setFilterQuality( SkFilterQuality::kNone_SkFilterQuality );
#endif // ENABLE_SKIA_HQ
    paint.setColor(SK_ColorRED);
    /// paint.setColor(0xffeeeeee);

    //printf("onDraw() 2\n");

#if 0
    paint.setColor(SK_ColorGREEN);
    sk_canvas->drawRect({ 1000, 1700, 50, 50 }, paint);
#endif // 0

    /*paint.setColor(SK_ColorBLUE);
    sk_canvas->drawRect({ 500, 700, 300, 500 }, paint);

    paint.setColor(SK_ColorWHITE);
    sk_canvas->drawCircle(m_pos.x(), m_pos.y(), m_size, paint);
      //printf("onDraw() 2.1\n");*/

#ifdef ENABLE_SK_EFFECTS
    {
      SkColor colors[4] = {SK_ColorCYAN, SK_ColorMAGENTA, SK_ColorYELLOW, SK_ColorCYAN};

      //printf("onDraw() 2.2\n");
      sk_sp<SkShader> shdr = SkGradientShader::MakeSweep(128.0f, 128.0f, colors, nullptr, 4, 0, nullptr);

      //printf("onDraw() 2.3\n");
      paint.setShader(shdr);
      //printf("onDraw() 2.4\n");
    }
#endif // ENABLE_SK_EFFECTS

    //printf("onDraw() 3\n");

    // canvas->drawLine(m_pos.x(), m_pos.y(), m_prev.x(), m_prev.y(), paint);

    paint.setColor(SK_ColorBLACK);
    paint.setStyle(SkPaint::kFill_Style);
      //printf("onDraw() 4\n");

    // TODO >>>
    //return;

#ifdef ENABLE_CUSTOM_FONTS
    CHECK(sktpForUICreated);// << "sktpForUICreated required";
    CHECK(sktpForUI);// << "sktpForUI required";
    //prepareUIFonts(); // see DCHECK(sktpForUI);

    SkFont skFont1ForUI(sktpForUI, FONT_SIZE_F, 1.0f, 0.0f);
    skFont1ForUI.setEdging(SkFont::Edging::kAntiAlias);
    DCHECK(sktpForUI);
    DCHECK(skFont1ForUI.getTypeface());
    SkFont skFont2ForUI(sktpForUI, 30.0f, 1.5f, 0.0f);
    skFont2ForUI.setEdging(SkFont::Edging::kAntiAlias);
    DCHECK(skFont2ForUI.getTypeface());

    // TODO >>>
    //return;
    //printf("onDraw z1\n");

//#if 0
    sk_canvas->drawString("1!1 Skia Test 1 Skia Test 1 "
                          "Skia Test 1!", 60, 32, skFont1ForUI, paint);

    //printf("onDraw z2\n");
    sk_canvas->drawString("2!2 Skia Test 2 Skia Test 2 "
                          "Skia Test 2!", 20, 97, skFont2ForUI, paint);
    //printf("onDraw z3\n");
//#endif // 0

    // TODO >>> !!! <<<
    //return;
    //printf("onDraw y1\n");

// see https://github.com/google/skia/blob/master/modules/skshaper/src/SkShaper_harfbuzz.cpp#L1221
#ifdef ENABLE_HARFBUZZ
        paint.setColor(SK_ColorBLACK);
        paint.setStyle(SkPaint::kFill_Style);

        SkPaint glyph_paint(paint);

        //glyph_paint.setFlags(
        //    SkPaint::kAntiAlias_Flag |
        //    SkPaint::kSubpixelText_Flag);  // ... avoid waggly text when rotating.
        glyph_paint.setColor(SK_ColorBLACK);
        /*paint.setTextEncoding(SkPaint::kGlyphID_TextEncoding);
        paint.setTypeface(sk_ref_sp(font->currentTypeface()));
        glyph_paint.setTextSize(config.font_size->value);
        glyph_paint.setTypeface(face->fSkiaTypeface);
        glyph_paint.setTextEncoding(SkPaint::kGlyphID_TextEncoding);*/

        double current_x = 400.0;
        double current_y = 400.0;
        double line_spacing_ratio = 1.5;
        double font_size = static_cast<double>(FONT_SIZE_F);
        DCHECK(font_size > 0);

        auto WriteLine = [&skFont1ForUI,
          &current_x, &current_y, &line_spacing_ratio,
          &font_size, &glyph_paint, &sk_canvas](const char* text) {
            /* Create hb-buffer and populate. */
            hb_buffer_t* hb_buffer = hb_buffer_create();

            hb_buffer_set_direction(hb_buffer, HB_DIRECTION_LTR);
#ifdef HARFBUZZ_UNICODE
            hb_buffer_set_script(hb_buffer, HB_SCRIPT_CYRILLIC);
            //hb_buffer_set_language(hb_buffer, hb_language_from_string("ru", 2));
            hb_buffer_set_language(hb_buffer,
              hb_language_from_string("en", 2));
#else // HARFBUZZ_UNICODE
            hb_buffer_set_script(hb_buffer, HB_SCRIPT_LATIN);
            hb_buffer_set_language(hb_buffer, hb_language_from_string("en", 2));
#endif // HARFBUZZ_UNICODE
            //hb_buffer_add_latin1(hb_buffer, text, -1, 0, -1);
            hb_buffer_add_utf8 (hb_buffer, text, -1, 0, -1);

            //hb_buffer_add_utf8 (hb_buffer, text, strlen(text), 0, -1);
            hb_buffer_guess_segment_properties (hb_buffer);
            /* Shape it! */
            DCHECK(fHarfBuzzFont);
            hb_shape (fHarfBuzzFont.get(), hb_buffer, nullptr, 0);
            unsigned len = hb_buffer_get_length(hb_buffer);
            if (len == 0) {
                printf("empty hb_buffer_get_length\n");
                return;
            }
            DrawGlyphs(current_x, current_y, glyph_paint,
                       skFont1ForUI, sk_canvas, hb_buffer);

            hb_buffer_destroy(hb_buffer);

            // Advance to the next line.
            current_y += line_spacing_ratio * font_size;
            /*if (current_y > config.page_height->value) {
            pdfDocument->endPage();
            NewPage();
        }*/
        };

#ifdef HARFBUZZ_UNICODE
        const char *textLine1 = "harfbuzz_skia_ex1 РУССКИЙ ТЕКСТ";
        const char *textLine2 = "2 РУССКИЙ ТЕКСТ";
        const char *textLine3 = "2 РУССКИЙ ТЕКСТ";
#else // HARFBUZZ_UNICODE
        const char *textLine1 = "harfbuzz_skia_ex1 ENGLISH TEXT";
        const char *textLine2 = "2 ENGLISH TEXT";
        const char *textLine3 = "3 ENGLISH TEXT";
#endif // HARFBUZZ_UNICODE

        WriteLine(textLine1);
        WriteLine(textLine2);
        WriteLine(textLine3);

        // test again without hb
        DCHECK(skFont2ForUI.getTypeface());
        auto blob3 = SkTextBlob::MakeFromString("blob3blob3blob3", skFont2ForUI);
//#if 0
        sk_canvas->drawTextBlob(blob3.get(), 500, 500, glyph_paint);
//#endif // 0

        //printf("onDraw y2\n");
#endif // ENABLE_HARFBUZZ


    // TODO >>>
    //return;

#ifdef ENABLE_SK_EFFECTS
    // see https://skia.org/user/api/skpaint_overview
    {
      const SkScalar sigma = 1.65f;
      const SkScalar xDrop = 2.0f;
      const SkScalar yDrop = 2.0f;
      const SkScalar x = 8.0f;
      const SkScalar y = 52.0f;
      //const SkScalar textSize = 48.0f;
      const uint8_t blurAlpha = 127;
      DCHECK(skFont1ForUI.getTypeface());
      auto blob1 = SkTextBlob::MakeFromString("?123Skia! skFont1UIThread",
         skFont1ForUI);
      DCHECK(skFont2ForUI.getTypeface());
      auto blob2 = SkTextBlob::MakeFromString("dfsdf !!! skFont2UIThread",
         skFont2ForUI);
      SkPaint blur(paint);
      blur.setAlpha(blurAlpha);
      blur.setMaskFilter(SkMaskFilter::MakeBlur(kNormal_SkBlurStyle, sigma, 0));
      // canvas->drawColor(SK_ColorWHITE);
#if 0
      sk_canvas->drawTextBlob(blob1.get(), x + xDrop, y + yDrop, blur);
      sk_canvas->drawTextBlob(blob1.get(), x, y, paint);

      sk_canvas->drawTextBlob(blob2.get(), x + xDrop, 50 + y + yDrop, blur);

      SkPaint strokePaint(paint);
      strokePaint.setStyle(SkPaint::kStroke_Style);
      strokePaint.setStrokeWidth(3.0f);
      sk_canvas->drawTextBlob(blob2.get(), x, 80 + y, strokePaint);
#endif // 0
    }
#endif // ENABLE_SK_EFFECTS
#endif // ENABLE_CUSTOM_FONTS

      //printf("onDraw() 5\n");

    // TODO >>>
    //return;

#ifdef ENABLE_SKOTTIE
      //printf("onDraw() 6\n");

    if (fAnimation) {
      //printf("SkAutoCanvasRestore...\n");
      SkAutoCanvasRestore acr(sk_canvas, true);
#ifdef ENABLE_SKIA_HQ
      paint.setAntiAlias(true);
      paint.setFilterQuality( SkFilterQuality::kMedium_SkFilterQuality );
#else // ENABLE_SKIA_HQ
      paint.setAntiAlias(false);
      paint.setFilterQuality( SkFilterQuality::kNone_SkFilterQuality );
#endif // ENABLE_SKIA_HQ
      //printf("SkRect::MakeSize...\n");
      const auto dstR = SkRect::MakeSize(fWinSize);
      //printf("fAnimation->render...\n");
      // TODO: SkSGRenderNode.cpp:19: fatal error: "assert(!this->hasInval())"

#if 0
      fAnimation->render(sk_canvas, &dstR);
#endif // 0

      //printf("fAnimation->rendered\n");
      /*if (fShowAnimationStats) {
          draw_stats_box(canvas, fAnimationStats);
      }*/
    }
#endif // ENABLE_SKOTTIE

    //std::cout << std::endl; // flush

    //printf("ENABLE_BLINK_UI 1\n");
#ifdef ENABLE_BLINK_UI
    cc::SkiaPaintCanvas cc_skia_paint_canvas(sk_canvas);
#endif // ENABLE_BLINK_UI

    //printf("onDraw() x1\n");

    // TODO >>>
    //return;

#ifdef ENABLE_BLINK_PLATFORM
  // see https://chromium.googlesource.com/chromium/src/+/master/third_party/blink/renderer/platform/graphics/paint/README.md
  // see https://blog.csdn.net/tornmy/article/details/82593718
  // see https://github.com/chromium/chromium/blob/master/third_party/blink/renderer/platform/graphics/graphics_context_test.cc
  auto paint_controller = ::std::make_unique<blink::PaintController>();
  blink::GraphicsContext context(*paint_controller);
  //
  blink::PaintFlags flags;
  //
  //
#ifdef ENABLE_SKIA_HQ
    flags.setAntiAlias(true);
    flags.setFilterQuality(SkFilterQuality::kMedium_SkFilterQuality);
    context.SetShouldAntialias(true);
    context.SetImageInterpolationQuality(blink::InterpolationQuality::kInterpolationMedium);
#else // ENABLE_SKIA_HQ
    flags.setAntiAlias(false);
    flags.setFilterQuality(SkFilterQuality::kNone_SkFilterQuality);
    context.SetShouldAntialias(false);
    // SetImageInterpolationQuality calls setFilterQuality
    context.SetImageInterpolationQuality(blink::InterpolationQuality::kInterpolationNone);
#endif // ENABLE_SKIA_HQ
  //context.SetMiterLimit(1);
  context.SetStrokeThickness(5);
  context.SetLineCap(blink::kSquareCap);
  context.SetStrokeStyle(blink::kSolidStroke);
  //
  context.BeginRecording(blink::FloatRect(blink::LayoutRect::InfiniteIntRect()));
  //
#if 0
  //printf("FillRect 1\n");
  context.FillRect(blink::FloatRect(0, 0, 50, 50),
    blink::Color(1.0f, 1.0f, 0.0f, 0.5f),
    SkBlendMode::kSrcOver);
  //printf("FillRect 2\n");
  context.FillRect(blink::FloatRect(40, 40, 150, 550),
    blink::Color(0.0f, 1.0f, 1.0f, 0.5f),
    SkBlendMode::kSrcOver);
  //printf("FillRect 3\n");
  context.DrawOval(blink::FloatRect(40, 40, 150, 550),
    flags);
  //
  //printf("FillRect 4\n");
  // Clip to the left edge of the opaque area.
  ////context.Clip(blink::IntRect(100, 80, 500, 400));
  //
  // Draw a path that gets clipped. This should destroy the opaque area, but
  // only inside the clip.
  //printf("FillRect 5\n");
  blink::Path path;
  path.MoveTo(blink::FloatPoint(10, 10));
  path.AddLineTo(blink::FloatPoint(40, 40));
  flags.setColor(blink::Color(0.0f, 1.0f, 0.0f, 0.5f).Rgb());
  flags.setBlendMode(SkBlendMode::kSrcOver);
  context.DrawPath(path.GetSkPath(), flags);

  //printf("FillRect 6\n");
  // Make skia unable to compute fast bounds for our paths.
  DashArray dash_array;
  dash_array.push_back(1);
  dash_array.push_back(0);
  context.SetLineDash(dash_array, 0);

  //printf("FillRect 7\n");
  // Make the device opaque in 10,10 40x40.
  context.FillRoundedRect(blink::FloatRoundedRect(400, 400, 600, 600),
    blink::Color(0.0f, 1.0f, 0.5f, 0.5f));


  //printf("FillRect 8\n");
  // The inset shadow case.
  blink::GraphicsContext::Edges clipped_edges = blink::GraphicsContext::kNoEdge;
  context.DrawInnerShadow(blink::FloatRoundedRect(510, 510, 640, 640),
    blink::Color(0.0f, 1.0f, 0.5f, 0.5f),
    blink::FloatSize(10, 10),
    5.0f,
    5.0f,
    clipped_edges);
  //printf("FillRect 9\n");

  //printf("onDraw() x2\n");
#endif // 0

#if defined(ENABLE_BLINK_UI)
#if defined(ENABLE_IMAGES)
  if (!sStaticBitmapImage || sStaticBitmapImage->IsNull() || !sStaticBitmapImage->IsValid()) {
    printf("Invalid StaticBitmapImage\n");
  }

  //blink::Image* img = blink::ImageBitmap::cr;
  //  CreatePaintImageBuilder()
  //        .set_image(::std::move(image), cc::PaintImage::GetNextContentId())
  //blink::ImageBitmap image_bitmap_no_crop();
  //
  //blink::Image img = gfx::ImageSkia(imageSkia->GetRepresentation(10.0f));

  //printf("FillRect 70\n");
  DCHECK(sStaticBitmapImage);
//#if 0
  context.DrawImageTiled(sStaticBitmapImage.get(),
    blink::FloatRect(0, 0, 400, 400),
    blink::FloatRect(0, 0, 1000, 1000),
    blink::FloatSize(1, 1),
    blink::FloatPoint{1.0, 1.0},
    blink::FloatSize(0.0, 0.0),
    SkBlendMode::kSrcOver);
//#endif // 0
#endif // ENABLE_IMAGES

#ifdef ENABLE_BLINK_UI_NATIVE_THEME
  ui::NativeThemeAura* nativeThemeAura =
  // Up arrow, sized for 1x.
#ifdef ENABLE_COBALT
     base::polymorphic_downcast<ui::NativeThemeAura*>(
#else
     static_cast<ui::NativeThemeAura*>(
#endif // ENABLE_COBALT
        ui::NativeTheme::GetInstanceForNativeUi());
  DCHECK(nativeThemeAura);

#if 0
  SkPath theme_path;
  theme_path =
      //ui::NativeThemeAura::instance()->
      nativeThemeAura->PathForArrow(gfx::Rect(100, 200, 170, 170),
      ui::NativeTheme::kScrollbarUpArrow);
  DCHECK(!theme_path.isEmpty());
  context.DrawPath(theme_path, flags);
#endif // 0

//#if 0

  //cc::PaintCanvas cc_paint_canvas(&cc_skia_paint_canvas);
  ui::NativeTheme::ButtonExtraParams button;
  button.checked = true;
  button.has_border = true;
  button.background_color = SkColorSetARGB(0, 0, 188, 112);
  nativeThemeAura->PaintCheckbox(&cc_skia_paint_canvas,
    ui::NativeTheme::State::kDisabled,
    gfx::Rect(200, 200, 300, 300),
    button);
//#endif // 0

#endif // ENABLE_BLINK_UI_NATIVE_THEME

  //printf("FillRect 71\n");

  /// \note I`ll get "Check failed: canvas_" (see graphics_context.cc)
  /// if you will try to use same context again without BeginRecording
  auto rr = context.EndRecording();
  //printf("FillRect 81\n");
  cc_skia_paint_canvas.drawPicture(rr);

#endif // ENABLE_UI

  //printf("onDraw() x3\n");

  // Must be called when a painting is finished. Updates the current paint
  // artifact with the new paintings.
  paint_controller->CommitNewDisplayItems();

  //printf("FillRect 91\n");
#endif // ENABLE_BLINK_PLATFORM

    // TODO >>>
    //return;

    //printf("ENABLE_BLINK_UI 1\n");
#ifdef ENABLE_BLINK_UI
    //printf("ENABLE_UI 2\n");
    gfx::Canvas gfx_canvas(&cc_skia_paint_canvas, 1.0f);
#if 0
    /*{
      cc::PaintFlags paintFlags;
      paintFlags.setAntiAlias(true);
      paint.setColor(SK_ColorDKGRAY);
      paint.setStrokeWidth(2);
      gfx_canvas.DrawCircle(gfx::Point(200,200), 100, paintFlags);
      paint.setColor(SK_ColorMAGENTA);
      gfx_canvas.DrawRoundRect(gfx::Rect(550, 550, 200, 200), 50, paintFlags);
    }*/
    {
      // see https://github.com/codebyravi/electron/blob/master/atom/common/api/atom_api_native_image.cc#L115
      //gfx_canvas.DrawImageInt(*imageSkia.get(), 330, 330);
      DCHECK(gfxImageSkia);
      gfx_canvas.DrawImageInt(
        gfx::ImageSkia(gfxImageSkia->GetRepresentation(10.0f)),
          10, 10);
    }
    {
      cc::PaintFlags paintFlags;
      paintFlags.setShader(
          gfx::CreateGradientShader(0, 500, SK_ColorRED, SK_ColorGREEN));
      paintFlags.setStyle(cc::PaintFlags::kFill_Style);
      paintFlags.setAntiAlias(true);
      gfx_canvas.DrawRoundRect(gfx::Rect(350, 350, 200, 400), 50, paintFlags);
    }
    {
      cc::TransformOperations operations_from;
      operations_from.AppendIdentity();
      cc::TransformOperations operations_to;
      operations_to.AppendIdentity();
      gfx::BoxF box(1.f, 2.f, 3.f);
      gfx::BoxF bounds;
      SkMScalar min_progress = 0.f;
      SkMScalar max_progress = 1.f;
      const bool bres = operations_to.BlendedBoundsForBox(
        box, operations_from, min_progress, max_progress, &bounds);
      // 1 box 0.000000,0.000000,0.000000 1.000000x2.000000x3.000000 bounds 0.000000,0.000000,0.000000 1.000000x2.000000x3.000000
      //printf("%i box %s bounds %s\n", bres, box.ToString().c_str(), bounds.ToString().c_str());
    }
#endif // 0

    //printf("onDraw() x4\n");

#if defined(ENABLE_CUSTOM_FONTS) && defined(ENABLE_HARFBUZZ)
    {
      //gfx::RenderText::CreateHarfBuzzInstance();
      // https://github.com/chromium/chromium/blob/422f901782f0a5f274a6065fbff3983279ef3c0b/chrome/browser/vr/elements/text.cc#L424
      ::std::unique_ptr<gfx::RenderText> render_text_ptr = gfx::RenderText::CreateHarfBuzzInstance();
      //auto render_text_ptr = std::make_unique<gfx::RenderTextHarfBuzz>();
      gfx::RenderText* render_text = render_text_ptr.get();
      render_text->SetDisplayRect(gfx::Rect(0, 0, 500, 500));
      render_text->SetHorizontalAlignment(gfx::ALIGN_TO_HEAD);
      render_text->SetColor(SK_ColorBLACK);
      render_text->SetMultiline(true);
      render_text->SetStyle(gfx::TEXT_STYLE_UNDERLINE, true);
      //render_text->SetBorder(views::CreateSolidBorder(2, SK_ColorGRAY));
      //WTF::String render_test_string = String::FromUTF8("some very long text here r\xC3\xA9sum\xC3\xA9");
      //render_text->SetText(render_test_string.Characters16());
      gfx::FontRenderParams fontRenderParams;
      //fontRenderParams.antialiasing = false;
      //fontRenderParams.use_bitmaps = true;
      DCHECK(sktpForUI);
      // TODO: free mem
      if(!customPlatformFont) {
        printf("creating customPlatformFont\n");

        /*CHECK(sktpForUICreated);// << "sktpForUICreated required";
        CHECK(sktpForUI);// << "sktpForUI required";
        //prepareUIFonts(); // see DCHECK(sktpForUI);
        SkFont skFont3ForUI(sktpForUI, FONT_SIZE_F, 1.0f, 0.0f);
        skFont3ForUI.setEdging(SkFont::Edging::kAntiAlias);
        DCHECK(sktpForUI);
        DCHECK(skFont3ForUI.getTypeface());*/

        //gfx::PlatformFontSkia::InitFromDetails

        customPlatformFont = new gfx::PlatformFontSkia(
          sktpForUI, /*family*/ "Arial", /*size_pixels*/ 22,
          /*style*/ gfx::Font::NORMAL,
          /*weight*/ gfx::Font::Weight::NORMAL,
          /*params*/ fontRenderParams);
      }

      if(!default_font) {
        printf("creating default_font\n");
        // TODO: free mem
        DCHECK(customPlatformFont);
        default_font = new gfx::Font(customPlatformFont);
      }

      //gfx::Font defaultFont(customPlatformFont);
      //defaultFont = gfx::Font("Arial", 10);
      //gfx::FontList font_list(defaultFont);

      const gfx::FontList& font_list = GetTextFontList();
      render_text->SetText(base::UTF8ToUTF16("x render_text x render_text x"));
      render_text->SetFontList(font_list);
      gfx::ShadowValues shadows(1,
                            gfx::ShadowValue(gfx::Vector2d(), 1, SK_ColorRED));
      constexpr gfx::ShadowValue shadow(gfx::Vector2d(2, 2), 0, SK_ColorGRAY);
      shadows.push_back(shadow);
      render_text->set_shadows(shadows);
      render_text->SetColor(
        blink::Color(0.0f, 1.0f, 0.5f, 0.5f).Rgb());
      render_text->SetElideBehavior(gfx::ELIDE_TAIL);
      render_text->set_selection_background_focused_color(
          SkColorSetARGB(150, 0, 188, 112));
      render_text->set_selection_color(
        //SkColorSetARGB(150, 0, 188, 182)
        blink::Color(0.0f, 0.0f, 1.0f, 0.5f).Rgb()
      );
      render_text->SetObscured(false);
      // render_text->SetSelectionModel(); // see SelectRange
      render_text->SetMultiline(true);
      render_text->SetHorizontalAlignment(gfx::ALIGN_TO_HEAD);
      render_text->SetWordWrapBehavior(gfx::WRAP_LONG_WORDS);
      render_text->set_clip_to_display_rect(false);
      render_text->set_focused(true);
      // see https://github.com/chromium/chromium/blob/master/chrome/browser/vr/elements/text.cc#L384
      int selection_start_ = 0;
      int selection_end_ = 2;
      render_text->SetCursorPosition(selection_end_);
      gfx::Range range(selection_start_, selection_end_);
      render_text->SetSelection(gfx::SelectionModel(
          range, gfx::LogicalCursorDirection::CURSOR_FORWARD));
      render_text->SelectRange({0, 2});
//#if 0
      render_text->Draw(&gfx_canvas); // calls DrawSelection if focused
//#endif // 0

      gfx_canvas.DrawStringRect(
        base::UTF8ToUTF16("1 DrawStringRect ☎,☎,☎👶🏻 or 👩🏼‍🎨 🔍 💥 \uf08a ⚡💨 :) DrawStringRect 3"),
        font_list,
        SkColorSetARGB(150, 0, 188, 112),
        gfx::Rect(500, 200, 500, 500));
    }

    // TODO: textfield
    // https://github.com/chromium/chromium/blob/master/ui/views/controls/textfield/textfield.cc
#endif // ENABLE_CUSTOM_FONTS && ENABLE_HARFBUZZ
    {
      //views::Label* zoom_label_;
    }
    {
      using namespace ::gfx;
      // using ::gfx::CommandType;
      const VectorIconRep rep_list[] = {{tiger_icon_elements, base::size(tiger_icon_elements)}};
      const VectorIcon icon_src = {rep_list, 1u};
      const SkColor icon_color = SK_ColorRED;
      // see https://skia.org/user/api/SkCanvas_Reference
      gfx_canvas.Save();
      gfx_canvas.Translate(gfx::Vector2d(
                            (BROWSER_WIDTH / 2) - (tiger_icon_dip_size / 2),
                            (BROWSER_HEIGHT / 2) - (tiger_icon_dip_size / 2)));
#if 0
      gfx::PaintVectorIcon(&gfx_canvas, icon_src, tiger_icon_dip_size, icon_color);
#endif // 0
      gfx_canvas.Restore();
      //gfx::Image icon =
      //  gfx::CreateVectorIcon(kFooBarIcon, 32, color_utils::DeriveDefaultIconColor(text_color));
    }
#endif // ENABLE_BLINK_UI

#if defined(ENABLE_BLINK_UI_VIEWS)
  // see https://github.com/blockspacer/skia-opengl-emscripten/blob/master/src/chromium/ui/views/examples/examples_main.cc
  // see https://github.com/blockspacer/skia-opengl-emscripten/blob/master/src/chromium/ui/views/examples/example_base.cc#L39
  // TODO: ui::MaterialDesignController::Initialize();

  // TODO: CleanupSharedInstance
  //ui::ResourceBundle::InitSharedInstance(&delegate);
  // see https://github.com/blockspacer/skia-opengl-emscripten/blob/main/src/chromium/services/service_manager/embedder/main.cc#L205
  if(!ui::ResourceBundle::HasSharedInstance()) {
    printf("creating ui::ResourceBundle\n");
    ui::ResourceBundle::InitSharedInstanceWithLocale(
        "en-US", NULL, ui::ResourceBundle::LOAD_COMMON_RESOURCES);
  }

  /*if(!context_factories_) {
    context_factories_ =
        std::make_unique<ui::TestContextFactories>(enable_pixel_output_);
  }*/

  if(!container_) {
    printf("creating container\n");
    container_ = new ContainerView();
    // see https://github.com/blockspacer/skia-opengl-emscripten/blob/master/src/chromium/ui/views/examples/textfield_example.cc
    auto layout_manager = std::make_unique<views::GridLayout>(container_);
    views::ColumnSet* column_set = layout_manager->AddColumnSet(/* id */0);
    //column_set->AddColumn(views::GridLayout::LEADING, views::GridLayout::FILL,
    //                      0.2f, views::GridLayout::USE_PREF, 0, 0);
    //column_set->AddColumn(views::GridLayout::FILL, views::GridLayout::FILL,
    //                      0.8f, views::GridLayout::USE_PREF, 0, 0);
    column_set->AddColumn(views::GridLayout::FILL, views::GridLayout::FILL,
                          0.5f, views::GridLayout::USE_PREF, 0, 0);
    column_set->AddColumn(views::GridLayout::FILL, views::GridLayout::FILL,
                          0.5f, views::GridLayout::USE_PREF, 0, 0);
    views::ColumnSet* column_set2 = layout_manager->AddColumnSet(/* id */ 1);
    column_set2->AddColumn(views::GridLayout::FILL, views::GridLayout::FILL,
                          1.0f, views::GridLayout::USE_PREF, 0, 1);
    container_->addChildren(layout_manager.get());

    //container_->SetLayoutManager(std::make_unique<views::FillLayout>());
    container_->SetLayoutManager(std::move(layout_manager));
  }

  if(!widget_) {
    printf("creating widget\n");
    widget_ = new views::Widget;
    views::Widget::InitParams params(
      views::Widget::InitParams::TYPE_POPUP/*TYPE_WINDOW_FRAMELESS*/);
    //params.bounds = gfx::Rect(0, 0, 800, 800);
    params.ownership = views::Widget::InitParams::WIDGET_OWNS_NATIVE_WIDGET;
    //params.delegate = container_;
    ///params.delegate =
    ///    new ExamplesWindowContents(std::move(on_close), std::move(examples));
    ///params.context = window_context;
    widget_->Init(params);
    //widget_->Show();
  }

  DCHECK(widget_);
  //widget_->SetSize(gfx::Size(800, 800));
  //widget_->SetBounds(gfx::Rect(0, 0, 800, 800));
  widget_->SetFullscreen(/*true*/false);
  widget_->SetVisibleOnAllWorkspaces(true);

  DCHECK(container_);

  ////
  widget_->SetContentsView(container_);

  views::View* root_view = widget_->GetRootView();
  DCHECK(root_view);
  if(!root_view->Contains(container_)) {
    root_view->AddChildView(container_);
    printf("added container_ to root_view\n");
  }
  DCHECK(root_view->GetEffectiveViewTargeter());

  //container_->SetVisible(true);
  container_->SetEnabled(true);
  //container_->SetBounds(0, 0, 800, 800);
  //container_->SetSize(gfx::Size(800, 800));
  //container_->SetPreferredSize(gfx::Size(800, 800));
  //container_->SetNativeTheme(nativeThemeAura);
  container_->SetBackground(views::CreateSolidBackground(
        blink::Color(0.9f, 1.0f, 1.0f, 0.9f).Rgb()));
  //container_->SetBorder(views::CreateSolidBorder(2, SK_ColorBLUE));
  const gfx::Insets child_margins(1, 1);
  // TODO: free mem?
  //container_->SetProperty(views::kMarginsKey,
  //  new gfx::Insets(child_margins));

  //cc::layer_tree_host()

  //container_->SetPaintToLayer(ui::LAYER_TEXTURED);

  //blink::Path path;
  //path.AddRect({0, 0, 800, 800});
  //container_->set_clip_path(path.GetSkPath());

  container_->set_owned_by_client(); // prevents view_to_be_deleted by parent

  //container_->InvalidateLayout();
  //container_->SizeToPreferredSize();
  //container_->Layout();
  //DCHECK(container_->GetLocalBounds() == gfx::Rect(0, 0, 800, 800));
  container_->SchedulePaint();
  container_->SetFocusBehavior(views::View::FocusBehavior::ALWAYS);
  DCHECK(container_->IsFocusable());

  //root_view->SetBoundsRect(container_->GetLocalBounds());
  //root_view->SetBounds(0, 0, 800, 800);
  root_view->InvalidateLayout();
  root_view->SizeToPreferredSize();
  root_view->Layout();

  /*widget->non_client_view()->SetFrameView(frame);  // Owns |frame|.

  WidgetAutoclosePtr widget(CreateTopLevelFramelessPlatformWidget());
  widget->GetRootView()->AddChildView(view);

  widget->SetBounds(gfx::Rect(0, 0, 100, 80));
  widget->Show();
  widget->SetFullscreen(false);
  widget->Show();
  widget->Maximize();*/

  widget_->SetFullscreen(/*true*/false);
  widget_->Maximize();
  widget_->Show(); // TODO: widget_.Close();
  widget_->Activate(); // TODO: wait?

  DCHECK(widget_->GetContentsView());

  //root_view->InvalidateLayout();
  //root_view->Layout();
  //widget_->LayoutRootViewIfNecessary();
  //DCHECK(!widget_->GetRootView()->GetLocalBounds().IsEmpty());

  ///DCHECK(container_->GetWidget());
  //DCHECK(!container_->size().IsEmpty());
  //DCHECK(!container_->GetPreferredSize().IsEmpty());
  //DCHECK(!container_->GetBoundsInScreen().IsEmpty());
  //DCHECK(!container_->GetLocalBounds().IsEmpty());
  //DCHECK(!container_->GetMirroredBounds().IsEmpty());

  // see https://github.com/blockspacer/skia-opengl-emscripten/blob/master/src/chromium/ui/views/controls/label_unittest.cc#L61
  gfx::Rect first_paint(1, 1);
  SkBitmap bitmap;
  auto raster_scale = 1.0f;
  //auto list = base::MakeRefCounted<cc::DisplayItemList>();
  scoped_refptr<cc::DisplayItemList> list =
    base::MakeRefCounted<cc::DisplayItemList>(
        cc::DisplayItemList::kToBeReleasedAsPaintOpBuffer);
  //gfx::Size container_size(800, 800/*container_->GetPreferredSize()*/);
  gfx::Size container_size =
    root_view->GetMirroredBounds().size();
    //container_->GetMirroredBounds().size();
  DCHECK(!container_size.IsEmpty());

  ui::CanvasPainter canvasPainter =
    ui::CanvasPainter(&bitmap, container_size,
    raster_scale, //SK_ColorRED,
    SK_ColorTRANSPARENT,
     // is_pixel_canvas - if the paint commands are
    // recorded at pixel size instead of DIP.
    /*is_pixel_canvas()*/ false);

  views::PaintInfo container_paint_info = views::PaintInfo::CreateRootPaintInfo(
      ui::PaintContext(list.get(), 1.f, /* invalidation */ first_paint, /*true*/ false),
      //canvasPainter.context(),
      container_size);
  DCHECK(!container_paint_info.paint_recording_bounds().IsEmpty());
  DCHECK(container_);

  //container_->Paint(container_paint_info);
  //ui::Compositor* compositor = widget_->GetCompositor();
  //DCHECK(compositor);
  ui::PaintContext container_paint_context(
                             list.get(),
                             1.f,//compositor->device_scale_factor(),
                             //gfx::Rect(0, 0, 800, 800),//
                             gfx::Rect(container_size),
                             false /*true*/);

  widget_->GetRootView()->SchedulePaint();
  container_->SchedulePaint();
  widget_->GetRootView()->SchedulePaint();
  DCHECK(!container_->scroll_view_->GetLocalBounds().IsEmpty());
  container_->scroll_view_->SchedulePaint();
  DCHECK(!container_->scrollable_->GetLocalBounds().IsEmpty());
  container_->scrollable_->SchedulePaint();

  //widget_->GetRootView()->OnPaintLayer(container_paint_context);
  //widget_->GetRootView()->OnPaint(&gfx_canvas);
  //widget_->GetRootView()->PaintDebugRects(container_paint_info);

  // TODO
  // DCHECK(widget_->GetCompositor());

  /*DCHECK(!container_->scroll_view_->contents()
    ->GetLocalBounds().IsEmpty());*/

  widget_->GetRootView()->PaintFromPaintRoot(container_paint_context);

  //container_->scroll_view_->contents_viewport_->PaintFromPaintRoot(container_paint_context);

  //container_->scroll_view_->PaintFromPaintRoot(container_paint_context);

  //container_->scrollable_->PaintFromPaintRoot(container_paint_context);

  //container_->OnPaintLayer(container_paint_context);
  //container_->PaintDebugRects(container_paint_info);

  /*DCHECK(!bitmap.isNull());
  //canvasPainter.context().InvalidationForTesting();
  gfx::ImageSkia container_image(
    gfx::ImageSkiaRep(bitmap, raster_scale));
  DCHECK(!container_image.isNull());
  gfx_canvas.DrawImageInt(container_image, 100, 100);
  gfx_canvas.DrawImageInt(gfx::ImageSkia(gfxImageSkia->GetRepresentation(10.0f)), 630, 30);
  */
  // see recorder_->finishRecordingAsPicture
  //display_item_list_->StartPaint();
  //list->EndPaintOfUnpaired(gfx::Rect());
//#if 0
  //list->Finalize();
  sk_sp<cc::PaintRecord> paintRecord =
    list->ReleaseAsRecord();
  //cc_skia_paint_canvas.save();
  //cc_skia_paint_canvas.drawPicture(paintRecord);
  //cc_skia_paint_canvas.restore();
  gfx::ImageSkia tmp_image(gfx::ImageSkiaRep(paintRecord,
    gfx::ScaleToCeiledSize(container_size, 1.0), 1.0));
  DCHECK(!tmp_image.isNull());
  gfx_canvas.DrawImageInt(tmp_image,
    0, 0);
//#endif // 0

#if 0
  //list->Finalize();
  list->Raster(sk_canvas);
#endif // 0

  /*std::unique_ptr<views::PaintInfo> last_paint_info_
    = container_->GetLastPaintInfo();
  DCHECK(last_paint_info_);
  DCHECK(last_paint_info_->context().list_);
  last_paint_info_->context().list_->Raster(sk_canvas);*/

  //gfx::Canvas gfx_canvas(&cc_skia_paint_canvas, 1.0f);

  // TODO: make views paint recursive or FIX Paint via views::PaintInfo
  //container_->ForcePaint(&gfx_canvas);
  /*views::View::Views children = container_->GetChildrenInZOrder();
  for (auto* child : children) {
    if (!child->layer())
      //(child->*func)(info);
      child->OnPaint(&gfx_canvas);
  }*/

#if 0
  DCHECK(container_->width() > 0);
  DCHECK(container_->height() > 0);
  /*sk_sp<SkSurface> tmpRasterSurface = SkSurface::MakeRaster(SkImageInfo::MakeN32(
    container_->width(), container_->height(),
    kPremul_SkAlphaType)); // TODO);
  DCHECK(tmpRasterSurface);
  cc::SkiaPaintCanvas tmpSkiaPaintCanvas(tmpRasterSurface->getCanvas());
  */

  ///SkBitmap tmpbitmap;
  /*SkImageInfo info = SkImageInfo::MakeN32Premul(
    container_->width(), container_->height());
  // TODO: width() * 4?
  bitmap.allocPixels(info, container_->width() * 4);
  bitmap.eraseColor(SK_AlphaTRANSPARENT);*/
  //SkBitmap tmpbitmap = AllocBitmap(container_->width(), container_->height());
  ///tmpbitmap.allocN32Pixels(container_->width(), container_->height());
  ///cc::SkiaPaintCanvas tmpSkiaPaintCanvas(tmpbitmap);
  ///gfx::Canvas tmp_gfx_canvas(&tmpSkiaPaintCanvas, 1.0f);
  gfx::Canvas tmp_gfx_canvas(
    gfx::Size(container_->width(), container_->height()),
    1.0f,
    /*opaque*/ true);
  //tmp_gfx_canvas.Save();
  //tmpRasterSurface->getCanvas()->clear(
  //  SkColorSetARGB(255, 255, 255, 255));
  recursivePaintHelper(container_, &tmp_gfx_canvas);
  //tmp_gfx_canvas.GetBitmap();
  ///tmpSkiaPaintCanvas.flush();
  //tmpRasterSurface->getCanvas()->flush();
  ///DCHECK(!tmp_gfx_canvas.GetBitmap().isNull());
  ///DCHECK(!tmpbitmap.isNull());
  DCHECK(!tmp_gfx_canvas.GetBitmap().isNull());
  gfx::ImageSkia tmp_image(
    gfx::ImageSkiaRep(/*tmpbitmap*/
      tmp_gfx_canvas.GetBitmap(), raster_scale));
  //DCHECK(!tmp_image.isNull());
  gfx_canvas.DrawImageInt(tmp_image, container_->width(), container_->height());
  //tmp_gfx_canvas.Restore();
  //tmpRasterSurface.release();
#endif // 0

#if 0
  if(!test_label) {
    printf("creating test_label\n");
    const gfx::FontList gfxFontList = GetTextFontList();
    views::Label::CustomFont labelCustomFont{gfxFontList};
    test_label = new TestLabel(base::UTF8ToUTF16("test_label ! test_label ! test_label !"),
                               labelCustomFont);
  }
  DCHECK(test_label);
  //test_label->SetFontList(GetTextFontList());
  test_label->SetHorizontalAlignment(gfx::ALIGN_RIGHT);
  test_label->SetObscured(false);
  test_label->SetSelectable(true);
  test_label->SetSelectionBackgroundColor(
    blink::Color(0.1f, 0.2f, 0.0f, 0.5f).Rgb());
  test_label->SetSelectionTextColor(
    blink::Color(0.4f, 0.4f, 0.9f, 0.5f).Rgb());
  test_label->SetBackgroundColor(
    blink::Color(0.9f, 0.0f, 0.9f, 0.5f).Rgb());
  test_label->SetElideBehavior(gfx::ELIDE_TAIL);
  test_label->SetEnabledColor(
    blink::Color(1.0f, 0.0f, 1.0f, 0.5f).Rgb());
  //test_label->SetText();
  test_label->SelectRange({0, 4});
  //message_->RecalculateFont();
  test_label->SetBorder(views::CreateSolidBorder(2, SK_ColorCYAN));
  test_label->SetHorizontalAlignment(gfx::ALIGN_TO_HEAD);
  //AddChildView(message_);
  test_label->SimulatePaint();
  DCHECK(!test_label->bitmap.isNull());
  //canvasPainter.context().InvalidationForTesting();
  gfx::ImageSkia test_label_image(
    gfx::ImageSkiaRep(test_label->bitmap, raster_scale));
  DCHECK(!test_label_image.isNull());
  gfx_canvas.Save();
  gfx_canvas.Translate(gfx::Vector2d(200, 200));
  gfx_canvas.DrawImageInt(test_label_image, test_label->width(), test_label->height());
  gfx_canvas.Restore();
#endif // 0

  /*ui::PaintCache paint_cache_;
  ui::PaintRecorder recorder(paint_info.context(), paint_info.paint_recording_size(),
                             paint_info.paint_recording_scale_x(),
                             paint_info.paint_recording_scale_y(),
                             &paint_cache_);
  //auto record = recorder.finishRecordingAsPicture();
  sk_sp<SkImage> imgRes = SkImage::MakeFromBitmap(recorder.canvas()->GetBitmap());
  DCHECK(imgRes);
  sRasterSurface->getCanvas()->drawImage(imgRes, 0, 0);*/

  //context.DrawImage(imgRes.get(), );
  //gfx_canvas.DrawImageInt(gfx::ImageSkia(imgRes.get()), 330, 330);
  //recorder.canvas()->GetBitmap()
  //pinf.context().
  /*cc::PaintCanvas* canvas = ResourceProvider()->Canvas();
  {
    sk_sp<PaintRecord> recording = recorder_->finishRecordingAsPicture();
    canvas->drawPicture(recording);
  }*/

  /*ui::PaintContext ui_context(list.get(),
                           compositor->device_scale_factor(),
                           gfx::Rect(compositor->size()), true);
  container_->PaintFromPaintRoot(ui_context);*/

  /*if(!widget_) {
    widget_ = new views::Widget;
    views::Widget::InitParams params;
    ///params.delegate =
    ///    new ExamplesWindowContents(std::move(on_close), std::move(examples));
    ///params.context = window_context;
    widget_->Init(params);
    widget_->Show();
  }
  widget_->OnNativeWidgetPaint(context);*/
#endif // ENABLE_BLINK_UI_VIEWS

    //printf("onDraw() 7\n");
  }

  SkPainter(SkColor color, SkScalar size) : m_color(color), m_size(size) {}
};

// see https://github.com/flutter/engine/blob/master/shell/gpu/gpu_surface_gl.cc#L125
void SkiaUiDemo::initUISkiaSurface(int /*w*/, int /*h*/) {
#if defined(OS_EMSCRIPTEN) && !defined(DISABLE_PTHREADS) && defined(ENABLE_COBALT)
  DCHECK(false);// << "can`t initUiSkiaSurface on WASM MT";
#endif

#ifdef SKIA_GR_CONTEXT
  {
    auto sInterface =
        emscripten_GrGLMakeNativeInterface(); // sk_sp<const
                                              // GrGLInterface>(GrGLCreateNativeInterface());
    if (sInterface == nullptr) {
      printf("Error while creating GrGLInterface\n");
      abort();
    }
    // Validates that the GrGLInterface supports its advertised standard. This means the necessary
    // function pointers have been initialized for both the GL version and any advertised
    // extensions.
    if (!sInterface->validate()) {
      printf("Error while validating GrGLInterface\n");
      abort();
    }

    printf("create GrContext...\n");

    GrContextOptions options;
    grContext = GrContext::MakeGL(::std::move(sInterface), options).release();
    if (!grContext) {
      printf("failed to create grContext.\n");
    }
    SkASSERT(grContext);

    printf("created GrContext...\n");

    GrGLint bufferID;
    // Wrap the frame buffer object attached to the screen in a Skia render target so Skia can
    // render to it
    GR_GL_GetIntegerv(sInterface.get(), GR_GL_FRAMEBUFFER_BINDING, &bufferID);
    GrGLFramebufferInfo info;
    info.fFBOID = (GrGLuint)bufferID;
    SkColorType colorType;

// TODO: we use em_ctx for now
//#if defined(ENABLE_HTML5_SDL) || !defined(__EMSCRIPTEN__)
//    printf("SDL_GetWindowPixelFormat...\n");
//
//    uint32_t windowFormat = SDL_GetWindowPixelFormat(window);
//    int contextType;
//
//    // TODO: use em_ctx
//    //em_ctx
//    SDL_GL_GetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, &contextType);
//
//    printf("get windowFormat...\n");
//
//    // SkDebugf("%s", SDL_GetPixelFormatName(windowFormat));
//    // TODO: the windowFormat is never any of these?
//    if (SDL_PIXELFORMAT_RGBA8888 == windowFormat) {
//      info.fFormat = GR_GL_RGBA8;
//      colorType = kRGBA_8888_SkColorType;
//    } else {
//      colorType = kBGRA_8888_SkColorType;
//      // TODO: use em_ctx
//      //em_ctx
//      if (SDL_GL_CONTEXT_PROFILE_ES == contextType) {
//        info.fFormat = GR_GL_BGRA8;
//      } else {
//        // We assume the internal format is RGBA8 on desktop GL
//        info.fFormat = GR_GL_RGBA8;
//      }
//    }
//#endif // ENABLE_HTML5_SDL || !__EMSCRIPTEN__

    // TODO: we use em_ctx for now
    info.fFormat = GR_GL_BGRA8; //  TODO
    colorType = kBGRA_8888_SkColorType;

    printf("create GrBackendRenderTarget...\n");

    GrBackendRenderTarget target(width, height, kMsaaSampleCount, kStencilBits, info);

    // setup SkSurface
    // To use distance field text, use commented out SkSurfaceProps instead
    // SkSurfaceProps props(SkSurfaceProps::kUseDeviceIndependentFonts_Flag,
    //                      SkSurfaceProps::kLegacyFontHost_InitType);
    SkSurfaceProps props(SkSurfaceProps::kLegacyFontHost_InitType);

    sk_sp<SkSurface> surface(SkSurface::MakeFromBackendRenderTarget(
        grContext, target, kBottomLeft_GrSurfaceOrigin, colorType, nullptr, &props));
    if (!surface) {
      printf("failed to create surface.\n");
    }
  }
#endif // SKIA_GR_CONTEXT
  // see https://github.com/midasitdev/aliceui/blob/master/example/OpenGLExample/OpenGLExample/main.cpp#L391
  const SkImageInfo info = SkImageInfo::MakeN32(
    /*width*/ DRAW_SURFACE_WIDTH, /*height*/ DRAW_SURFACE_HEIGHT,
    kPremul_SkAlphaType); // TODO
  sRasterSurface = SkSurface::MakeRaster(info);
  if (!sRasterSurface) {
    printf("failed to create raster surface\n");
  }

        printf("Initializing skia view...\n");

#ifdef ENABLE_SKIA
        if(!myView) {
          printf("creating myView\n");
          myView = new SkPainter(SK_ColorRED, 200);
        }
#endif // ENABLE_SKIA

#ifdef ENABLE_SKOTTIE
        {
          printf("Initializing skottie animations...\n");

          //
          // sk_sp<skottie_utils::FileResourceProvider> frp
          //  = skottie_utils::FileResourceProvider::Make(SkOSPath::Dirname(fPath.c_str()));
          // frp->load(fPath.c_str(), "data.json");
          //

          skottie::Animation::Builder builder;
          /*fAnimation      = builder
                    //.setLogger(logger)
                    .setResourceProvider(
                        skottie_utils::FileResourceProvider::Make(SkOSPath::Dirname(fPath.c_str())))
                    .makeFromFile(fPath.c_str());*/
          /*
          defined(ENABLE_HTML5_SDL) || !defined(__EMSCRIPTEN__)
          SDL_RWops* fileHandle = SDL_RWFromFile(fPath.c_str(), "r");
          if (fileHandle == nullptr) {
              SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Can't find the file!\n");
          }*/

          printf("Reading skottie animations...\n");

          char* fileString = nullptr;
          long int fsize;
          int readRes = read_file(fAnimPath.c_str(), fileString, fsize, true);
          if (readRes != 0) {
            printf("can`t read skottie anim %s\n", fAnimPath.c_str());
            NOTREACHED();
          }
          DCHECK(fileString != nullptr);

          /*FILE* f = fopen(fPath.c_str(), "rb");
          if (!f) {
            printf("failed to open file: %s\n", fPath.c_str());
            return 1;
          }
          fseek(f, 0, SEEK_END);
          long int fsize = ftell(f);
          fseek(f, 0, SEEK_SET);
          char* fileString = new char[fsize + 1];
          fread(fileString, 1, fsize, f);
          fclose(f);
          fileString[fsize] = 0;*/

          printf("Building skottie animations...\n");

          // see https://github.com/google/skia/blob/master/modules/skottie/src/Skottie.cpp#L459
          fAnimation = builder.make(fileString, static_cast<size_t>(fsize));
          printf("Built skottie animations...\n");

          printf("Get skottie stats...\n");
          fAnimationStats = builder.getStats();
          fTimeBase = 0; // force a time reset
          if (fAnimation) {
            fAnimation->setShowInval(fShowAnimationInval);
            printf("Loaded Bodymovin animation v: %s, size: [%f %f]\n", fAnimation->version().c_str(),
                   static_cast<double>(fAnimation->size().width()),
                   static_cast<double>(fAnimation->size().height()));
            DCHECK(fAnimation->size().width() > 0);
            DCHECK(fAnimation->size().height() > 0);
          } else {
            printf("failed to load Bodymovin animation: %s\n", fAnimPath.c_str());
            NOTREACHED();
          }
          printf("Got skottie stats...\n");

          delete[] fileString; // TODO
          printf("skottie animations are ready...\n");
        }
#endif // ENABLE_SKOTTIE
}

void SkiaUiDemo::cleanup_skia_ui() {
#if defined(OS_EMSCRIPTEN) && !defined(DISABLE_PTHREADS) && defined(ENABLE_COBALT)
  DCHECK(!sRasterSurface);// << "can`t use sRasterSurface on WASM MT";
#endif
  if (sRasterSurface.get())
    delete sRasterSurface.release();
}


#if defined(ENABLE_SK_UI)
static sk_sp<SkImage> UIDemoImage;

// TODO: replace mutex with task queue/emscripten_async_run_in_main_runtime_thread
//#define ENABLE_UI_IMG_LOCKS 1

// TODO: use PostTask
#if defined(ENABLE_UI_IMG_LOCKS)
static std::mutex UIDemoImageMutex;
#endif // ENABLE_UI_IMG_LOCKS)

sk_sp<SkImage> getUiSkImage() {
#if defined(ENABLE_UI_IMG_LOCKS)
  std::scoped_lock lock(UIDemoImageMutex);
#endif // ENABLE_UI_IMG_LOCKS)
  return UIDemoImage;
}

static int testRed = 0;

void SkiaUiDemo::loadUIAssets() {
#ifdef ENABLE_BLINK_UI
  display::Display display(0, gfx::Rect(0, 0, 1024, 1024));
  std::cout << display.bounds().ToString().c_str() << std::endl;
  std::cout << display.work_area().ToString().c_str() << std::endl;
#endif // ENABLE_UI

#if defined(OS_EMSCRIPTEN) && !defined(DISABLE_PTHREADS) && defined(ENABLE_COBALT)
  DCHECK(false);// << "can`t loadUIAssets on WASM MT";
#endif // defined(OS_EMSCRIPTEN) && !defined(DISABLE_PTHREADS)

#if defined(ENABLE_IMAGES) && defined(ENABLE_BLINK_UI)
  ///DCHECK(thread->IsCurrentThread());
  ///
  ///blink::ThreadState::AttachCurrentThread();
  {
      SkImageInfo skImageInfo = SkImageInfo::Make(
          100,
          100,
          SkColorType::kRGBA_8888_SkColorType,
          SkAlphaType::kPremul_SkAlphaType, // alpha type can also be opaque if there is no alpha information
          SkColorSpace::MakeSRGBLinear()
          );
      //SkBitmap::Config config = SkImageInfoToBitmapConfig(info, &isOpaque);
      //SkBitmap fetched_bitmap;
      void* pixels = nullptr; // TODO: free mem
      char* fileData = nullptr; // TODO: free mem
      long int fsize;
      //size_t fsize;
      int readRes = read_file(fImagePath.c_str(), fileData, fsize, true);
      if (readRes != 0) {
          printf("can`t read file %s\n", fImagePath.c_str());
      }
      DCHECK(fileData != nullptr);
      ::std::unique_ptr<SkBitmap> decoded(new SkBitmap());
      /*
static bool Decode(const unsigned char* input, size_t input_size,
         ColorFormat format, ::std::vector<unsigned char>* output,
         int* w, int* h);
static bool Decode(const unsigned char* input, size_t input_size,
         SkBitmap* bitmap);
*/
      const unsigned char* data =
          reinterpret_cast<const unsigned char*>(fileData);
      if (!gfx::PNGCodec::Decode(data, static_cast<size_t>(fsize), decoded.get())) {
          printf("can`t decode png file %s\n", fImagePath.c_str());
          decoded = gfx::JPEGCodec::Decode(data, static_cast<size_t>(fsize));
          if (!decoded) {
              printf("can`t decode JPEG file %s\n", fImagePath.c_str());
          }
      }
      //fetched_bitmap.setPixels(pixels);
      //fetched_bitmap.setIsOpaque(isOpaque);
      //gfx::ImageSkia imageSkia(gfx::ImageSkiaRep(*decoded, /*scale=*/1.0));
      gfx::ImageSkiaRep rep = gfx::ImageSkiaRep(*decoded, /*scale=*/1.0);
      gfxImageSkia = ::std::make_unique<gfx::ImageSkia>(rep);
      if (!gfxImageSkia || gfxImageSkia->isNull()) {
          printf("can`t get gfxImageSkia\n");
      }
      //skImageSp = SkImage::MakeFromBitmap(
      //  gfxImageSkia->GetRepresentation(/*scale=*/1.0).GetBitmap());
      /// \note requires parent thread (TLS)
      skImageSp = SkImage::MakeFromBitmap(rep.GetBitmap());
      if (!skImageSp || !skImageSp->isValid(nullptr)) {
          printf("can`t get skImageSp\n");
      }
      DCHECK(skImageSp);
      //std::cout << std::endl; // flush

//#ifdef __TODO__
      base::WeakPtr<blink::WebGraphicsContext3DProviderWrapper> wptr = nullptr;
      DCHECK(skImageSp);
      sStaticBitmapImage =
          blink::StaticBitmapImage::Create(skImageSp, ::std::move(wptr));
//#endif
      // Shutdown the thread (via its scheduler).
      //thread_->Scheduler()->Shutdown();

      ///
      ///blink::ThreadState::DetachCurrentThread();
  }
#endif // ENABLE_IMAGES
}

void SkiaUiDemo::refreshUIDemo() {
#if defined(OS_EMSCRIPTEN) && !defined(DISABLE_PTHREADS) && defined(ENABLE_COBALT)
  // TODO: support multiple skia threads on WASM
  DCHECK(false);// << "can`t refreshUI on WASM MT";
  DCHECK_RUNS_IN_THEAD_ON_MT_WASM();
#endif

  if (isDebugPeriodReached()) printf("refreshUIDemo...\n");

  //printf("refreshUIDemo 2...\n");

  //DCHECK(g_cobaltTester);
  //DCHECK_EQ(base::MessageLoopCurrent::Get().task_runner(), g_cobaltTester->self_task_runner_);
  //DCHECK(g_cobaltTester->thread_checker_.CalledOnValidThread());

  // Draw to the surface via its SkCanvas.
  // We don't manage this pointer's lifetime.
  SkCanvas* uicanvas =
      //  getRasterizerSkSurface()->getCanvas();
      sRasterSurface->getCanvas();

  if(++testRed >= 255) {
    testRed = 0;
  }
  DCHECK_LE(testRed, 255);
  uicanvas->clear(SkColorSetARGB(testRed, 255, 255, 255));

  ///if (isDebugPeriodReached()) printf("Draw() 3\n");

  //printf("refreshUIDemo 2.1...\n");
  myView->onDraw(uicanvas);
  //printf("refreshUIDemo 2.2...\n");

  //if (isDebugPeriodReached()) printf("Draw() 4\n");

  sRasterSurface->flush();
  //if (isDebugPeriodReached()) printf("Draw() 5\n");

  //if(!UIDemoImage)
  {
#if defined(ENABLE_UI_IMG_LOCKS)
    std::scoped_lock lock(UIDemoImageMutex);
#endif // ENABLE_UI_IMG_LOCKS
    UIDemoImage = sRasterSurface->makeImageSnapshot();
    if (nullptr == UIDemoImage) {
      printf("UIDemoImage can`t makeImageSnapshot\n");
    }
  }

  //printf("refreshUIDemo 3...\n");
}

void SkiaUiDemo::drawUIDemo() {
  if (isDebugPeriodReached()) printf("drawUIDemo...\n");

#if defined(OS_EMSCRIPTEN) && !defined(DISABLE_PTHREADS) && defined(ENABLE_COBALT)
  DCHECK(false);// << "can`t drawUI on WASM MT";
#endif

  SkPixmap uiPixmap;
  {
    sk_sp<SkImage> res = getUiSkImage();
    if (nullptr == res) {
        ///if (isDebugPeriodReached())
        printf("can`t get UIDemoImage\n");
    }
    //const sk_sp<SkImage> res = getRasterizerSkSurface()->makeImageSnapshot();
    if(res /*&& !res->isAlphaOnly()
  && res->width() > 0 && res->height() > 0*/) {
        ///if (isDebugPeriodReached()) printf("Draw() 7\n");
        ///res->flush(nullptr);
        //DCHECK(res->isValid());
        //SkPixmap pixmap;
        if (!res->peekPixels(&uiPixmap)) {
            ///if (isDebugPeriodReached())
            printf("can`t peekPixels\n");
        }
        if(uiPixmap.bounds().isEmpty()) {
          ///if (isDebugPeriodReached()) printf("Draw() 7.1\n");
          ///if (isDebugPeriodReached())
          printf("can`t get UIDemoImage pixmap\n");
        } else {
          //uiPixmap = pixmap;
        }
    }
    // TODO: move GL* out of scoped_lock
    drawGLTexture(uiPixmap.width(), uiPixmap.height(), uiPixmap.addr(), skia_texture);
  }
}
#endif // ENABLE_SK_UI

SkiaUiDemo::SkiaUiDemo() {

}

void SkiaUiDemo::resetAssets() {
#if defined(ENABLE_SKIA) && defined(ENABLE_SKOTTIE)
  fAnimation.reset();
#endif
}

void SkiaUiDemo::animateUI() {
#if defined(OS_EMSCRIPTEN) && !defined(DISABLE_PTHREADS) && defined(ENABLE_COBALT)
  DCHECK(false);// << "can`t animateUI on WASM MT";
#endif

#if defined(ENABLE_SKIA) && defined(ENABLE_SKOTTIE)

#if defined(ENABLE_HTML5_SDL) || !defined(__EMSCRIPTEN__)
  if (fTimeBase == 0) {
    // Reset the animation time.
    fTimeBase = SDL_GetTicks();
  }

  //printf("animate 2\n");

  // see https://github.com/google/skia/blob/master/platform_tools/android/apps/skottie/src/main/cpp/native-lib.cpp
  if (fAnimation && fAnimation->duration() > 0.0f) {
    const SkMSec tElapsed = SDL_GetTicks() - fTimeBase;
    //printf("animate 3\n");
    const SkScalar duration = fAnimation->duration() * 1000.0f;
    //printf("animate 4\n");
    const double animPos = ::std::fmod(tElapsed, duration) / duration;
    //printf("animate 5\n");
    fAnimation->seek(static_cast<SkScalar>(animPos));
  }

  //printf("animate 6\n");

#else
//#error "TODO: port SDL_GetTicks without SDL"
  //EM_LOG("animate 7\n");
  if (fTimeBase == 0) {
    // Reset the animation time.
    fTimeBase = SkTime::GetMSecs();
  }
  //EM_LOG("animate 8\n");
  // see https://github.com/google/skia/blob/master/platform_tools/android/apps/skottie/src/main/cpp/native-lib.cpp
  if (fAnimation && fAnimation->duration() > 0.0f) {
    const SkMSec tElapsed = SkTime::GetMSecs() - fTimeBase;
    //EM_LOG("animate 9\n");
    const SkScalar duration = fAnimation->duration() * 1000.0f;
    //EM_LOG("animate 10\n");
    const double animPos = ::std::fmod(tElapsed, duration) / duration;
    //EM_LOG("animate 11\n");
    fAnimation->seek(static_cast<SkScalar>(animPos));
  }
  //EM_LOG("animate 12\n");
#endif
#endif // defined(ENABLE_SKIA) && defined(ENABLE_SKOTTIE)

}

void SkiaUiDemo::drawTexture(const int texWidth, const int texHeight, const void* texData) {
  drawGLTexture(texWidth, texHeight, texData, skia_texture);
}

void SkiaUiDemo::prepare_gl() {
  GL_CHECK( glGenTextures(1, &skia_texture) );
}

void SkiaUiDemo::cleanup_gl() {
  GL_CHECK( glDeleteTextures(1, &skia_texture) );
}

#endif // ENABLE_SKIA
