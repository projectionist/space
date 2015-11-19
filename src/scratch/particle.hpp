#pragma once

#include <GLES2/gl2.h>

namespace scratch {
  class particle {
  public:
    GLfloat lifespan = 0.0;
    GLfloat spawned_at = 0.0;
    GLfloat angle = 0.0;
    //explicit particle(const GLfloat spawned_at) : spawned_at(spawned_at) {}
  };
}
