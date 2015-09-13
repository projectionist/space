#pragma once
#include <wayland-client.h>

#include <projection/geometry.hpp>
#include <projection/display.hpp>
#include <projection/window.hpp>

namespace projection {
  class projection {
    struct display display;
    struct window window;
    struct wl_registry_listener registry_listener;
    static void registry_handle_global(void *, struct wl_registry *, uint32_t , const char *, uint32_t);
    static void registry_handle_global_remove(void *, struct wl_registry *, uint32_t);
    static void init_egl(struct display *);
  public:
    explicit projection();
    ~projection();
  };
}
