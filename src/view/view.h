#pragma once
#include "../core/string.h"
#include "animation.h"

/*
 * Represent's an abstraction of a basic view, it can be inherited to create
 * custom views that can then be spawned using the viewpool. Everything spawned
 * in the viewpool is later rendered on screen.
 */
class RView {
 public:
  RView() { init(); }
  RView(const r_string& tag) : tag_{tag} { init(); }
  virtual ~RView() = default;
  /*
   * Called when the view is spawned
   */
  virtual void on_spawn() {}

  /*
   * Render function to be implemented by derived views
   */
  virtual void render() = 0;

  /*
   * Each view should have a name
   */
  virtual constexpr const char* get_name() const = 0;

  /*
   * Get's the view tag (if one was assigned)
   */
  const r_string& get_tag() const { return tag_; }

  /*
   * Update's the view before rendering, return's false if the view was removed
   */
  bool update();

 protected:
  /*
   * The view tag
   */
  r_string tag_;

  /*
   * The view unique identifier
   */
  r_string uuid_;

  /*
   * When set to false destroy's the window during the next frame
   */
  bool open_ = true;

  /*
   * The opening animated value
   */
  RAnimVal opening_anim_val_{
      0.0f,
      1.0f,
      0.4f,
      RAnimInterpolationType::EASE_OUT_CUBIC,
  };

  /*
   * Play's the opening animation of the view
   */
  virtual void play_opening_anim();

 private:
  void init();
};
