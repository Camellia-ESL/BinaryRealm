#include "settings_view.h"

#include "../../../external/imgui/imgui.h"
#include "../../config/config_manager.h"

void render_background_settings();

void RSettingsView::render() {
  begin_frame();

  render_background_settings();

  end_frame();
}

void render_background_settings() {
  auto& desktop_bg_mngr = RConfigsManager::get().get_desktop_bg_mngr();
  auto& loaded_bgs = desktop_bg_mngr.get_loaded_bgs();

  const float img_button_size = 150.0f;

  int imgs_in_line = 0;
  for (auto& bg : loaded_bgs) {
    if (ImGui::ImageButton(
            r_str_to_cstr("##" +
                          std::to_string((uint64_t)bg.img->get_host_srv())),
            (ImTextureID)bg.img->get_host_srv(),
            {img_button_size, img_button_size})) {
      desktop_bg_mngr.set_background((RDesktopBackground*)&bg);
    }

    imgs_in_line++;
    if (ImGui::GetContentRegionAvail().x >
        (img_button_size * (imgs_in_line + 1)))
      ImGui::SameLine();
    else
      imgs_in_line = 0;
  }
}
