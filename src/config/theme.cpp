#include "theme.h"

#include <fstream>
#include <memory>

#include "../../external/nhlomannjson/json.hpp"
#include "../core/filesystem_utils.h"
#include "../core/string.h"
#include "config_manager.h"

using json = nlohmann::json;

const r_string RThemeManager::get_theme_dir_path() {
  return RConfigsManager::get().get_config_dir_path() + "\\themes";
}

RResult<std::shared_ptr<RTheme>> RThemeManager::create_theme(
    const std::string& name) {
  // Create's the theme
  std::shared_ptr<RTheme> new_thm = std::make_shared<RTheme>();
  new_thm->name = name;
  new_thm->imgui_style = ImGui::GetStyle();
  themes_.emplace_back(new_thm);

  // Tries to save the theme manager state in the config
  if (!save()) {
    return RResult<std::shared_ptr<RTheme>>::create_err(
        RError{"Could not save the themes in configs, something went wrong!"});
  }

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

// Serialize ImGuiStyle
void to_json(json& j, const ImGuiStyle& style) {
  j = json{
      {"WindowMinSize", style.WindowMinSize},
      {"FramePadding", style.FramePadding},
      {"ItemSpacing", style.ItemSpacing},
      {"ItemInnerSpacing", style.ItemInnerSpacing},
      {"Alpha", style.Alpha},
      {"WindowRounding", style.WindowRounding},
      {"FrameRounding", style.FrameRounding},
      {"IndentSpacing", style.IndentSpacing},
      {"ColumnsMinSpacing", style.ColumnsMinSpacing},
      {"GrabMinSize", style.GrabMinSize},
      {"GrabRounding", style.GrabRounding},
      {"ScrollbarSize", style.ScrollbarSize},
      {"ScrollbarRounding", style.ScrollbarRounding},
  };
  for (int i = 0; i < ImGuiCol_COUNT; ++i) {
    j["Colors"].push_back(style.Colors[i]);
  }
}

void from_json(const json& j, ImGuiStyle& style) {
  style.WindowMinSize = j.value("WindowMinSize", ImVec2());
  style.FramePadding = j.value("FramePadding", ImVec2());
  style.ItemSpacing = j.value("ItemSpacing", ImVec2());
  style.ItemInnerSpacing = j.value("ItemInnerSpacing", ImVec2());
  style.Alpha = j.value("Alpha", 1.0f);
  style.WindowRounding = j.value("WindowRounding", 0.0f);
  style.FrameRounding = j.value("FrameRounding", 0.0f);
  style.IndentSpacing = j.value("IndentSpacing", 0.0f);
  style.ColumnsMinSpacing = j.value("ColumnsMinSpacing", 0.0f);
  style.GrabMinSize = j.value("GrabMinSize", 0.0f);
  style.GrabRounding = j.value("GrabRounding", 0.0f);
  style.ScrollbarSize = j.value("ScrollbarSize", 0.0f);
  style.ScrollbarRounding = j.value("ScrollbarRounding", 0.0f);

  if (j.contains("Colors")) {
    for (size_t i = 0; i < ImGuiCol_COUNT && i < j["Colors"].size(); ++i) {
      style.Colors[i] = j["Colors"][i];
    }
  }
}

// Serialize RTheme
void to_json(json& j, const RTheme& theme) {
  j = json{{"name", theme.name}, {"imgui_style", theme.imgui_style}};
}

void from_json(const json& j, RTheme& theme) {
  theme.name = j.value("name", "");
  theme.imgui_style = j.value("imgui_style", ImGui::GetStyle());
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
  auto dir_read_res = RFilesystemUtils::get_files_in_dir(get_theme_dir_path());
  if (!dir_read_res.ok()) return false;

  // If no themes are present in the dir, it create's a default theme, load and
  // save it
  if (dir_read_res.val().empty()) {
    auto theme_create_res = create_theme("Default");
    if (!theme_create_res.ok()) return false;

    load_default_theme(*theme_create_res.val());
    save();
  }

  for (const auto& file_path : dir_read_res.val()) {
    auto file_read_res = RFilesystemUtils::load_file(file_path);
    if (!file_read_res.ok()) return false;
    std::ifstream& file = file_read_res.val();

    json j_theme;
    file >> j_theme;
    std::shared_ptr<RTheme> p_theme = std::make_shared<RTheme>();
    *p_theme = j_theme.get<RTheme>();
    themes_.emplace_back(p_theme);
    file.close();
  }
  return true;
}

void RThemeManager::load_default_theme(RTheme& theme) {
  ImVec4* colors = theme.imgui_style.Colors;
  colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
  colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
  colors[ImGuiCol_WindowBg] = ImVec4(0.10f, 0.10f, 0.10f, 0.30f);
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
  ImGui::GetStyle() = theme.imgui_style;
}
