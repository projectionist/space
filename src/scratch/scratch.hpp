#pragma once

#include <memory>
#include <vector>
#include <cstdio>

#include <projection/drawing.hpp>
#include <projection/program_helper.hpp>

#include <GLES2/gl2.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <scratch/particle.hpp>

#define NUM_VBOS 2
#define POSITION_VBO 0
#define OFFSET_VBO 1

namespace scratch {
  class scratch : public projection::drawing {
    std::unique_ptr<projection::program_helper> program_helper;
    GLuint program;
    GLuint vbos[NUM_VBOS];
    std::vector<particle> particles;
  public:
    void setup();
    void update();
    void draw();
    void spawn();
    scratch() {  };
  };
}
