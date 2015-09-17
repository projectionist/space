#pragma once

#include <wayland-client.h>
#include <wayland-cursor.h>

#include <EGL/egl.h>

namespace projection {
  struct display {
    void *projection;
    struct wl_display *display;
    struct wl_registry *registry;
    struct wl_compositor *compositor;
    struct wl_shell *shell;
    struct wl_seat *seat;
    struct wl_keyboard *keyboard;
    struct wl_shm *shm;
    struct {
      EGLDisplay dpy;
      EGLContext ctx;
      EGLConfig conf;
    } egl;
    struct window *window;
  };
}
