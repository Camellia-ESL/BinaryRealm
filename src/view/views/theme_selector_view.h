#pragma once

#include "../../config/config_manager.h"
#include "../view.h"

class RThemeSelectorView : public RWindowView {
 public:
  using RWindowView::RWindowView;
  void on_spawn() override;
  void render() override;
  constexpr const char* get_name() const override {
    return "Theme Selector View";
  };
};
