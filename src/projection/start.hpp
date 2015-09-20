#include <cassert>

#include <wayland-client.h>

#include <projection/funcs.hpp>
#include <projection/egl/egl.hpp>
#include <projection/wayland/configure.hpp>
#include <projection/wayland/surface.hpp>
#include <projection/wayland/registry.hpp>

#include <projection/display.hpp>

#include <scratch/scratch.hpp>

using namespace std;

namespace projection {

  void play(scratch::scratch s) {
    struct display display;

    display.display = wl_display_connect(NULL);
    assert(display.display);

    wayland::init_registry(&display);

    wl_display_dispatch(display.display);

    egl::init_egl(&display);
    wayland::create_surface(&display);
    init_gl(&display);

    int ret = 0;
    while (!display.stop_flag) {
      ret = wl_display_dispatch(display.display);
      if (ret == -1) break;
    }

    wayland::destroy_surface(&display);
    egl::fini_egl(&display);

    wayland::destroy_registry(&display);
    wl_display_flush(display.display);
    wl_display_disconnect(display.display);
  }
}
