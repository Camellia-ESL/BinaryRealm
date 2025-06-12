#pragma once

#include <memory>

#include "../config/static_configs.h"
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
  RScreen(const RScreenRect& rect, const int monitor_index, bool is_primary)
      : rect_(rect), monitor_index_(monitor_index), is_primary_{is_primary} {}

  /*
   * Initialize a screen
   */
  void init(RWindowApi win_api, RGraphicsApiType gfx_api);

  /*
   * Get's the resolution rectangle in pixels of the screen
   */
  const RScreenRect& get_rect() const { return rect_; }

  /*
   * Get's the screen width
   */
  const float get_width() const { return rect_.right - rect_.left; }

  /*
   * Get's the screen height
   */
  const float get_height() const { return rect_.bottom - rect_.top; }

  /*
   * Get's the index of the monitor
   */
  const int get_monitor_index() const { return monitor_index_; }

  /*
   * Whether this is the primary monitor or not
   */
  const bool is_primary() const { return is_primary_; }

  /*
   * Get's the computed font size adapted to this screen size
   */
  const float get_adapted_font_size() const;

  /*
   * Get's a multiplier to dynamically resize ui components adapted to this
   * screen size
   */
  const float get_adapted_pixel_size() const;

  /*
   * Get's the background window covering the screen (borderless) used to render
   * static/dynamic background behaviours.
   */
  RIWindow& get_bg_window() { return *background_window_; }

 private:
  bool is_primary_;
  RWindowApi win_api_;
  RGraphicsApiType gfx_api_;
  int monitor_index_;
  RScreenRect rect_;
  std::shared_ptr<RIWindow> background_window_;
};
