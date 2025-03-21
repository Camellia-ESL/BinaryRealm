#include "image.h"

#include <memory>

#include "../app/app.h"

RResult<std::shared_ptr<RImage>> RImage::new_from_file(const r_string& path) {
  std::shared_ptr<RImage> img_res_buff = std::make_shared<RImage>();
  auto& screens = RApp::get().get_screens();

  // Load screens srvs
  for (auto& screen : screens) {
    // TODO: This is insanely inefficient as it loads an SRV per graphics
    // instance (allowing multiple copies of the same image) it would be better
    // to implement a shared SRV system on every native graphics api
    auto img_srv_load_res =
        screen->get_bg_window().get_gfx().load_img_from_file(path);
    if (!img_srv_load_res.ok())
      return RResult<std::shared_ptr<RImage>>::create_err(
          img_srv_load_res.err());
    img_res_buff->screen_srvs_.insert(
        {screen->get_monitor_index(), img_srv_load_res.val()});
  }

  // Load the host srv
  auto host_img_srv_load_res =
      RApp::get().get_host_win().get_gfx().load_img_from_file(path);
  if (!host_img_srv_load_res.ok())
    return RResult<std::shared_ptr<RImage>>::create_err(
        host_img_srv_load_res.err());
  img_res_buff->host_srv_ = host_img_srv_load_res.val();

  // Set image info
  img_res_buff->file_path_ = path;

  return RResult<std::shared_ptr<RImage>>::create_ok(img_res_buff);
}
