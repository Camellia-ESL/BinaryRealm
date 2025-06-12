#include "d3d11_api.h"

#include "../../../external/imgui/imgui.h"
#include "../../../external/imgui/imgui_impl_dx11.h"
#include "../../../external/imgui/imgui_impl_win32.h"
#include "../../../external/stb/stb_image.h"
#include "../../config/config_manager.h"

bool RD3D11Api::init(void* p_native_handle, int left, int top, int right,
                     int bottom, bool enable_viewports) {
  HWND hwnd = *((HWND*)p_native_handle);

  DXGI_SWAP_CHAIN_DESC sd = {};
  sd.BufferCount = 2;
  sd.BufferDesc.Width = right - left;
  sd.BufferDesc.Height = bottom - top;
  sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
  sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
  sd.OutputWindow = hwnd;
  sd.SampleDesc.Count = 1;
  sd.Windowed = TRUE;
  sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

  D3D_FEATURE_LEVEL feature_lvl;
  const D3D_FEATURE_LEVEL feature_lvls[] = {D3D_FEATURE_LEVEL_11_0,
                                            D3D_FEATURE_LEVEL_10_0};

  HRESULT res = D3D11CreateDeviceAndSwapChain(
      nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, feature_lvls, 2,
      D3D11_SDK_VERSION, &sd, &p_swapchain_, &p_d3d_device_, &feature_lvl,
      &p_d3d_device_context_);

  if (FAILED(res)) return false;
  init_render_target();

  // Blend state creation
  D3D11_BLEND_DESC desc;
  ZeroMemory(&desc, sizeof(desc));
  desc.AlphaToCoverageEnable = false;
  desc.RenderTarget[0].BlendEnable = true;
  desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
  desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
  desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
  desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_INV_DEST_ALPHA;
  desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
  desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
  desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

  // Create the blend state
  HRESULT hr = p_d3d_device_->CreateBlendState(&desc, &p_blend_state_);
  if (FAILED(hr)) return false;

  init_imgui_(enable_viewports);

  ImGui_ImplWin32_Init(hwnd);
  ImGui_ImplDX11_Init(p_d3d_device_, p_d3d_device_context_);

  return true;
}

void RD3D11Api::init_render_target() {
  ID3D11Texture2D* p_back_buffer;
  p_swapchain_->GetBuffer(0, IID_PPV_ARGS(&p_back_buffer));
  p_d3d_device_->CreateRenderTargetView(p_back_buffer, nullptr,
                                        &p_main_render_target_view_);
  p_back_buffer->Release();
}

void RD3D11Api::destroy_render_target() {
  if (p_main_render_target_view_) {
    p_main_render_target_view_->Release();
    p_main_render_target_view_ = nullptr;
  }
}

void RD3D11Api::destroy() {
  ImGui_ImplDX11_Shutdown();
  ImGui_ImplWin32_Shutdown();
  ImGui::DestroyContext();
  destroy_render_target();
  if (p_swapchain_) {
    p_swapchain_->Release();
    p_swapchain_ = nullptr;
  }
  if (p_d3d_device_context_) {
    p_d3d_device_context_->Release();
    p_d3d_device_context_ = nullptr;
  }
  if (p_d3d_device_) {
    p_d3d_device_->Release();
    p_d3d_device_ = nullptr;
  }
}

void RD3D11Api::begin_render() {
  ImGui::SetCurrentContext(imgui_ctx_);
  ImGui_ImplDX11_NewFrame();
  ImGui_ImplWin32_NewFrame();
  ImGui::NewFrame();
}

void RD3D11Api::render() {
  ImGui::Render();
  constexpr const float clear_color[4] = {0.0f, 0.0f, 0.0f, 0.0f};
  p_d3d_device_context_->OMSetRenderTargets(1, &p_main_render_target_view_,
                                            nullptr);
  p_d3d_device_context_->ClearRenderTargetView(p_main_render_target_view_,
                                               clear_color);
  p_d3d_device_context_->OMSetBlendState(p_blend_state_, nullptr, 0xFFFFFFFF);
  ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
  if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
    ImGui::UpdatePlatformWindows();
    ImGui::RenderPlatformWindowsDefault();
  }

  p_swapchain_->Present(
      RConfigsManager::get().get_app_settings_mngr().get_app_settings().vsync,
      0);
}

RResult<RImageResource> RD3D11Api::load_img_from_file(const r_string& path) {
  int width, height, channels;

  // Load the image from file
  unsigned char* data = stbi_load(path.c_str(), &width, &height, &channels, 4);

  // Create a texture description
  D3D11_TEXTURE2D_DESC tex_desc = {};
  tex_desc.Width = width;
  tex_desc.Height = height;
  tex_desc.MipLevels = 1;
  tex_desc.ArraySize = 1;
  tex_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
  tex_desc.SampleDesc.Count = 1;
  tex_desc.Usage = D3D11_USAGE_DEFAULT;
  tex_desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
  tex_desc.CPUAccessFlags = 0;
  tex_desc.MiscFlags = D3D11_RESOURCE_MISC_SHARED;

  // Create texture
  D3D11_SUBRESOURCE_DATA init_data = {};
  init_data.pSysMem = data;
  init_data.SysMemPitch = width * 4;  // RGBA
  init_data.SysMemSlicePitch = 0;

  ID3D11Texture2D* texture = nullptr;
  HRESULT hr = p_d3d_device_->CreateTexture2D(&tex_desc, &init_data, &texture);

  stbi_image_free(data);
  if (FAILED(hr))
    return RResult<RImageResource>::create_err("Failed to create texture");

  // Get shared handle
  HANDLE shared_handle = nullptr;
  IDXGIResource* dxgi_resource = nullptr;
  hr = texture->QueryInterface(__uuidof(IDXGIResource), (void**)&dxgi_resource);

  if (SUCCEEDED(hr)) {
    dxgi_resource->GetSharedHandle(&shared_handle);
    dxgi_resource->Release();
  } else {
    texture->Release();
    return RResult<RImageResource>::create_err("Failed to query IDXGIResource");
  }

  // Create shader resource view
  ID3D11ShaderResourceView* texture_srv = nullptr;
  hr = p_d3d_device_->CreateShaderResourceView(texture, nullptr, &texture_srv);

  // Release the texture (it's no longer needed after creating the SRV)
  texture->Release();

  if (FAILED(hr))
    return RResult<RImageResource>::create_err(
        "Failed to create shader resource view");

  return RResult<RImageResource>::create_ok(
      {(RpImageSRV)shared_handle, width, height});
}

RResult<RpImageSRV> RD3D11Api::get_img_from_shared_handle(
    RpSharedImageHandle handle) {
  ID3D11Resource* shared_resource = nullptr;
  HRESULT hr = p_d3d_device_->OpenSharedResource(
      handle, __uuidof(ID3D11Resource), (void**)&shared_resource);

  if (SUCCEEDED(hr)) {
    ID3D11ShaderResourceView* shared_srv = nullptr;
    p_d3d_device_->CreateShaderResourceView(shared_resource, nullptr,
                                            &shared_srv);

    return RResult<RpImageSRV>::create_ok((RpImageSRV)shared_srv);
    shared_resource->Release();
  }
  return RResult<RpImageSRV>::create_err(
      "Failed to create shader resource view");
}
