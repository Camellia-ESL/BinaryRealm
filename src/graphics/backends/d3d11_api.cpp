#include "d3d11_api.h"
#include "../../../external/imgui/imgui.h"
#include "../../../external/imgui/imgui_impl_dx11.h"
#include "../../../external/imgui/imgui_impl_win32.h"

bool RD3D11Api::init(void *p_native_handle) {
  HWND hwnd = *((HWND *)p_native_handle);

  DXGI_SWAP_CHAIN_DESC sd = {};
  sd.BufferCount = 2;
  sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
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
      D3D11_SDK_VERSION, &sd, &p_swapchain, &p_d3d_device, &feature_lvl,
      &p_d3d_device_context);

  if (FAILED(res))
    return false;
  init_render_target();

  // Setup ImGui
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
  io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
  io.ConfigViewportsNoDefaultParent = false;

  ImGui::StyleColorsDark();
  ImGui_ImplWin32_Init(hwnd);
  ImGui_ImplDX11_Init(p_d3d_device, p_d3d_device_context);

  return true;
}

void RD3D11Api::init_render_target() {
  ID3D11Texture2D *p_back_buffer;
  p_swapchain->GetBuffer(0, IID_PPV_ARGS(&p_back_buffer));
  p_d3d_device->CreateRenderTargetView(p_back_buffer, nullptr,
                                       &p_main_render_target_view);
  p_back_buffer->Release();
}

void RD3D11Api::destroy_render_target() {
  if (p_main_render_target_view) {
    p_main_render_target_view->Release();
    p_main_render_target_view = nullptr;
  }
}

void RD3D11Api::destroy() {
  ImGui_ImplDX11_Shutdown();
  ImGui_ImplWin32_Shutdown();
  ImGui::DestroyContext();
  destroy_render_target();
  if (p_swapchain) {
    p_swapchain->Release();
    p_swapchain = nullptr;
  }
  if (p_d3d_device_context) {
    p_d3d_device_context->Release();
    p_d3d_device_context = nullptr;
  }
  if (p_d3d_device) {
    p_d3d_device->Release();
    p_d3d_device = nullptr;
  }
}

void RD3D11Api::begin_render() {
  ImGui_ImplDX11_NewFrame();
  ImGui_ImplWin32_NewFrame();
  ImGui::NewFrame();
}

void RD3D11Api::render() {
  ImGui::Render();
  float clear_color[4] = {0, 0, 0, 0};
  p_d3d_device_context->OMSetRenderTargets(1, &p_main_render_target_view,
                                           nullptr);
  p_d3d_device_context->ClearRenderTargetView(p_main_render_target_view,
                                              clear_color);
  ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
  if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
    ImGui::UpdatePlatformWindows();
    ImGui::RenderPlatformWindowsDefault();
  }

  p_swapchain->Present(1, 0);
}
