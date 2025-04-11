/*
 * Contains animation logic for views
 */

#pragma once

#include <math.h>

#include <chrono>

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
   * Whether the animation is playing or not
   */
  bool is_playing;

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
        is_playing(false),
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
    is_playing = true;
  }

  /*
   * Stop's the animation
   */
  void stop() { is_playing = false; }

  /*
   * Stop the animation and reset's it
   */
  void reset() {
    stop();
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
   * Get's the duration time scaled (useful to animate properly without relying
   * on a wrong scaled duration frame per second)
   */
  float duration_time_scaled() { return duration / TIME_SCALE_FACTOR; }

 private:
  RAnimVal* next_ = nullptr;
  float next_timeout_ = 0.0f;
  float accumulator_ = 0.0f;

  float lerp_(float t);
  float quadratic_bezier_(float t);
  float cubic_bezier_(float t);
  float ease_out_cubic_(float t);
};
