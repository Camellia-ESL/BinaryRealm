#include "graphics.h"

void RIGraphicsApi::init_imgui_(bool enable_viewports) {
  // Check imgui version integrity
  IMGUI_CHECKVERSION();

  // Create's an imgui context for this graphics instance
  imgui_ctx_ = ImGui::CreateContext();
  ImGui::SetCurrentContext(imgui_ctx_);

  ImGuiIO& io = ImGui::GetIO();
  // TODO: Remove imgui navigation keybinds

  // Required to detach imgui window viewports from the app
  static bool test = false;
  if (enable_viewports) io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

  io.ConfigViewportsNoDefaultParent = true;
  io.ConfigViewportsNoAutoMerge = true;

  ImGui::StyleColorsDark();
}
