#pragma once

#include <string>
#include <memory>

#include <GLES2/gl2.h>

#include <projection/shader.hpp>

namespace projection {
  class program_helper {
    GLuint gl_program;
    std::unique_ptr<projection::shader> vertex_shader;
    std::unique_ptr<projection::shader> fragment_shader;
  public:
    explicit program_helper(std::string vertex_shader_path, std::string fragment_shader_path);
    void link();
    GLuint program() { return gl_program; };
  };
}
