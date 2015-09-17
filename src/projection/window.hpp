#pragma once

#include <wayland-egl.h>
#include <GLES2/gl2.h>
#include <EGL/egl.h>

namespace projection {
  struct window {
    struct display *display;
    struct geometry geometry;
    struct {
      GLuint rotation_uniform;
      GLuint pos;
      GLuint col;
    } gl;

    struct wl_egl_window *native;
    struct wl_surface *surface;
    struct wl_shell_surface *shell_surface;
    EGLSurface egl_surface;
    struct wl_callback *callback;
    int configured;
  };
}
