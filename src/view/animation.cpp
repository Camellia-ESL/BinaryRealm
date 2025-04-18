#include "animation.h"

float RAnimVal::val() {
  if (state_ >= RAnimValState::ON_END_CALLBACK_PASS) return end_val;
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

void RAnimVal::play() {
  start_t_point = RClock::now();
  state_ = RAnimValState::PLAYING;
}

void RAnimVal::stop() { state_ = RAnimValState::STOPPED; }

void RAnimVal::resume() { state_ = RAnimValState::PLAYING; }

void RAnimVal::reset() {
  state_ = RAnimValState::COMPLETED;
  elapsed = 0.0f;
}

void RAnimVal::update() {
  if (state_ == RAnimValState::COMPLETED) return;

  auto now = RClock::now();
  std::chrono::duration<float> delta = now - start_t_point;
  start_t_point = now;

  elapsed += delta.count();

  if (state_ == RAnimValState::PLAYING && elapsed >= duration) {
    play_count++;
    elapsed = 0.0f;
    state_ = (on_anim_end_) ? RAnimValState::ON_END_CALLBACK_PASS
                            : (next_ ? RAnimValState::ON_NEXT_ANIM_PASS
                                     : RAnimValState::COMPLETED);
  }

  if (state_ == RAnimValState::ON_END_CALLBACK_PASS &&
      elapsed >= on_anim_end_timeout_) {
    elapsed = 0.0f;
    on_anim_end_();
    state_ =
        next_ ? RAnimValState::ON_NEXT_ANIM_PASS : RAnimValState::COMPLETED;
  }

  if (state_ == RAnimValState::ON_NEXT_ANIM_PASS && elapsed >= next_timeout_) {
    elapsed = 0.0f;
    next_->play();
    state_ = RAnimValState::COMPLETED;
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
