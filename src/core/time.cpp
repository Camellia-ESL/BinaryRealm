#include "time.h"

bool RFrameRateLimiter::should_update() {
  auto now = std::chrono::steady_clock::now();
  auto interval = std::chrono::milliseconds((int)(1000.0f / framerate_));

  if (now - last_t_point_ >= interval) {
    last_t_point_ = now;
    return true;
  }
  return false;
}
