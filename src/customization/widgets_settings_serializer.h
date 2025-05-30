/*
 * Widget settings are saved PER theme, so every theme has its unique widget
 * settings.
 */

#pragma once

#include "../../external/nhlomannjson/json.hpp"
#include "generic_types_serializer.h"

using json = nlohmann::json;

/*
 * A widget possible alignments
 */
enum class RWidgetAlignment { LEFT, CENTER, RIGHT };

inline void to_json(json& j, const RWidgetAlignment& alignment) {
  j = static_cast<int>(alignment);
}

inline void from_json(const json& j, RWidgetAlignment& alignment) {
  alignment = static_cast<RWidgetAlignment>(j.get<int>());
}

/*
 * Represent a single widget settings
 */
struct RWidgetSettings {
  /*
   * Where to align this widget
   */
  RWidgetAlignment alignment = RWidgetAlignment::RIGHT;
  /*
   * The alignment priority
   */
  int alignment_priority = 0;
  /*
   * Whether the widget is enabled or not
   */
  bool enabled = true;
};

/*
 * Contains all the settings about a date widget
 */
struct RDateWidgetSettings : public RWidgetSettings {
  /*
   * The color of the date text
   */
  ImVec4 date_text_color = {1.0f, 1.0f, 1.0f, 1.0f};
  /*
   * The color of the time text
   */
  ImVec4 time_text_color = {1.0f, 1.0f, 1.0f, 1.0f};
};

inline void to_json(json& j, const RDateWidgetSettings& settings) {
  j = json{{"date_text_color", settings.date_text_color},
           {"alignment", settings.alignment},
           {"alignment_priority", settings.alignment_priority},
           {"time_text_color", settings.time_text_color}};
}

inline void from_json(const json& j, RDateWidgetSettings& settings) {
  settings.date_text_color =
      j.value("date_text_color", ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
  settings.time_text_color =
      j.value("time_text_color", ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
  settings.alignment = j.value("alignment", RWidgetAlignment::RIGHT);
  settings.alignment_priority = j.value("alignment_priority", 0);
}

/*
 * Contains all the settings about every widget
 */
struct RWidgetsSettings {
  RDateWidgetSettings date_widget{};
};

inline void to_json(json& j, const RWidgetsSettings& settings) {
  j = json{{"date_widget", settings.date_widget}};
}

inline void from_json(const json& j, RWidgetsSettings& settings) {
  settings.date_widget = j.value("date_widget", RDateWidgetSettings{});
}
