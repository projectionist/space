#pragma once


#include <wayland-egl.h>

namespace projection {
  // void handle_ping(void *data, struct wl_shell_surface *shell_surface, uint32_t serial)
  void handle_ping(void *, struct wl_shell_surface *shell_surface, uint32_t serial)
  {
    wl_shell_surface_pong(shell_surface, serial);
  }

  // void handle_configure(void *data, struct wl_shell_surface *shell_surface, uint32_t edges, int32_t width, int32_t height)
  void handle_configure(void *data, struct wl_shell_surface *, uint32_t, int32_t width, int32_t height)
  {
    struct display *display = (struct display *)data;

    if (display->native)
      wl_egl_window_resize(display->native, width, height, 0, 0);

    display->geometry.width = width;
    display->geometry.height = height;
  }

  void handle_popup_done(void *, struct wl_shell_surface *)
  {
  }

  const struct wl_shell_surface_listener shell_surface_listener = {
    handle_ping,
    handle_configure,
    handle_popup_done
  };

  void create_surface(struct display *display)
  {
    EGLBoolean ret;

    display->surface = wl_compositor_create_surface(display->compositor);
    display->shell_surface = wl_shell_get_shell_surface(display->shell,
                   display->surface);

    wl_shell_surface_add_listener(display->shell_surface,
                &shell_surface_listener, display);

    display->native = wl_egl_window_create(display->surface, 1, 1);

    display->egl_surface =
      eglCreateWindowSurface((EGLDisplay) display->egl.dpy, display->egl.conf, (EGLNativeWindowType) display->native, NULL);

    wl_shell_surface_set_title(display->shell_surface, "projection");

    ret = eglMakeCurrent(display->egl.dpy, display->egl_surface,
             display->egl_surface, display->egl.ctx);
    assert(ret == EGL_TRUE);

    struct wl_callback *callback;

    display->configured = 0;

    wl_shell_surface_set_fullscreen(display->shell_surface,
      WL_SHELL_SURFACE_FULLSCREEN_METHOD_DEFAULT, 0, NULL);

    callback = wl_display_sync(display->display);
    wl_callback_add_listener(callback, &configure_callback_listener, display);
  }

  void destroy_surface(struct display *display)
  {
    /* Required, otherwise segfault in egl_dri2.c: dri2_make_current()
     * on eglReleaseThread(). */
    eglMakeCurrent(display->egl.dpy, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);

    eglDestroySurface(display->egl.dpy, display->egl_surface);
    wl_egl_window_destroy(display->native);

    wl_surface_destroy(display->surface);

    if (display->callback)
      wl_callback_destroy(display->callback);
  }
}
