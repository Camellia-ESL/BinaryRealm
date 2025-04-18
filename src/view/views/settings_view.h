#pragma once

#include "../../config/config_manager.h"
#include "../view.h"

// App settings view
class RSettingsView : public RWindowView {
 public:
  using RWindowView::RWindowView;
  void render() override;
  constexpr const char* get_name() const override { return "Settings View"; };

 private:
  static constexpr const float BOTTOM_GAP_ = 30.0f;

  ImGuiTextFilter thm_colors_filter_;
  ImGuiTextFilter themes_list_filter_;
  ImGuiColorEditFlags thm_alpha_flags_ = 0;
  std::shared_ptr<RTheme> thm_selected_ = nullptr;
  RAnimVal thm_editor_in_anim_val_{
      1.0f, 0.0f, 0.4f, RAnimInterpolationType::CUBIC_BEZIER, 0.84f, 0.34f};

  void render_general_settings_();
  void render_background_settings_();
  void render_themes_manager_();
  void render_theme_settings_(RTheme& theme);
  void render_theme_settings_generic_tab_(RTheme& theme);
  void render_theme_settings_colors_tab_(RTheme& theme);
  void render_theme_settings_rendering_tab_(RTheme& theme);
};
