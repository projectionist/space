#pragma once

#include <memory>

#include <projection/drawing.hpp>
#include <projection/play.hpp>

#include "egl.hpp"
#include "rpi_display.hpp"

namespace projection {
  using namespace rpi;

  void play(std::shared_ptr<projection::drawing> drawing)
  {
    rpi_display display(drawing);

    init_egl(&display);

    while (true) {
      display.drawing->redraw();
      eglSwapBuffers(display.display, display.surface);
    }

    fini_egl(&display);
  }
}
