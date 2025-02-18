#include "background.h"

#include "../../external/imgui/imgui.h"

void RDesktopBackgroundManager::render() {

  // If there is no background image set simply returns
  if (!cur_bg_)
    return;

  // Draw's the background using ImGui background drawlist
  ImGui::GetBackgroundDrawList()->AddImage(
      (ImTextureID)*cur_bg_.get(), {0.0f, 0.0f}, ImGui::GetIO().DisplaySize);
}
