#pragma once

#include "../../config/config_manager.h"
#include "../view.h"

// App settings view
class RSettingsView : public RWindowView {
 public:
  using RWindowView::RWindowView;
  // Render function to be implemented by derived views
  void render() override;
  // Return's settings view name
  constexpr const char* get_name() const override { return "Settings View"; };

 private:
  ImGuiTextFilter thm_colors_filter_;
  ImGuiColorEditFlags thm_alpha_flags_ = 0;
  std::shared_ptr<RTheme> thm_selected_ = nullptr;
  RAnimVal thm_editor_in_anim_val_{
      1.0f, 0.0f, 0.2f, RAnimInterpolationType::CUBIC_BEZIER, 0.84f, 0.34f};

  void render_background_settings_();
  void render_themes_manager_();
  void render_theme_settings_(RTheme& theme);
};
