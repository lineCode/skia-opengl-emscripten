// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_CONTROLS_TEXTFIELD_TEXTFIELD_TEST_API_H_
#define UI_VIEWS_CONTROLS_TEXTFIELD_TEXTFIELD_TEST_API_H_

#include "base/i18n/rtl.h"
#include "base/macros.h"
#include "ui/views/controls/textfield/textfield.h"

namespace views {

// Helper class to access internal state of Textfield in tests.
class TextfieldTestApi {
 public:
  explicit TextfieldTestApi(Textfield* textfield);

  void UpdateContextMenu();

  gfx::RenderText* GetRenderText() const;

  void CreateTouchSelectionControllerAndNotifyIt();

  void ResetTouchSelectionController();

  TextfieldModel* model() const { return textfield_->model_.get(); }

  void ExecuteTextEditCommand(ui::TextEditCommand command) {
    textfield_->ExecuteTextEditCommand(command);
  }

  ui::MenuModel* context_menu_contents() const {
    return textfield_->context_menu_contents_.get();
  }

  ui::TouchEditingControllerDeprecated* touch_selection_controller() const {
    return textfield_->touch_selection_controller_.get();
  }

  ui::TextEditCommand scheduled_text_edit_command() const {
    return textfield_->scheduled_text_edit_command_;
  }

  bool IsCursorBlinkTimerRunning() const {
#if !defined(DISABLE_PTHREADS)
    return textfield_->cursor_blink_timer_.IsRunning();
#else
    return false;
#endif // DISABLE_PTHREADS
  }

  gfx::Rect GetCursorViewRect() { return textfield_->cursor_view_.bounds(); }
  void SetCursorViewRect(gfx::Rect bounds);

  bool IsCursorVisible() const { return textfield_->cursor_view_.visible(); }

  bool IsTextDirectionCheckedInContextMenu(
      base::i18n::TextDirection direction) const;

 private:
  Textfield* textfield_;

  DISALLOW_COPY_AND_ASSIGN(TextfieldTestApi);
};

}  // namespace views

#endif  // UI_VIEWS_CONTROLS_TEXTFIELD_TEXTFIELD_TEST_API_H_
