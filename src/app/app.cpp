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
  auto monitors = reinterpret_cast<std::vector<RScreenRect>*>(dwData);

  RScreenRect rect{*(RScreenRect*)lprcMonitor};
  monitors->emplace_back(rect);

  return TRUE;
}

#endif

std::vector<RScreenRect> fetch_screen_rects();

void RApp::run(RWindowApi win_api, RGraphicsApiType gfx_api) {
// Enables Process DPI awareness for windows platform, it's important to do it
// at the start before every other initialization in order to properly handle
// DPI changes across different screens
#ifdef _WIN32
  SetProcessDpiAwareness(PROCESS_PER_MONITOR_DPI_AWARE);
#endif

  // Load the host window
  if (win_api == RWindowApi::RWIN32)
    host_window_ = std::make_shared<RWin32Api>();

  host_window_->init(gfx_api, 0, 0, 0, 0, true);

  // Load all the screens and initialize the windows and graphics apis
  auto screen_rects = fetch_screen_rects();
  for (int i = 0; i < screen_rects.size(); i++) {
    auto screen =
        screens_.emplace_back(std::make_shared<RScreen>(screen_rects[i], i));
    screen->init(win_api, gfx_api);
  }

  // Load the configs
  RConfigsManager::get().load_all();

  // Register the custom console commands
  r_register_console_cmds();

  // Main application loop
  bool is_running = true;
  while (is_running) {
    // Process messages from windows, process keybinds and check if app should
    // quit
    if (win_api == RWindowApi::RWIN32) {
      is_running = RWin32Api::process_messages();
      r_process_keybinds();
    }

    // Render's host window content (mainly viewports)
    host_window_->get_gfx().begin_render();
    {
      RViewPool::get().render();
      RConfigsManager::get().get_desktop_bg_mngr().update();
    }
    host_window_->get_gfx().render();

    // Render's screen content
    for (auto& screen : screens_) {
      screen->get_bg_window().get_gfx().begin_render();

      RConfigsManager::get().get_desktop_bg_mngr().render(*screen);

      screen->get_bg_window().get_gfx().render();
    }
  }
}

std::vector<RScreenRect> fetch_screen_rects() {
  std::vector<RScreenRect> screen_rects;
#ifdef _WIN32
  EnumDisplayMonitors(NULL, NULL, MonitorEnumProc,
                      reinterpret_cast<LPARAM>(&screen_rects));
#endif
  return screen_rects;
}
