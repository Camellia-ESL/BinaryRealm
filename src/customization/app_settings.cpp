#include "app_settings.h"

#include "../config/config_manager.h"
#include "app_settings_serializer.h"

const r_string RAppSettingsManager::get_settings_dir_path() {
  return RConfigsManager::get().get_config_dir_path() + "\\settings";
}

bool RAppSettingsManager::save() {
  // Serialize the theme in json format
  json j_settings = app_settings_;
  return RFilesystemUtils::save_file(
      get_settings_dir_path() + "\\" + APP_SETTINGS_FILE_NAME,
      j_settings.dump(4));
}

bool RAppSettingsManager::load() {
  auto file_read_res = RFilesystemUtils::load_file(
      get_settings_dir_path() + "\\" + APP_SETTINGS_FILE_NAME);

  if (!file_read_res.ok()) {
    save();
    return false;
  };
  std::ifstream& file = file_read_res.val();

  json j_settings;
  file >> j_settings;
  file.close();
  app_settings_ = j_settings.get<RAppSettings>();
  return true;
}
