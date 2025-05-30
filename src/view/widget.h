#pragma once

#include "../../external/imgui/imgui.h"
#include "../core/string.h"

/*
 * Represent a widget, these are special views used only for widget's that
 * are usually fixed in a specific object or point of the screen. Widget's
 * differ from default views as they are not spawned in the viewpool like every
 * other view, they are managed through other views that integrates them.
 */
class RWidgetView {
 public:
  RWidgetView() { init(); }
  RWidgetView(const r_string& tag) : tag_{tag} { init(); }
  virtual ~RWidgetView() = default;

  /*
   * Render function to be implemented by derived widgets
   */
  virtual void render() = 0;
  /*
   * Get's the computed size of the widget
   */
  virtual const ImVec2 get_size() = 0;
  /*
   * Get's the name of the widget
   */
  virtual constexpr const char* get_name() const { return "Widget View"; };

 protected:
  /*
   * The view tag
   */
  r_string tag_;

  /*
   * The view unique identifier
   */
  r_string uuid_;

  /*
   * Begins a managed widget platform (useful when widgets are not integrated
   * inside another view so that the widget can exist in a standalone fashion)
   */
  bool begin_frame_(ImGuiWindowFlags flags = 0);

  /*
   * End's the managed widget platform
   */
  void end_frame_();

 private:
  void init();
};
