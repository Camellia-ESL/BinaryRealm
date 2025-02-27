#pragma once

#include <memory>

#include "../graphics/graphics.h"
#include "window.h"

/**
 * [SINGLETON] Represent's the app instance, containing abstracted window,
 * graphics apis, etc...
 */
class RApp {
 public:
  // Get's the static app instance
  static RApp& get() {
    static RApp instance{};
    return instance;
  }

  // Start's the application
  // NOTE: It is called only once by the main thread IT SHOULD NOT BE CALLED by
  // other threads or called again by main thread
  void run(RWindowApi win_api, RGraphicsApi gfx_api);

  // Get's the window handler
  RIWindow& get_window() { return *window_; }
  // Get's the graphics api handler
  RIGraphicsApi& get_graphics() { return *graphics_; }

 private:
  std::shared_ptr<RIWindow> window_;
  std::shared_ptr<RIGraphicsApi> graphics_;
};
