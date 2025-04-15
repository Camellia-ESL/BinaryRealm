#pragma once

#include <cstdlib>
#include <iostream>

/*
 * Assert's a condition, if it fails logs informations about the error and the
 * condition + a custom msg.
 */
#define r_assert(cond, msg)                                             \
  do {                                                                  \
    if (!(cond)) {                                                      \
      std::cerr << "[ASSERTION FAILED] " << __FILE__ << ":" << __LINE__ \
                << " in function " << __func__ << "\n"                  \
                << "  Condition: " << #cond << "\n"                     \
                << "  Message:   " << msg << "\n";                      \
      std::abort();                                                     \
    }                                                                   \
  } while (0)
