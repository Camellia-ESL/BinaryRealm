#include "theme.h"

#include <filesystem>
#include <fstream>
#include <future>
#include <memory>

#include "../app/app.h"
#include "../config/config_manager.h"
#include "../core/filesystem_utils.h"
#include "../core/string.h"
#include "theme_serializer.h"

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
  new_thm->uuid = r_str_uuid();
  new_thm->imgui_style = ImGui::GetStyle();
  themes_.emplace_back(new_thm);

  // Tries to save the theme manager state in the config
  if (!save()) {
    themes_.pop_back();
    return RResult<std::shared_ptr<RTheme>>::create_err(
        "Could not save the themes in configs, something went wrong!");
  }

  ImGui::SetCurrentContext(prev_ctx);
  return RResult<std::shared_ptr<RTheme>>::create_ok(new_thm);
}

bool RThemeManager::delete_theme(std::shared_ptr<RTheme> theme) {
  if (selected_theme_ == theme) return false;

  if (std::erase_if(themes_, [&](const std::shared_ptr<RTheme>& obj) {
        if (obj != theme) return false;
        return std::filesystem::remove(get_theme_dir_path() + "\\" + obj->uuid +
                                       ".json ");
      }) > 0)
    return true;

  return false;
}

bool RThemeManager::save() {
  // Serialize all themes
  // TODO: Could be optimizied with async serialization
  for (auto& theme : themes_)
    if (!save_theme(*theme)) return false;

  return true;
}

bool RThemeManager::save_theme(RTheme& theme) {
  // Serialize the theme in json format
  json j_theme = theme;
  return RFilesystemUtils::save_file(
      get_theme_dir_path() + "\\" + theme.uuid + ".json ", j_theme.dump(4));
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
    p_theme->is_default = true;
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
