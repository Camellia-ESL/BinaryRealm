#include "settings_view.h"

#include "../../../external/imgui/imgui.h"
#include "../../config/config_manager.h"
#include "../viewpool.h"

void render_background_settings() {
  auto& desktop_bg_mngr = RConfigsManager::get().get_desktop_bg_mngr();
  auto& loaded_bgs = desktop_bg_mngr.get_loaded_bgs();

  const float img_button_size = 150.0f;

  for (auto& bg : loaded_bgs) {
    if (ImGui::ImageButton(
            r_str_to_cstr("##" +
                          std::to_string((uint64_t)bg.img->get_host_srv())),
            (ImTextureID)bg.img->get_host_srv(),
            {img_button_size, img_button_size})) {
      desktop_bg_mngr.set_background((RDesktopBackground*)&bg);
    }
  }
}

void RSettingsView::render() {
  ImGui::Begin(
      r_str_to_cstr(get_name() + "##" + std::to_string((uint64_t)this)),
      &open_);

  render_background_settings();

  ImGui::End();

  // Destroy's the view when it get's closed
  if (!open_) RViewPool::get().destroy(this);
}
