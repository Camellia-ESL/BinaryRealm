#include "config_manager.h"

void RConfigsManager::load_all() {
  // Load themes
  theme_mngr_.load();
}

void RConfigsManager::save_all() {
  // Save themes
  theme_mngr_.save();
}
