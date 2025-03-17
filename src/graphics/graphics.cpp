#include "graphics.h"

#include "../../external/imgui/imgui.h"

void RIGraphicsApi::init_imgui_() {
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO();
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
  io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

  // Required to detach imgui window viewports from the app
  io.ConfigViewportsNoDefaultParent = true;
  io.ConfigViewportsNoAutoMerge = true;

  ImGui::StyleColorsDark();
}
