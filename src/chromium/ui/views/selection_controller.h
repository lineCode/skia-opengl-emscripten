// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_SELECTION_CONTROLLER_H_
#define UI_VIEWS_SELECTION_CONTROLLER_H_

#include "base/time/time.h"
#if !(defined(OS_EMSCRIPTEN) && defined(DISABLE_PTHREADS))
#include "base/timer/timer.h"
#endif
#include "ui/gfx/geometry/point.h"
#include "ui/gfx/range/range.h"
#include "ui/gfx/selection_model.h"
#include "ui/views/views_export.h"

namespace gfx {
class RenderText;
}

namespace ui {
class MouseEvent;
}

namespace views {
class SelectionControllerDelegate;

// Helper class used to facilitate mouse event handling and text selection. To
// use, clients must implement the SelectionControllerDelegate interface.
// TODO(karandeepb): Also make this class handle gesture events.
class VIEWS_EXPORT SelectionController {
 public:
  // Describes whether the view managing the delegate was initially focused when
  // the mouse press was received.
  enum InitialFocusStateOnMousePress {
    FOCUSED,
    UNFOCUSED,
  };

  // |delegate| must be non-null.
  explicit SelectionController(SelectionControllerDelegate* delegate);

  // Handle mouse events forwarded by |delegate_|. |handled| specifies whether
  // the event has already been handled by the |delegate_|. If |handled| is
  // true, the mouse event is just used to update the internal state without
  // updating the state of the associated RenderText instance.
  bool OnMousePressed(const ui::MouseEvent& event,
                      bool handled,
                      InitialFocusStateOnMousePress initial_focus_state);
  bool OnMouseDragged(const ui::MouseEvent& event);
  void OnMouseReleased(const ui::MouseEvent& event);
  void OnMouseCaptureLost();

  // Returns the latest click location in root coordinates.
  const gfx::Point& last_click_root_location() const {
    return last_click_root_location_;
  }

  // Sets whether the SelectionController should update or paste the
  // selection clipboard on middle-click. Default is false.
  void set_handles_selection_clipboard(bool value) {
    handles_selection_clipboard_ = value;
  }

  // Offsets the double-clicked word's range. This is only used in the unusual
  // case where the text changes on the second mousedown of a double-click.
  // This is harmless if there is not a currently double-clicked word.
  void OffsetDoubleClickWord(int offset);

 private:
  // Tracks the mouse clicks for single/double/triple clicks.
  void TrackMouseClicks(const ui::MouseEvent& event);

  // Selects the word at the given |point|.
  void SelectWord(const gfx::Point& point);

  // Selects all the text.
  void SelectAll();

  // Returns the associated render text instance via the |delegate_|.
  gfx::RenderText* GetRenderText();

  // Helper function to update the selection on a mouse drag as per
  // |last_drag_location_|. Can be called asynchronously, through a timer.
  void SelectThroughLastDragLocation();

  // Returns whether |point| is inside any substring of the text.
  bool IsInsideText(const gfx::Point& point);

  // A timer and point used to modify the selection when dragging. The
  // |first_drag_location_| field is used to store where the drag-to-select
  // started.
#if !defined(DISABLE_PTHREADS)
  base::RepeatingTimer drag_selection_timer_;
#endif // DISABLE_PTHREADS
  gfx::Point last_drag_location_;
  gfx::Point first_drag_location_;

  // State variables used to track the last click time and location.
  base::TimeTicks last_click_time_;
  gfx::Point last_click_root_location_;

  // Used to track double and triple clicks. Can take the values 0, 1 and 2
  // which specify a single, double and triple click respectively. Alternates
  // between a double and triple click for continuous clicks.
  size_t aggregated_clicks_;

  // The range selected on a double click.
  gfx::Range double_click_word_;

  // Weak pointer.
  SelectionControllerDelegate* delegate_;

  // Whether the selection clipboard is handled.
  bool handles_selection_clipboard_;

  DISALLOW_COPY_AND_ASSIGN(SelectionController);
};

}  // namespace views

#endif  // UI_VIEWS_SELECTION_CONTROLLER_H_
