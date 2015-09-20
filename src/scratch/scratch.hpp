#pragma once

#include <GLES2/gl2.h>

namespace scratch {
  class scratch {
    static GLuint create_shader(const char *source, GLenum shader_type);
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
