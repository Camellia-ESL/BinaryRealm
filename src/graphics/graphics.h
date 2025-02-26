/*
 * This file contains an abstraction of graphics apis.
 *
 * Support:
 * - D3D11
 */

#pragma once

#include "../app/window.h"
#include "../core/containers.h"

// The image resource view abstracted (ex. D3D11ShaderResourceView* for d3d11)
// it can be passed to ImGui to render images
typedef void* RImage;

// All the available graphics apis
enum RGraphicsApi { D3D11 };

// Represent's an abstracted graphics api backend
class RIGraphicsApi {
 public:
  RIGraphicsApi() = default;
  virtual ~RIGraphicsApi() {}

  // Init a new instance
  virtual bool init(RWindowNativeHandle p_native_handle) = 0;
  // Destroy the instance
  virtual void destroy() = 0;
  // Start's a new frame
  virtual void begin_render() = 0;
  // End's a frame, render's and present
  virtual void render() = 0;
  // Load's an image from file then returns the texture resource ptr (ex. d3d11
  // returns D3D11ShaderResourceView*) ready to be passed to ImGui::Image()
  virtual RResult<RImage> load_img_from_file(const r_string& path) = 0;
};
