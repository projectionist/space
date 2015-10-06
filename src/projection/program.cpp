#include <projection/program.hpp>
#include <cassert>
#include <iostream>

using namespace std;

namespace projection {
  program::program(string vertex_shader_path, string fragment_shader_path)
  {
    _gl_program = glCreateProgram();
    assert(_gl_program != 0);

    vertex_shader = make_unique<projection::shader>(vertex_shader_path, GL_VERTEX_SHADER);
    fragment_shader = make_unique<projection::shader>(fragment_shader_path, GL_FRAGMENT_SHADER);

    glAttachShader(gl_program(), vertex_shader->get());
    glAttachShader(gl_program(), fragment_shader->get());
  }

  void program::link()
  {
    GLint linked;

    glLinkProgram(gl_program());

    glGetProgramiv(gl_program(), GL_LINK_STATUS, &linked);

    if (!linked) {
      GLint info_len = 0;

      glGetProgramiv(gl_program(), GL_INFO_LOG_LENGTH, &info_len);

      if(info_len > 1)
      {
         char* info_log = (char *)malloc (sizeof(char) * info_len);
         assert(info_log);

         glGetProgramInfoLog(gl_program(), info_len, NULL, info_log);
         fprintf(stderr, "Error linking program:\n%s\n", info_log);

         free(info_log);
      }

      glDeleteProgram(gl_program());
      throw runtime_error("Error linking program.");
    }
  }
}
