/*
 * Contains all the JSON Serialization overrides for themes so that it is easier
 * to serialize themes
 */

#pragma once

#include "../../external/nhlomannjson/json.hpp"
#include "generic_types_serializer.h"
#include "theme.h"
#include "widgets_settings_serializer.h"

using json = nlohmann::json;

inline void to_json(json& j, const RTheme& theme) {
  j = json{{"name", theme.name},
           {"uuid", theme.uuid},
           {"imgui_style", theme.imgui_style},
           {"is_default", theme.is_default},
           {"is_taskbar_enabled", theme.is_taskbar_enabled},
           {"viewport_default_size", theme.viewport_default_size},
           {"notification_size", theme.notification_size},
           {"default_bg_name", theme.default_bg_name},
           {"font_name", theme.font_name},
           {"widgets_settings", theme.widgets_settings}};
}

inline void from_json(const json& j, RTheme& theme) {
  theme.name = j.value("name", "");
  theme.uuid = j.value("uuid", "");
  theme.imgui_style = j.value("imgui_style", ImGui::GetStyle());
  theme.is_default = j.value("is_default", false);
  theme.is_taskbar_enabled = j.value("is_taskbar_enabled", true);
  theme.viewport_default_size =
      j.value("viewport_default_size", ImVec2(800, 600));
  theme.notification_size = j.value("notification_size", ImVec2(250, 120));
  theme.default_bg_name = j.value("default_bg_name", "");
  theme.font_name = j.value("font_name", "");
  theme.widgets_settings = j.value("widgets_settings", RWidgetsSettings{});
}
