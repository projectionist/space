#pragma once

#include <wayland-client.h>
#include <wayland-egl.h>
#include <EGL/egl.h>
#include <GLES2/gl2.h>

#include <scratch/scratch.hpp>

namespace projection {
  struct display {
    struct wl_display *display;
    bool configured;
    bool stop_flag;
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
    struct {
      int width;
      int height;
    } geometry;
    struct wl_egl_window *native;
    struct wl_surface *surface;
    struct wl_shell_surface *shell_surface;
    EGLSurface egl_surface;
    struct wl_callback *callback;
    scratch::scratch drawing;
  };
}
