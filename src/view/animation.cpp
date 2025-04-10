#include "animation.h"

float RAnimVal::val() {
  if (!is_playing) return end_val;
  float t = elapsed / duration;
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

void RAnimVal::update(float delta) {
  if (next_ != nullptr) {
    if (!is_playing && (next_->play_count > 0 || next_->is_playing)) return;
  } else if (!is_playing)
    return;

  // Check for the end of this animation
  if (elapsed >= duration && is_playing) {
    play_count++;
    reset();
  }

  // Check for the end of the next animation
  if (elapsed >= next_timeout_ && !is_playing && next_) {
    next_->play();
    reset();
  }

  accumulator_ += delta;

  while (accumulator_ >= FIXED_DELTA) {
    elapsed += FIXED_DELTA;
    accumulator_ -= FIXED_DELTA;
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
