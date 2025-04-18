#include "settings_view.h"

#include "../../../external/imgui/imgui.h"
#include "../../app/app.h"
#include "../../core/filesystem_utils.h"

void RSettingsView::render() {
  if (!begin_frame_()) {
    end_frame_();
    return;
  }

  // Update's animation values
  thm_editor_in_anim_val_.update();

  ImGui::Text("Settings");

  ImGui::PushStyleColor(ImGuiCol_TabSelected, {0.0f, 0.0f, 0.0f, 0.0f});
  ImGui::BeginTabBar("##settings_tab", ImGuiTabBarFlags_DrawSelectedOverline);
  ImGui::PopStyleColor();

  if (ImGui::BeginTabItem("General")) {
    render_general_settings_();
    ImGui::EndTabItem();
  }

  if (ImGui::BeginTabItem("Themes")) {
    if (thm_selected_)
      render_theme_settings_(*thm_selected_);
    else
      render_themes_manager_();
    ImGui::EndTabItem();
  }

  ImGui::EndTabBar();

  ImGui::SetCursorPos(
      {ImGui::GetWindowWidth() - 50.0f, ImGui::GetWindowHeight() - 30.0f});
  if (ImGui::Button("Save")) {
    RConfigsManager::get().save_all(true);
  }

  end_frame_();
}

void RSettingsView::render_general_settings_() {
  auto& app_settings_mngr = RConfigsManager::get().get_app_settings_mngr();
  auto& app_settings = app_settings_mngr.get_app_settings();
  ImGui::ToggleButton("Vsync", &app_settings.vsync);

  ImGui::PushItemWidth(ImGui::GetFontSize() * 8);
  ImGui::DragFloat("View FPS Limit", &app_settings.view_fps_limit, 1.0f, 1.0f,
                   520.0f, "%.0f");
  RApp::get().get_host_fps_limiter().set_framerate(app_settings.view_fps_limit);
  ImGui::PopItemWidth();
}

void RSettingsView::render_background_settings_() {
  auto& desktop_bg_mngr = RConfigsManager::get().get_desktop_bg_mngr();
  auto& loaded_bgs = desktop_bg_mngr.get_loaded_bgs();

  const float img_button_size = 100.0f;

  int imgs_in_line = 0;
  for (auto& bg : loaded_bgs) {
    ImGui::PushStyleColor(ImGuiCol_Button,
                          desktop_bg_mngr.get_current_bg() == &bg
                              ? ImGui::GetStyleColorVec4(ImGuiCol_CheckMark)
                              : ImVec4{0.0f, 0.0f, 0.0f, 0.0f});
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered,
                          ImGui::GetStyleColorVec4(ImGuiCol_CheckMark));
    if (ImGui::ImageButton(
            r_str_to_cstr("##" +
                          std::to_string((uint64_t)bg.img->get_host_srv())),
            (ImTextureID)bg.img->get_host_srv(),
            {img_button_size, img_button_size})) {
      desktop_bg_mngr.set_background((RDesktopBackground*)&bg);
    }
    ImGui::PopStyleColor(2);

    imgs_in_line++;
    if (ImGui::GetContentRegionAvail().x >
        (img_button_size * (imgs_in_line + 1)))
      ImGui::SameLine();
    else
      imgs_in_line = 0;
  }
}

void RSettingsView::render_themes_manager_() {
  ImGui::BeginChild("##themes_manager",
                    {ImGui::GetWindowWidth(),
                     ImGui::GetContentRegionAvail().y - BOTTOM_GAP_});
  auto& thm_mngr = RConfigsManager::get().get_theme_mngr();

  ImGui::SetNextItemWidth(150.0f);
  themes_list_filter_.Draw("##search_filter");

  ImGui::SameLine();

  if (ImGui::Button("+")) {
    thm_mngr.create_theme("New Theme");
  }

  ImGui::Spacing();

  int thm_itx = 0;
  while (thm_itx < thm_mngr.get_themes().size()) {
    auto& thm = thm_mngr.get_themes()[thm_itx];
    thm_itx++;
    if (!themes_list_filter_.PassFilter(thm->name.c_str())) continue;

    ImGui::TextUnformatted(thm->name.c_str());
    ImGui::SameLine();
    if (ImGui::SmallButton(r_str_to_cstr("Edit##" + thm->uuid))) {
      thm_selected_ = thm;
      thm_editor_in_anim_val_.play();
    }
    ImGui::SameLine();
    if (ImGui::SmallButton(r_str_to_cstr("Delete##" + thm->uuid)))
      thm_mngr.delete_theme(thm);
  }
  ImGui::EndChild();
}

void RSettingsView::render_theme_settings_(RTheme& theme) {
  // TODO: Add filter for every option and not only colors (maybe with an helper
  // for easier integration)

  ImGui::SetCursorPosX(-ImGui::GetContentRegionAvail().x *
                           thm_editor_in_anim_val_.val() +
                       ImGui::GetStyle().WindowPadding.x);
  ImGui::BeginChild("##theme_editor",
                    {ImGui::GetContentRegionAvail().x,
                     ImGui::GetContentRegionAvail().y - BOTTOM_GAP_});

  if (ImGui::ArrowButton("##back_button", ImGuiDir_Left)) {
    thm_selected_ = nullptr;
    ImGui::EndChild();
    return;
  }

  ImGui::SameLine();

  ImGui::PushStyleColor(ImGuiCol_TabSelected, {0.0f, 0.0f, 0.0f, 0.0f});
  ImGui::BeginTabBar("##theme_edit_settings_tab",
                     ImGuiTabBarFlags_DrawSelectedOverline);
  ImGui::PopStyleColor();

  if (ImGui::BeginTabItem("General")) {
    render_theme_settings_generic_tab_(theme);
    ImGui::EndTabItem();
  }

  if (ImGui::BeginTabItem("Colors")) {
    render_theme_settings_colors_tab_(theme);
    ImGui::EndTabItem();
  }

  if (ImGui::BeginTabItem("Rendering")) {
    render_theme_settings_rendering_tab_(theme);
    ImGui::EndTabItem();
  }

  ImGui::EndTabBar();

  const auto& active_theme =
      RConfigsManager::get().get_theme_mngr().get_active_theme();
  if (active_theme.get() == &theme) ImGui::GetStyle() = theme.imgui_style;

  ImGui::EndChild();
}

void RSettingsView::render_theme_settings_generic_tab_(RTheme& theme) {
  auto& thm_mngr = RConfigsManager::get().get_theme_mngr();
  auto& bg_mngr = RConfigsManager::get().get_desktop_bg_mngr();
  ImGui::BeginChild("##theme_general_edit");
  ImGui::SetNextItemWidth(150.0f);
  ImGui::InputText("Name", &theme.name);
  // TODO: Add combo to select default background and font

  // Combo box to select the default theme background
  ImGui::SetNextItemWidth(150.0f);
  if (ImGui::BeginCombo(
          "Default Background",
          RFilesystemUtils::rm_filename_extension(theme.default_bg_name)
              .c_str())) {
    for (const auto& bg : bg_mngr.get_loaded_bgs()) {
      bool is_selected = (theme.default_bg_name == bg.img->get_name());
      if (ImGui::Selectable(
              RFilesystemUtils::rm_filename_extension(bg.img->get_name())
                  .c_str(),
              is_selected)) {
        theme.default_bg_name = bg.img->get_name();
        bg_mngr.set_background((RDesktopBackground*)&bg);
      }
      if (is_selected) ImGui::SetItemDefaultFocus();
    }
    ImGui::EndCombo();
  }

  // Combo box to select the theme font
  ImGui::SetNextItemWidth(150.0f);
  if (ImGui::BeginCombo(
          "Font",
          RFilesystemUtils::rm_filename_extension(theme.font_name).c_str())) {
    for (const auto& font : thm_mngr.get_fonts()) {
      bool is_selected = (theme.font_name == font->name);
      if (ImGui::Selectable(
              RFilesystemUtils::rm_filename_extension(font->name).c_str(),
              is_selected)) {
        theme.font_name = font->name;
        ImGui::GetIO().FontDefault = font->p_font;
      }
      if (is_selected) ImGui::SetItemDefaultFocus();
    }
    ImGui::EndCombo();
  }

  // Misc settings
  ImGui::ToggleButton("Set as default on load", &theme.is_default);

  ImGui::EndChild();
}

void RSettingsView::render_theme_settings_colors_tab_(RTheme& theme) {
  ImGuiStyle& style = theme.imgui_style;
  ImGui::BeginChild("##theme_color_edit");
  thm_colors_filter_.Draw("Search##search_filter");

  if (ImGui::RadioButton("Opaque",
                         thm_alpha_flags_ == ImGuiColorEditFlags_AlphaOpaque))
    thm_alpha_flags_ = ImGuiColorEditFlags_AlphaOpaque;
  ImGui::SameLine();
  if (ImGui::RadioButton("Alpha", thm_alpha_flags_ == ImGuiColorEditFlags_None))
    thm_alpha_flags_ = ImGuiColorEditFlags_None;
  ImGui::SameLine();
  if (ImGui::RadioButton(
          "Both", thm_alpha_flags_ == ImGuiColorEditFlags_AlphaPreviewHalf))
    thm_alpha_flags_ = ImGuiColorEditFlags_AlphaPreviewHalf;

  for (int i = 0; i < ImGuiCol_COUNT; i++) {
    const char* name = ImGui::GetStyleColorName(i);
    if (!thm_colors_filter_.PassFilter(name)) continue;
    ImGui::PushID(i);
    ImGui::ColorEdit4("##color", (float*)&style.Colors[i],
                      ImGuiColorEditFlags_AlphaBar | thm_alpha_flags_);

    ImGui::SameLine(0.0f, style.ItemInnerSpacing.x);
    ImGui::TextUnformatted(name);
    ImGui::PopID();
  }

  ImGui::EndChild();
}

void RSettingsView::render_theme_settings_rendering_tab_(RTheme& theme) {
  ImGuiStyle& style = theme.imgui_style;
  ImGui::BeginChild("##theme_rendering_edit");
  ImGui::ToggleButton("Glass Background", &style.GlassBlurBackground);
  ImGui::ToggleButton("Anti-aliased lines", &style.AntiAliasedLines);
  ImGui::ToggleButton("Anti-aliased lines use texture",
                      &style.AntiAliasedLinesUseTex);
  ImGui::ToggleButton("Anti-aliased fill", &style.AntiAliasedFill);
  ImGui::PushItemWidth(ImGui::GetFontSize() * 8);
  ImGui::DragFloat("Curve Tessellation Tolerance", &style.CurveTessellationTol,
                   0.02f, 0.10f, 10.0f, "%.2f");
  if (style.CurveTessellationTol < 0.10f) style.CurveTessellationTol = 0.10f;

  ImGui::DragFloat("Circle Tessellation Max Error",
                   &style.CircleTessellationMaxError, 0.005f, 0.10f, 5.0f,
                   "%.2f", ImGuiSliderFlags_AlwaysClamp);

  ImGui::DragFloat("Global Alpha", &style.Alpha, 0.005f, 0.20f, 1.0f, "%.2f");
  ImGui::DragFloat("Disabled Alpha", &style.DisabledAlpha, 0.005f, 0.0f, 1.0f,
                   "%.2f");
  ImGui::PopItemWidth();
  ImGui::EndChild();
}
