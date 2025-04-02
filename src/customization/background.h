#pragma once

#include <memory>
#include <vector>

#include "../app/screen.h"
#include "../core/filesystem_utils.h"
#include "../view/animation.h"

/*
 * Represent's a desktop background
 */
struct RDesktopBackground {
  std::shared_ptr<RImage> img;
};

/*
 * Handles the interactions with the desktop background.
 */
class RDesktopBackgroundManager {
 public:
  /*
   * Get's the config directory where are stored all the background images
   */
  static const r_string get_bg_images_dir_path();

  /*
   * Called once per frame
   */
  void update();

  /*
   * Called per screen render, render's a desktop background
   */
  void render(RScreen& screen);

  /*
   * Load's all the backgrounds
   */
  bool load();

  /*
   * Set the current background
   */
  void set_background(RDesktopBackground* bg);

  /*
   * Load a new background from file, it also makes a copy in the configs to
   * save it
   */
  RResult<RDesktopBackground> load_new_bg_from_file(const r_string& path);

  /*
   * Return's the current active background or nullptr if none are active
   */
  const RDesktopBackground* get_current_bg() const { return cur_bg_; }

  /*
   * Return's the loaded backgrounds
   */
  const std::vector<RDesktopBackground>& get_loaded_bgs() const {
    return loaded_bgs_;
  }

 private:
  /*
   * The current background set
   */
  RDesktopBackground* cur_bg_ = nullptr;

  /*
   * The previously selected background
   */
  RDesktopBackground* prev_bg_ = nullptr;

  /*
   * All the loaded backgrounds
   */
  std::vector<RDesktopBackground> loaded_bgs_;

  /*
   * The background change animated value, first rotation
   */
  RAnimVal bg_change_anim_val_rot_1_{
      0.34f, 1.0f, 0.2f, RAnimInterpolationType::CUBIC_BEZIER, 0.54f, 0.84f};

  /*
   * The background change animated value, second rotation
   */
  RAnimVal bg_change_anim_val_rot_2_{
      0.0f, 1.0f, 0.2f, RAnimInterpolationType::CUBIC_BEZIER, 0.54f, 0.84f};
};
