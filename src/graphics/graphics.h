/*
 * This file contains an abstraction of graphics apis.
 *
 * Support:
 * - D3D11
 */

#pragma once

#include "../../external/imgui/imgui.h"
#include "../core/containers.h"
#include "image.h"

/*
 * All the available graphics apis
 */
enum class RGraphicsApiType { D3D11 };

/*
 * Represent's an abstracted graphics api backend
 */
class RIGraphicsApi {
 public:
  RIGraphicsApi() = default;
  virtual ~RIGraphicsApi() {}

  /*
   * Init a new instance
   */
  virtual bool init(void* p_native_handle, int left, int top, int right,
                    int bottom, bool enable_viewports) = 0;

  /*
   * Destroy the instance
   */
  virtual void destroy() = 0;

  /*
   * Start's a new frame
   */
  virtual void begin_render() = 0;

  /*
   * End's a frame, render's and present
   */
  virtual void render() = 0;

  /*
   * Load's an image from file then returns the shared handle that can be used
   * to fetch the SRV to pass to ImGui
   */
  virtual RResult<RpSharedImageHandle> load_img_from_file(
      const r_string& path) = 0;

  virtual RResult<RpImageSRV> get_img_from_shared_handle(
      RpSharedImageHandle handle) = 0;

  /*
   * Get's the imgui context for this instance
   */
  ImGuiContext* get_imgui_ctx() { return imgui_ctx_; }

  /*
   * Get's the imgui font awesome icons set 6
   */
  ImFont* const get_fa_icon_font_6() const { return fa_icon_font_6_; }

 protected:
  ImGuiContext* imgui_ctx_;
  ImFont* fa_icon_font_6_;

  /*
   * Should be called once in every init() backend, init's all the needed imgui
   * contexts
   */
  void init_imgui_(bool enable_viewports);
};
