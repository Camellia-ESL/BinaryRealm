#pragma once

#include "../view/view.h"

// App settings view
class RSettingsView : public RView {
 public:
  using RView::RView;
  // Render function to be implemented by derived views
  void render() override;
  // Return's settings view name
  constexpr const char* get_name() const override { return "Settings View"; };

 private:
  // When set to false destroy's the window
  bool open_ = true;
};
