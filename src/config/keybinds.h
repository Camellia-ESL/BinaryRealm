/*
 * Exposes functions to check keybind states per frame and helpers for key
 * inputs
 */
#pragma once

/*
 * Process all the keybinds to check if something was pressed [PRIVATE_USAGE]
 */
void r_process_keybinds();

/*
 * Whether if the view close button is pressed in the current frame
 */
bool r_is_view_close_keybind_pressed();
