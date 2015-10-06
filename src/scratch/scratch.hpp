#pragma once

#include <projection/drawing.hpp>
#include <projection/program.hpp>
#include <memory>
#include <vector>
#include <GLES2/gl2.h>


namespace scratch {
  class scratch : public projection::drawing {
    std::unique_ptr<projection::program> program;
    std::vector<GLfloat> bars;
    void generate_bars();
  public:
    void setup();
    void draw(int width, int height, uint32_t time);
    scratch() {};
  };
}
