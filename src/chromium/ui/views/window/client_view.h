// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_WINDOW_CLIENT_VIEW_H_
#define UI_VIEWS_WINDOW_CLIENT_VIEW_H_

#include "ui/views/view.h"

namespace views {

class DialogClientView;
class Widget;

///////////////////////////////////////////////////////////////////////////////
// ClientView
//
//  A ClientView is a View subclass that is used to occupy the "client area"
//  of a widget. It provides basic information to the widget that contains it
//  such as non-client hit testing information, sizing etc. Sub-classes of
//  ClientView are used to create more elaborate contents, e.g.
//  "DialogClientView".
class VIEWS_EXPORT ClientView : public View {
 public:
  // Internal class name
  static const char kViewClassName[];

  // Constructs a ClientView object for the specified widget with the specified
  // contents. Since this object is created during the process of creating
  // |widget|, |contents_view| must be valid if you want the initial size of
  // the widget to be based on |contents_view|'s preferred size.
  ClientView(Widget* widget, View* contents_view);
  ~ClientView() override = default;

  // Manual RTTI ftw.
  virtual DialogClientView* AsDialogClientView();
  virtual const DialogClientView* AsDialogClientView() const;

  // Returns true to signal that the Widget can be closed. Specialized
  // ClientView subclasses can override this default behavior to allow the
  // close to be blocked until the user corrects mistakes, accepts a warning
  // dialog, etc.
  virtual bool CanClose();

  // Notification that the widget is closing.
  virtual void WidgetClosing();

  // Tests to see if the specified point (in view coordinates) is within the
  // bounds of this view. If so, it returns HTCLIENT in this default
  // implementation. If it is outside the bounds of this view, this must return
  // HTNOWHERE to tell the caller to do further processing to determine where
  // in the non-client area it is (if it is).
  // Subclasses of ClientView can extend this logic by overriding this method
  // to detect if regions within the client area count as parts of the "non-
  // client" area. A good example of this is the size box at the bottom right
  // corner of resizable dialog boxes.
  virtual int NonClientHitTest(const gfx::Point& point);

  // Overridden from View:
  gfx::Size CalculatePreferredSize() const override;
  gfx::Size GetMinimumSize() const override;
  gfx::Size GetMaximumSize() const override;
  void Layout() override;
  const char* GetClassName() const override;

 protected:
  // Overridden from View:
#if !defined(UI_VIEWS_NO_AX)
  void GetAccessibleNodeData(ui::AXNodeData* node_data) override;
#endif // UI_VIEWS_NO_AX
  void OnBoundsChanged(const gfx::Rect& previous_bounds) override;
  void ViewHierarchyChanged(
      const ViewHierarchyChangedDetails& details) override;

  // Accessors for private data members.
  View* contents_view() const { return contents_view_; }
  void set_contents_view(View* contents_view) {
    contents_view_ = contents_view;
  }

 private:
  // The View that this ClientView contains.
  View* contents_view_;
};

}  // namespace views

#endif  // UI_VIEWS_WINDOW_CLIENT_VIEW_H_
