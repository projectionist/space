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
    struct wl_registry_listener registry_listener;
    static void registry_handle_global(void *, struct wl_registry *, uint32_t , const char *, uint32_t);
    static void registry_handle_global_remove(void *, struct wl_registry *, uint32_t);
    static void init_egl(struct display *);
    static void fini_egl(struct display *);
    static void create_surface(struct window *);
    static void destroy_surface(struct window *);
    static void handle_ping(void *, struct wl_shell_surface *, uint32_t);
    static void handle_popup_done(void *, struct wl_shell_surface *);
  public:
    explicit projection();
    ~projection();
    void run();
    void stop();
  };
}
