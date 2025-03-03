#include "settings_view.h"

#include "../../../external/imgui/imgui.h"
#include "../viewpool.h"

void RSettingsView::render() {
  ImGui::Begin(
      r_str_to_cstr(get_name() + "##" + std::to_string((uint64_t)this)),
      &open_);

  ImGui::End();

  // Destroy's the view when it get's closed
  if (!open_) RViewPool::get().destroy(this);
}
