#include "settings_view.h"

#include "../../../external/imgui/imgui.h"
#include "../../config/config_manager.h"
#include "../viewpool.h"

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

void RSettingsView::render() {
  ImGui::SetNextWindowSize({400.0f, 250.0f}, ImGuiCond_FirstUseEver);
  ImGui::Begin(
      r_str_to_cstr(get_name() + "##" + std::to_string((uint64_t)this)), &open_,
      ImGuiWindowFlags_NoTitleBar);

  if (ImGui::IsKeyPressed(ImGuiKey_Escape)) open_ = false;

  render_background_settings();

  ImGui::End();

  // Destroy's the view when it get's closed
  if (!open_) RViewPool::get().destroy(this);
}
