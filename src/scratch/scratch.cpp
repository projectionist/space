#include <math.h>
#include <cstdio>
#include <cstdlib>
#include <cassert>
#include <stdexcept>
#include <scratch/scratch.hpp>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#include <projection/shader.hpp>

using namespace std;

namespace scratch {

  void scratch::initialize()
  {
    program = glCreateProgram();

    GLint linked;

    vertex_shader = make_unique<projection::shader>("./src/scratch/vert.glsl", GL_VERTEX_SHADER);
    fragment_shader = make_unique<projection::shader>("./src/scratch/frag.glsl", GL_FRAGMENT_SHADER);

    glAttachShader(program, vertex_shader->get());
    glAttachShader(program, fragment_shader->get());

    // Bind vPosition to attribute 0
    glBindAttribLocation(program, 0, "vPosition");

    glLinkProgram(program);

    glGetProgramiv(program, GL_LINK_STATUS, &linked);

    if (!linked) {
      GLint info_len = 0;

      glGetProgramiv(program, GL_INFO_LOG_LENGTH, &info_len);

      if ( info_len > 1 )
      {
         char* info_log = (char *)malloc (sizeof(char) * info_len );
         // TODO check info log allocation

         glGetProgramInfoLog(program, info_len, NULL, info_log);
         fprintf(stderr, "Error linking program:\n%s\n", info_log);

         free(info_log);
      }

      glDeleteProgram(program);
      throw runtime_error("Error linking program. Bail.");
    }

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
  }

  void scratch::redraw(int width, int height, uint32_t time)
  {
    glUseProgram(program);

    GLfloat vVertices[] = {  0.0f,  0.5f, 0.0f,
                            -0.5f, -0.5f, 0.0f,
                             0.5f, -0.5f, 0.0f };

    // Set the viewport
    glViewport(0, 0, width, height);

    // Clear the color buffer
    glClear(GL_COLOR_BUFFER_BIT);

    // Load the vertex data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, vVertices);
    glEnableVertexAttribArray(0);

    glDrawArrays(GL_TRIANGLES, 0, 3);
  }
}
