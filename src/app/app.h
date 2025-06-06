#pragma once

#include <memory>
#include <vector>

#include "../core/network.h"
#include "../core/time.h"
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
  RIWindow& get_host_win() { return *host_window_; }

  /*
   * Get's the main imgui context
   */
  ImGuiContext* get_main_imgui_ctx() {
    return get_host_win().get_gfx().get_imgui_ctx();
  }

  /*
   * Get's the host framerate limiter
   */
  RFrameRateLimiter& get_host_fps_limiter() { return host_render_fps_limiter_; }

  /*
   * Get's the background framerate limiter
   */
  RFrameRateLimiter& get_bg_fps_limiter() { return bg_render_fps_limiter_; }

 private:
  // The host window containing the viewports
  std::shared_ptr<RIWindow> host_window_;
  // All the screens
  std::vector<std::shared_ptr<RScreen>> screens_;
  // Framerate limiters
  RFrameRateLimiter host_render_fps_limiter_;
  RFrameRateLimiter bg_render_fps_limiter_;
  // Network adapters manager
  RNetworkAdaptersManager network_;
};
