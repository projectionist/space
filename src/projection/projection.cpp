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

    window.window_size.width  = 250;
    window.window_size.height = 250;

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

  }

  void projection::run() {
    int ret = 0;
    while (running && ret != -1) {
      ret = wl_display_dispatch(display.display);
    }
  }

  void projection::stop() {
    cerr << "STOP!" << endl;
    running = false;
  }

  void projection::registry_handle_global(void *data, struct wl_registry *registry, uint32_t name, const char *interface, uint32_t)
  {
    struct display *display = (struct display *)data;

    if (strcmp(interface, "wl_compositor") == 0) {
      display->compositor = (wl_compositor *)wl_registry_bind(registry, name, &wl_compositor_interface, 1);
    } else if (strcmp(interface, "wl_shell") == 0) {
      display->shell = (wl_shell *)wl_registry_bind(registry, name, &wl_shell_interface, 1);
    } else if (strcmp(interface, "wl_shm") == 0) {
      display->shm = (wl_shm *)wl_registry_bind(registry, name, &wl_shm_interface, 1);
      display->cursor_theme = wl_cursor_theme_load(NULL, 32, display->shm);
      display->default_cursor = wl_cursor_theme_get_cursor(display->cursor_theme, "left_ptr");
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

  void projection::create_surface(struct window *window)
  {
    struct display *display = window->display;
    EGLBoolean ret;

    window->surface = wl_compositor_create_surface(display->compositor);
    window->shell_surface = wl_shell_get_shell_surface(display->shell,
                   window->surface);

    wl_shell_surface_add_listener(window->shell_surface,
                &shell_surface_listener, window);

    window->native =
      wl_egl_window_create(window->surface,
               window->window_size.width,
               window->window_size.height);
    window->egl_surface =
      eglCreateWindowSurface((EGLDisplay) display->egl.dpy, display->egl.conf, (EGLNativeWindowType) window->native, NULL);

    wl_shell_surface_set_title(window->shell_surface, "simple-egl");

    ret = eglMakeCurrent(window->display->egl.dpy, window->egl_surface,
             window->egl_surface, window->display->egl.ctx);
    assert(ret == EGL_TRUE);

    toggle_fullscreen(window, 1);
  }

  void projection::toggle_fullscreen(struct window *window, int fullscreen)
  {
  	struct wl_callback *callback;

  	window->fullscreen = fullscreen;
  	window->configured = 0;

  	if (fullscreen) {
  		wl_shell_surface_set_fullscreen(window->shell_surface,
  						WL_SHELL_SURFACE_FULLSCREEN_METHOD_DEFAULT,
  						0, NULL);
  	} else {
  		wl_shell_surface_set_toplevel(window->shell_surface);
  		handle_configure(window, window->shell_surface, 0,
  				 window->window_size.width,
  				 window->window_size.height);
  	}

  	callback = wl_display_sync(window->display->display);
  	wl_callback_add_listener(callback, &configure_callback_listener,
  				 window);
  }
}
