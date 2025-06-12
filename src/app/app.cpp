#include "app.h"

#include <memory>
#include <vector>

#include "../config/config_manager.h"
#include "../config/keybinds.h"
#include "../customization/background.h"
#include "../view/viewpool.h"
#include "../view/views/console_view.h"
#include "screen.h"

#ifdef _WIN32
#include <shellscalingapi.h>

#include "window_apis/win32_api.h"

#pragma comment(lib, "Shcore.lib")

static BOOL CALLBACK MonitorEnumProc(HMONITOR hMonitor, HDC hdcMonitor,
                                     LPRECT lprcMonitor, LPARAM dwData) {
  auto monitors =
      reinterpret_cast<std::vector<std::shared_ptr<RScreen>>*>(dwData);

  RScreenRect rect{*(RScreenRect*)lprcMonitor};

  MONITORINFO mi = {sizeof(MONITORINFO)};
  if (GetMonitorInfo(hMonitor, &mi)) {
    monitors->emplace_back(std::make_shared<RScreen>(
        rect, monitors->size(), (mi.dwFlags & MONITORINFOF_PRIMARY) != 0));
  }
  return TRUE;
}
#endif

void RApp::run(RWindowApi win_api, RGraphicsApiType gfx_api) {
#ifdef _WIN32
#ifndef _DEBUG
  // Hide console if we are not in debug
  HWND console_hwnd = GetConsoleWindow();
  ShowWindow(console_hwnd, SW_HIDE);
  SetWindowLong(console_hwnd, GWL_EXSTYLE,
                GetWindowLong(console_hwnd, GWL_EXSTYLE) | WS_EX_TOOLWINDOW);
#endif
  // Enables Process DPI awareness for windows platform, it's important to do it
  // at the start before every other initialization in order to properly handle
  // DPI changes across different screens
  SetProcessDpiAwareness(PROCESS_PER_MONITOR_DPI_AWARE);
#endif

  // Load all the screens and initialize the windows and graphics apis
  // IMPORTANT: Lot's of code workflows under this one relies on screens apis so
  // this has to be the first thing done
  fetch_and_update_screens_(win_api, gfx_api);

  // Load the host window
  if (win_api == RWindowApi::RWIN32)
    host_window_ = std::make_shared<RWin32Api>();

  host_window_->init(gfx_api, 0, 0, 0, 0, true);

  // Load the configs
  RConfigsManager::get().load_all();

  // Register the custom console commands
  r_register_console_cmds();

  // Main application loop
  host_render_fps_limiter_ = RFrameRateLimiter{RConfigsManager::get()
                                                   .get_app_settings_mngr()
                                                   .get_app_settings()
                                                   .view_fps_limit};

  bg_render_fps_limiter_ = RFrameRateLimiter{240.0f};

  bool is_running = true;
  while (is_running) {
    bool did_render_host = false;
    bool did_render_bg = false;

    // Process messages from windows, process keybinds and check if app should
    // quit
    if (win_api == RWindowApi::RWIN32) {
      is_running = RWin32Api::process_messages();
      r_process_keybinds();
    }

    // Render's host window content (mainly viewports)
    if (host_render_fps_limiter_.should_update()) {
      did_render_host = true;
      host_window_->get_gfx().begin_render();
      {
        // Refresh network stats
        network_adapters_mngr_.update();

        // Render
        RViewPool::get().render();
        RConfigsManager::get().get_desktop_bg_mngr().update();
      }
      host_window_->get_gfx().render();
    }

    // Render's screen content
    if (bg_render_fps_limiter_.should_update()) {
      did_render_bg = true;
      for (auto& screen : screens_) {
        screen->get_bg_window().get_gfx().begin_render();

        RConfigsManager::get().get_desktop_bg_mngr().render(*screen);

        screen->get_bg_window().get_gfx().render();
      }
    }

    if (!did_render_host && !did_render_bg) {
      std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
  }
}

void RApp::fetch_and_update_screens_(RWindowApi win_api,
                                     RGraphicsApiType gfx_api) {
  // Clear and destroy every previous monitor
  screens_.clear();

  // Fill monitor array with screens ready to be initialized
#ifdef _WIN32
  EnumDisplayMonitors(NULL, NULL, MonitorEnumProc,
                      reinterpret_cast<LPARAM>(&screens_));
#endif

  // Set the main screen ptr before initializing every screen
  for (auto& screen : screens_) {
    if (screen->is_primary()) main_screen_ = screen;
  }

  // Init window platform and graphics driver for every screen
  for (auto& screen : screens_) {
    screen->init(win_api, gfx_api);
  }
}
