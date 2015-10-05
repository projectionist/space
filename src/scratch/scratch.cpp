#include <math.h>
#include <cstdio>
#include <cstdlib>
#include <cassert>
#include <stdexcept>
#include <iostream>
#include <scratch/scratch.hpp>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#include <projection/shader.hpp>

using namespace std;

namespace scratch {

  scratch::scratch(): gl_program(0) {}

  void scratch::initialize()
  {
    gl_program = glCreateProgram();

    GLint linked;

    vertex_shader = make_unique<projection::shader>("./src/scratch/vert.glsl", GL_VERTEX_SHADER);
    fragment_shader = make_unique<projection::shader>("./src/scratch/frag.glsl", GL_FRAGMENT_SHADER);

    glAttachShader(gl_program, vertex_shader->get());
    glAttachShader(gl_program, fragment_shader->get());

    glBindAttribLocation(gl_program, 0, "a_position");

    glLinkProgram(gl_program);

    glGetProgramiv(gl_program, GL_LINK_STATUS, &linked);

    if (!linked) {
      GLint info_len = 0;

      glGetProgramiv(gl_program, GL_INFO_LOG_LENGTH, &info_len);

      if ( info_len > 1 )
      {
         char* info_log = (char *)malloc (sizeof(char) * info_len );
         // TODO check info log allocation

         glGetProgramInfoLog(gl_program, info_len, NULL, info_log);
         fprintf(stderr, "Error linking program:\n%s\n", info_log);

         free(info_log);
      }

      glDeleteProgram(gl_program);
      throw runtime_error("Error linking program. Bail.");
    }

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
  }

  void scratch::reload()
  {
    if (gl_program != 0) glDeleteProgram(gl_program);
    initialize();
  }

  void scratch::redraw(int width, int height, uint32_t time)
  {
    auto hot_shaders = getenv("HOT_SHADERS");

    if(hot_shaders && string(hot_shaders) == "1") {
      reload();
    }

    draw(width, height, time);
  }

  vector<GLfloat> scratch::bars()
  {
    vector<GLfloat> vertices;

    const int steps = 16; // twice the number of bars (bar, gap...)
    // normalized description of bar heights and gap heights
    //bar, gap, bar, gap etc.
    GLfloat description[steps] =
    { 0.2841398731256567, 0.056827974625131346,
      0.11365594925026269, 0.056827974625131346,
      0.056827974625131346, 0.056827974625131346,
      0.03342822036772432, 0.056827974625131346,
      0.021856913317358208, 0.056827974625131346,
      0.01535891206084631, 0.056827974625131346,
      0.011365594925026269, 0.056827974625131346,
      0.008742765326943285, 0.056827974625131346 };

    GLfloat y = 1.0f;

    for(int bar_idx = 0, step_idx = 1; bar_idx < steps; bar_idx += 2, step_idx += 2) {
      GLfloat height = description[bar_idx ] * 2;
      GLfloat gap    = description[step_idx] * 2;

      // GL_TRIANGLE_STRIP

      // top left
      vertices.push_back(-1.0f);
      vertices.push_back(y);
      vertices.push_back(0.0f);

      // bottom left
      vertices.push_back(-1.0f);
      vertices.push_back(y - height);
      vertices.push_back(0.0f);

      // top right
      vertices.push_back(1.0f);
      vertices.push_back(y);
      vertices.push_back(0.0f);

      // bottom right
      vertices.push_back(1.0f);
      vertices.push_back(y - height);
      vertices.push_back(0.0f);

      y -= height + gap;
    }

    return vertices;
  }

  void scratch::draw(int width, int height, uint32_t time)
  {
    glUseProgram(gl_program);

    auto vert_vector = bars();
    GLfloat *vertices = &vert_vector[0];

    // Set the viewport
    glViewport(0, 0, width, height);

    // Clear the color buffer
    glClear(GL_COLOR_BUFFER_BIT);

    // enable alpha blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Load the vertex data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, vertices);
    glEnableVertexAttribArray(0);

    for (int i = 0; i < vert_vector.size() / 3; i += 4) {
      glDrawArrays(GL_TRIANGLE_STRIP, i, 4);
    }
  }
}
