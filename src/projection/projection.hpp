#pragma once
#include <wayland-client.h>

#include <projection/display.hpp>

namespace projection {
  class projection {
    struct display display;
  public:
    explicit projection();
    ~projection();
    void run();
    void stop();
  };
}
