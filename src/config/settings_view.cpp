#include "settings_view.h"

#include "../../external/imgui/imgui.h"

void RSettingsView::render() {
  ImGui::Begin(r_str_to_cstr(get_name() + "#HERE NEEDS TO BE PUT A UUID"));

  ImGui::End();
}
