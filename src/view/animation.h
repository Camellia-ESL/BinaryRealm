/*
 * Contains animation logic for views.
 *
 * NOTE: It's important to notice that when working with animation logic u need
 * to remember to properly time scale time values as they are not binded to a
 * fixed framerate and they need to be properly time scaled. (ex. you can watch
 * RAnimVal::duration_time_scaled)
 */

#pragma once

#include <math.h>

#include <chrono>
#include <functional>

/*
 * The interpolation type of animation
 */
enum class RAnimInterpolationType {
  LINEAR,
  QUADRATIC_BEZIER,
  CUBIC_BEZIER,
  EASE_OUT_CUBIC
};

/*
 * The state of an animated value
 */
enum class RAnimValState {
  STOPPED,
  PLAYING,
  ON_END_CALLBACK_PASS,
  ON_NEXT_ANIM_PASS,
  COMPLETED
};

/*
 * Represent's an animated value over time and frames
 */
class RAnimVal {
 public:
  using RClock = std::chrono::high_resolution_clock;
  using RTimePoint = std::chrono::time_point<RClock>;

  /*
   * Higher values generates animations refreshed more times per second
   * (visually it appears smoother)
   */
  static constexpr const float TIME_SCALE_FACTOR = 4.0f;

  /*
   * The animation time step per second
   */
  static constexpr const float FIXED_TIME_STEP =
      1.0f / (60.0f * TIME_SCALE_FACTOR);

  /*
   * Total animation time
   */
  float duration;

  /*
   * Elapsed time
   */
  float elapsed;

  /*
   * Start value and end value
   */
  float start_val, end_val;

  /*
   * The interpolation type
   */
  RAnimInterpolationType interp_type;

  /*
   * Bezier control points
   */
  float bezier_p1, bezier_p2;

  /*
   * How many times the animation was played
   */
  int play_count;

  /*
   * The start time point
   */
  RTimePoint start_t_point;

  RAnimVal(float start, float end, float time,
           RAnimInterpolationType interpType, float bp1 = 0.25f,
           float bp2 = 0.5f, float bp3 = 0.75f)
      : duration(time),
        elapsed(0.0f),
        start_val(start),
        end_val(end),
        interp_type(interpType),
        bezier_p1(bp1),
        bezier_p2(bp2),
        play_count(0) {}

  /*
   * Return's the interpolated value
   */
  float val();

  /*
   * Update's the whole animation state
   */
  void update();

  /*
   * Play's the animation
   */
  void play() {
    start_t_point = RClock::now();
    state_ = RAnimValState::PLAYING;
  }

  /*
   * Stop's the animation
   */
  void stop() { state_ = RAnimValState::STOPPED; }

  /*
   * Resume's the animation
   */
  void resume() { state_ = RAnimValState::PLAYING; }

  /*
   * Stop the animation and reset's it
   */
  void reset() {
    state_ = RAnimValState::COMPLETED;
    elapsed = 0.0f;
  }

  /*
   * Set the next animation to play once this one is finished
   */
  void concatenate(RAnimVal* next, float timeout = 0.0f) {
    next_ = next;
    next_timeout_ = timeout;
  }

  /*
   * Set's a listener that get's called when the animation ends.
   */
  void set_on_anim_end_callback(std::function<void()> callback,
                                float timeout = 0.0f) {
    on_anim_end_ = callback;
    on_anim_end_timeout_ = timeout;
  }

  /*
   * Get's the duration time scaled (useful to animate properly without relying
   * on a wrong scaled duration frame per second)
   */
  const float duration_time_scaled() const {
    return duration / TIME_SCALE_FACTOR;
  }

  /*
   * Get's the next timeout time scaled (useful to animate properly without
   * relying on a wrong scaled duration frame per second)
   */
  const float next_timeout_time_scaled() const {
    return next_timeout_ / TIME_SCALE_FACTOR;
  }

  /*
   * Get's the duration time scaled (useful to animate properly without relying
   * on a wrong scaled duration frame per second)
   */
  const float on_anim_end_timeout_time_scaled() const {
    return on_anim_end_timeout_ / TIME_SCALE_FACTOR;
  }

  /*
   * The state of the animation
   */
  const RAnimValState get_state() const { return state_; }

 private:
  RAnimValState state_ = RAnimValState::COMPLETED;
  RAnimVal* next_ = nullptr;
  float next_timeout_ = 0.0f;
  float accumulator_ = 0.0f;
  std::function<void()> on_anim_end_ = nullptr;
  float on_anim_end_timeout_ = 0.0f;

  float lerp_(float t);
  float quadratic_bezier_(float t);
  float cubic_bezier_(float t);
  float ease_out_cubic_(float t);
};
