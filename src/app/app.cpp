#include "app.h"

#include "../desktop/background.h"
#include "../graphics/backends/d3d11_api.h"
#include "window_apis/win32_api.h"

void RApp::run(RWindowApi win_api, RGraphicsApi gfx_api) {

  if (win_api == RWindowApi::RWIN32)
    window_ = std::make_shared<RWin32Api>();

  if (gfx_api == RGraphicsApi::D3D11)
    graphics_ = std::make_shared<RD3D11Api>();

  // Init window system and graphics api
  window_->init();
  graphics_->init(window_->get_native_handle());

  while (window_->is_running()) {
    window_->process_messages();
    graphics_->begin_render();

    RDesktopBackgroundManager::get().render();

    graphics_->render();
  }
}
