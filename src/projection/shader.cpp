#include <projection/shader.hpp>
#include <fstream>
#include <sstream>
#include <iostream>
#include <stdexcept>
#include <cstring>
#include <cassert>

using namespace std;

namespace projection {

  shader::shader(string path, GLenum shader_type):
    src(NULL),
    path(path),
    gl_shader_type(shader_type),
    gl_shader(0)
  {
  }

  shader::~shader()
  {
    unload();
  }

  void shader::unload()
  {
    if (src != NULL) {
      glDeleteShader(gl_shader);
      free(src);
      src = NULL;
    }
  }

  void shader::read_source()
  {
    if (src != NULL) return;
    // have a look to see if the file is there
    ifstream f(path);
    if(!f.is_open()) {
      throw runtime_error("can't open file: " + path);
    }

    stringstream buffer;
    buffer << f.rdbuf();

    string str = buffer.str();

    // allocate a heap c string
    src = (char *) calloc(str.length() + 1, sizeof(char));
    memcpy(src, str.c_str(), str.length());
  }

  void shader::compile_shader()
  {
    GLint compiled;

    gl_shader = glCreateShader(gl_shader_type);
    assert(gl_shader != 0);

    glShaderSource(gl_shader, 1, (const char **) &src, NULL);
    glCompileShader(gl_shader);

    glGetShaderiv(gl_shader, GL_COMPILE_STATUS, &compiled);

    if (!compiled) {
      GLint infoLen = 0;
      glGetShaderiv(gl_shader, GL_INFO_LOG_LENGTH, &infoLen);

      if(infoLen > 1)
      {
        char* infoLog = (char *) malloc(sizeof(char) * infoLen);
        glGetShaderInfoLog(gl_shader, infoLen, NULL, infoLog);
        fprintf(stderr, "Error compiling shader:\n%s\n", infoLog);
        free(infoLog);
      }

      glDeleteShader(gl_shader);
      throw runtime_error("Shader compilation failed: " + path);
    }
  }

  void shader::load()
  {
    read_source();
    compile_shader();
  }

  void shader::reload()
  {
    unload();
    load();
  }

  GLuint shader::get()
  {
    auto hot_shaders = getenv("HOT_SHADERS");

    if((src == NULL) || (hot_shaders && string(hot_shaders) == "1")) {
      reload();
    }

    return gl_shader;
  }
}
