#include "settings_view.h"

#include "../../../external/imgui/imgui.h"

void RSettingsView::render() {
  begin_frame();

  // Update's animation values
  thm_editor_in_anim_val_.update(ImGui::GetIO().DeltaTime);

  ImGui::Text("Settings");

  ImGui::BeginTabBar("##settings_tab");

  if (ImGui::BeginTabItem("Background")) {
    render_background_settings_();
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

  end_frame();
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
    if (ImGui::ImageButton(
            r_str_to_cstr("##" +
                          std::to_string((uint64_t)bg.img->get_host_srv())),
            (ImTextureID)bg.img->get_host_srv(),
            {img_button_size, img_button_size})) {
      desktop_bg_mngr.set_background((RDesktopBackground*)&bg);
    }
    ImGui::PopStyleColor();

    imgs_in_line++;
    if (ImGui::GetContentRegionAvail().x >
        (img_button_size * (imgs_in_line + 1)))
      ImGui::SameLine();
    else
      imgs_in_line = 0;
  }
}

void RSettingsView::render_themes_manager_() {
  ImGui::BeginChild("##themes_manager", {ImGui::GetWindowWidth(),
                                         ImGui::GetContentRegionAvail().y});
  auto& thm_mngr = RConfigsManager::get().get_theme_mngr();

  if (ImGui::Button("+")) {
    thm_mngr.create_theme("New Theme");
  }

  ImGui::SameLine();

  if (ImGui::Button("Save All")) {
    thm_mngr.save();
  }

  int thm_itx = 0;
  while (thm_itx < thm_mngr.get_themes().size()) {
    auto& thm = thm_mngr.get_themes()[thm_itx];

    ImGui::TextUnformatted(thm->name.c_str());
    ImGui::SameLine();
    if (ImGui::SmallButton(r_str_to_cstr("Edit##" + thm->uuid))) {
      thm_selected_ = thm;
      thm_editor_in_anim_val_.play();
    }
    ImGui::SameLine();
    if (ImGui::SmallButton(r_str_to_cstr("Delete##" + thm->uuid)))
      thm_mngr.delete_theme(thm);

    thm_itx++;
  }
  ImGui::EndChild();
}

void RSettingsView::render_theme_settings_(RTheme& theme) {
  // TODO: Add filter for every option and not only colors (maybe with an helper
  // for easier integration)

  // TODO: Add all the remaining options for the theme + save support to save
  // all the themes

  // TODO: Change tab bar with something different (double separator is not
  // aesthetic at all)

  ImGui::SetCursorPosX(-ImGui::GetContentRegionAvail().x *
                           thm_editor_in_anim_val_.val() +
                       ImGui::GetStyle().WindowPadding.x);
  ImGui::BeginChild("##theme_editor", ImGui::GetContentRegionAvail());

  ImGuiStyle& style = theme.imgui_style;

  ImGui::PushStyleColor(ImGuiCol_Separator, {0.0f, 0.0f, 0.0f, 0.0f});
  ImGui::BeginTabBar("##theme_edit_settings_tab");
  ImGui::PopStyleColor();

  if (ImGui::ArrowButton("##back_button", ImGuiDir_Left)) {
    thm_selected_ = nullptr;
    ImGui::EndTabBar();
    ImGui::EndChild();
    return;
  }

  ImGui::SameLine();

  if (ImGui::BeginTabItem("General")) {
    ImGui::SetNextItemWidth(150.0f);
    ImGui::InputText("Name", &theme.name);
    // TODO: Add combo to select default background and font
    ImGui::EndTabItem();
  }

  if (ImGui::BeginTabItem("Colors")) {
    thm_colors_filter_.Draw("Search##search_filter");

    if (ImGui::RadioButton("Opaque",
                           thm_alpha_flags_ == ImGuiColorEditFlags_AlphaOpaque))
      thm_alpha_flags_ = ImGuiColorEditFlags_AlphaOpaque;
    ImGui::SameLine();
    if (ImGui::RadioButton("Alpha",
                           thm_alpha_flags_ == ImGuiColorEditFlags_None))
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

    ImGui::EndTabItem();
  }

  if (ImGui::BeginTabItem("Rendering")) {
    ImGui::Checkbox("Glass Background", &style.GlassBlurBackground);
    ImGui::Checkbox("Anti-aliased lines", &style.AntiAliasedLines);
    ImGui::Checkbox("Anti-aliased lines use texture",
                    &style.AntiAliasedLinesUseTex);
    ImGui::Checkbox("Anti-aliased fill", &style.AntiAliasedFill);
    ImGui::PushItemWidth(ImGui::GetFontSize() * 8);
    ImGui::DragFloat("Curve Tessellation Tolerance",
                     &style.CurveTessellationTol, 0.02f, 0.10f, 10.0f, "%.2f");
    if (style.CurveTessellationTol < 0.10f) style.CurveTessellationTol = 0.10f;

    ImGui::DragFloat("Circle Tessellation Max Error",
                     &style.CircleTessellationMaxError, 0.005f, 0.10f, 5.0f,
                     "%.2f", ImGuiSliderFlags_AlwaysClamp);

    ImGui::DragFloat("Global Alpha", &style.Alpha, 0.005f, 0.20f, 1.0f, "%.2f");
    ImGui::DragFloat("Disabled Alpha", &style.DisabledAlpha, 0.005f, 0.0f, 1.0f,
                     "%.2f");
    ImGui::PopItemWidth();
    ImGui::EndTabItem();
  }

  ImGui::EndTabBar();

  const auto& active_theme =
      RConfigsManager::get().get_theme_mngr().get_active_theme();
  if (active_theme.get() == &theme) ImGui::GetStyle() = theme.imgui_style;

  ImGui::EndChild();
}
