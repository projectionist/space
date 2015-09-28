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
    if (src != NULL) free(src);

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

  GLuint shader::get()
  {
    auto hot_shaders = getenv("HOT_SHADERS");

    if(hot_shaders && string(hot_shaders) == "1") {
      reload();
    }

    GLuint shader;
    GLint status;

    shader = glCreateShader(shader_type);
    assert(shader != 0);

    glShaderSource(shader, 1, (const char **) &src, NULL);
    glCompileShader(shader);

    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (!status) {
      char log[1000];
      GLsizei len;
      glGetShaderInfoLog(shader, 1000, &len, log);
      fprintf(stderr, "Error: compiling %s: %*s\n",
        shader_type == GL_VERTEX_SHADER ? "vertex" : "fragment",
        len, log);
      exit(1);
    }

    return shader;
  }
}
