#include "config_manager.h"

#include "../core/filesystem_utils.h"
#include "../view/viewpool.h"
#include "static_configs.h"

const r_string RConfigsManager::get_config_dir_path() {
  return RFilesystemUtils::get_config_path().string() + "\\" +
         RStaticConfigs::APP_NAME;
}

void RConfigsManager::load_all() {
  // TODO: We should load the backgrounds and themes asynchronously to save time
  // (remember that backgrounds needs to finish before themes get applied
  // because they relate to eachother)

  app_settings_mngr_.load();
  desktop_bg_mngr_.load();
  theme_mngr_.load();
}

void RConfigsManager::save_all(bool send_notification) {
  // TODO: Save asynchronously.
  auto& notification = RViewPool::get().spawn<RNotificationView>();
  if (!app_settings_mngr_.save()) {
    notification.title = "Error Saving";
    notification.description = "Something went wrong serializing app settings!";
    return;
  }

  if (!theme_mngr_.save()) {
    notification.title = "Error Saving";
    notification.description = "Something went wrong serializing themes!";
    return;
  }

  // Notifies everything went good
  notification.title = "Settings Saved";
  notification.description = "Changes saved successfully!";

  // Destroy the notification if not wanted
  if (!send_notification) RViewPool::get().destroy(&notification);
}
