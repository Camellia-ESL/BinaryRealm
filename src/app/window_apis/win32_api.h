#pragma once

#include "../window.h"

#include <Windows.h>

// Win32 native window handler abstraction
class RWin32Api : public RIWindow {
public:
  RWin32Api();
  ~RWin32Api();

  // Init a the window overlay
  bool init() override;
  // Process window messages
  void process_messages() override;
  // Get's the native window handler (HWND*)
  RWindowNativeHandle get_native_handle() const override {
    return (void *)&hwnd;
  }
  // Whether if the window is running or not
  bool is_running() const override { return !done; }
  // Fetch monitor infos using EnumDisplayMonitors api
  void fetch_monitors() override;

private:
  HWND hwnd;
  bool done = false;
  WNDCLASSEXW wc;
};
