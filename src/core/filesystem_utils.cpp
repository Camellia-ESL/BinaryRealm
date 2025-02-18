#include "filesystem_utils.h"

std::filesystem::path RFilesystemUtils::get_config_path() {
#ifdef _WIN32
  char *appData = std::getenv("APPDATA");
  if (appData) {
    return std::filesystem::path(appData);
  }
#elif defined(__APPLE__)
  char *home = std::getenv("HOME");
  if (home) {
    return fs::path(home) / "Library/Application Support";
  }
#else // Linux and other Unix-based systems
  char *home = std::getenv("XDG_CONFIG_HOME");
  if (home) {
    return fs::path(home);
  }
  home = std::getenv("HOME");
  if (home) {
    return fs::path(home) / ".config";
  }
#endif
  throw std::runtime_error("Could not determine app data path");
}
