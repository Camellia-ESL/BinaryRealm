#include "time.h"

#include "assert.h"

bool RFrameRateLimiter::should_update() {
  r_assert(
      framerate_ > 0.0f,
      "Framerate limit is set to 0.0f, did you forget the initialization?");

  auto now = std::chrono::steady_clock::now();
  auto interval = std::chrono::milliseconds((int)(1000.0f / framerate_));

  if (now - last_t_point_ >= interval) {
    last_t_point_ = now;
    return true;
  }
  return false;
}
