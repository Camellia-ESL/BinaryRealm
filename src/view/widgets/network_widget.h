#pragma once

#include "../../core/network.h"
#include "../widget.h"

/*
 * Represents a network widget that display informations about the network
 * adapters installed in the device
 */
class RNetworkWidget : public RWidgetView {
 public:
  /*
   * Whether the widget should be platformed or not
   */
  bool is_platformed = false;

  using RWidgetView::RWidgetView;
  void render() override;
  const ImVec2 get_size() override;
  constexpr const char* get_name() const override { return "Date Widget"; };

 private:
  RNetAdapter* curr_adapter_;
};
