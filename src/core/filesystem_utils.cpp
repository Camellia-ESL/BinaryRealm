#include "filesystem_utils.h"

std::filesystem::path RFilesystemUtils::get_config_path() {
#ifdef _WIN32
  char *appdata = std::getenv("APPDATA");
  if (appdata) {
    return std::filesystem::path(appdata);
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

bool ensure_dir_exist(const std::string &path) {
  if (!std::filesystem::exists(path)) {
    if (!std::filesystem::create_directories(path)) {
      return false;
    }
  }
  return true;
}

bool save_file(const std::string &path, const std::string &content) {
  ensure_dir_exist(std::filesystem::path(path).parent_path().string());
  std::ofstream file(path);
  if (file) {
    file << content;
    return true;
  } else {
    return false;
  }
}
