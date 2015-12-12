#include <stdexcept>
#include <vector>

#include <GLES2/gl2.h>

#include <projection/drawing.hpp>

using namespace std;
using namespace std::chrono;

namespace projection {
  drawing::drawing() {}

  void drawing::initialize(float width, float height)
  {
    start_time = std::chrono::high_resolution_clock::now();
    _w = width;
    _h = height;
    _elapsed = 0.0;
    _initialized = true;
    setup();
  }

  void drawing::redraw()
  {
    auto now = high_resolution_clock::now();
    auto time_span = duration_cast<duration<float>>(now - start_time);
    _elapsed = time_span.count();
    _frames++;

    draw();
  }

  /**
    returns the name of the open gl error by reversing the definitions
    from GLES2/gl2.h

    #define GL_NO_ERROR                       0
    #define GL_INVALID_ENUM                   0x0500
    #define GL_INVALID_VALUE                  0x0501
    #define GL_INVALID_OPERATION              0x0502
    #define GL_OUT_OF_MEMORY                  0x0505
  */
  std::string drawing::gl_error_code_name(const unsigned int error_code)
  {
    switch (error_code) {
      case GL_NO_ERROR:
        return "GL_NO_ERROR";
      case GL_INVALID_ENUM:
        return "GL_INVALID_ENUM";
      case GL_INVALID_VALUE:
        return "GL_INVALID_VALUE";
      case GL_INVALID_OPERATION:
        return "GL_INVALID_OPERATION";
      case GL_OUT_OF_MEMORY:
        return "GL_OUT_OF_MEMORY";
      default:
        return "None GL error code: " + error_code;
    }
  }
}
