#pragma once
#include "../../external/imgui/imgui.h"
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
   * Called when the view is spawned [MUSTCALLBASEFN]
   */
  virtual void on_spawn() {}

  /*
   * Render function to be implemented by derived views
   */
  virtual void render() = 0;

  /*
   * Update's the view before rendering, return's false if the view was removed
   */
  virtual bool update();

  /*
   * Each view should have a name
   */
  virtual constexpr const char* get_name() const = 0;

  /*
   * Get's the view tag (if one was assigned)
   */
  const r_string& get_tag() const { return tag_; }

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

 private:
  void init();
};

class RWindowView : public RView {
 public:
  using RView::RView;

 protected:
  /*
   * The opening animated value
   */
  RAnimVal opening_anim_val_{
      0.34f, 1.0f, 0.6f, RAnimInterpolationType::CUBIC_BEZIER, 1.56f, 0.84f};

  /*
   * The closing animated value
   */
  RAnimVal closing_anim_val_{
      1.0f, 0.44f, 0.3f, RAnimInterpolationType::CUBIC_BEZIER, 1.36f, 1.14f};

  virtual void on_spawn() override;
  virtual bool update() override;

  /*
   * The opening animation of the view
   */
  virtual void opening_anim();

  /*
   * The opening animation of the view
   */
  virtual void closing_anim();

  /*
   * Begin's a fully managed ImGui window viewport frame, with animations and
   * keybinds managed.
   */
  void begin_frame(ImGuiWindowFlags flags = 0);

  /*
   * End's the fully managed ImGui window viewport frame
   */
  void end_frame();

 private:
  ImVec2 closing_anim_win_start_pos_{};
  ImVec2 closing_anim_win_start_size_{};
};
