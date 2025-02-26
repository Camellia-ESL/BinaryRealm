/*
 * This file contains an abstraction of platforms window system.
 *
 * Support:
 * - Win32
 */

#pragma once

#include <vector>

// Represent's a pointer to the native window handle (ex. HWND*) if dereferenced
// get's the native handle
typedef void* RWindowNativeHandle;

// All the available platforms window apis
enum RWindowApi {
  // Win32 native window apis
  RWIN32
};

// The monitor resolution rectangle
struct RMonitorRect {
  int left, top, right, bottom;
};

// Represent's a user screen monitor
struct RMonitor {
  // The monitor resolution rectangle
  RMonitorRect rect;
};

// Represent's an abstracted window
class RIWindow {
 public:
  // Init the window
  virtual bool init() { return false; }
  // Process window messages
  virtual void process_messages() {}
  // Get's the native window handler (ex. HWND*)
  virtual RWindowNativeHandle get_native_handle() const { return nullptr; }
  // Whether if the window is running or not
  virtual bool is_running() const { return false; }
  // Update the monitors info
  virtual void fetch_monitors() {}

  // Get's the monitors
  const std::vector<RMonitor>& get_monitors() const { return monitors_; }

 protected:
  std::vector<RMonitor> monitors_;
};
