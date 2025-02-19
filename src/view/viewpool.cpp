#include "viewpool.h"

void RViewPool::render() {
  for (auto &view : views_) {
    view->render();
  }
}
