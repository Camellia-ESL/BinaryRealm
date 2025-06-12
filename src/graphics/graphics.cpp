#include "graphics.h"

#include "../../external/font-awesome-icons/font_awesome_icons_6.h"
#include "../app/app.h"

void RIGraphicsApi::init_imgui_(bool enable_viewports) {
  // Check imgui version integrity
  IMGUI_CHECKVERSION();

  // Create's an imgui context for this graphics instance
  imgui_ctx_ = ImGui::CreateContext();
  ImGui::SetCurrentContext(imgui_ctx_);

  ImGuiIO& io = ImGui::GetIO();

  // Disable INI file serializing
  io.IniFilename = NULL;

  // Required to detach imgui window viewports from the app
  static bool test = false;
  if (enable_viewports) io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

  io.ConfigViewportsNoDefaultParent = true;
  io.ConfigViewportsNoAutoMerge = true;

  ImGui::StyleColorsDark();

  // Load icon fonts
  io.Fonts->AddFontDefault();

  float icon_font_size = RApp::get().get_main_screen().get_adapted_font_size();
  static const ImWchar icons_ranges[] = {ICON_MIN_FA, ICON_MAX_16_FA, 0};
  ImFontConfig icons_config;
  icons_config.MergeMode = true;
  icons_config.PixelSnapH = true;
  icons_config.GlyphMinAdvanceX = icon_font_size;
  icons_config.GlyphOffset.y = 0.000592f * powf(icon_font_size, 3.08f);
  fa_icon_font_6_ = io.Fonts->AddFontFromFileTTF(
      "assets/fonts/icons-fonts/" FONT_ICON_FILE_NAME_FAS, icon_font_size,
      &icons_config, icons_ranges);
}
