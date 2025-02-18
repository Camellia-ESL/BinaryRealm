#pragma once

#include <filesystem>
#include <iostream>

// A set of utilities to interact with file system
class RFilesystemUtils {
public:
  // Get the config directory path
  static std::filesystem::path get_config_path();
};
