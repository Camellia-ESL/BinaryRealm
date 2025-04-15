/*
 * Contains all the JSON Serialization overrides for app settings so that it is
 * easier to serialize settings
 */

#pragma once

#include "../../external/nhlomannjson/json.hpp"
#include "app_settings.h"

using json = nlohmann::json;

inline void to_json(json& j, const RAppSettings& settings) {
  j = json{{"vsync", settings.vsync},
           {"view_fps_limit", settings.view_fps_limit}};
}

inline void from_json(const json& j, RAppSettings& settings) {
  settings.vsync = j.value("vsync", false);
  settings.view_fps_limit = j.value("view_fps_limit", 240.0f);
}
