
#include "console_view.h"

#include <filesystem>
#include <sstream>

#include "../../../external/imgui/imgui.h"
#include "../viewpool.h"

/**
 * Private Global variable that contains all the custom commands that can be
 * executed by the console.
 */
std::unordered_map<r_string, std::function<void(const r_string&)>> g_commands_;

// Register a new custom command in the commands map
static void g_register_command_(const r_string& name,
                                std::function<void(const r_string&)> func) {
  g_commands_[name] = func;
}

void RConsoleView::render() {
  ImGui::Begin(
      r_str_to_cstr(get_name() + "##" + std::to_string((uint64_t)this)), &open_,
      ImGuiWindowFlags_NoScrollbar);

  // Scrollable history region
  ImGui::BeginChild("ScrollingRegion",
                    ImVec2(0, -ImGui::GetTextLineHeightWithSpacing()), true);
  {
    std::lock_guard<std::mutex> lock(cmds_history_mtx_);
    for (const auto& line : cmds_history_) {
      ImGui::TextUnformatted(line.c_str());
    }
  }
  ImGui::EndChild();

  // Command input
  if (ImGui::InputText("##input", cmd_input_buffer_,
                       IM_ARRAYSIZE(cmd_input_buffer_),
                       ImGuiInputTextFlags_EnterReturnsTrue)) {
    r_string command(cmd_input_buffer_);
    add_cmd_to_history_("> " + command);
    process_command_async_(command);
    memset(cmd_input_buffer_, 0, sizeof(cmd_input_buffer_));
    ImGui::SetKeyboardFocusHere(-1);
  }

  ImGui::End();

  // Destroy's the view when it get's closed
  if (!open_) RViewPool::get().destroy(this);
}

void RConsoleView::process_command_async_(const r_string& command) {
  std::istringstream iss(command);
  r_string cmd;
  iss >> cmd;

  auto cmd_ith = g_commands_.find(cmd);
  // Launch a thread for command execution
  std::jthread worker([this, cmd_ith, command]() -> void {
    // Check if it is a custom command or a system command and execute's it
    cmd_ith != g_commands_.end() ? cmd_ith->second(command)
                                 : exec_sys_cmd(command);
  });
  worker.detach();
}

void RConsoleView::exec_sys_cmd(const r_string& command) {
  // Handle's cd, help and clear (that are special commands because they
  // interact with either the process, g_commands_ or the cmds_history_)
  if (command.rfind("cd ", 0) == 0) {
    r_string new_path = command.substr(3);
    if (std::filesystem::exists(new_path) &&
        std::filesystem::is_directory(new_path)) {
      std::filesystem::current_path(new_path);
      add_cmd_to_history_("Changed directory to: " + new_path);
    } else {
      add_cmd_to_history_("Error: Invalid directory!");
    }
    return;
  }

  if (command.rfind("clear", 0) == 0) {
    std::lock_guard<std::mutex> lock(cmds_history_mtx_);
    cmds_history_.clear();
    return;
  }

  // TODO: Add "help" cmd

  r_string full_cmd = command + " 2>&1";

#ifdef _WIN32
  full_cmd = "cmd /c " + full_cmd;
#else
  fullCommand = "bash -c \"" + fullCommand + "\"";
#endif

  FILE* pipe = _popen(full_cmd.c_str(), "r");
  if (!pipe) {
    add_cmd_to_history_("Error executing command.");
    return;
  }

  char buffer[256];
  r_string result;
  while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
    result += buffer;
  }
  _pclose(pipe);

  add_cmd_to_history_(result);
}

void RConsoleView::add_cmd_to_history_(const r_string& command) {
  std::lock_guard<std::mutex> lock(cmds_history_mtx_);
  cmds_history_.emplace_back(command);
}
