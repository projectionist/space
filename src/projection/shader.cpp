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
    shader_type(shader_type)
  {
    reload();
  }
  shader::~shader()
  {
    // release the c string
    free(src);
  }

  void shader::reload()
  {
    ifstream f(path);
    if(!f.is_open()) {
      throw runtime_error("can't open file: " + path);
    }

    stringstream buffer;
    buffer << f.rdbuf();

    string str = buffer.str();

    if (src != NULL) free(src);

    // allocate a heap c string
    src = (char *) calloc(str.length() + 1, sizeof(char));
    memcpy(src, str.c_str(), str.length());
  }

  GLuint shader::get()
  {
    auto hot_shaders = getenv("HOT_SHADERS");

    if(hot_shaders && string(hot_shaders) == "1") {
      reload();
    }

    GLuint shader;
    GLint compiled;

    shader = glCreateShader(shader_type);
    assert(shader != 0);

    glShaderSource(shader, 1, (const char **) &src, NULL);
    glCompileShader(shader);

    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
    if (!compiled) {
      GLint infoLen = 0;
      glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);

      if(infoLen > 1)
      {
        char* infoLog = (char *) malloc(sizeof(char) * infoLen);
        glGetShaderInfoLog(shader, infoLen, NULL, infoLog);
        fprintf(stderr, "Error compiling shader:\n%s\n", infoLog);
        free(infoLog);
      }

      glDeleteShader(shader);
      throw runtime_error("Shader compilation failed: " + path);
    }

    return shader;
  }
}
