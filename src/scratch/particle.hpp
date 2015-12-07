#pragma once

#include <GLES2/gl2.h>

namespace scratch {
  class particle {
    static const GLfloat lifespan;
    GLfloat spawned;
    GLfloat rate;
    void reset(GLfloat elapsed);
  public:
    GLfloat x;
    GLfloat y;
    GLfloat z;
    explicit particle(const GLfloat spawned);
    void update(GLfloat elapsed);
  };
}
