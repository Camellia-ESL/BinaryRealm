#include "background.h"

#include "../../external/imgui/imgui.h"
#include "../app/app.h"

void RDesktopBackgroundManager::render() {
  // If there is no background image set simply returns
  if (!cur_bg_) return;

  auto& io = ImGui::GetIO();
  ImDrawList* draw_list = ImGui::GetBackgroundDrawList();

  const auto& monitors = RApp::get().get_window().get_monitors();

  // Render the monitors, adjusting each one to align
  for (size_t i = 0; i < monitors.size(); ++i) {
    // Calculate the new left and right coordinates based on the previous
    // monitor
    ImVec2 topLeft, bottomRight;

    if (i == 0) {
      // For the first monitor, use the original position
      topLeft =
          ImVec2((float)monitors[i].rect.left, (float)monitors[i].rect.top);
    } else {
      // For subsequent monitors, align the left to the previous monitor's right
      topLeft = ImVec2((float)(monitors[i - 1].rect.right),
                       (float)monitors[i].rect.top);
    }

    bottomRight =
        ImVec2((float)monitors[i].rect.right, (float)monitors[i].rect.bottom);

    draw_list->AddImage((ImTextureID)*cur_bg_.get(), topLeft, bottomRight);
  }
}
