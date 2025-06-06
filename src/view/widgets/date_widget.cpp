#include "date_widget.h"

#include <utility>

#include "../../../external/font-awesome-icons/font_awesome_icons_6.h"
#include "../../app/app.h"
#include "../../config/config_manager.h"

void RDateWidget::render() {
  const auto& theme = RConfigsManager::get()
                          .get_theme_mngr()
                          .get_active_theme()
                          ->widgets_settings.date_widget;
  if (!theme.enabled) return;

  if (is_platformed) begin_frame_();
  ImGui::PushFont(RApp::get().get_host_win().get_gfx().get_fa_icon_font_6());
  ImGui::TextColored(theme.date_text_color, ICON_FA_CALENDAR_WEEK);
  ImGui::PopFont();
  ImGui::SameLine();
  ImGui::TextColored(theme.date_text_color, r_date_as_str().c_str());
  ImGui::SameLine();
  ImGui::PushFont(RApp::get().get_host_win().get_gfx().get_fa_icon_font_6());
  ImGui::TextColored(theme.time_text_color, ICON_FA_CLOCK);
  ImGui::PopFont();
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

  // TODO: Simple optimization reusing the same vec2
  ImVec2 size_text =
      ImGui::CalcTextSize(r_str_to_cstr(r_date_as_str() + r_time_as_str()));
  size_text.x += (ImGui::GetStyle().ItemSpacing.x * 3.0f);

  ImGui::PushFont(RApp::get().get_host_win().get_gfx().get_fa_icon_font_6());
  ImVec2 size_icons =
      ImGui::CalcTextSize(ICON_FA_CALENDAR_WEEK "" ICON_FA_CLOCK);
  ImGui::PopFont();

  ImVec2 size = {size_text.x + size_icons.x,
                 size_text.y >= size_icons.y ? size_text.y : size_icons.y};
  return std::move(size);
}
