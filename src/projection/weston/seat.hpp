#pragma once

#include <linux/input.h>

#include <iostream>

using namespace std;

namespace projection {
  namespace weston {

    void keyboard_handle_keymap(void *, struct wl_keyboard *, uint32_t, int, uint32_t)
    {
    }

    void keyboard_handle_enter(void *, struct wl_keyboard *, uint32_t, struct wl_surface *, struct wl_array *)
    {
    }

    void keyboard_handle_leave(void *, struct wl_keyboard *, uint32_t, struct wl_surface *)
    {
    }

    void keyboard_handle_modifiers(void *, struct wl_keyboard *, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t)
    {
    }

    void keyboard_handle_key(void *data, struct wl_keyboard *, uint32_t, uint32_t, uint32_t key, uint32_t state)
    {
      struct display *d = (struct display *)data;

      if ((key == KEY_ESC || key == KEY_Q) && state) {
        d->stop_flag = true;
      }
    }

    void keyboard_repeat_info(void *, struct wl_keyboard *, int32_t, int32_t) {}

    const struct wl_keyboard_listener keyboard_listener = {
      keyboard_handle_keymap,
      keyboard_handle_enter,
      keyboard_handle_leave,
      keyboard_handle_key,
      keyboard_handle_modifiers,
      keyboard_repeat_info
    };

    void seat_handle_capabilities(void *data, struct wl_seat *seat, enum wl_seat_capability caps)
    {
      struct display *d = (struct display *)data;
      if ((caps & WL_SEAT_CAPABILITY_KEYBOARD) && !d->keyboard) {
        d->keyboard = wl_seat_get_keyboard(seat);
        wl_keyboard_add_listener(d->keyboard, &keyboard_listener, d);
      } else if (!(caps & WL_SEAT_CAPABILITY_KEYBOARD) && d->keyboard) {
        wl_keyboard_destroy(d->keyboard);
        d->keyboard = NULL;
      }
    }

    void seat_name(void *, struct wl_seat *, const char *) {}

    const struct wl_seat_listener seat_listener = {
      [](void *data, struct wl_seat *seat, uint32_t caps) {
        seat_handle_capabilities(data, seat, (enum wl_seat_capability)caps);
      },
      seat_name
    };
  }
}
