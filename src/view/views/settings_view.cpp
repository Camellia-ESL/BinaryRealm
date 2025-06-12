#include "settings_view.h"

#include <vector>

#include "../../../external/imgui/imgui.h"
#include "../../app/app.h"
#include "../../core/filesystem_utils.h"
#include "../../view/viewpool.h"
#include "../../view/views/taskbar_view.h"

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
      {ImGui::GetWindowWidth() -
           (50.0f * RApp::get().get_main_screen().get_adapted_pixel_size()),
       ImGui::GetWindowHeight() -
           (30.0f * RApp::get().get_main_screen().get_adapted_pixel_size())});
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

  if (ImGui::BeginTabItem("Taskbar")) {
    render_taskbar_settings_tab_(theme);
    ImGui::EndTabItem();
  }

  if (ImGui::BeginTabItem("Widgets")) {
    render_widgets_settings_tab_(theme);
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

void RSettingsView::render_taskbar_settings_tab_(RTheme& theme) {
  if (ImGui::ToggleButton("Enabled", &theme.is_taskbar_enabled) &&
      &theme ==
          RConfigsManager::get().get_theme_mngr().get_active_theme().get()) {
    RViewPool::get().destroy(RTaskbarView::TAG);
    if (theme.is_taskbar_enabled) RViewPool::get().spawn<RTaskbarView>();
  }

  std::vector<RWidgetSettings*> widget_settings{};
  widget_settings.push_back(&theme.widgets_settings.date_widget);
  widget_settings.push_back(&theme.widgets_settings.network_widget);

  ImGui::Text("Drag widgets between columns to organize taskbar");
  ImGui::Separator();

  // Helper lambda to get alignment name
  auto GetAlignmentName = [](RWidgetAlignment alignment) -> const char* {
    switch (alignment) {
      case RWidgetAlignment::LEFT:
        return "Left";
      case RWidgetAlignment::CENTER:
        return "Center";
      case RWidgetAlignment::RIGHT:
        return "Right";
      default:
        return "Unknown";
    }
  };

  // Helper lambda to get widgets for specific alignment
  auto GetWidgetsForAlignment = [&](RWidgetAlignment alignment) {
    std::vector<std::pair<int, RWidgetSettings*>> result;
    for (size_t i = 0; i < widget_settings.size(); ++i) {
      if (widget_settings[i]->alignment == alignment &&
          widget_settings[i]->enabled) {
        result.push_back({static_cast<int>(i), widget_settings[i]});
      }
    }

    // Sort by priority
    std::sort(result.begin(), result.end(), [](const auto& a, const auto& b) {
      return a.second->alignment_priority < b.second->alignment_priority;
    });

    return result;
  };

  // Helper lambda to update priorities after reordering
  auto UpdatePriorities = [&](RWidgetAlignment alignment) {
    auto widgets = GetWidgetsForAlignment(alignment);
    for (size_t i = 0; i < widgets.size(); ++i) {
      widgets[i].second->alignment_priority = static_cast<int>(i);
    }
  };

  // Three column layout
  if (ImGui::BeginTable(
          "##organizer", 3,
          ImGuiTableFlags_Borders | ImGuiTableFlags_SizingFixedFit)) {
    ImGui::TableSetupColumn("Left", ImGuiTableColumnFlags_WidthFixed, 200.0f);
    ImGui::TableSetupColumn("Center", ImGuiTableColumnFlags_WidthFixed, 200.0f);
    ImGui::TableSetupColumn("Right", ImGuiTableColumnFlags_WidthFixed, 200.0f);

    ImGui::TableNextRow();

    // Draw each alignment column
    for (int col = 0; col < 3; ++col) {
      ImGui::TableSetColumnIndex(col);

      RWidgetAlignment alignment = static_cast<RWidgetAlignment>(col);

      // Begin child window to make entire column a drop target
      char child_id[32];
      snprintf(child_id, sizeof(child_id), "Column%d", col);

      ImVec2 child_size =
          ImVec2(0, ImGui::GetContentRegionAvail().y -
                        60);  // Leave space for disabled widgets
      ImGui::BeginChild(child_id, child_size, true,
                        ImGuiWindowFlags_AlwaysUseWindowPadding);

      // Column header
      ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 0.0f, 1.0f));
      ImGui::Text("%s", GetAlignmentName(alignment));
      ImGui::PopStyleColor();
      ImGui::Separator();

      // Get widgets for this alignment
      auto widgets = GetWidgetsForAlignment(alignment);

      // Draw each widget
      for (const auto& [widget_idx, widget_ptr] : widgets) {
        ImGui::PushID(widget_idx);

        // Widget item
        const char* widget_name = widget_settings[widget_idx]->get_name();

        ImGui::Selectable(
            widget_name, false, 0,
            ImVec2(0, ImGui::GetTextLineHeightWithSpacing() * 1.2f));

        // Drag source
        if (ImGui::BeginDragDropSource()) {
          ImGui::SetDragDropPayload("WIDGET", &widget_idx, sizeof(int));
          ImGui::Text("Moving: %s", widget_name);
          ImGui::EndDragDropSource();
        }

        // Drop target for reordering within same alignment
        if (ImGui::BeginDragDropTarget()) {
          if (const ImGuiPayload* payload =
                  ImGui::AcceptDragDropPayload("WIDGET")) {
            int dropped_idx = *(const int*)payload->Data;
            if (dropped_idx != widget_idx &&
                widget_settings[dropped_idx]->alignment == alignment) {
              // Swap priorities
              int temp_priority =
                  widget_settings[dropped_idx]->alignment_priority;
              widget_settings[dropped_idx]->alignment_priority =
                  widget_ptr->alignment_priority;
              widget_ptr->alignment_priority = temp_priority;
              UpdatePriorities(alignment);
            }
          }
          ImGui::EndDragDropTarget();
        }

        // Context menu
        if (ImGui::BeginPopupContextItem()) {
          if (ImGui::MenuItem("Disable")) {
            widget_ptr->enabled = false;
          }
          ImGui::EndPopup();
        }

        ImGui::PopID();
      }

      // Make the entire child window a drop target
      ImGui::EndChild();

      if (ImGui::BeginDragDropTarget()) {
        if (const ImGuiPayload* payload =
                ImGui::AcceptDragDropPayload("WIDGET")) {
          int dropped_idx = *(const int*)payload->Data;
          if (widget_settings[dropped_idx]->alignment != alignment) {
            widget_settings[dropped_idx]->alignment = alignment;
            widget_settings[dropped_idx]->alignment_priority =
                static_cast<int>(widgets.size());
            UpdatePriorities(alignment);
          }
        }
        ImGui::EndDragDropTarget();
      }

      // Visual feedback for entire column when hovering during drag
      if (ImGui::IsItemHovered() && ImGui::GetDragDropPayload()) {
        ImDrawList* draw_list = ImGui::GetWindowDrawList();
        ImVec2 p_min = ImGui::GetItemRectMin();
        ImVec2 p_max = ImGui::GetItemRectMax();
        draw_list->AddRectFilled(p_min, p_max, IM_COL32(100, 200, 100, 30));
        draw_list->AddRect(p_min, p_max, IM_COL32(100, 200, 100, 150), 0.0f, 0,
                           2.0f);
      }
    }

    ImGui::EndTable();
  }

  // Disabled widgets section
  ImGui::Separator();
  ImGui::Text("Disabled Widgets:");

  for (size_t i = 0; i < widget_settings.size(); ++i) {
    if (!widget_settings[i]->enabled) {
      ImGui::PushID(static_cast<int>(i));

      const char* widget_name = widget_settings[i]->get_name();

      ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.5f, 0.5f, 0.5f, 1.0f));
      ImGui::Selectable(widget_name);
      ImGui::PopStyleColor();

      if (ImGui::BeginPopupContextItem()) {
        if (ImGui::MenuItem("Enable")) {
          widget_settings[i]->enabled = true;
        }
        ImGui::EndPopup();
      }

      ImGui::PopID();
    }
  }
}

void RSettingsView::render_widgets_settings_tab_(RTheme& theme) {
  const char* widget_names[] = {
      theme.widgets_settings.date_widget.get_name(),
      theme.widgets_settings.network_widget.get_name()};
  const int widget_count = IM_ARRAYSIZE(widget_names);

  // Left Panel: Widget List
  ImGui::BeginChild("WidgetList", ImVec2(150, 0), true);
  for (int i = 0; i < widget_count; i++) {
    if (ImGui::Selectable(widget_names[i], widget_selected_idx == i)) {
      widget_selected_idx = i;
    }
  }
  ImGui::EndChild();

  // Right Panel: Widget Settings
  ImGui::SameLine();
  ImGui::BeginChild("WidgetConfig", ImVec2(0, 0), true);

  // Date widget
  if (widget_selected_idx == 0) {
    render_date_widget_settings(theme);
  }

  // Network widget
  if (widget_selected_idx == 1) {
    render_network_widget_settings(theme);
  }

  ImGui::EndChild();
}

void RSettingsView::render_date_widget_settings(RTheme& theme) {
  RDateWidgetSettings& settings = theme.widgets_settings.date_widget;
  ImGui::SeparatorText("Date Widget Settings");

  ImGui::ToggleButton("Enabled", &settings.enabled);

  ImGui::ColorEdit4("##date_color", (float*)&settings.date_text_color,
                    ImGuiColorEditFlags_AlphaBar);
  ImGui::SameLine(0.0f, theme.imgui_style.ItemInnerSpacing.x);
  ImGui::TextUnformatted("Date Color");

  ImGui::ColorEdit4("##time_color", (float*)&settings.time_text_color,
                    ImGuiColorEditFlags_AlphaBar);
  ImGui::SameLine(0.0f, theme.imgui_style.ItemInnerSpacing.x);
  ImGui::TextUnformatted("Time Color");
}

void RSettingsView::render_network_widget_settings(RTheme& theme) {
  RNetworkWidgetSettings& settings = theme.widgets_settings.network_widget;
  ImGui::SeparatorText("Network Widget Settings");

  ImGui::ToggleButton("Enabled", &settings.enabled);

  ImGui::ColorEdit4("##adapter_color", (float*)&settings.adapter_color,
                    ImGuiColorEditFlags_AlphaBar);
  ImGui::SameLine(0.0f, theme.imgui_style.ItemInnerSpacing.x);
  ImGui::TextUnformatted("Adapter Color");

  ImGui::ColorEdit4("##download_kbps_color",
                    (float*)&settings.download_kbps_color,
                    ImGuiColorEditFlags_AlphaBar);
  ImGui::SameLine(0.0f, theme.imgui_style.ItemInnerSpacing.x);
  ImGui::TextUnformatted("Download Kbps Color");

  ImGui::ColorEdit4("##upload_kbps_color", (float*)&settings.upload_kbps_color,
                    ImGuiColorEditFlags_AlphaBar);
  ImGui::SameLine(0.0f, theme.imgui_style.ItemInnerSpacing.x);
  ImGui::TextUnformatted("Upload Kbps Color");
}
