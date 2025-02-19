#pragma once
#include "../../external/imgui/imgui.h"
#include "../core/containers.h"
#include "../core/string.h"

#include <memory>
#include <vector>

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
  // Create's a new theme and returns it in a result
  RResult<RTheme &> create_theme(const std::string &name);

  // Saves the current state of the theme manager in the theme configs
  bool save();

  // Loads the themes from the theme configs
  bool load();

private:
  std::shared_ptr<RTheme> selected_theme_;
  std::vector<std::shared_ptr<RTheme>> themes_;
};
