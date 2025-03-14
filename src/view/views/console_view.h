
#pragma once

#include <deque>
#include <functional>
#include <mutex>

#include "../view.h"

/*
 * Allocate an heap map and register all the available custom executable
 * commands inside of it.
 */
void r_register_console_cmds();

/*
 * A custom console that can execute normal console commands + custom commands
 */
class RConsoleView : public RView {
 public:
  using RView::RView;
  /*
   * Render function to be implemented by derived views
   */
  void render() override;
  /*
   * Return's settings view name
   */
  constexpr const char* get_name() const override { return "Console View"; };

  /*
   * Safely add's an element to the cmds_history_ [THREAD SAFE]
   */
  void add_exec_res_to_history_(const r_string& command);

 private:
  /*
   * When set to false destroy's the window
   */
  bool open_ = true;
  char cmd_input_buffer_[256] = {0};
  std::deque<r_string> cmds_history_;
  std::mutex cmds_history_mtx_;

  /*
   * Process a command asynchronously and thread safe [THREAD SAFE]
   */
  void process_command_async_(const r_string& command);
  /*
   * Execute's a system command synchronously
   */
  void exec_sys_cmd(const r_string& command);
};
