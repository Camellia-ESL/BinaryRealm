#pragma once

#include "../widget.h"

/*
 * Represents a date widget that display the current date + time
 */
class RDateWidget : public RWidgetView {
 public:
  /*
   * Whether the widget should be platformed or not
   */
  bool is_platformed = false;

  using RWidgetView::RWidgetView;
  void render() override;
  const ImVec2 get_size() override;
  constexpr const char* get_name() const override { return "Date Widget"; };
};
