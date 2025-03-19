#pragma once

#include <memory>
#include <vector>

#include "../graphics/graphics.h"
#include "screen.h"

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
  void run(RWindowApi win_api, RGraphicsApiType gfx_api);

  /*
   * Get's the background windows.
   * Background windows are the borderless windows covering the user screen
   */
  std::vector<std::shared_ptr<RScreen>>& get_screens() { return screens_; }

  /*
   * Get's the main monitor
   */
  RScreen& get_main_screen() { return *screens_[0]; }

  /*
   * Get's the host window used as a platform support for viewports
   */
  RIWindow& get_host_gfx_handler() { return *host_window_; }

  /*
   * Get's the main imgui context
   */
  ImGuiContext* get_main_imgui_ctx() {
    return get_host_gfx_handler().get_gfx().get_imgui_ctx();
  }

 private:
  // The host window containing the viewports
  std::shared_ptr<RIWindow> host_window_;
  // All the screens
  std::vector<std::shared_ptr<RScreen>> screens_;
};
