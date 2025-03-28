#pragma once

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
  ImGuiTextFilter thm_opts_filter_;
  ImGuiColorEditFlags thm_alpha_flags_ = 0;

  void render_background_settings_();
  void render_theme_settings_();
};
