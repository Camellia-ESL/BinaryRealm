#include "theme.h"

#include <filesystem>
#include <fstream>
#include <future>
#include <memory>

#include "../../external/nhlomannjson/json.hpp"
#include "../app/app.h"
#include "../config/config_manager.h"
#include "../core/filesystem_utils.h"
#include "../core/string.h"

using json = nlohmann::json;

const r_string RThemeManager::get_theme_dir_path() {
  return RConfigsManager::get().get_config_dir_path() + "\\themes";
}
const r_string RThemeManager::get_font_dir_path() {
  return RConfigsManager::get().get_config_dir_path() + "\\fonts";
}

RResult<std::shared_ptr<RTheme>> RThemeManager::create_theme(
    const std::string& name) {
  auto prev_ctx = ImGui::GetCurrentContext();
  ImGui::SetCurrentContext(RApp::get().get_main_imgui_ctx());
  // Create's the theme
  std::shared_ptr<RTheme> new_thm = std::make_shared<RTheme>();
  new_thm->name = name;
  new_thm->imgui_style = ImGui::GetStyle();
  themes_.emplace_back(new_thm);

  // Tries to save the theme manager state in the config
  if (!save()) {
    return RResult<std::shared_ptr<RTheme>>::create_err(
        "Could not save the themes in configs, something went wrong!");
  }

  ImGui::SetCurrentContext(prev_ctx);
  return RResult<std::shared_ptr<RTheme>>::create_ok(new_thm);
}

/*
 * Json Serialization Override for specific types
 */

// Serialize ImVec2
void to_json(json& j, const ImVec2& v) { j = json{{"x", v.x}, {"y", v.y}}; }

void from_json(const json& j, ImVec2& v) {
  v.x = j.value("x", 0.0f);
  v.y = j.value("y", 0.0f);
}

// Serialize ImVec4
void to_json(json& j, const ImVec4& v) {
  j = json{{"r", v.x}, {"g", v.y}, {"b", v.z}, {"a", v.w}};
}

void from_json(const json& j, ImVec4& v) {
  v.x = j.value("r", 0.0f);
  v.y = j.value("g", 0.0f);
  v.z = j.value("b", 0.0f);
  v.w = j.value("a", 0.0f);
}

inline void to_json(json& j, const ImGuiDir& dir) { j = static_cast<int>(dir); }
inline void from_json(const json& j, ImGuiDir& dir) {
  dir = static_cast<ImGuiDir>(j.get<int>());
}

inline void to_json(json& j, const ImGuiHoveredFlags& flags) {
  j = static_cast<int>(flags);
}
inline void from_json(const json& j, ImGuiHoveredFlags& flags) {
  flags = static_cast<ImGuiHoveredFlags>(j.get<int>());
}

// Serialize ImGuiStyle
void to_json(json& j, const ImGuiStyle& style) {
  j = json{
      {"Alpha", style.Alpha},
      {"DisabledAlpha", style.DisabledAlpha},
      {"WindowPadding", style.WindowPadding},
      {"WindowRounding", style.WindowRounding},
      {"WindowBorderSize", style.WindowBorderSize},
      {"WindowBorderHoverPadding", style.WindowBorderHoverPadding},
      {"WindowMinSize", style.WindowMinSize},
      {"WindowTitleAlign", style.WindowTitleAlign},
      {"WindowMenuButtonPosition",
       static_cast<int>(style.WindowMenuButtonPosition)},
      {"ChildRounding", style.ChildRounding},
      {"ChildBorderSize", style.ChildBorderSize},
      {"PopupRounding", style.PopupRounding},
      {"PopupBorderSize", style.PopupBorderSize},
      {"FramePadding", style.FramePadding},
      {"FrameRounding", style.FrameRounding},
      {"FrameBorderSize", style.FrameBorderSize},
      {"ItemSpacing", style.ItemSpacing},
      {"ItemInnerSpacing", style.ItemInnerSpacing},
      {"CellPadding", style.CellPadding},
      {"TouchExtraPadding", style.TouchExtraPadding},
      {"IndentSpacing", style.IndentSpacing},
      {"ColumnsMinSpacing", style.ColumnsMinSpacing},
      {"ScrollbarSize", style.ScrollbarSize},
      {"ScrollbarRounding", style.ScrollbarRounding},
      {"GrabMinSize", style.GrabMinSize},
      {"GrabRounding", style.GrabRounding},
      {"LogSliderDeadzone", style.LogSliderDeadzone},
      {"ImageBorderSize", style.ImageBorderSize},
      {"TabRounding", style.TabRounding},
      {"TabBorderSize", style.TabBorderSize},
      {"TabCloseButtonMinWidthSelected", style.TabCloseButtonMinWidthSelected},
      {"TabCloseButtonMinWidthUnselected",
       style.TabCloseButtonMinWidthUnselected},
      {"TabBarBorderSize", style.TabBarBorderSize},
      {"TabBarOverlineSize", style.TabBarOverlineSize},
      {"TableAngledHeadersAngle", style.TableAngledHeadersAngle},
      {"TableAngledHeadersTextAlign", style.TableAngledHeadersTextAlign},
      {"ColorButtonPosition", static_cast<int>(style.ColorButtonPosition)},
      {"ButtonTextAlign", style.ButtonTextAlign},
      {"SelectableTextAlign", style.SelectableTextAlign},
      {"SeparatorTextBorderSize", style.SeparatorTextBorderSize},
      {"SeparatorTextAlign", style.SeparatorTextAlign},
      {"SeparatorTextPadding", style.SeparatorTextPadding},
      {"DisplayWindowPadding", style.DisplayWindowPadding},
      {"DisplaySafeAreaPadding", style.DisplaySafeAreaPadding},
      {"DockingSeparatorSize", style.DockingSeparatorSize},
      {"MouseCursorScale", style.MouseCursorScale},
      {"AntiAliasedLines", style.AntiAliasedLines},
      {"AntiAliasedLinesUseTex", style.AntiAliasedLinesUseTex},
      {"AntiAliasedFill", style.AntiAliasedFill},
      {"CurveTessellationTol", style.CurveTessellationTol},
      {"CircleTessellationMaxError", style.CircleTessellationMaxError},
      {"GlassBlurBackground", style.GlassBlurBackground},
      {"HoverStationaryDelay", style.HoverStationaryDelay},
      {"HoverDelayShort", style.HoverDelayShort},
      {"HoverDelayNormal", style.HoverDelayNormal},
      {"HoverFlagsForTooltipMouse",
       static_cast<int>(style.HoverFlagsForTooltipMouse)},
      {"HoverFlagsForTooltipNav",
       static_cast<int>(style.HoverFlagsForTooltipNav)}};
  for (int i = 0; i < ImGuiCol_COUNT; ++i) {
    j["Colors"].emplace_back(style.Colors[i]);
  }
}

void from_json(const json& j, ImGuiStyle& style) {
  style.Alpha = j.value("Alpha", style.Alpha);
  style.DisabledAlpha = j.value("DisabledAlpha", style.DisabledAlpha);
  style.WindowPadding = j.value("WindowPadding", style.WindowPadding);
  style.WindowRounding = j.value("WindowRounding", style.WindowRounding);
  style.WindowBorderSize = j.value("WindowBorderSize", style.WindowBorderSize);
  style.WindowBorderHoverPadding =
      j.value("WindowBorderHoverPadding", style.WindowBorderHoverPadding);
  style.WindowMinSize = j.value("WindowMinSize", style.WindowMinSize);
  style.WindowTitleAlign = j.value("WindowTitleAlign", style.WindowTitleAlign);
  style.WindowMenuButtonPosition = static_cast<ImGuiDir>(
      j.value("WindowMenuButtonPosition",
              static_cast<int>(style.WindowMenuButtonPosition)));
  style.ChildRounding = j.value("ChildRounding", style.ChildRounding);
  style.ChildBorderSize = j.value("ChildBorderSize", style.ChildBorderSize);
  style.PopupRounding = j.value("PopupRounding", style.PopupRounding);
  style.PopupBorderSize = j.value("PopupBorderSize", style.PopupBorderSize);
  style.FramePadding = j.value("FramePadding", style.FramePadding);
  style.FrameRounding = j.value("FrameRounding", style.FrameRounding);
  style.FrameBorderSize = j.value("FrameBorderSize", style.FrameBorderSize);
  style.ItemSpacing = j.value("ItemSpacing", style.ItemSpacing);
  style.ItemInnerSpacing = j.value("ItemInnerSpacing", style.ItemInnerSpacing);
  style.CellPadding = j.value("CellPadding", style.CellPadding);
  style.TouchExtraPadding =
      j.value("TouchExtraPadding", style.TouchExtraPadding);
  style.IndentSpacing = j.value("IndentSpacing", style.IndentSpacing);
  style.ColumnsMinSpacing =
      j.value("ColumnsMinSpacing", style.ColumnsMinSpacing);
  style.ScrollbarSize = j.value("ScrollbarSize", style.ScrollbarSize);
  style.ScrollbarRounding =
      j.value("ScrollbarRounding", style.ScrollbarRounding);
  style.GrabMinSize = j.value("GrabMinSize", style.GrabMinSize);
  style.GrabRounding = j.value("GrabRounding", style.GrabRounding);
  style.LogSliderDeadzone =
      j.value("LogSliderDeadzone", style.LogSliderDeadzone);
  style.ImageBorderSize = j.value("ImageBorderSize", style.ImageBorderSize);
  style.TabRounding = j.value("TabRounding", style.TabRounding);
  style.TabBorderSize = j.value("TabBorderSize", style.TabBorderSize);
  style.TabCloseButtonMinWidthSelected = j.value(
      "TabCloseButtonMinWidthSelected", style.TabCloseButtonMinWidthSelected);
  style.TabCloseButtonMinWidthUnselected =
      j.value("TabCloseButtonMinWidthUnselected",
              style.TabCloseButtonMinWidthUnselected);
  style.TabBarBorderSize = j.value("TabBarBorderSize", style.TabBarBorderSize);
  style.TabBarOverlineSize =
      j.value("TabBarOverlineSize", style.TabBarOverlineSize);
  style.TableAngledHeadersAngle =
      j.value("TableAngledHeadersAngle", style.TableAngledHeadersAngle);
  style.TableAngledHeadersTextAlign =
      j.value("TableAngledHeadersTextAlign", style.TableAngledHeadersTextAlign);
  style.ColorButtonPosition = static_cast<ImGuiDir>(j.value(
      "ColorButtonPosition", static_cast<int>(style.ColorButtonPosition)));
  style.ButtonTextAlign = j.value("ButtonTextAlign", style.ButtonTextAlign);
  style.SelectableTextAlign =
      j.value("SelectableTextAlign", style.SelectableTextAlign);
  style.SeparatorTextBorderSize =
      j.value("SeparatorTextBorderSize", style.SeparatorTextBorderSize);
  style.SeparatorTextAlign =
      j.value("SeparatorTextAlign", style.SeparatorTextAlign);
  style.SeparatorTextPadding =
      j.value("SeparatorTextPadding", style.SeparatorTextPadding);
  style.DisplayWindowPadding =
      j.value("DisplayWindowPadding", style.DisplayWindowPadding);
  style.DisplaySafeAreaPadding =
      j.value("DisplaySafeAreaPadding", style.DisplaySafeAreaPadding);
  style.DockingSeparatorSize =
      j.value("DockingSeparatorSize", style.DockingSeparatorSize);
  style.MouseCursorScale = j.value("MouseCursorScale", style.MouseCursorScale);
  style.AntiAliasedLines = j.value("AntiAliasedLines", style.AntiAliasedLines);
  style.AntiAliasedLinesUseTex =
      j.value("AntiAliasedLinesUseTex", style.AntiAliasedLinesUseTex);
  style.AntiAliasedFill = j.value("AntiAliasedFill", style.AntiAliasedFill);
  style.CurveTessellationTol =
      j.value("CurveTessellationTol", style.CurveTessellationTol);
  style.CircleTessellationMaxError =
      j.value("CircleTessellationMaxError", style.CircleTessellationMaxError);
  style.GlassBlurBackground =
      j.value("GlassBlurBackground", style.GlassBlurBackground);
  style.HoverStationaryDelay =
      j.value("HoverStationaryDelay", style.HoverStationaryDelay);
  style.HoverDelayShort = j.value("HoverDelayShort", style.HoverDelayShort);
  style.HoverDelayNormal = j.value("HoverDelayNormal", style.HoverDelayNormal);
  style.HoverFlagsForTooltipMouse = static_cast<ImGuiHoveredFlags>(
      j.value("HoverFlagsForTooltipMouse",
              static_cast<int>(style.HoverFlagsForTooltipMouse)));
  style.HoverFlagsForTooltipNav = static_cast<ImGuiHoveredFlags>(
      j.value("HoverFlagsForTooltipNav",
              static_cast<int>(style.HoverFlagsForTooltipNav)));

  if (j.contains("Colors")) {
    for (size_t i = 0; i < ImGuiCol_COUNT && i < j["Colors"].size(); ++i) {
      style.Colors[i] = j["Colors"][i];
    }
  }
}

// Serialize RTheme
void to_json(json& j, const RTheme& theme) {
  j = json{{"name", theme.name},
           {"imgui_style", theme.imgui_style},
           {"is_default", theme.is_default},
           {"viewport_default_size", theme.viewport_default_size},
           {"default_bg_name", theme.default_bg_name},
           {"font_name", theme.font_name}};
}

void from_json(const json& j, RTheme& theme) {
  theme.name = j.value("name", "");
  theme.imgui_style = j.value("imgui_style", ImGui::GetStyle());
  theme.is_default = j.value("is_default", false);
  theme.viewport_default_size = j.value("viewport_default_size", ImVec2());
  theme.default_bg_name = j.value("default_bg_name", "");
  theme.font_name = j.value("font_name", "");
}

bool RThemeManager::save() {
  // Serialize all themes in json format
  for (auto& theme : themes_) {
    json j_theme = *theme;
    if (!RFilesystemUtils::save_file(
            get_theme_dir_path() + "\\" + theme->name + ".json ",
            j_theme.dump(4)))
      return false;
  }

  return true;
}

bool RThemeManager::load() {
  auto prev_ctx = ImGui::GetCurrentContext();
  ImGui::SetCurrentContext(RApp::get().get_main_imgui_ctx());

  auto dir_read_res = RFilesystemUtils::get_files_in_dir(get_theme_dir_path());
  if (!dir_read_res.ok()) return false;

  // If no themes are present in the dir, it create's a default theme, load and
  // save it
  if (dir_read_res.val().empty()) {
    auto theme_create_res = create_theme("Default");
    if (!theme_create_res.ok()) return false;
    std::shared_ptr<RTheme> p_theme = theme_create_res.val();

    load_default_theme(*p_theme);
    set_active_theme(p_theme);
    save();
    return true;
  }

  // Clear the themes vector before loading
  themes_.clear();

  // Loads themes from the config dir asynchronously
  std::vector<std::future<std::shared_ptr<RTheme>>> futures;

  for (const auto& file_path : dir_read_res.val()) {
    futures.emplace_back(std::async(
        std::launch::async, [file_path]() -> std::shared_ptr<RTheme> {
          auto file_read_res = RFilesystemUtils::load_file(file_path);
          if (!file_read_res.ok()) return nullptr;
          std::ifstream& file = file_read_res.val();

          json j_theme;
          file >> j_theme;
          file.close();

          auto p_theme = std::make_shared<RTheme>(j_theme.get<RTheme>());
          return p_theme;
        }));
  }

  // Loads fonts while loading themes
  load_fonts_();

  // Collect results
  for (auto& fut : futures) {
    auto p_theme = fut.get();
    if (!p_theme) continue;
    themes_.emplace_back(p_theme);
    if (p_theme->is_default) set_active_theme(p_theme);
  }

  // Set back the previous context
  ImGui::SetCurrentContext(prev_ctx);

  return true;
}

void RThemeManager::set_active_theme(std::shared_ptr<RTheme> theme) {
  if (!theme) return;

  // Swap context before theme swapping
  auto prev_ctx = ImGui::GetCurrentContext();
  ImGui::SetCurrentContext(RApp::get().get_main_imgui_ctx());

  // Set the theme as the selected one
  selected_theme_ = theme;
  selected_theme_->is_default = true;

  // Set the imgui style
  ImGui::GetStyle() = selected_theme_->imgui_style;

  // Set the default image background for this theme
  auto& bg_mngr = RConfigsManager::get().get_desktop_bg_mngr();
  for (const auto& bg : bg_mngr.get_loaded_bgs())
    if (std::filesystem::path(bg.img->get_file_path()).filename().string() ==
        theme->default_bg_name)
      bg_mngr.set_background((RDesktopBackground*)&bg);

  // Set the default font for this theme
  for (const auto& font : fonts_)
    if (font->name == theme->font_name)
      ImGui::GetIO().FontDefault = font->p_font;

  // Reset the context back
  ImGui::SetCurrentContext(prev_ctx);
}

void RThemeManager::load_default_theme(RTheme& theme) {
  ImVec4* colors = theme.imgui_style.Colors;
  colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
  colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
  colors[ImGuiCol_WindowBg] = ImVec4(0.10f, 0.10f, 0.10f, 0.2f);
  colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
  colors[ImGuiCol_PopupBg] = ImVec4(0.19f, 0.19f, 0.19f, 0.92f);
  colors[ImGuiCol_Border] = ImVec4(0.19f, 0.19f, 0.19f, 0.29f);
  colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.24f);
  colors[ImGuiCol_FrameBg] = ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
  colors[ImGuiCol_FrameBgHovered] = ImVec4(0.19f, 0.19f, 0.19f, 0.54f);
  colors[ImGuiCol_FrameBgActive] = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
  colors[ImGuiCol_TitleBg] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
  colors[ImGuiCol_TitleBgActive] = ImVec4(0.06f, 0.06f, 0.06f, 1.00f);
  colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
  colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
  colors[ImGuiCol_ScrollbarBg] = ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
  colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.34f, 0.34f, 0.34f, 0.54f);
  colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.40f, 0.40f, 0.40f, 0.54f);
  colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.56f, 0.56f, 0.56f, 0.54f);
  colors[ImGuiCol_CheckMark] = ImVec4(0.33f, 0.67f, 0.86f, 1.00f);
  colors[ImGuiCol_SliderGrab] = ImVec4(0.34f, 0.34f, 0.34f, 0.54f);
  colors[ImGuiCol_SliderGrabActive] = ImVec4(0.56f, 0.56f, 0.56f, 0.54f);
  colors[ImGuiCol_Button] = ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
  colors[ImGuiCol_ButtonHovered] = ImVec4(0.19f, 0.19f, 0.19f, 0.54f);
  colors[ImGuiCol_ButtonActive] = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
  colors[ImGuiCol_Header] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
  colors[ImGuiCol_HeaderHovered] = ImVec4(0.00f, 0.00f, 0.00f, 0.36f);
  colors[ImGuiCol_HeaderActive] = ImVec4(0.20f, 0.22f, 0.23f, 0.33f);
  colors[ImGuiCol_Separator] = ImVec4(0.28f, 0.28f, 0.28f, 0.29f);
  colors[ImGuiCol_SeparatorHovered] = ImVec4(0.44f, 0.44f, 0.44f, 0.29f);
  colors[ImGuiCol_SeparatorActive] = ImVec4(0.40f, 0.44f, 0.47f, 1.00f);
  colors[ImGuiCol_ResizeGrip] = ImVec4(0.28f, 0.28f, 0.28f, 0.29f);
  colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.44f, 0.44f, 0.44f, 0.29f);
  colors[ImGuiCol_ResizeGripActive] = ImVec4(0.40f, 0.44f, 0.47f, 1.00f);
  colors[ImGuiCol_Tab] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
  colors[ImGuiCol_TabHovered] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
  colors[ImGuiCol_TabActive] = ImVec4(0.20f, 0.20f, 0.20f, 0.36f);
  colors[ImGuiCol_TabUnfocused] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
  colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
  colors[ImGuiCol_DockingPreview] = ImVec4(0.33f, 0.67f, 0.86f, 1.00f);
  colors[ImGuiCol_DockingEmptyBg] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
  colors[ImGuiCol_PlotLines] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
  colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
  colors[ImGuiCol_PlotHistogram] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
  colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
  colors[ImGuiCol_TableHeaderBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
  colors[ImGuiCol_TableBorderStrong] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
  colors[ImGuiCol_TableBorderLight] = ImVec4(0.28f, 0.28f, 0.28f, 0.29f);
  colors[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
  colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
  colors[ImGuiCol_TextSelectedBg] = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
  colors[ImGuiCol_DragDropTarget] = ImVec4(0.33f, 0.67f, 0.86f, 1.00f);
  colors[ImGuiCol_NavHighlight] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
  colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 0.00f, 0.00f, 0.70f);
  colors[ImGuiCol_NavWindowingDimBg] = ImVec4(1.00f, 0.00f, 0.00f, 0.20f);
  colors[ImGuiCol_ModalWindowDimBg] = ImVec4(1.00f, 0.00f, 0.00f, 0.35f);

  ImGuiStyle& style = theme.imgui_style;
  style.WindowPadding = ImVec2(8.00f, 8.00f);
  style.FramePadding = ImVec2(5.00f, 2.00f);
  style.CellPadding = ImVec2(6.00f, 6.00f);
  style.ItemSpacing = ImVec2(6.00f, 6.00f);
  style.ItemInnerSpacing = ImVec2(6.00f, 6.00f);
  style.TouchExtraPadding = ImVec2(0.00f, 0.00f);
  style.IndentSpacing = 25;
  style.ScrollbarSize = 15;
  style.GrabMinSize = 10;
  style.WindowBorderSize = 1;
  style.ChildBorderSize = 1;
  style.PopupBorderSize = 1;
  style.FrameBorderSize = 1;
  style.TabBorderSize = 1;
  style.WindowRounding = 7;
  style.ChildRounding = 4;
  style.FrameRounding = 3;
  style.PopupRounding = 4;
  style.ScrollbarRounding = 9;
  style.GrabRounding = 3;
  style.LogSliderDeadzone = 4;
  style.TabRounding = 4;
  style.GlassBlurBackground = true;
}

void RThemeManager::load_fonts_() {
  auto dir_read_res = RFilesystemUtils::get_files_in_dir(get_font_dir_path());
  if (!dir_read_res.ok()) return;

  for (const auto& file_path : dir_read_res.val()) {
    r_string file_path_str = file_path.string();
    ImFont* font_rsc =
        ImGui::GetIO().Fonts->AddFontFromFileTTF(file_path_str.c_str(), 14.0f);
    if (!font_rsc) continue;

    auto font_info_buff = std::make_shared<RFont>();
    font_info_buff->p_font = font_rsc;
    font_info_buff->file_path = file_path_str;
    font_info_buff->name = std::filesystem::path(file_path).filename().string();
    fonts_.push_back(font_info_buff);
  }
}
