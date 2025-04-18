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

  friend class RViewPool;
};

/*
 * Represent a simple window view with handled animations and window behaviours
 */
class RWindowView : public RView {
 public:
  using RView::RView;

  virtual void on_spawn() override;
  virtual bool update() override;

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

  /*
   * The opening animation of the view
   */
  virtual void opening_anim_(const ImVec2& size = {0.0f, 0.0f});

  /*
   * The opening animation of the view
   */
  virtual void closing_anim_();

  /*
   * Begin's a fully managed ImGui window viewport frame, with animations and
   * keybinds managed, return's false if the window is in a NON renderable
   * state (rendering in this situation should be avoided to avoid graphical
   * artificats).
   */
  bool begin_frame_(ImGuiWindowFlags flags = 0,
                    const ImVec2& size = {0.0f, 0.0f});

  /*
   * End's the fully managed ImGui window viewport frame
   */
  void end_frame_();

 private:
  ImVec2 closing_anim_win_start_pos_{};
  ImVec2 closing_anim_win_start_size_{};
};

/*
 * Represent a notification appearing on screen, can be used to notify the user
 * about something that happened
 */
class RNotificationView : public RView {
 public:
  /*
   * The notification title
   */
  r_string title;

  /*
   * The notification description (body)
   */
  r_string description;

  using RView::RView;

  virtual void on_spawn() override;
  virtual bool update() override;
  virtual void render() override;
  virtual constexpr const char* get_name() const override {
    return "Notification View";
  };

 protected:
  static constexpr const float VERTICAL_GAP_ = 10.0f;
  static constexpr const float HORIZONTAL_GAP_ = 10.0f;

  /*
   * The opening animated value
   */
  RAnimVal opening_anim_val_{
      0.0f, 1.0f, 0.8f, RAnimInterpolationType::CUBIC_BEZIER, 0.34f, 1.34f};

  /*
   * The closing animated value
   */
  RAnimVal closing_anim_val_{
      1.0f, 0.0f, 0.6f, RAnimInterpolationType::CUBIC_BEZIER, 1.34f, 0.34f};

  /*
   * The opening animation of the view
   */
  virtual void opening_anim_();

  /*
   * The opening animation of the view
   */
  virtual void closing_anim_();

 private:
  ImVec2 closing_anim_win_start_pos_{};
  ImVec2 closing_anim_win_start_size_{};

  void slide_from_right_anim_();
};
