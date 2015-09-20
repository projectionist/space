#pragma once
#include <projection/funcs.hpp>

namespace projection {
  void configure_callback(void *data, struct wl_callback *callback, uint32_t  time)
  {
    struct display *display = (struct display *)data;

    wl_callback_destroy(callback);

    display->configured = 1;

    if (display->callback == NULL)
      redraw(data, NULL, time);
  }

  struct wl_callback_listener configure_callback_listener = {
    configure_callback
  };
}
