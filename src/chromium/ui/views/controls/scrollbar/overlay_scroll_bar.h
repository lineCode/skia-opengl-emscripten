// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_CONTROLS_SCROLLBAR_OVERLAY_SCROLL_BAR_H_
#define UI_VIEWS_CONTROLS_SCROLLBAR_OVERLAY_SCROLL_BAR_H_

#include "base/macros.h"
#if !(defined(OS_EMSCRIPTEN) && defined(DISABLE_PTHREADS))
#include "base/timer/timer.h"
#endif
#include "ui/views/controls/scrollbar/base_scroll_bar_thumb.h"
#include "ui/views/controls/scrollbar/scroll_bar.h"

namespace views {

// The transparent scrollbar which overlays its contents.
class VIEWS_EXPORT OverlayScrollBar : public ScrollBar {
 public:
  explicit OverlayScrollBar(bool horizontal);
  ~OverlayScrollBar() override;

 protected:
  // ScrollBar overrides:
  gfx::Rect GetTrackBounds() const override;

  // ScrollBar overrides:
  int GetThickness() const override;
  bool OverlapsContent() const override;

  // View overrides:
  void Layout() override;
  void OnMouseEntered(const ui::MouseEvent& event) override;
  void OnMouseExited(const ui::MouseEvent& event) override;

 private:
  class Thumb : public BaseScrollBarThumb {
   public:
    explicit Thumb(OverlayScrollBar* scroll_bar);
    ~Thumb() override;

    void Init();

   protected:
    // BaseScrollBarThumb:
    gfx::Size CalculatePreferredSize() const override;
    void OnPaint(gfx::Canvas* canvas) override;
    void OnBoundsChanged(const gfx::Rect& previous_bounds) override;
    void OnStateChanged() override;

   private:
    OverlayScrollBar* scroll_bar_;

    DISALLOW_COPY_AND_ASSIGN(Thumb);
  };
  friend class Thumb;

  // Shows this (effectively, the thumb) without delay.
  void Show();
  // Hides this with a delay.
  void Hide();
  // Starts a countdown that hides this when it fires.
  void StartHideCountdown();

#if !defined(DISABLE_PTHREADS)
  base::OneShotTimer hide_timer_;
#endif // DISABLE_PTHREADS

  DISALLOW_COPY_AND_ASSIGN(OverlayScrollBar);
};

}  // namespace views

#endif  // UI_VIEWS_CONTROLS_SCROLLBAR_OVERLAY_SCROLL_BAR_H_
