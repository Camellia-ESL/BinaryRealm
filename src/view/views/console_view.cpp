
#include "console_view.h"

#include <filesystem>
#include <memory>
#include <regex>
#include <sstream>

#include "../../../external/imgui/imgui.h"
#include "../../config/config_manager.h"

/*
 * Register a new command.
 */
#define r_register_cmd(cmd_name, callback, description_str, usage_str) \
  RConsoleCommand cmd_name##_info{};                                   \
  cmd_name##_info.executor = callback;                                 \
  cmd_name##_info.description = description_str;                       \
  cmd_name##_info.usage = usage_str;                                   \
  r_register_command(#cmd_name, cmd_name##_info);

/*
 * Contains all the info about a single console executable command.
 */
struct RConsoleCommand {
  /* The description of the command */
  r_string description;
  /* The usage of the command */
  r_string usage;
  /* The function to execute the command */
  std::function<void(const r_string&, RConsoleView&, RConsoleCommand&)>
      executor;
};

/*
 * Private Global variable that contains all the custom commands that can be
 * executed by the console.
 */
std::unique_ptr<std::unordered_map<r_string, RConsoleCommand>> r_g_commands_;

/*
 * Register a new command.
 */
inline constexpr void r_register_command(const r_string& name,
                                         const RConsoleCommand& cmd_info) {
  (*r_g_commands_)[name] = cmd_info;
}

/*
 * Return's all the flags inside a command string
 */
static std::vector<r_string> r_extract_cmd_flags(const r_string& args) {
  std::vector<r_string> arguments;
  std::regex arg_pattern(R"(-\w+)");
  std::sregex_iterator it(args.begin(), args.end(), arg_pattern);
  std::sregex_iterator end;

  while (it != end) {
    arguments.emplace_back(it->str());
    ++it;
  }

  return arguments;
}

void RConsoleView::on_spawn() {
  RWindowView::on_spawn();
  working_dir_path_ = std::filesystem::current_path().string();
}
void RConsoleView::render() {
  begin_frame(ImGuiWindowFlags_NoScrollbar);

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
  ImGui::TextUnformatted(working_dir_path_.c_str());
  ImGui::SameLine();
  ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
  if (ImGui::InputText("##input", cmd_input_buffer_,
                       IM_ARRAYSIZE(cmd_input_buffer_),
                       ImGuiInputTextFlags_EnterReturnsTrue)) {
    r_string command(cmd_input_buffer_);
    add_exec_res_to_history_("> " + command);
    process_command_async_(command);
    memset(cmd_input_buffer_, 0, sizeof(cmd_input_buffer_));
    ImGui::SetKeyboardFocusHere(-1);
  }

  end_frame();
}

void RConsoleView::process_command_async_(const r_string& command) {
  std::istringstream iss(command);
  r_string cmd;
  iss >> cmd;

  auto cmd_ith = r_g_commands_->find(cmd);
  // Launch a thread for command execution
  std::jthread worker([this, cmd_ith, command]() -> void {
    // Check if it is a custom command or a system command and execute's it
    cmd_ith != r_g_commands_->end()
        ? cmd_ith->second.executor(command, *this, cmd_ith->second)
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
      working_dir_path_ = std::filesystem::current_path().string();
      add_exec_res_to_history_("Changed directory to: " + new_path);
    } else {
      add_exec_res_to_history_("Error: Invalid directory!");
    }
    return;
  }

  if (command.rfind("clear", 0) == 0) {
    std::lock_guard<std::mutex> lock(cmds_history_mtx_);
    cmds_history_.clear();
    return;
  }

  r_string full_cmd = command + " 2>&1";

#ifdef _WIN32
  full_cmd = "cmd /c " + full_cmd;
#else
  fullCommand = "bash -c \"" + fullCommand + "\"";
#endif

  FILE* pipe = _popen(full_cmd.c_str(), "r");
  if (!pipe) {
    add_exec_res_to_history_("Error executing command.");
    return;
  }

  char buffer[256];
  r_string result;
  while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
    result += buffer;
  }
  _pclose(pipe);

  add_exec_res_to_history_(result);
}

void RConsoleView::add_exec_res_to_history_(const r_string& command) {
  std::lock_guard<std::mutex> lock(cmds_history_mtx_);
  cmds_history_.emplace_back(command);
}

static void rlmshow_executor(const r_string& cmd_string, RConsoleView& console,
                             RConsoleCommand& command) {
  auto flags = r_extract_cmd_flags(cmd_string);

  // If flags are empty show's the usage
  if (flags.empty()) {
    console.add_exec_res_to_history_(command.description + "\n" +
                                     command.usage);
    return;
  }

  r_string exec_res_str = "";

  for (r_string& flag : flags) {
    // Shows all the available themes
    if (flag == "-themes") {
      auto& themes = RConfigsManager::get().get_theme_mngr().get_themes();
      for (auto& theme : themes) {
        exec_res_str += "--> " + theme->name + "\n";
      }
      continue;
    }

    // Nothing was found shows an error
    exec_res_str += "flag: " + flag + "was not found!";
  }

  // Print's the result to screen
  console.add_exec_res_to_history_(exec_res_str);
}

void r_register_console_cmds() {
  if (!r_g_commands_) {
    r_g_commands_ =
        std::make_unique<std::unordered_map<r_string, RConsoleCommand>>();
  }

  r_g_commands_->clear();

  // TODO: Add "help" cmd

  r_register_cmd(rlmshow, rlmshow_executor,
                 "Shows informations about Binary Realm", R"(
    rlmshow [flags]
    
    [flags] --> an array of flags.

    <available flags>:
    -themes --> shows all the available themes.
  )");
}
