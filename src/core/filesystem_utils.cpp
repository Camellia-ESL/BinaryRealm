#include "filesystem_utils.h"

#include <fstream>

#include "containers.h"

std::filesystem::path RFilesystemUtils::get_config_path() {
#ifdef _WIN32
  char* appdata = std::getenv("APPDATA");
  if (appdata) {
    return std::filesystem::path(appdata);
  }
#elif defined(__APPLE__)
  char* home = std::getenv("HOME");
  if (home) {
    return fs::path(home) / "Library/Application Support";
  }
#else  // Linux and other Unix-based systems
  char* home = std::getenv("XDG_CONFIG_HOME");
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

bool RFilesystemUtils::ensure_dir_exist(const std::filesystem::path& path) {
  if (!std::filesystem::exists(path)) {
    if (!std::filesystem::create_directories(path)) {
      return false;
    }
  }
  return true;
}

bool RFilesystemUtils::save_file(const std::filesystem::path& path,
                                 const std::string& content) {
  ensure_dir_exist(std::filesystem::path(path).parent_path().string());
  std::ofstream file(path);
  if (file) {
    file << content;
    return true;
  } else {
    return false;
  }
}

RResult<std::ifstream> RFilesystemUtils::load_file(
    const std::filesystem::path& path) {
  ensure_dir_exist(std::filesystem::path(path).parent_path().string());
  std::ifstream file(path);
  if (file.is_open()) {
    return RResult<std::ifstream>::create_ok(std::move(file));
  }
  return RResult<std::ifstream>::create_err(
      RError{"Error loading the file, something went wrong!"});
}

RResult<std::vector<std::filesystem::path>> RFilesystemUtils::get_files_in_dir(
    const std::filesystem::path& path) {
  ensure_dir_exist(path);
  std::vector<std::filesystem::path> file_paths;

  if (std::filesystem::exists(path) && std::filesystem::is_directory(path)) {
    for (const auto& entry : std::filesystem::directory_iterator(path)) {
      if (std::filesystem::is_regular_file(entry.status())) {
        file_paths.push_back(entry.path());
      }
    }
  } else {
    return RResult<std::vector<std::filesystem::path>>::create_err(
        RError{"Error trying to read a dir that does not exist!"});
  }

  return RResult<std::vector<std::filesystem::path>>::create_ok(
      std::move(file_paths));
}
