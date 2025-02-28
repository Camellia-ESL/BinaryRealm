#include "keybinds.h"

#include <Windows.h>
#include <winuser.h>

#include "../../external/imgui/imgui.h"
#include "../view/viewpool.h"
#include "settings_view.h"

// Win32 key watcher used to check if a key is pressed
class KeyWatcherWin32 {
 public:
  KeyWatcherWin32(int key) : key_(key), active_(false) {}

  bool is_pressed(int mod_key = 0) {
    const bool is_mod_key_down =
        mod_key != 0 ? GetAsyncKeyState(mod_key) : true;
    if (GetAsyncKeyState(key_) && is_mod_key_down) {
      if (!active_) {
        active_ = true;
        return true;
      }
    } else
      active_ = false;
    return false;
  }

  const bool is_down() const { return GetAsyncKeyState(key_) & 0x80; }
  const bool is_up() const { return !(GetAsyncKeyState(key_) & 0x80); }

  operator bool() { return is_pressed(); }

 private:
  int key_;
  bool active_;
};

static void r_on_settings_win_opener_pressed() {
  // Spawn's a new setting window
  RViewPool::get().spawn<RSettingsView>();
}

void r_process_keybinds_win32() {
  ImGuiIO& io = ImGui::GetIO();

  // Setup watchers
  static KeyWatcherWin32 tab_key{VK_TAB};
  static KeyWatcherWin32 shift_key{VK_SHIFT};

  if (tab_key.is_pressed(VK_SHIFT)) r_on_settings_win_opener_pressed();
}
