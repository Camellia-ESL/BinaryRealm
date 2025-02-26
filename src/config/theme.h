#pragma once
#include <memory>
#include <vector>

#include "../../external/imgui/imgui.h"
#include "../../external/nhlomannjson/json.hpp"
#include "../core/containers.h"
#include "../core/string.h"

// Represent's an application theme
struct RTheme {
  // The name of the theme
  r_string name;
  // The color style of imgui
  ImGuiStyle imgui_style;
};

// Handle's application themes
class RThemeManager {
 public:
  // Get's the themes dir path
  static const r_string get_theme_dir_path();

  // Create's a new theme, save's it in a file and returns it in a result
  // NOTE: This can only be called between an initialized ImGui instance
  RResult<std::shared_ptr<RTheme>> create_theme(const std::string& name);

  // Saves the current state of the theme manager in the theme configs
  // NOTE: This can only be called between an initialized ImGui instance
  bool save();

  // Loads the themes from the theme configs
  // NOTE: This can only be called between an initialized ImGui instance
  bool load();

  // Load the default theme in a theme
  // NOTE: This can only be called between an initialized ImGui instance
  void load_default_theme(RTheme& theme);

 private:
  std::shared_ptr<RTheme> selected_theme_;
  std::vector<std::shared_ptr<RTheme>> themes_;
};
