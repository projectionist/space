#pragma once
#include <wayland-client.h>

#include <projection/geometry.hpp>
#include <projection/display.hpp>
#include <projection/window.hpp>

namespace projection {
  class projection {
    bool running;
    struct display display;
    struct window window;
  public:
    explicit projection();
    ~projection();
    void run();
    void stop();
  };
}
