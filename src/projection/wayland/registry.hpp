#pragma once

#include <wayland-client.h>
#include <projection/display.hpp>
#include <projection/wayland/seat_listener.hpp>

namespace projection {
  namespace wayland {
    void registry_handle_global(void *data, struct wl_registry *registry, uint32_t name, const char *interface, uint32_t)
    {
      struct display *display = (struct display *)data;

      if (strcmp(interface, "wl_compositor") == 0) {
        display->compositor = (wl_compositor *)wl_registry_bind(registry, name, &wl_compositor_interface, 1);
      } else if (strcmp(interface, "wl_shell") == 0) {
        display->shell = (wl_shell *)wl_registry_bind(registry, name, &wl_shell_interface, 1);
      } else if (strcmp(interface, "wl_seat") == 0) {
        display->seat = (wl_seat *)wl_registry_bind(registry, name, &wl_seat_interface, 1);
        wl_seat_add_listener(display->seat, &seat_listener, display);
      } else if (strcmp(interface, "wl_shm") == 0) {
        display->shm = (wl_shm *)wl_registry_bind(registry, name, &wl_shm_interface, 1);
      }
    }

    void registry_handle_global_remove(void *, struct wl_registry *, uint32_t)
    {
    }

    const struct wl_registry_listener registry_listener = {
      registry_handle_global,
      registry_handle_global_remove
    };

    void init_registry(struct display *display)
    {
      display->registry = wl_display_get_registry(display->display);
      wl_registry_add_listener(display->registry, &registry_listener, display);
    }

    void destroy_registry(struct display* display)
    {
      if (display->shell) {
        wl_shell_destroy(display->shell);
      }

      if (display->compositor) {
        wl_compositor_destroy(display->compositor);
      }

      wl_registry_destroy(display->registry);
    }
  }
}
