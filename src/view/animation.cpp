#include "animation.h"

float RAnimVal::val() {
  if (state_ >= RAnimValState::ON_END_CALLBACK_PASS) return end_val;
  float t = elapsed / duration_time_scaled();
  t = t > 1.0f ? 1.0f : t;

  switch (interp_type) {
    case RAnimInterpolationType::LINEAR:
      return lerp_(t);
    case RAnimInterpolationType::QUADRATIC_BEZIER:
      return quadratic_bezier_(t);
    case RAnimInterpolationType::CUBIC_BEZIER:
      return cubic_bezier_(t);
    case RAnimInterpolationType::EASE_OUT_CUBIC:
      return ease_out_cubic_(t);
    default:
      return end_val;
  }
}

void RAnimVal::update() {
  if (state_ == RAnimValState::COMPLETED) return;

  // Accumulate the real time into our accumulator
  if (state_ != RAnimValState::STOPPED)
    accumulator_ = (RClock::now() - start_t_point).count();

  // Update in fixed time steps
  if (accumulator_ >= FIXED_TIME_STEP) {
    elapsed += FIXED_TIME_STEP;
    accumulator_ = 0.0f;

    // Check for the end of this animation
    if (state_ == RAnimValState::PLAYING && elapsed >= duration_time_scaled()) {
      play_count++;
      elapsed = 0.0f;
      if (on_anim_end_)
        state_ = RAnimValState::ON_END_CALLBACK_PASS;
      else if (next_)
        state_ = RAnimValState::ON_NEXT_ANIM_PASS;
      else
        state_ = RAnimValState::COMPLETED;
    }

    // Check to play on anim end callback
    if (state_ == RAnimValState::ON_END_CALLBACK_PASS &&
        elapsed >= on_anim_end_timeout_time_scaled()) {
      elapsed = 0.0f;
      on_anim_end_();
      if (next_)
        state_ = RAnimValState::ON_NEXT_ANIM_PASS;
      else
        state_ = RAnimValState::COMPLETED;
    }

    // Check for the start of the next animation
    if (state_ == RAnimValState::ON_NEXT_ANIM_PASS &&
        elapsed >= next_timeout_) {
      next_->play();
      elapsed = 0.0f;
      state_ = RAnimValState::COMPLETED;
    }
  }
}

float RAnimVal::lerp_(float t) { return start_val + (end_val - start_val) * t; }

float RAnimVal::quadratic_bezier_(float t) {
  float u = 1 - t;
  return u * u * start_val + 2 * u * t * bezier_p1 + t * t * end_val;
}

float RAnimVal::cubic_bezier_(float t) {
  float u = 1 - t;
  float tt = t * t;
  float uu = u * u;
  float uuu = uu * u;
  float ttt = tt * t;

  return (uuu * start_val) + (3 * uu * t * bezier_p1) +
         (3 * u * tt * bezier_p2) + (ttt * end_val);
}

float RAnimVal::ease_out_cubic_(float t) { return 1 - pow(1 - t, 3); }
