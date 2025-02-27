#pragma once

#include <memory>
#include <vector>

#include "../core/filesystem_utils.h"
#include "../graphics/graphics.h"

// Handles the interactions with the desktop background.
class RDesktopBackgroundManager {
 public:
  // Get's the config directory where are stored all the background images
  static const r_string get_bg_images_dir_path();

  // Render's a desktop background
  void render();

  // Load's all the backgrounds
  bool load();

  // Set the current background
  void set_background(RImage img) { cur_bg_ = img; }

  // Load a new background from file, it also makes a copy in the configs to
  // save it
  RResult<RImage> load_new_bg_from_file(const r_string& path);

 private:
  // The current background set
  RImage cur_bg_;
  // All the loaded backgrounds
  std::vector<RImage> loaded_bgs_;
};
