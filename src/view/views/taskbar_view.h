#pragma once

#include "../../customization/widgets_settings_serializer.h"
#include "../view.h"
#include "../widgets/date_widget.h"
#include "../widgets/network_widget.h"

/*
 * Represents the main taskbar which can contain multiple widgets
 */
class RTaskbarView : public RView {
 public:
  static constexpr const char* TAG = "taskbar";
  static constexpr float WIDGETS_HORIZONTAL_PADDING = 10.0f;
  static constexpr float TASKBAR_HEIGHT = 30.0f;
  static constexpr float TASKBAR_HORIZONTAL_MARGIN = 20.0f;
  static constexpr float TASKBAR_VERTICAL_MARGIN = 10.0f;

  using RView::RView;
  void on_spawn() override;
  void render() override;
  constexpr const char* get_name() const override { return "Taskbar View"; };

 private:
  struct TaskbarWidget {
    RWidgetSettings* p_settings;
    RWidgetView* p_widget;
  };

  std::vector<TaskbarWidget> all_widgets_{};
  RDateWidget date_widget{};
  RNetworkWidget network_widget{};
};
