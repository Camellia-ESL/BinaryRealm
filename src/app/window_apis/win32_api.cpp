#include "win32_api.h"

#include <winuser.h>

#include "../../../external/imgui/imgui.h"
#include "../../graphics/backends/d3d11_api.h"

// Forward declaration
LRESULT WINAPI WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd,
                                                             UINT msg,
                                                             WPARAM wparam,
                                                             LPARAM lparam);

void set_window_on_bottom(HWND hwnd);

RWin32Api::RWin32Api() {}

RWin32Api::~RWin32Api() {
  ::DestroyWindow(hwnd_);
  ::UnregisterClassW(wc_.lpszClassName, wc_.hInstance);
}

bool RWin32Api::init(RGraphicsApiType gfx_api_type, int left, int top,
                     int right, int bottom, bool enable_viewports) {
  wc_ = {sizeof(wc_),
         CS_CLASSDC,
         WndProc,
         0L,
         0L,
         GetModuleHandle(nullptr),
         nullptr,
         nullptr,
         nullptr,
         nullptr,
         L"BinaryRealm",
         nullptr};
  ::RegisterClassExW(&wc_);
  hwnd_ =
      ::CreateWindowExW(WS_EX_LAYERED | WS_EX_TOOLWINDOW, wc_.lpszClassName,
                        L"BinaryRealm", WS_POPUP, left, top, right - left,
                        bottom - top, nullptr, nullptr, wc_.hInstance, nullptr);

  SetLayeredWindowAttributes(hwnd_, 0, 255, LWA_ALPHA);
  set_window_on_bottom(hwnd_);

  ::ShowWindow(hwnd_, SW_SHOWDEFAULT);
  ::UpdateWindow(hwnd_);

  // Init graphics api for the window
  if (gfx_api_type == RGraphicsApiType::D3D11)
    graphics_ = std::make_shared<RD3D11Api>();

  graphics_->init(get_native_handle(), left, top, right, bottom,
                  enable_viewports);

  return true;
}

bool RWin32Api::process_messages() {
  MSG msg;
  while (::PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE)) {
    ::TranslateMessage(&msg);
    ::DispatchMessage(&msg);
    if (msg.message == WM_QUIT) return false;
  }
  return true;
}

LRESULT WINAPI WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
  if (msg == WM_DESTROY) {
    ::PostQuitMessage(0);
    return 0;
  }

  if (msg == WM_CREATE || msg == WM_ACTIVATE) {
    set_window_on_bottom(hwnd);
  }

  // TODO: ImGui inputs should be correctly handled based on the context and the
  // HWND
  if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam)) return true;

  return ::DefWindowProcW(hwnd, msg, wparam, lparam);
}

void set_window_on_bottom(HWND hwnd) {
  SetWindowPos(hwnd, HWND_BOTTOM, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
}
