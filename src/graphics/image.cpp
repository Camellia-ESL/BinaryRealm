#include "image.h"

#include <filesystem>
#include <memory>

#include "../app/app.h"

RResult<std::shared_ptr<RImage>> RImage::new_from_file(const r_string& path) {
  std::shared_ptr<RImage> img_res_buff = std::make_shared<RImage>();
  auto& screens = RApp::get().get_screens();

  // Load the host srv
  auto shared_handle =
      RApp::get().get_host_win().get_gfx().load_img_from_file(path);
  if (!shared_handle.ok())
    return RResult<std::shared_ptr<RImage>>::create_err(shared_handle.err());

  img_res_buff->width_ = shared_handle.val().width;
  img_res_buff->height_ = shared_handle.val().height;
  img_res_buff->shared_handle_ = shared_handle.val().shared_handle;

  auto host_img_srv_load_res =
      RApp::get().get_host_win().get_gfx().get_img_from_shared_handle(
          shared_handle.val().shared_handle);
  if (!host_img_srv_load_res.ok())
    return RResult<std::shared_ptr<RImage>>::create_err(
        host_img_srv_load_res.err());
  img_res_buff->host_srv_ = host_img_srv_load_res.val();

  // Load screens srvs
  for (auto& screen : screens) {
    auto img_srv_load_res =
        screen->get_bg_window().get_gfx().get_img_from_shared_handle(
            shared_handle.val().shared_handle);
    if (!img_srv_load_res.ok())
      return RResult<std::shared_ptr<RImage>>::create_err(
          img_srv_load_res.err());
    img_res_buff->screen_srvs_.insert(
        {screen->get_monitor_index(), img_srv_load_res.val()});
  }

  // Set image info
  img_res_buff->file_path_ = path;
  img_res_buff->file_name_ = std::filesystem::path(path).filename().string();

  return RResult<std::shared_ptr<RImage>>::create_ok(img_res_buff);
}
