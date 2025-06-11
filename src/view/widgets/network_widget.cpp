#include "network_widget.h"

#include <utility>

#include "../../../external/font-awesome-icons/font_awesome_icons_6.h"
#include "../../app/app.h"
#include "../../config/config_manager.h"

RNetAdapter* get_most_active_adapter() {
  const auto& adapters = RApp::get().get_net_adapters_mngr().get_adapters();
  if (adapters.empty()) return nullptr;

  const RNetAdapter* most_active = &adapters[0];
  double max_activity = adapters[0].download_kbps + adapters[0].upload_kpbs;

  for (const auto& adapter : adapters) {
    double activity = adapter.download_kbps + adapter.upload_kpbs;
    if (activity > max_activity) {
      max_activity = activity;
      most_active = &adapter;
    }
  }

  return (RNetAdapter*)most_active;
}

void RNetworkWidget::render() {
  const auto& theme = RConfigsManager::get()
                          .get_theme_mngr()
                          .get_active_theme()
                          ->widgets_settings.network_widget;

  curr_adapter_ = get_most_active_adapter();

  if (!theme.enabled || !curr_adapter_) return;

  if (is_platformed) begin_frame_();
  // Wifi icon
  ImGui::PushFont(RApp::get().get_host_win().get_gfx().get_fa_icon_font_6());
  ImGui::TextColored(theme.adapter_color, ICON_FA_WIFI);
  ImGui::PopFont();

  ImGui::SameLine();

  // Adapter name
  if (!curr_adapter_->network_name.empty())
    ImGui::TextColored(theme.adapter_color,
                       curr_adapter_->network_name.c_str());
  else
    ImGui::TextColored(theme.adapter_color, curr_adapter_->name.c_str());

  ImGui::SameLine();

  // Download Icon
  ImGui::PushFont(RApp::get().get_host_win().get_gfx().get_fa_icon_font_6());
  ImGui::TextColored(theme.download_kbps_color, ICON_FA_DOWNLOAD);
  ImGui::PopFont();

  ImGui::SameLine();

  // Download kbps
  if (curr_adapter_->download_kbps >= 100.0) {
    ImGui::TextColored(theme.download_kbps_color, "%.2f mbps",
                       curr_adapter_->download_kbps / 1000.0);
  } else {
    ImGui::TextColored(theme.download_kbps_color, "%.2f kbps",
                       curr_adapter_->download_kbps);
  }

  ImGui::SameLine();

  // Upload Icon
  ImGui::PushFont(RApp::get().get_host_win().get_gfx().get_fa_icon_font_6());
  ImGui::TextColored(theme.upload_kbps_color, ICON_FA_UPLOAD);
  ImGui::PopFont();

  ImGui::SameLine();

  // Upload kbps
  if (curr_adapter_->upload_kpbs >= 100.0) {
    ImGui::TextColored(theme.upload_kbps_color, "%.2f mbps",
                       curr_adapter_->upload_kpbs / 1000.0);
  } else {
    ImGui::TextColored(theme.upload_kbps_color, "%.2f kbps",
                       curr_adapter_->upload_kpbs);
  }

  if (is_platformed) end_frame_();
}

const ImVec2 RNetworkWidget::get_size() {
  const auto& theme = RConfigsManager::get()
                          .get_theme_mngr()
                          .get_active_theme()
                          ->widgets_settings.date_widget;
  if (!theme.enabled || !curr_adapter_) return ImVec2{0.0f, 0.0f};

  // TODO: Simple optimization reusing the same vec2
  ImVec2 size_text = ImGui::CalcTextSize(
      !curr_adapter_->network_name.empty() ? curr_adapter_->network_name.c_str()
                                           : curr_adapter_->name.c_str());
  size_text.x += (ImGui::CalcTextSize("00.00 kbps").x * 2.0f);
  size_text.x += (ImGui::GetStyle().ItemSpacing.x * 5.0f);

  ImGui::PushFont(RApp::get().get_host_win().get_gfx().get_fa_icon_font_6());
  ImVec2 size_icons = ImGui::CalcTextSize(ICON_FA_WIFI);
  size_icons.x += ImGui::CalcTextSize(ICON_FA_DOWNLOAD).x;
  size_icons.x += ImGui::CalcTextSize(ICON_FA_UPLOAD).x;
  ImGui::PopFont();

  ImVec2 size = {size_text.x + size_icons.x,
                 size_text.y >= size_icons.y ? size_text.y : size_icons.y};
  return std::move(size);
}
