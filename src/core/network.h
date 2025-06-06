#pragma once
#include <chrono>
#include <cstdint>
#include <string>
#include <vector>

#include "network_imports.h"
#include "string.h"

/*
 * Contains all the info about a network adapter
 */
struct RNetAdapter {
  /*
   * The adapter name
   */
  r_string name;
  /*
   * The network name to which the adapter is connected to
   */
  r_string network_name;
  /*
   * The network interface index
   */
  uint32_t index;
  /*
   * The current dowload usage in kbps
   */
  double download_kbps = 0.0;
  /*
   * The current upload usage in kbps
   */
  double upload_kpbs = 0.0;
};

/*
 * Handles data fetching from network adapters installed
 */
class RNetworkAdaptersManager {
 public:
  RNetworkAdaptersManager();
  /*
   * Update's data about network adapters
   */
  void update();

  /*
   * Get's all the the various network adapters
   */
  const std::vector<RNetAdapter>& get_adapters() const;

 private:
  struct NetSnapshot {
    uint32_t index;
    uint64_t in_octets = 0;
    uint64_t out_octets = 0;
  };

  std::vector<RNetAdapter> adapters_;
  std::vector<NetSnapshot> last_snapshots_;
  std::chrono::steady_clock::time_point last_updated_;
  std::chrono::steady_clock::time_point last_refresh_;

  void refresh_adapters();
};
