#pragma once

#include <chrono>

/*
 * Helper class used to handle frame rate limiting of piecies of code, it keeps
 * track of the time elapsed and returns thru exposed functions whether if a
 * certain amount of time is elapsed or not.
 */
class RFrameRateLimiter {
 public:
  RFrameRateLimiter() : last_t_point_(std::chrono::steady_clock::now()) {}
  RFrameRateLimiter(float framerate)
      : framerate_(framerate),
        last_t_point_(std::chrono::steady_clock::now()) {}

  /*
   * Returns whether if the said piece of code should update or not (basically
   * if enough time is elapsed)
   *
   * NOTE: It also updates the frame rate limiter status.
   */
  bool should_update();

  /*
   * Set the framerate limit
   */
  void set_framerate(float framerate) { framerate_ = framerate; };

  const float get_framerate() const { return framerate_; }

 private:
  float framerate_;
  std::chrono::steady_clock::time_point last_t_point_;
};
