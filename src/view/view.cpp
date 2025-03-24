#include "view.h"

#include "../../external/imgui/imgui.h"
#include "../config/config_manager.h"
#include "../config/keybinds.h"
#include "viewpool.h"


bool RView::update() {
  if (r_is_view_close_keybind_pressed()) open_ = false;

  // Destroy's the view when it get's closed
  if (!open_) {
    RViewPool::get().destroy(this);
    return false;
  }

  // Update the opening animation of the view
  opening_anim_val_.update(ImGui::GetIO().DeltaTime);

  return true;
}

void RView::play_opening_anim() {
  if (opening_anim_val_.is_playing) {
    float scale = opening_anim_val_.val();
    const auto& theme =
        RConfigsManager::get().get_theme_mngr().get_active_theme();

    ImGui::SetNextWindowSize(ImVec2(theme->viewport_default_size.x * scale,
                                    theme->viewport_default_size.y * scale));
    ImGui::SetNextWindowBgAlpha(scale);
  }
}

void RView::init() {
  uuid_ = r_str_uuid();
  opening_anim_val_.play();
}
