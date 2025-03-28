#include "settings_view.h"

#include "../../../external/imgui/imgui.h"
#include "../../config/config_manager.h"

void RSettingsView::render() {
  begin_frame();

  ImGui::Text("Settings");

  ImGui::BeginTabBar("##settings_tab");

  if (ImGui::BeginTabItem("Background")) {
    render_background_settings_();
    ImGui::EndTabItem();
  }

  if (ImGui::BeginTabItem("Themes")) {
    render_theme_settings_();
    ImGui::EndTabItem();
  }

  ImGui::EndTabBar();

  end_frame();
}

void RSettingsView::render_background_settings_() {
  auto& desktop_bg_mngr = RConfigsManager::get().get_desktop_bg_mngr();
  auto& loaded_bgs = desktop_bg_mngr.get_loaded_bgs();

  const float img_button_size = 150.0f;

  int imgs_in_line = 0;
  for (auto& bg : loaded_bgs) {
    if (ImGui::ImageButton(
            r_str_to_cstr("##" +
                          std::to_string((uint64_t)bg.img->get_host_srv())),
            (ImTextureID)bg.img->get_host_srv(),
            {img_button_size, img_button_size})) {
      desktop_bg_mngr.set_background((RDesktopBackground*)&bg);
    }

    imgs_in_line++;
    if (ImGui::GetContentRegionAvail().x >
        (img_button_size * (imgs_in_line + 1)))
      ImGui::SameLine();
    else
      imgs_in_line = 0;
  }
}

void RSettingsView::render_theme_settings_() {
  // TODO: Add filter for every option and not only colors (maybe with an helper
  // for easier integration)

  // TODO: Add multiple themes support in the ui

  // TODO: Add all the remaining options for the theme + save support to save
  // all the themes

  thm_opts_filter_.Draw("##search_filter");

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

  ImGuiStyle& style = ImGui::GetStyle();

  for (int i = 0; i < ImGuiCol_COUNT; i++) {
    const char* name = ImGui::GetStyleColorName(i);
    if (!thm_opts_filter_.PassFilter(name)) continue;
    ImGui::PushID(i);
    ImGui::ColorEdit4("##color", (float*)&style.Colors[i],
                      ImGuiColorEditFlags_AlphaBar | thm_alpha_flags_);

    ImGui::SameLine(0.0f, style.ItemInnerSpacing.x);
    ImGui::TextUnformatted(name);
    ImGui::PopID();
  }

  ImGui::Checkbox("Anti-aliased lines", &style.AntiAliasedLines);
  ImGui::SameLine();

  ImGui::Checkbox("Anti-aliased lines use texture",
                  &style.AntiAliasedLinesUseTex);
  ImGui::SameLine();

  ImGui::Checkbox("Anti-aliased fill", &style.AntiAliasedFill);
  ImGui::PushItemWidth(ImGui::GetFontSize() * 8);
  ImGui::DragFloat("Curve Tessellation Tolerance", &style.CurveTessellationTol,
                   0.02f, 0.10f, 10.0f, "%.2f");
  if (style.CurveTessellationTol < 0.10f) style.CurveTessellationTol = 0.10f;

  ImGui::DragFloat("Circle Tessellation Max Error",
                   &style.CircleTessellationMaxError, 0.005f, 0.10f, 5.0f,
                   "%.2f", ImGuiSliderFlags_AlwaysClamp);
  ImGui::SameLine();

  ImGui::DragFloat("Global Alpha", &style.Alpha, 0.005f, 0.20f, 1.0f, "%.2f");
  ImGui::DragFloat("Disabled Alpha", &style.DisabledAlpha, 0.005f, 0.0f, 1.0f,
                   "%.2f");
  ImGui::SameLine();
  ImGui::PopItemWidth();
}
