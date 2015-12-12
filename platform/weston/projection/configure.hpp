#pragma once

#include "redraw.hpp"

namespace projection {
  namespace weston {

    void configure_callback(void *data, struct wl_callback *callback, uint32_t time)
    {
      struct display *display = (struct display *)data;

      wl_callback_destroy(callback);

      display->configured = true;

      if (display->callback == NULL) {
        redraw(data, NULL, time);
      }
    }

    struct wl_callback_listener configure_callback_listener = {
      configure_callback
    };
  }
}
