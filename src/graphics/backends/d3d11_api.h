#include <d3d11.h>
#include <d3dcompiler.h>

#include "../graphics.h"

/*
 * D3D11 graphics api abstraction
 */
class RD3D11Api : public RIGraphicsApi {
 public:
  RD3D11Api() = default;
  ~RD3D11Api() override { destroy(); }

  bool init(void* p_native_handle, int left, int top, int right, int bottom,
            bool enable_viewports) override;
  void destroy() override;
  void begin_render() override;
  void render() override;
  RResult<RpSharedImageHandle> load_img_from_file(
      const r_string& path) override;
  RResult<RpImageSRV> get_img_from_shared_handle(
      RpSharedImageHandle handle) override;

 private:
  ID3D11Device* p_d3d_device_ = nullptr;
  ID3D11DeviceContext* p_d3d_device_context_ = nullptr;
  IDXGISwapChain* p_swapchain_ = nullptr;
  ID3D11RenderTargetView* p_main_render_target_view_ = nullptr;
  ID3D11BlendState* p_blend_state_ = nullptr;

  void init_render_target();
  void destroy_render_target();
};
