#include <d3d11.h>
#include <d3dcompiler.h>

#include "../graphics.h"

// D3D11 graphics api abstraction
class RD3D11Api : public RIGraphicsApi {
 public:
  RD3D11Api() = default;
  ~RD3D11Api() override { destroy(); }

  // Init d3d11 instance
  bool init(RWindowNativeHandle p_native_handle) override;
  // Destroy the d3d11 instance
  void destroy() override;
  // Start's a new frame
  void begin_render() override;
  // End's a frame, render's and present
  void render() override;
  // Load's a d3d11 image resource shader view from file
  RResult<RImage> load_img_from_file(const r_string& path) override;

 private:
  ID3D11Device* p_d3d_device_ = nullptr;
  ID3D11DeviceContext* p_d3d_device_context_ = nullptr;
  IDXGISwapChain* p_swapchain_ = nullptr;
  ID3D11RenderTargetView* p_main_render_target_view_ = nullptr;
  ID3D11BlendState* p_blend_state_ = nullptr;

  void init_render_target();
  void destroy_render_target();
};
