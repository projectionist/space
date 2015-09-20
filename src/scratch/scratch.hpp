#pragma once

#include <GLES2/gl2.h>

namespace scratch {
  class scratch {
  public:
    struct {
      GLuint rotation_uniform;
      GLuint pos;
      GLuint col;
    } gl;
    void initialize();
    void redraw(int width, int height, uint32_t time);
  };
}
