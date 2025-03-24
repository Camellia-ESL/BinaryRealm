#include "keybinds.h"

#include <Windows.h>
#include <winuser.h>

#include <vector>

#include "../../external/imgui/imgui.h"
#include "../view/viewpool.h"
#include "../view/views/console_view.h"
#include "../view/views/settings_view.h"

// Key watcher used to check if a key is pressed, down or up
class KeyWatcher {
 public:
  KeyWatcher(int key) : key_(key), active_(false) {}

  bool is_pressed(std::vector<int> mod_keys = {}) {
    for (auto mod_key : mod_keys) {
      if (!GetAsyncKeyState(mod_key)) return false;
    }

    if (GetAsyncKeyState(key_)) {
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

void r_process_keybinds() {
  ImGuiIO& io = ImGui::GetIO();

  // Setup watchers
  static KeyWatcher oem_5_key{VK_OEM_5};

  // Open console window when shift + oem_5 is pressed
  if (oem_5_key.is_pressed({VK_CONTROL}))
    RViewPool::get().spawn<RConsoleView>();

  // Open settings window when alt + oem_5 is pressed
  if (oem_5_key.is_pressed({VK_RMENU})) RViewPool::get().spawn<RSettingsView>();
}

bool r_is_view_close_keybind_pressed() {
  static KeyWatcher esc_key{VK_ESCAPE};
  return esc_key.is_pressed();
}
