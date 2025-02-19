#pragma once

#include <filesystem>
#include <fstream>
#include <iostream>

// A set of utilities to interact with file system
class RFilesystemUtils {
public:
  // Get the config directory path
  static std::filesystem::path get_config_path();
  // Ensure that a directory exist and creates it if not, return true if it
  // exist or was created
  static bool ensure_dir_exist(const std::string &path);
  // Save a file with the given content, it also ensure that the directory that
  // should contain the file exist
  static bool save_file(const std::string &path, const std::string &content);
};
