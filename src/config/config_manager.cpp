#include "config_manager.h"

#include "../core/filesystem_utils.h"
#include "static_configs.h"

const r_string RConfigsManager::get_config_dir_path() {
  return RFilesystemUtils::get_config_path().string() + "\\" +
         RStaticConfigs::APP_NAME;
}

void RConfigsManager::load_all() {
  // Load themes
  theme_mngr_.load();
  desktop_bg_mngr_.load();
}

void RConfigsManager::save_all() {
  // Save themes
  theme_mngr_.save();
}
