#pragma once

#include <memory>

#include <EGL/egl.h>
#include <EGL/eglext.h>

namespace projection {
  namespace rpi {
    struct rpi_display {
      EGLDisplay display;
      EGLContext context;
      EGLConfig config;
      EGLSurface surface;
      uint32_t screen_width;
      uint32_t screen_height;
      std::shared_ptr<projection::drawing> drawing;

      rpi_display(std::shared_ptr<projection::drawing> _drawing): drawing(_drawing) {};
    };
  }
}
