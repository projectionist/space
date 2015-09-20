#include <cassert>

#include <wayland-client.h>

#include <projection/egl.hpp>
#include <projection/redraw.hpp>
#include <projection/configure.hpp>
#include <projection/surface.hpp>
#include <projection/registry.hpp>

#include <projection/display.hpp>

#include <scratch/scratch.hpp>

using namespace std;

namespace projection {

  void play(scratch::scratch s) {
    struct display *display = (struct display *) malloc(sizeof(struct display));
    assert(display);

    display->display = wl_display_connect(NULL);
    assert(display->display);

    init_registry(display);

    wl_display_dispatch(display->display);

    init_egl(display);
    create_surface(display);
    init_gl(display);

    int ret = 0;
    while (!display->stop_flag) {
      ret = wl_display_dispatch(display->display);
      if (ret == -1) break;
    }

    destroy_surface(display);
    fini_egl(display);

    destroy_registry(display);
    wl_display_flush(display->display);
    wl_display_disconnect(display->display);
    free(display);
  }
}
