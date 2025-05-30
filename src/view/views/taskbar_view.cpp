#include "taskbar_view.h"

#include "../../app/app.h"
#include "../../config/config_manager.h"

void RTaskbarView::on_spawn() {
  // Required for the taskbar to be allocated properly
  tag_ = TAG;
  RView::on_spawn();

  // Add widgets to the array
  all_widgets_.push_back({&RConfigsManager::get()
                               .get_theme_mngr()
                               .get_active_theme()
                               ->widgets_settings.date_widget,
                          &date_widget});
}

void RTaskbarView::render() {
  ImGui::SetNextWindowPos({TASKBAR_HORIZONTAL_MARGIN, TASKBAR_VERTICAL_MARGIN});
  ImGui::SetNextWindowSize({RApp::get().get_main_screen().get_width() -
                                (TASKBAR_HORIZONTAL_MARGIN * 2.0f),
                            TASKBAR_HEIGHT});

  ImGuiWindowClass window_class;
  window_class.ViewportFlagsOverrideSet = ImGuiViewportFlags_NoTaskBarIcon;
  ImGui::SetNextWindowClass(&window_class);
  ImGui::Begin(r_str_to_cstr(get_name() + "##" + uuid_), nullptr,
               ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize |
                   ImGuiWindowFlags_NoDecoration |
                   ImGuiWindowFlags_NoFocusOnAppearing);

  // Create's support vectors to render on every possible alignment
  std::vector<TaskbarWidget> left_aligned_widgets, center_aligned_widgets,
      right_aligned_widgets;

  for (const auto& widget : all_widgets_) {
    switch (widget.p_settings->alignment) {
      case RWidgetAlignment::LEFT:
        left_aligned_widgets.push_back(widget);
        break;
      case RWidgetAlignment::CENTER:
        center_aligned_widgets.push_back(widget);
        break;
      case RWidgetAlignment::RIGHT:
        right_aligned_widgets.push_back(widget);
        break;
    }
  }

  // Sort widgets by priority
  auto sort_by_priority = [](const TaskbarWidget& a, const TaskbarWidget& b) {
    return a.p_settings->alignment_priority < b.p_settings->alignment_priority;
  };

  std::sort(left_aligned_widgets.begin(), left_aligned_widgets.end(),
            sort_by_priority);
  std::sort(center_aligned_widgets.begin(), center_aligned_widgets.end(),
            sort_by_priority);
  std::sort(right_aligned_widgets.begin(), right_aligned_widgets.end(),
            sort_by_priority);

  float win_width = ImGui::GetWindowSize().x;
  float win_padding = ImGui::GetStyle().WindowPadding.x;
  float content_width = win_width - (win_padding * 2.0f);

  // Calculate center widgets total width
  float total_center_width = 0.0f;
  if (!center_aligned_widgets.empty()) {
    for (size_t i = 0; i < center_aligned_widgets.size(); ++i) {
      total_center_width += center_aligned_widgets[i].p_widget->get_size().x;
      if (i < center_aligned_widgets.size() - 1) {
        total_center_width += WIDGETS_HORIZONTAL_PADDING;
      }
    }
  }

  // Calculate right widgets total width
  float total_right_width = 0.0f;
  if (!right_aligned_widgets.empty()) {
    for (size_t i = 0; i < right_aligned_widgets.size(); ++i) {
      total_right_width += right_aligned_widgets[i].p_widget->get_size().x;
      if (i < right_aligned_widgets.size() - 1) {
        total_right_width += WIDGETS_HORIZONTAL_PADDING;
      }
    }
  }

  // LEFT widgets rendering
  bool first = true;
  for (const auto& widget : left_aligned_widgets) {
    if (!first) ImGui::SameLine(0.0f, WIDGETS_HORIZONTAL_PADDING);
    widget.p_widget->render();
    first = false;
  }

  // CENTER widgets rendering
  if (!center_aligned_widgets.empty()) {
    float centerStartX =
        win_padding + (content_width - total_center_width) / 2.0f;
    ImGui::SetCursorPosX(centerStartX);

    first = true;
    for (const auto& widget : center_aligned_widgets) {
      if (!first) ImGui::SameLine(0.0f, WIDGETS_HORIZONTAL_PADDING);
      widget.p_widget->render();
      first = false;
    }
  }

  // RIGHT widgets rendering
  if (!right_aligned_widgets.empty()) {
    float rightStartX = win_width - win_padding - total_right_width;
    ImGui::SetCursorPosX(rightStartX);

    first = true;
    for (const auto& widget : right_aligned_widgets) {
      if (!first) ImGui::SameLine(0.0f, WIDGETS_HORIZONTAL_PADDING);
      widget.p_widget->render();
      first = false;
    }
  }

  ImGui::End();
}
