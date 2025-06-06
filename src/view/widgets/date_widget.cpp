#include "date_widget.h"

#include <utility>

#include "../../config/config_manager.h"

void RDateWidget::render() {
  const auto& theme = RConfigsManager::get()
                          .get_theme_mngr()
                          .get_active_theme()
                          ->widgets_settings.date_widget;
  if (!theme.enabled) return;

  if (is_platformed) begin_frame_();
  ImGui::TextColored(theme.date_text_color, r_date_as_str().c_str());
  ImGui::SameLine();
  ImGui::TextUnformatted("-");
  ImGui::SameLine();
  ImGui::TextColored(theme.time_text_color, r_time_as_str().c_str());
  if (is_platformed) end_frame_();
}

const ImVec2 RDateWidget::get_size() {
  const auto& theme = RConfigsManager::get()
                          .get_theme_mngr()
                          .get_active_theme()
                          ->widgets_settings.date_widget;
  if (!theme.enabled) return ImVec2{0.0f, 0.0f};

  ImVec2 size = ImGui::CalcTextSize(
      r_str_to_cstr(r_date_as_str() + "-" + r_time_as_str()));
  size.x += (ImGui::GetStyle().ItemSpacing.x * 2.0f);
  return std::move(size);
}
