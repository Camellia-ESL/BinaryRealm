/*
 * Contains animation logic for views.
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
  using RClock = std::chrono::steady_clock;
  using RTimePoint = RClock::time_point;

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
           RAnimInterpolationType interpolation, float bp1 = 0.25f,
           float bp2 = 0.5f, float bp3 = 0.75f, bool uncapped_fps = true)
      : duration(time),
        elapsed(0.0f),
        start_val(start),
        end_val(end),
        interp_type(interpolation),
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
  void play();
  /*
   * Stop's the animation
   */
  void stop();

  /*
   * Resume's the animation
   */
  void resume();

  /*
   * Stop the animation and reset's it
   */
  void reset();

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
