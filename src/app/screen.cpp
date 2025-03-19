#include "screen.h"

#include "window_apis/win32_api.h"

void RScreen::init(RWindowApi win_api, RGraphicsApiType gfx_api) {
  win_api_ = win_api;
  gfx_api_ = gfx_api;
  if (win_api == RWindowApi::RWIN32)
    background_window_ = std::make_shared<RWin32Api>();

  // Init window system and graphics api
  background_window_->init(gfx_api, rect_.left, rect_.top, rect_.right,
                           rect_.bottom, false);
}
