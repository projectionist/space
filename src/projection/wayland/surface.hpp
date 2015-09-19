#pragma once

namespace projection {
  namespace wayland {
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

    void create_surface(struct window *window)
    {
      struct display *display = window->display;
      EGLBoolean ret;

      window->surface = wl_compositor_create_surface(display->compositor);
      window->shell_surface = wl_shell_get_shell_surface(display->shell,
                     window->surface);

      wl_shell_surface_add_listener(window->shell_surface,
                  &shell_surface_listener, window);

      window->native = wl_egl_window_create(window->surface, 1, 1);

      window->egl_surface =
        eglCreateWindowSurface((EGLDisplay) display->egl.dpy, display->egl.conf, (EGLNativeWindowType) window->native, NULL);

      wl_shell_surface_set_title(window->shell_surface, "projection");

      ret = eglMakeCurrent(window->display->egl.dpy, window->egl_surface,
               window->egl_surface, window->display->egl.ctx);
      assert(ret == EGL_TRUE);

      struct wl_callback *callback;

      window->configured = 0;

      wl_shell_surface_set_fullscreen(window->shell_surface,
        WL_SHELL_SURFACE_FULLSCREEN_METHOD_DEFAULT, 0, NULL);

      callback = wl_display_sync(window->display->display);
      wl_callback_add_listener(callback, &configure_callback_listener, window);
    }

    void destroy_surface(struct window *window)
    {
      /* Required, otherwise segfault in egl_dri2.c: dri2_make_current()
       * on eglReleaseThread(). */
      eglMakeCurrent(window->display->egl.dpy, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);

      eglDestroySurface(window->display->egl.dpy, window->egl_surface);
      wl_egl_window_destroy(window->native);

      wl_surface_destroy(window->surface);

      if (window->callback)
        wl_callback_destroy(window->callback);
    }
  }
}
