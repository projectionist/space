#include <projection/shader.hpp>
#include <fstream>
#include <sstream>
#include <iostream>
#include <stdexcept>
#include <cstring>

using namespace std;

namespace projection {

  shader::shader(string path): path(path)
  {
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
  shader::~shader()
  {
    // release the c string
    free(src);
  }
  // delete the copy/move ctor/assign
  // shader(const shader&) = delete;
  // shader& operator=(shader&) = delete;
  // shader(const shader&&) = delete;
  // shader& operator=(shader&&) = delete;
  // void reload();
  // GLuint get();
}
