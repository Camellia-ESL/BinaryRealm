#pragma once

#include <Windows.h>

#include "../window.h"

/*
 * Win32 native window handler abstraction
 */
class RWin32Api : public RIWindow {
 public:
  RWin32Api();
  ~RWin32Api();

  /*
   * Init a the window overlay
   */
  bool init(RGraphicsApiType gfx_api_type, int left, int top, int right,
            int bottom, bool enable_viewports) override;

  /*
   * Process windows messages, return's false if the app should be closed or
   * something went wrong.
   */
  static bool process_messages();

  /*
   * Get's the native window handler (HWND*)
   */
  RWindowNativeHandle get_native_handle() const override {
    return (void*)&hwnd_;
  }

 private:
  HWND hwnd_;
  WNDCLASSEXW wc_;
};
