#include "app.h"

#include "../../external/imgui/imgui.h"
#include "../graphics/backends/d3d11_api.h"
#include "window_apis/win32_api.h"

void RApp::run(RWindowApi win_api, RGraphicsApi gfx_api) {

  if (win_api == RWindowApi::RWIN32)
    window_ = std::make_shared<RWin32Api>();

  if (gfx_api == RGraphicsApi::D3D11)
    graphics_ = std::make_shared<RD3D11Api>();

  window_->init();
  graphics_->init(window_->get_native_handle());

  while (window_->is_running()) {
    window_->process_messages();
    graphics_->begin_render();

    ImGui::Begin("Overlay");
    ImGui::Text("Hello from a structured app!");
    ImGui::End();

    graphics_->render();
  }
}
