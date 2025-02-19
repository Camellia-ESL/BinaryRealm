/*
 * Contains a list of config utilities to save, read and load configs from the
 * user configuration folder.
 *
 * Configs are stored in the config directory (which is %appdata% on Windows for
 * example)
 */

#pragma once

#include "theme.h"

// [SINGLETON] Handles all the configs of the application stored in the
// configuration folder
class RConfigsManager {
public:
  // Get's the instance of the singleton
  static RConfigsManager &get() {
    static RConfigsManager instance{};
    return instance;
  }

  // Load all loadable configs from the configuration folder
  void load_all();

  // Save all the configs in the configuration folder
  void save_all();

private:
  RThemeManager theme_mngr_;
};
