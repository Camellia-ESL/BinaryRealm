#include "theme_selector_view.h"

void RThemeSelectorView::on_spawn() {
  // Overrides default window animation
  opening_anim_val_.duration = 0.45f;

  RWindowView::on_spawn();
  return;
}

void RThemeSelectorView::render() {
  if (!begin_frame_(ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize,
                    {200, 300})) {
    end_frame_();
    return;
  }

  auto& thm_mngr = RConfigsManager::get().get_theme_mngr();
  auto& themes = thm_mngr.get_themes();

  const ImVec2 window_size = ImGui::GetWindowSize();
  const float line_height = ImGui::GetTextLineHeightWithSpacing();
  const float total_height =
      static_cast<float>(thm_mngr.get_themes().size()) * line_height;

  // Set vertical starting position so the list is centered
  const float start_y = (window_size.y - total_height) * 0.5f;
  ImGui::SetCursorPosY(start_y);

  for (const auto& theme : themes) {
    const ImVec2 text_size = ImGui::CalcTextSize(theme->name.c_str());
    const float x_offset = (window_size.x - text_size.x) * 0.5f;
    const bool is_active = thm_mngr.get_active_theme() == theme;

    ImGui::SetCursorPosX(x_offset);

    ImGui::TextColored(is_active ? ImGui::GetStyleColorVec4(ImGuiCol_CheckMark)
                                 : ImGui::GetStyleColorVec4(ImGuiCol_Text),
                       "%s", theme->name.c_str());
  }

  if (ImGui::IsViewportFocused() && ImGui::IsKeyPressed(ImGuiKey_DownArrow)) {
    auto it =
        std::find(themes.begin(), themes.end(), thm_mngr.get_active_theme());
    if (it != themes.end() && (it + 1) != themes.end()) {
      thm_mngr.set_active_theme(*(it + 1));
    }
  }

  if (ImGui::IsViewportFocused() && ImGui::IsKeyPressed(ImGuiKey_UpArrow)) {
    auto it =
        std::find(themes.begin(), themes.end(), thm_mngr.get_active_theme());
    if (it != themes.end() && it != themes.begin()) {
      thm_mngr.set_active_theme(*(it - 1));
    }
  }
  end_frame_();
}
