#include <cassert>
#include <cstring>
#include <iostream>
#include <unistd.h>

#include <wayland-client.h>

#include <GLES2/gl2.h>

#include <projection/projection.hpp>
#include <projection/configure_callback_listener.hpp>
#include <projection/funcs.hpp>
#include <projection/egl/egl.hpp>
#include <projection/wayland/surface.hpp>
#include <projection/wayland/registry.hpp>


using namespace std;

namespace projection {

  projection::projection():
    running(true),
    display(),
    window()
  {
    window.display = &display;
    display.window = &window;
    display.projection = (void *)this;

    display.display = wl_display_connect(NULL);
    assert(display.display);

    wayland::init_registry(&display);

    wl_display_dispatch(display.display);

    egl::init_egl(&display);
    wayland::create_surface(&window);
    init_gl(&window);
  }
  projection::~projection () {
    wayland::destroy_surface(&window);
    egl::fini_egl(&display);

    wayland::destroy_registry(&display);
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
}
