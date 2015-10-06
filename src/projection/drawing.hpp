#pragma once

#include <projection/shader.hpp>
#include <memory>
#include <vector>
#include <GLES2/gl2.h>


namespace projection {
  class drawing {
  public:
    virtual void setup() =0;
    virtual void draw(int width, int height, uint32_t time) =0;
    drawing() {};
  };
}
