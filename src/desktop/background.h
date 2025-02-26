#pragma once

#include <memory>

#include "../core/filesystem_utils.h"
#include "../graphics/graphics.h"


// [SINGLETON] Handles the interactions with the desktop background.
class RDesktopBackgroundManager {
 public:
  // Get's the instance of the singleton
  static RDesktopBackgroundManager& get() {
    static RDesktopBackgroundManager instance{};
    return instance;
  }

  // Render's a desktop background
  void render();

  // Set the current background
  void set_background(RImage img) { cur_bg_ = std::make_shared<RImage>(img); }

 private:
  // The current background set
  std::shared_ptr<RImage> cur_bg_;
};
