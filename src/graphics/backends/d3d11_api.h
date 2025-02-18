#include "../graphics.h"

#include <d3d11.h>
#include <d3dcompiler.h>

// D3D11 graphics api abstraction
class RD3D11Api : public RIGraphicsApi {
public:
  RD3D11Api() = default;
  ~RD3D11Api() { destroy(); }

  // Init d3d11 instance
  bool init(RWindowNativeHandle p_native_handle) override;
  // Destroy the d3d11 instance
  void destroy() override;
  // Start's a new frame
  void begin_render() override;
  // End's a frame, render's and present
  void render() override;
  // Load's a d3d11 image resource shader view from file
  RResult<RImage> load_img_from_file(const std::string &path) override;

private:
  ID3D11Device *p_d3d_device = nullptr;
  ID3D11DeviceContext *p_d3d_device_context = nullptr;
  IDXGISwapChain *p_swapchain = nullptr;
  ID3D11RenderTargetView *p_main_render_target_view = nullptr;

  void init_render_target();
  void destroy_render_target();
};
