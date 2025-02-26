#pragma once
#include "../core/string.h"

// Represent's an abstraction of a basic view, it can be inherited to create
// custom views that can then be spawned using the viewpool. Everything spawned
// in the viewpool is later rendered on screen.
class RView {
 public:
  RView() = default;
  RView(const r_string& tag) : tag_{tag} {}
  virtual ~RView() = default;
  // Render function to be implemented by derived views
  virtual void render() = 0;
  // Each view should have a name
  virtual constexpr const char* get_name() const = 0;

  const r_string& get_tag() const { return tag_; }

 protected:
  r_string tag_;
};
