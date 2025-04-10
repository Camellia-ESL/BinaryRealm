#pragma once

#include <filesystem>
#include <fstream>
#include <iostream>

#include "containers.h"

/*
 * A set of utilities to interact with file system
 */
class RFilesystemUtils {
 public:
  /*
   * Get the config directory path
   */
  static std::filesystem::path get_config_path();

  /*
   * Ensure that a directory exist and creates it if not, return true if it
   * exist or was created
   */
  static bool ensure_dir_exist(const std::filesystem::path& path);

  /*
   * Save a file with the given content, it also ensure that the directory that
   * should contain the file exist
   */
  static bool save_file(const std::filesystem::path& path,
                        const r_string& content);

  /*
   * Loads a file and return's the ifstream, it also ensure that the directory
   * that should contain the file exist
   */
  static RResult<std::ifstream> load_file(const std::filesystem::path& path);

  /*
   * Copies a file from a dir to another, it also make sure that the dst
   * directory exist
   */
  static bool copy_file(const std::filesystem::path& src_path,
                        const std::filesystem::path& dst_path);

  /*
   * Get's all the files in a directory, it also ensure that the directory
   * exist.
   */
  static RResult<std::vector<std::filesystem::path>> get_files_in_dir(
      const std::filesystem::path& path);

  /*
   * Remove's the extension from a file name
   */
  static r_string rm_filename_extension(const r_string& filename);
};
