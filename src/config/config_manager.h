/*
 * Contains a list of config utilities to save, read and load configs from the
 * user configuration folder.
 *
 * Configs are stored in the config directory (which is %appdata% on Windows for
 * example)
 */

#pragma once

#include "../customization/app_settings.h"
#include "../customization/background.h"
#include "../customization/theme.h"

/*
 * Handles all the configs of the application stored in the
 * configuration folder [SINGLETON]
 */
class RConfigsManager {
 public:
  /*
   * Get's the config dir path
   */
  static const r_string get_config_dir_path();

  /*
   * Get's the instance of the singleton
   */
  static RConfigsManager& get() {
    static RConfigsManager instance{};
    return instance;
  }

  /*
   * Load all loadable configs from the configuration folder
   */
  void load_all();

  /*
   * Save all the configs in the configuration folder
   */
  void save_all(bool send_notification = false);

  /*
   * Get's the manager responsible for themes
   */
  RThemeManager& get_theme_mngr() { return theme_mngr_; }

  /*
   * Get's the manager responsible for desktop background
   */
  RDesktopBackgroundManager& get_desktop_bg_mngr() { return desktop_bg_mngr_; }

  /*
   * Get's the manager responsible for app settings
   */
  RAppSettingsManager& get_app_settings_mngr() { return app_settings_mngr_; }

 private:
  RThemeManager theme_mngr_;
  RDesktopBackgroundManager desktop_bg_mngr_;
  RAppSettingsManager app_settings_mngr_;
};
