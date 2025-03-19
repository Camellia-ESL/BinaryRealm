#pragma once

#include <memory>

#include "window.h"

/*
 * The monitor resolution rectangle
 */
struct RScreenRect {
  int left, top, right, bottom;
};

/*
 * Represent's a user screen
 */
class RScreen {
 public:
  RScreen(const RScreenRect& rect, const int monitor_index)
      : rect_(rect), monitor_index_(monitor_index) {}

  /*
   * Initialize a screen
   */
  void init(RWindowApi win_api, RGraphicsApiType gfx_api);

  /*
   * Get's the resolution rectangle in pixels of the screen
   */
  const RScreenRect& get_rect() const { return rect_; }

  /*
   * Get's the index of the monitor
   */
  const int get_monitor_index() const { return monitor_index_; }

  /*
   * Get's the background window covering the screen (borderless) used to render
   * static/dynamic background behaviours.
   */
  RIWindow& get_bg_window() { return *background_window_; }

 private:
  RWindowApi win_api_;
  RGraphicsApiType gfx_api_;
  int monitor_index_;
  RScreenRect rect_;
  std::shared_ptr<RIWindow> background_window_;
};
