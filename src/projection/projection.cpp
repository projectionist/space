#include <cassert>
#include <cstring>
#include <iostream>
#include <unistd.h>

#include <wayland-client.h>

#include <GLES2/gl2.h>

#include <projection/projection.hpp>
#include <projection/shell_surface_listener.hpp>
#include <projection/configure_callback_listener.hpp>
#include <projection/funcs.hpp>
#include <projection/seat_listener.hpp>

using namespace std;

namespace projection {

  projection::projection():
    running(true),
    display(),
    window(),
    registry_listener()
  {
    window.display = &display;
    display.window = &window;
    display.projection = (void *)this;

    registry_listener.global = registry_handle_global;
    registry_listener.global_remove = registry_handle_global_remove;

    display.display = wl_display_connect(NULL);
    assert(display.display);

    display.registry = wl_display_get_registry(display.display);
    wl_registry_add_listener(display.registry, &registry_listener, &display);

    wl_display_dispatch(display.display);

    init_egl(&display);
    create_surface(&window);
    init_gl(&window);
  }
  projection::~projection () {
    destroy_surface(&window);
    fini_egl(&display);

    if (display.shell)
      wl_shell_destroy(display.shell);

    if (display.compositor)
      wl_compositor_destroy(display.compositor);

    wl_registry_destroy(display.registry);
    wl_display_flush(display.display);
    wl_display_disconnect(display.display);
  }

  void projection::run() {
    int ret = 0;
    while (running && ret != -1) {
      ret = wl_display_dispatch(display.display);
    }
  }

  void projection::stop() {
    running = false;
  }

  void projection::registry_handle_global(void *data, struct wl_registry *registry, uint32_t name, const char *interface, uint32_t)
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

  void projection::registry_handle_global_remove(void *, struct wl_registry *, uint32_t)
  {
  }

  void projection::init_egl(struct display *display)
  {
    static const EGLint context_attribs[] = {
      EGL_CONTEXT_CLIENT_VERSION, 2,
      EGL_NONE
    };

    EGLint config_attribs[] = {
      EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
      EGL_RED_SIZE, 1,
      EGL_GREEN_SIZE, 1,
      EGL_BLUE_SIZE, 1,
      EGL_ALPHA_SIZE, 0,
      EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
      EGL_NONE
    };

    EGLint major, minor, n;
    EGLBoolean ret;

    display->egl.dpy = eglGetDisplay((EGLNativeDisplayType)display->display);
    assert(display->egl.dpy);

    ret = eglInitialize(display->egl.dpy, &major, &minor);
    assert(ret == EGL_TRUE);
    ret = eglBindAPI(EGL_OPENGL_ES_API);
    assert(ret == EGL_TRUE);

    ret = eglChooseConfig(display->egl.dpy, config_attribs,
              &display->egl.conf, 1, &n);
    assert(ret && n == 1);

    display->egl.ctx = eglCreateContext(display->egl.dpy,
                display->egl.conf,
                EGL_NO_CONTEXT, context_attribs);
    assert(display->egl.ctx);
  }

  void projection::fini_egl(struct display *display)
  {
    /* Required, otherwise segfault in egl_dri2.c: dri2_make_current()
     * on eglReleaseThread(). */
    eglMakeCurrent(display->egl.dpy, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);

    eglTerminate(display->egl.dpy);
    eglReleaseThread();
  }

  void projection::create_surface(struct window *window)
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

  void projection::destroy_surface(struct window *window)
  {
    /* Required, otherwise segfault in egl_dri2.c: dri2_make_current()
     * on eglReleaseThread(). */
    eglMakeCurrent(window->display->egl.dpy, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);

    eglDestroySurface(window->display->egl.dpy, window->egl_surface);
    wl_egl_window_destroy(window->native);

    // if (window->xdg_surface)
    //   xdg_surface_destroy(window->xdg_surface);
    // if (window->display->ivi_application)
    //   ivi_surface_destroy(window->ivi_surface);
    wl_surface_destroy(window->surface);

    if (window->callback)
      wl_callback_destroy(window->callback);
  }
}
