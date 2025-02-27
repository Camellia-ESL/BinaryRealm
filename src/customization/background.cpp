#include "background.h"

#include <filesystem>
#include <future>

#include "../../external/imgui/imgui.h"
#include "../app/app.h"
#include "../config/config_manager.h"

const r_string RDesktopBackgroundManager::get_bg_images_dir_path() {
  return RConfigsManager::get().get_config_dir_path() + "\\backgrounds\\images";
}

void RDesktopBackgroundManager::render() {
  // If there is no background image set simply returns
  if (!cur_bg_) return;

  auto& io = ImGui::GetIO();
  ImDrawList* draw_list = ImGui::GetBackgroundDrawList();

  const auto& monitors = RApp::get().get_window().get_monitors();

  // Render the monitors, adjusting each one to align
  for (size_t i = 0; i < monitors.size(); ++i) {
    // Calculate the new left and right coordinates based on the previous
    // monitor
    ImVec2 topLeft, bottomRight;

    if (i == 0) {
      // For the first monitor, use the original position
      topLeft =
          ImVec2((float)monitors[i].rect.left, (float)monitors[i].rect.top);
    } else {
      // For subsequent monitors, align the left to the previous monitor's right
      topLeft = ImVec2((float)(monitors[i - 1].rect.right),
                       (float)monitors[i].rect.top);
    }

    bottomRight =
        ImVec2((float)monitors[i].rect.right, (float)monitors[i].rect.bottom);

    draw_list->AddImage((ImTextureID)cur_bg_, topLeft, bottomRight);
  }
}

bool RDesktopBackgroundManager::load() {
  auto dir_read_res =
      RFilesystemUtils::get_files_in_dir(get_bg_images_dir_path());
  if (!dir_read_res.ok()) return false;

  // Clear the themes vector before loading
  loaded_bgs_.clear();

  // Loads themes from the config dir asynchronously
  std::vector<std::future<RResult<RImage>>> futures;

  for (const auto& file_path : dir_read_res.val()) {
    futures.emplace_back(
        std::async(std::launch::async, [file_path]() -> RResult<RImage> {
          return RApp::get().get_graphics().load_img_from_file(
              file_path.string());
        }));
  }

  // Collect results
  for (auto& fut : futures) {
    auto img_load_res = fut.get();
    if (!img_load_res.ok()) continue;
    loaded_bgs_.emplace_back(img_load_res.val());
  }

  return true;
}

RResult<RImage> RDesktopBackgroundManager::load_new_bg_from_file(
    const r_string& path) {
  // Tries to load the texture
  auto texture_load_res = RApp::get().get_graphics().load_img_from_file(path);
  if (!texture_load_res.ok()) return texture_load_res;

  // Tries to copy the texture in the config dir
  if (!RFilesystemUtils::copy_file(path, get_bg_images_dir_path()))
    return RResult<RImage>::create_err(
        RError{"Error copying the background in the configs!"});

  return texture_load_res;
}
