#pragma once

// Process all the keybinds to check if something was pressed
// NOTE: This function uses win32 to check inputs so it must be used within a
// valid win32 context
void r_process_keybinds_win32();
