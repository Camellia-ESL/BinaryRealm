#pragma once

#include <map>
#include <memory>

#include "../core/containers.h"

/*
 * The image resource view abstracted (ex. D3D11ShaderResourceView* for d3d11)
 * it can be passed to ImGui to render images
 */
typedef void* RpImageSRV;

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

 private:
  RpImageSRV host_srv_;
  // screen_monitor_index - RpImageSRV keypair
  std::map<int, RpImageSRV> screen_srvs_;
};
