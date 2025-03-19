/*
 * This file contains an abstraction of platforms window system.
 *
 * Support:
 * - Win32
 */

#pragma once

#include <memory>
#include <vector>

#include "../graphics/graphics.h"

/*
 * Represent's a pointer to the native window handle (ex. HWND*) if dereferenced
 * get's the native handle
 */
typedef void* RWindowNativeHandle;

/*
 * All the available platforms window apis
 */
enum RWindowApi {
  /*
   * Win32 native window apis
   */
  RWIN32
};

/*
 * Represent's an abstracted window
 */
class RIWindow {
 public:
  /*
   * Init the window
   */
  virtual bool init(RGraphicsApiType gfx_api_type, int left, int top, int right,
                    int bottom, bool enable_viewports) = 0;

  /*
   * Get's the native window handler (ex. HWND*)
   */
  virtual RWindowNativeHandle get_native_handle() const = 0;

  /*
   * Get's the graphics api handler (renderer)
   */
  RIGraphicsApi& get_gfx() { return *graphics_; }

 protected:
  std::shared_ptr<RIGraphicsApi> graphics_;
};
