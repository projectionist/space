#pragma once

#include <memory>
#include <vector>
#include <cstdio>

#include <projection/drawing.hpp>
#include <projection/program_helper.hpp>

#include <GLES2/gl2.h>

#include <scratch/particle.hpp>

#define VBO_COUNT 2
#define U_VBO 0
#define OFFSET_VBO 1

namespace scratch {
  class scratch : public projection::drawing {
    std::unique_ptr<projection::program_helper> program_helper;
    GLuint program;
    GLuint vbos[VBO_COUNT];
    std::vector<particle> particles{10000};
  public:
    void setup();
    void update();
    void draw();
    scratch() {  };
  };
}
