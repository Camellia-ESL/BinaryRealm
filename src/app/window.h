/*
 * This file contains an abstraction of platforms window system.
 *
 * Support:
 * - Win32
 */

#pragma once

// Represent's a pointer to the native window handle (ex. HWND*) if dereferenced
// get's the native handle
typedef void *RWindowNativeHandle;

// All the available platforms window apis
enum RWindowApi {
  // Win32 native window apis
  RWIN32
};

// Represent's an abstracted window
class RIWindow {
public:
  // Init a the window overlay
  virtual bool init() { return false; }
  // Process window messages
  virtual void process_messages() {}
  // Get's the native window handler (ex. HWND*)
  virtual RWindowNativeHandle get_native_handle() const { return nullptr; }
  // Whether if the window is running or not
  virtual bool is_running() const { return false; }
};
