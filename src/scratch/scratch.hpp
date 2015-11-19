#pragma once

#include <memory>
#include <vector>

#include <projection/drawing.hpp>
#include <projection/program_helper.hpp>

#include <GLES2/gl2.h>


namespace scratch {
  class scratch : public projection::drawing {
    std::unique_ptr<projection::program_helper> program_helper;
    GLuint program;
    std::vector<GLfloat> bars;
    void generate_bars();
  public:
    void setup();
    void draw();
    scratch() {};
  };
}
