#pragma once

#include <memory>
#include <vector>

#include "view.h"

// [SINGLETON] Manages interactions with views. Spawns, destroys and render
// them.
class RViewPool {
 public:
  // Get's the instance of the singleton
  static RViewPool& get() {
    static RViewPool instance;
    return instance;
  }

  // Spawn a new view
  template <typename T>
  void spawn(const r_string& tag = "") {
    views_.push_back(std::make_unique<T>(tag));
  }

  // Destroy a view
  void destroy(RView* p_view);

  // Render all views
  void render();

 private:
  std::vector<std::unique_ptr<RView>> views_;
};
