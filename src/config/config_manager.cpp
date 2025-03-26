#include "config_manager.h"

#include "../core/filesystem_utils.h"
#include "static_configs.h"

const r_string RConfigsManager::get_config_dir_path() {
  return RFilesystemUtils::get_config_path().string() + "\\" +
         RStaticConfigs::APP_NAME;
}

void RConfigsManager::load_all() {
  // TODO: We should load the backgrounds and themes asynchronously to save time
  // (remember that backgrounds needs to finish before themes get applied
  // because they relate to eachother)

  // Load themes
  desktop_bg_mngr_.load();
  theme_mngr_.load();
}

void RConfigsManager::save_all() {
  // Save themes
  theme_mngr_.save();
}
