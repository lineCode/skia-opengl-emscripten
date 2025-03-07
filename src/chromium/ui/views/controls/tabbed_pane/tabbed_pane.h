// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_CONTROLS_TABBED_PANE_TABBED_PANE_H_
#define UI_VIEWS_CONTROLS_TABBED_PANE_TABBED_PANE_H_

#include "base/compiler_specific.h"
#include "base/macros.h"
#include "base/strings/string16.h"
#include "ui/views/view.h"

namespace views {

class Label;
class Tab;
class TabbedPaneListener;
class TabStrip;

namespace test {
#if !defined(UI_VIEWS_NO_AX)
class TabbedPaneAccessibilityMacTest;
#endif // UI_VIEWS_NO_AX
class TabbedPaneTest;
}

// TabbedPane is a view that shows tabs. When the user clicks on a tab, the
// associated view is displayed.
// Support for horizontal-highlight and vertical-border modes is limited and
// may require additional polish.
class VIEWS_EXPORT TabbedPane : public View {
 public:
  // The orientation of the tab alignment.
  enum class Orientation {
    kHorizontal,
    kVertical,
  };

  // The style of the tab strip.
  enum class TabStripStyle {
    kBorder,     // Draw border around the selected tab.
    kHighlight,  // Highlight background and text of the selected tab.
  };

  // Internal class name.
  static const char kViewClassName[];

  explicit TabbedPane(Orientation orientation = Orientation::kHorizontal,
                      TabStripStyle style = TabStripStyle::kBorder);
  ~TabbedPane() override;

  TabbedPaneListener* listener() const { return listener_; }
  void set_listener(TabbedPaneListener* listener) { listener_ = listener; }

  // Returns the index of the currently selected tab, or
  // TabStrip::kNoSelectedTab if no tab is selected.
  size_t GetSelectedTabIndex() const;

  // Returns the number of tabs.
  size_t GetTabCount();

  // Adds a new tab at the end of this TabbedPane with the specified |title|.
  // |contents| is the view displayed when the tab is selected and is owned by
  // the TabbedPane.
  void AddTab(const base::string16& title, View* contents);

  // Adds a new tab at |index| with |title|. |contents| is the view displayed
  // when the tab is selected and is owned by the TabbedPane. If the tabbed pane
  // is currently empty, the new tab is selected.
  void AddTabAtIndex(size_t index, const base::string16& title, View* contents);

  // Selects the tab at |index|, which must be valid.
  void SelectTabAt(size_t index);

  // Selects |tab| (the tabstrip view, not its content) if it is valid.
  void SelectTab(Tab* tab);

  // Overridden from View:
  gfx::Size CalculatePreferredSize() const override;
  const char* GetClassName() const override;

  // Gets the orientation of the tab alignment.
  Orientation GetOrientation() const;

  // Gets the style of the tab strip.
  TabStripStyle GetStyle() const;

 private:
  friend class FocusTraversalTest;
  friend class Tab;
  friend class TabStrip;
  friend class test::TabbedPaneTest;
#if !defined(UI_VIEWS_NO_AX)
  friend class test::TabbedPaneAccessibilityMacTest;
#endif // UI_VIEWS_NO_AX

  // Get the Tab (the tabstrip view, not its content) at the selected index.
  Tab* GetSelectedTab();

  // Returns the content View of the currently selected Tab.
  View* GetSelectedTabContentView();

  // Moves the selection by |delta| tabs, where negative delta means leftwards
  // and positive delta means rightwards. Returns whether the selection could be
  // moved by that amount; the only way this can fail is if there is only one
  // tab.
  bool MoveSelectionBy(int delta);

  // Overridden from View:
  void Layout() override;
  void ViewHierarchyChanged(
      const ViewHierarchyChangedDetails& details) override;
  bool AcceleratorPressed(const ui::Accelerator& accelerator) override;
#if !defined(UI_VIEWS_NO_AX)
  void GetAccessibleNodeData(ui::AXNodeData* node_data) override;
#endif // UI_VIEWS_NO_AX

  // A listener notified when tab selection changes. Weak, not owned.
  TabbedPaneListener* listener_ = nullptr;

  // The tab strip and contents container. The child indices of these members
  // correspond to match each Tab with its respective content View.
  TabStrip* tab_strip_;
  View* contents_;

  DISALLOW_COPY_AND_ASSIGN(TabbedPane);
};

// The tab view shown in the tab strip.
class Tab : public View {
 public:
  // Internal class name.
  static const char kViewClassName[];

  Tab(TabbedPane* tabbed_pane, const base::string16& title, View* contents);
  ~Tab() override;

  View* contents() const { return contents_; }

  bool selected() const { return contents_->visible(); }
  void SetSelected(bool selected);

  // Overridden from View:
  bool OnMousePressed(const ui::MouseEvent& event) override;
  void OnMouseEntered(const ui::MouseEvent& event) override;
  void OnMouseExited(const ui::MouseEvent& event) override;
  void OnGestureEvent(ui::GestureEvent* event) override;
  gfx::Size CalculatePreferredSize() const override;
  const char* GetClassName() const override;
#if !defined(UI_VIEWS_NO_AX)
  void GetAccessibleNodeData(ui::AXNodeData* node_data) override;
  bool HandleAccessibleAction(const ui::AXActionData& action_data) override;
#endif // UI_VIEWS_NO_AX
  void OnFocus() override;
  void OnBlur() override;
  bool OnKeyPressed(const ui::KeyEvent& event) override;

 protected:
  Label* title() { return title_; }

  TabbedPane* tabbed_pane() { return tabbed_pane_; }

  // Called whenever |state_| changes.
  virtual void OnStateChanged();

 private:
  enum class State {
    kInactive,
    kActive,
    kHovered,
  };

  void SetState(State state);

  // views::View:
  void OnPaint(gfx::Canvas* canvas) override;

  TabbedPane* tabbed_pane_;
  Label* title_;
  gfx::Size preferred_title_size_;
  State state_;
  // The content view associated with this tab.
  View* contents_;

  DISALLOW_COPY_AND_ASSIGN(Tab);
};

// The tab strip shown above/left of the tab contents.
class TabStrip : public View {
 public:
  // The return value of GetSelectedTabIndex() when no tab is selected.
  static constexpr size_t kNoSelectedTab = size_t{-1};

  // Internal class name.
  static const char kViewClassName[];

  TabStrip(TabbedPane::Orientation orientation,
           TabbedPane::TabStripStyle style);
  ~TabStrip() override;

  // Called by TabStrip when the selected tab changes. This function is only
  // called if |from_tab| is not null, i.e., there was a previously selected
  // tab.
  virtual void OnSelectedTabChanged(Tab* from_tab, Tab* to_tab);

  // Overridden from View:
  const char* GetClassName() const override;
  void OnPaintBorder(gfx::Canvas* canvas) override;

  Tab* GetSelectedTab() const;
  Tab* GetTabAtDeltaFromSelected(int delta) const;
  Tab* GetTabAtIndex(size_t index) const;
  size_t GetSelectedTabIndex() const;

  TabbedPane::Orientation orientation() const { return orientation_; }

  TabbedPane::TabStripStyle style() const { return style_; }

 private:
  // The orientation of the tab alignment.
  const TabbedPane::Orientation orientation_;

  // The style of the tab strip.
  const TabbedPane::TabStripStyle style_;

  DISALLOW_COPY_AND_ASSIGN(TabStrip);
};

}  // namespace views

#endif  // UI_VIEWS_CONTROLS_TABBED_PANE_TABBED_PANE_H_
