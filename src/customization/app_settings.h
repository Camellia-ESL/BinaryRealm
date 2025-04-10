#pragma once

#include "../core/string.h"

struct RAppSettings {
  /*
   * Wheter if the app VSync is on
   */
  bool vsync = true;
};

/*
 * Handle's application settings
 */
class RAppSettingsManager {
 public:
  static constexpr const char* APP_SETTINGS_FILE_NAME = "app_settings.json";

  /*
   * Get's the settings dir path
   */
  static const r_string get_settings_dir_path();

  /*
   * Save the current app settings
   */
  bool save();

  /*
   * Load app settings from file
   */
  bool load();

  /*
   * Get's the application settings loaded
   */
  RAppSettings& get_app_settings() { return app_settings_; }

 private:
  RAppSettings app_settings_{};
};
