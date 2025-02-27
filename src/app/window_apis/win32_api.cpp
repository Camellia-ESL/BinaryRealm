#include "win32_api.h"

#include <winuser.h>

#include <algorithm>

#include "../../../external/imgui/imgui.h"

// Forward declaration
LRESULT WINAPI WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd,
                                                             UINT msg,
                                                             WPARAM wparam,
                                                             LPARAM lparam);

RWin32Api::RWin32Api() {}

RWin32Api::~RWin32Api() {
  ::DestroyWindow(hwnd);
  ::UnregisterClassW(wc.lpszClassName, wc.hInstance);
}

bool RWin32Api::init() {
  wc = {sizeof(wc),
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
  ::RegisterClassExW(&wc);

  hwnd = ::CreateWindowExW(
      WS_EX_LAYERED | WS_EX_NOACTIVATE, wc.lpszClassName, L"BinaryRealm",
      WS_POPUP, 0, 0, GetSystemMetrics(SM_CXSCREEN),
      GetSystemMetrics(SM_CYSCREEN), nullptr, nullptr, wc.hInstance, nullptr);

  SetLayeredWindowAttributes(hwnd, 0, 255, LWA_ALPHA);
  SetWindowPos(hwnd, HWND_BOTTOM, 0, 0, 0, 0,
               SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);

  ::ShowWindow(hwnd, SW_SHOWDEFAULT);
  ::UpdateWindow(hwnd);

  fetch_monitors();

  return true;
}

void RWin32Api::process_messages() {
  MSG msg;
  while (::PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE)) {
    ::TranslateMessage(&msg);
    ::DispatchMessage(&msg);
    if (msg.message == WM_QUIT) done = true;
  }
}

// Callback function for EnumDisplayMonitors
static BOOL CALLBACK MonitorEnumProc(HMONITOR hMonitor, HDC hdcMonitor,
                                     LPRECT lprcMonitor, LPARAM dwData) {
  auto monitors = reinterpret_cast<std::vector<RMonitor>*>(dwData);

  RMonitorRect rect{*(RMonitorRect*)lprcMonitor};
  RMonitor monitor{rect};

  monitors->push_back(monitor);
  return TRUE;
}

void RWin32Api::fetch_monitors() {
  EnumDisplayMonitors(NULL, NULL, MonitorEnumProc,
                      reinterpret_cast<LPARAM>(&monitors_));

  // Sort monitors by their 'left' coordinate
  std::sort(monitors_.begin(), monitors_.end(),
            [](const RMonitor& a, const RMonitor& b) {
              return a.rect.left < b.rect.left;
            });
}

LRESULT WINAPI WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
  if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam)) return true;

  if (msg == WM_DESTROY) {
    ::PostQuitMessage(0);
    return 0;
  }

  if (msg == WM_CREATE || msg == WM_ACTIVATE) {
    SetWindowPos(hwnd, HWND_BOTTOM, 0, 0, 0, 0,
                 SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
  }

  return ::DefWindowProcW(hwnd, msg, wparam, lparam);
}
