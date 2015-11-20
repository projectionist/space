#pragma once

#include <GLES2/gl2.h>

namespace scratch {
  class particle {
  public:
    GLfloat x;
    GLfloat y;
    GLfloat u;
    explicit particle();
    void update(GLfloat elapsed);
  };
}
