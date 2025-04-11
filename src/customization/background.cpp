#include "background.h"

#include <filesystem>
#include <future>

#include "../../external/imgui/imgui.h"
#include "../config/config_manager.h"

const r_string RDesktopBackgroundManager::get_bg_images_dir_path() {
  return RConfigsManager::get().get_config_dir_path() + "\\backgrounds\\images";
}

void RDesktopBackgroundManager::update() {
  static RDesktopBackground* prev_bg = nullptr;
  if (prev_bg != cur_bg_) {
    prev_bg = cur_bg_;
    bg_change_anim_val_rot_1_.concatenate(&bg_change_anim_val_rot_2_);
    bg_change_anim_val_rot_1_.play();
  }

  bg_change_anim_val_rot_1_.update();
  bg_change_anim_val_rot_2_.update();
}

void RDesktopBackgroundManager::render(RScreen& screen) {
  // If there is no background image set simply returns
  if (!cur_bg_) return;

  auto& io = ImGui::GetIO();
  ImDrawList* draw_list = ImGui::GetForegroundDrawList();

  if ((bg_change_anim_val_rot_1_.is_playing ||
       bg_change_anim_val_rot_2_.is_playing) &&
      prev_bg_) {
    const ImVec2 display_size = ImGui::GetIO().DisplaySize;
    draw_list->AddImage((ImTextureID)prev_bg_->img->get_screen_srvs().at(
                            screen.get_monitor_index()),
                        {0.0f, 0.0f}, display_size);
    draw_list->AddImageQuad(
        (ImTextureID)cur_bg_->img->get_screen_srvs().at(
            screen.get_monitor_index()),
        {0.0f, 0.0f}, {display_size.x * bg_change_anim_val_rot_1_.val(), 0.0f},
        {display_size.x * bg_change_anim_val_rot_1_.val(),
         display_size.y * (bg_change_anim_val_rot_2_.is_playing
                               ? bg_change_anim_val_rot_2_.val()
                               : 0.0f)},
        {0.0f, display_size.y}, {0.0f, 0.0f}, {1.0f, 0.0f},
        {1.0f,
         (bg_change_anim_val_rot_2_.is_playing ? bg_change_anim_val_rot_2_.val()
                                               : 0.0f)},
        {0.0f, 1.0f});
  } else {
    draw_list->AddImage(
        (ImTextureID)cur_bg_->img->get_screen_srvs().at(
            screen.get_monitor_index()),
        {0.0f, 0.0f},
        ImGui::GetIO().DisplaySize * bg_change_anim_val_rot_1_.val());
  }
}

bool RDesktopBackgroundManager::load() {
  auto dir_read_res =
      RFilesystemUtils::get_files_in_dir(get_bg_images_dir_path());
  if (!dir_read_res.ok()) return false;

  // Clear the themes vector before loading
  loaded_bgs_.clear();

  // Loads themes from the config dir asynchronously
  std::vector<std::future<RResult<RDesktopBackground>>> futures;

  for (const auto& file_path : dir_read_res.val()) {
    futures.emplace_back(std::async(
        std::launch::async, [this, file_path]() -> RResult<RDesktopBackground> {
          auto texture_load_res = RImage::new_from_file(file_path.string());
          if (!texture_load_res.ok())
            return RResult<RDesktopBackground>::create_err(
                texture_load_res.err());

          return RResult<RDesktopBackground>::create_ok(
              {texture_load_res.val()});
        }));
  }

  // Collect results
  for (auto& fut : futures) {
    auto img_load_res = fut.get();
    if (!img_load_res.ok()) {
      std::cout << img_load_res.err().msg();
      continue;
    }
    loaded_bgs_.emplace_back(img_load_res.val());
  }

  set_background(&loaded_bgs_[0]);

  return true;
}
void RDesktopBackgroundManager::set_background(RDesktopBackground* bg) {
  prev_bg_ = cur_bg_;
  cur_bg_ = bg;
}

RResult<RDesktopBackground> RDesktopBackgroundManager::load_new_bg_from_file(
    const r_string& path) {
  // Tries to copy the texture in the config dir
  if (!RFilesystemUtils::copy_file(path, get_bg_images_dir_path()))
    return RResult<RDesktopBackground>::create_err(
        "Error copying the background in the configs!");

  // Tries to load the image
  auto texture_load_res =
      RImage::new_from_file(get_bg_images_dir_path() + "\\" +
                            std::filesystem::path(path).filename().string());
  if (!texture_load_res.ok())
    return RResult<RDesktopBackground>::create_err(texture_load_res.err());

  loaded_bgs_.emplace_back(texture_load_res.val());

  return RResult<RDesktopBackground>::create_ok({texture_load_res.val()});
}
