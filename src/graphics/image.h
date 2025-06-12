#pragma once

#include <map>
#include <memory>

#include "../core/containers.h"

/*
 * Represent's a shared handle that can be used to fetch an RpImageSRV
 */
typedef void* RpSharedImageHandle;

/*
 * The image resource view abstracted (ex. D3D11ShaderResourceView* for d3d11)
 * it can be passed to ImGui to render images
 */
typedef void* RpImageSRV;

/*
 * Contains informations about a single loaded image resource SRV, width,
 * height, etc..
 */
struct RImageResource {
  RpSharedImageHandle shared_handle;
  int width, height;
};

/*
 * Represent's an abstracted image, it contains informations about the image,
 * the file used to load it, all the Image RSV per RendererAPI instance, etc..
 */
class RImage {
 public:
  /*
   * Load's an image from file and return's a pointer to it or an error if
   * something went wrong
   */
  static RResult<std::shared_ptr<RImage>> new_from_file(const r_string& path);

  /*
   * Get's the image srvs contained by the instance
   */
  const std::map<int, RpImageSRV>& get_screen_srvs() const {
    return screen_srvs_;
  }

  /*
   * Get's the host srv (it can be used within host windows, basically
   * viewports)
   */
  const RpImageSRV get_host_srv() const { return host_srv_; }

  /*
   * Get's the path to the file used to load the image
   */
  const r_string& get_file_path() const { return file_path_; }

  /*
   * Get's the image name based on the file path
   */
  const r_string& get_name() const { return file_name_; }

  /*
   * Get's the image width
   */
  const int get_width() const { return width_; }

  /*
   * Get's the image height
   */
  const int get_height() const { return height_; }

 private:
  r_string file_path_;
  r_string file_name_;
  int width_, height_;
  RpImageSRV host_srv_;
  RpSharedImageHandle shared_handle_;
  // screen_monitor_index - RpImageSRV keypair
  std::map<int, RpImageSRV> screen_srvs_;
};
