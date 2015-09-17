#pragma once

#include <stdio.h>

namespace projection {
  // void handle_ping(void *data, struct wl_shell_surface *shell_surface, uint32_t serial)
  void handle_ping(void *, struct wl_shell_surface *shell_surface, uint32_t serial)
  {
    wl_shell_surface_pong(shell_surface, serial);
  }

  // void handle_configure(void *data, struct wl_shell_surface *shell_surface, uint32_t edges, int32_t width, int32_t height)
  void handle_configure(void *data, struct wl_shell_surface *, uint32_t, int32_t width, int32_t height)
  {

    struct window *window = (struct window *)data;

    printf("configure: wxh: %dx%d\n", width, height);

    if (window->native)
      wl_egl_window_resize(window->native, width, height, 0, 0);

    window->geometry.width = width;
    window->geometry.height = height;
  }

  void handle_popup_done(void *, struct wl_shell_surface *)
  {
  }

  const struct wl_shell_surface_listener shell_surface_listener = {
    handle_ping,
    handle_configure,
    handle_popup_done
  };
}
