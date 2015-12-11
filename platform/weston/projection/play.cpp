#include <cassert>
#include <memory>

#include <wayland-client.h>

#include <projection/drawing.hpp>
#include <projection/play.hpp>

#include "display.hpp"
#include "egl.hpp"
#include "redraw.hpp"
#include "configure.hpp"
#include "surface.hpp"
#include "registry.hpp"

namespace projection {
  using namespace weston;

  void play(std::shared_ptr<projection::drawing> drawing) {
    struct display *display = (struct display *) malloc(sizeof(struct display));
    assert(display);

    display->drawing = drawing;

    display->display = wl_display_connect(NULL);
    assert(display->display);

    init_registry(display);

    wl_display_dispatch(display->display);

    init_egl(display);
    create_surface(display);

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
