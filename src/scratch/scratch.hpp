#pragma once

#include <memory>
#include <vector>

#include <projection/drawing.hpp>
#include <projection/program_helper.hpp>

#include <GLES2/gl2.h>

#include <scratch/particle.hpp>

namespace scratch {
  class scratch : public projection::drawing {
    std::unique_ptr<projection::program_helper> program_helper;
    GLuint program;
    GLuint vbos[1];
    particle p0;
  public:
    void setup();
    void update();
    void draw();
    scratch() {};
  };
}
