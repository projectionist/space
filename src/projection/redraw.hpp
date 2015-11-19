#pragma once

#include <cassert>

#include <wayland-client.h>
#include <GLES2/gl2.h>
#include <EGL/egl.h>

#include <projection/display.hpp>

namespace projection {

  void redraw(void *data, struct wl_callback *callback, uint32_t);

  const struct wl_callback_listener frame_listener = {
    redraw
  };


  void redraw(void *data, struct wl_callback *callback, uint32_t)
  {
    struct display *display = (struct display *)data;

    struct wl_region *region;

    assert(display->callback == callback);
    display->callback = NULL;

    if (callback)
      wl_callback_destroy(callback);

    if (!display->configured)
      return;

    if (!display->drawing->initialized()) {
      display->drawing->initialize(display->geometry.width, display->geometry.height);
    }

    display->drawing->redraw();

    region = wl_compositor_create_region(display->compositor);
    wl_region_add(region, 0, 0,
            display->geometry.width,
            display->geometry.height);
    wl_surface_set_opaque_region(display->surface, region);
    wl_region_destroy(region);


    display->callback = wl_surface_frame(display->surface);
    wl_callback_add_listener(display->callback, &frame_listener, display);

    eglSwapBuffers(display->egl.dpy, display->egl_surface);
  }
}
