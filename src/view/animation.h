/*
 * Contains animation logic for views
 */

#pragma once

#include <math.h>

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
  /*
   * The fixed delta time for the animation
   */
  static constexpr const float FIXED_DELTA = 1.0f / 144.0f;

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
  void update(float delta);

  /*
   * Play's the animation
   */
  void play() { is_playing = true; }

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

 private:
  float accumulator_ = 0.0f;

  float lerp_(float t);
  float quadratic_bezier_(float t);
  float cubic_bezier_(float t);
  float ease_out_cubic_(float t);
};
