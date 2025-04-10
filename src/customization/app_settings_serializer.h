/*
 * Contains all the JSON Serialization overrides for app settings so that it is
 * easier to serialize settings
 */

#pragma once

#include "../../external/nhlomannjson/json.hpp"
#include "app_settings.h"

using json = nlohmann::json;

inline void to_json(json& j, const RAppSettings& settings) {
  j = json{{"vsync", settings.vsync}};
}

inline void from_json(const json& j, RAppSettings& settings) {
  settings.vsync = j.value("vsync", false);
}
