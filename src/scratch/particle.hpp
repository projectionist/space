#pragma once

#include <GLES2/gl2.h>

namespace scratch {
  class particle {
    static const GLfloat lifespan;
  public:
    GLfloat spawned;
    GLfloat offset;
    GLfloat u;
    explicit particle(const GLfloat spawned);
    void update(GLfloat elapsed);
  };
}
