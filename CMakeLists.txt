cmake_minimum_required(VERSION 3.20)
project(BinaryRealm)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Find Windows SDK
find_path(D3D11_INCLUDE_DIR d3d11.h PATHS "$ENV{WindowsSdkDir}/Include")
find_library(D3D11_LIBRARY d3d11.lib PATHS "$ENV{WindowsSdkDir}/Lib")

# Add ImGui
add_library(imgui STATIC
    external/imgui/imgui.cpp
    external/imgui/imgui_draw.cpp
    external/imgui/imgui_tables.cpp
    external/imgui/imgui_widgets.cpp
    external/imgui/imgui_impl_win32.cpp
    external/imgui/imgui_impl_dx11.cpp
)
target_include_directories(imgui PRIVATE external/imgui)
target_link_libraries(imgui PRIVATE d3d11 dxgi d3dcompiler)

# Main executable
file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS src/*.cpp)
add_executable(BinaryRealm ${SRC_FILES})
target_include_directories(BinaryRealm PRIVATE external/stb external/nlohmannjson)
target_link_libraries(BinaryRealm PRIVATE imgui d3d11 dxgi d3dcompiler)
