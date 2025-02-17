#include "app/app.h"

int main() {
  RApp::get().run(RWindowApi::RWIN32, RGraphicsApi::D3D11);
  return 0;
}
