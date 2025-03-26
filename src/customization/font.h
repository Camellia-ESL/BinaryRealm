#pragma once

#include "../../external/imgui/imgui.h"
#include "../core/string.h"

/*
 * Holds data about a loaded font
 */
struct RFont {
  /*
   * The pointer to the imgui font resource used for rendering
   */
  ImFont* p_font;

  /*
   * The path to the file from which the font was loaded
   */
  r_string file_path;

  /*
   * The name of the font
   */
  r_string name;
};
