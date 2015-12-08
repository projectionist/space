#pragma once

#include <memory>
#include <vector>
#include <cstdio>

#include <projection/drawing.hpp>
#include <projection/program_helper.hpp>

#include <GLES2/gl2.h>

#include <scratch/particle.hpp>

#define NUM_VBOS 1
#define POSITION_VBO 0

namespace scratch {
  class scratch : public projection::drawing {
    std::unique_ptr<projection::program_helper> program_helper;
    GLuint program;
    GLuint vbos[NUM_VBOS];
    std::unique_ptr<std::vector<particle>> particles;
  public:
    void setup();
    void update();
    void draw();
    void spawn();
    scratch() : particles(std::make_unique<std::vector<particle>>()) {  };
  };
}
