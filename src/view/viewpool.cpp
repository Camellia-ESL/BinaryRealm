#include "viewpool.h"

void RViewPool::render() {
  // The views_ vector is looped in this way to avoid crashes caused by erasing
  // elements from the vector that can cause problems to iterators while
  // looping. Looping it this way prevents any sort of crashes.
  int view_itx = 0;
  while (view_itx < views_.size()) {
    auto& view = views_[view_itx];
    if (view->update()) view->render();
    view_itx++;
  }
}

void RViewPool::destroy(RView* p_view) {
  std::erase_if(views_, [&](const std::unique_ptr<RView>& obj) {
    return obj.get() == p_view;
  });
}
