/*
 *The main header containing themes logic
 *
 *  NOTE: Themes are loaded only for the first host viewport instance as it is
 * the host used as platform for the viewports (basically every window
 * rendered / imgui component except for few exceptions like the desktop
 * background manager)
 */
#pragma once
#include <memory>
#include <vector>

#include "../../external/imgui/imgui.h"
#include "../../external/nhlomannjson/json.hpp"
#include "../core/containers.h"
#include "../core/string.h"
#include "font.h"

/*
 * Represent's an application theme
 */
struct RTheme {
  /*
   * The name of the theme
   */
  r_string name;
  /*
   * The unique identifier of the theme
   */
  r_string uuid;
  /*
   * The internal style of imgui
   */
  ImGuiStyle imgui_style;
  /*
   * Whether if the theme is set to be the default one
   */
  bool is_default = false;
  /*
   * Viewports default size when opened
   */
  ImVec2 viewport_default_size = {800, 600};
  /*
   * Notifications size when opened
   */
  ImVec2 notification_size = {250, 80};
  /*
   * Default background name
   */
  r_string default_bg_name;
  /*
   * The active font name
   */
  r_string font_name;
};

/*
 * Handle's application themes
 */
class RThemeManager {
 public:
  /*
   * Get's the themes dir path
   */
  static const r_string get_theme_dir_path();

  /*
   * Get's the font dir path
   */
  static const r_string get_font_dir_path();

  /*
   * Create's a new theme, save's it in a file and returns it in a result
   *  NOTE: This can only be called between an initialized ImGui instance
   */
  RResult<std::shared_ptr<RTheme>> create_theme(const std::string& name);

  /*
   * Delete's a theme, returns false if the theme is the active theme or
   * something went wrong
   */
  bool delete_theme(std::shared_ptr<RTheme> theme);

  /*
   * Saves the current state of the theme manager in the theme configs
   *  NOTE: This can only be called between an initialized ImGui instance
   */
  bool save();

  /*
   * Saves the current state of the theme manager in the theme configs
   *  NOTE: This can only be called between an initialized ImGui instance
   */
  bool save_theme(RTheme& theme);

  /*
   * Loads the themes from the theme configs
   *  NOTE: This can only be called between an initialized ImGui instance
   */
  bool load();

  /*
   * Set the active theme
   *  NOTE: This can only be called between an initialized ImGui instance
   */
  void set_active_theme(std::shared_ptr<RTheme> theme);

  /*
   * Get's the current active theme or nullptr if there isn't one
   */
  std::shared_ptr<RTheme> get_active_theme() const { return selected_theme_; }

  /*
   * Get's the current available themes
   */
  const std::vector<std::shared_ptr<RTheme>>& get_themes() const {
    return themes_;
  }

  /*
   * Get's the current available fonts
   */
  const std::vector<std::shared_ptr<RFont>>& get_fonts() const {
    return fonts_;
  }

 private:
  std::shared_ptr<RTheme> selected_theme_;
  std::vector<std::shared_ptr<RTheme>> themes_;
  std::vector<std::shared_ptr<RFont>> fonts_;

  /*
   * Load fonts into imgui font atlas
   */
  void load_fonts_();
};
