#include "app.h"

#include "../config/config_manager.h"
#include "../config/keybinds.h"
#include "../customization/background.h"
#include "../graphics/backends/d3d11_api.h"
#include "../view/viewpool.h"
#include "../view/views/console_view.h"
#include "window_apis/win32_api.h"

void RApp::run(RWindowApi win_api, RGraphicsApi gfx_api) {
  if (win_api == RWindowApi::RWIN32) window_ = std::make_shared<RWin32Api>();

  if (gfx_api == RGraphicsApi::D3D11) graphics_ = std::make_shared<RD3D11Api>();

  // Init window system and graphics api
  window_->init();
  graphics_->init(window_->get_native_handle());

  // Load the configs
  RConfigsManager::get().load_all();

  // Register the custom console commands
  r_register_console_cmds();

  while (window_->is_running()) {
    window_->process_messages();
    graphics_->begin_render();

    if (win_api == RWindowApi::RWIN32) r_process_keybinds_win32();

    RConfigsManager::get().get_desktop_bg_mngr().render();

    RViewPool::get().render();

    graphics_->render();
  }
}
