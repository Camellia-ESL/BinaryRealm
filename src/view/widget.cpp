#include "widget.h"

void RWidgetView::init() { uuid_ = r_str_uuid(); }

bool RWidgetView::begin_frame_(ImGuiWindowFlags flags) {
  return ImGui::Begin(r_str_to_cstr(get_name() + "##" + uuid_), nullptr,
                      ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize |
                          ImGuiWindowFlags_NoDecoration);
}

void RWidgetView::end_frame_() { ImGui::End(); }
