#pragma once

#include <memory>
#include <vector>

#include "../app/screen.h"
#include "../core/filesystem_utils.h"

/*
 * Represent's a desktop background
 */
struct RDesktopBackground {
  std::shared_ptr<RImage> img;
};

// Handles the interactions with the desktop background.
class RDesktopBackgroundManager {
 public:
  // Get's the config directory where are stored all the background images
  static const r_string get_bg_images_dir_path();

  // Render's a desktop background
  void render(RScreen& screen);

  // Load's all the backgrounds
  bool load();

  // Set the current background
  void set_background(RDesktopBackground* bg) { cur_bg_ = bg; }

  // Load a new background from file, it also makes a copy in the configs to
  // save it
  RResult<RDesktopBackground> load_new_bg_from_file(const r_string& path);

 private:
  // The current background set
  RDesktopBackground* cur_bg_;
  // All the loaded backgrounds
  std::vector<RDesktopBackground> loaded_bgs_;
};
