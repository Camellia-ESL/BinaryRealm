/*
 * This file contains an abstraction of graphics apis.
 *
 * Support:
 * - D3D11
 */

#pragma once

#include "../app/window.h"

// All the available graphics apis
enum RGraphicsApi { D3D11 };

// Represent's an abstracted graphics api backend
class RIGraphicsApi {
public:
  RIGraphicsApi() = default;
  ~RIGraphicsApi() { destroy(); }

  // Init a new instance
  virtual bool init(RWindowNativeHandle p_native_handle) { return false; }
  // Destroy the instance
  virtual void destroy() {}
  // Start's a new frame
  virtual void begin_render() {}
  // End's a frame, render's and present
  virtual void render() {}
};
