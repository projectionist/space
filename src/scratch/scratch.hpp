#pragma once

#include <projection/shader.hpp>
#include <memory>
#include <GLES2/gl2.h>

namespace scratch {
  class scratch {
    GLuint program;
    std::unique_ptr<projection::shader> vertex_shader;
    std::unique_ptr<projection::shader> fragment_shader;
  public:
    void initialize();
    void redraw(int width, int height, uint32_t time);
  };
}
