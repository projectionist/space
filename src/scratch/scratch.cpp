#include <math.h>
#include <cstdio>
#include <cstdlib>
#include <cassert>
#include <scratch/scratch.hpp>
#include <GLES2/gl2.h>

#include <projection/shader.hpp>

namespace scratch {

  void scratch::initialize()
  {
    GLuint program;
    GLuint frag, vert;
    GLint status;

    frag = projection::shader("./src/scratch/frag.glsl", GL_FRAGMENT_SHADER).get();
    vert = projection::shader("./src/scratch/vert.glsl", GL_VERTEX_SHADER).get();

    program = glCreateProgram();

    glAttachShader(program, frag);
    glAttachShader(program, vert);
    glLinkProgram(program);

    glGetProgramiv(program, GL_LINK_STATUS, &status);
    if (!status) {
      char log[1000];
      GLsizei len;
      glGetProgramInfoLog(program, 1000, &len, log);
      fprintf(stderr, "Error: linking:\n%*s\n", len, log);
      exit(1);
    }

    glUseProgram(program);

    pos = 0;
    col = 1;

    glBindAttribLocation(program, pos, "pos");
    glBindAttribLocation(program, col, "color");
    glLinkProgram(program);

    rotation_uniform =
      glGetUniformLocation(program, "rotation");
  }

  void scratch::redraw(int width, int height, uint32_t time)
  {
    static const GLfloat verts[3][2] = {
      { -0.5, -0.5 },
      {  0.5, -0.5 },
      {  0,    0.5 }
    };
    static const GLfloat colors[3][3] = {
      { 1, 0, 0 },
      { 0, 1, 0 },
      { 0, 0, 1 }
    };
    GLfloat angle;
    GLfloat rotation[4][4] = {
      { 1, 0, 0, 0 },
      { 0, 1, 0, 0 },
      { 0, 0, 1, 0 },
      { 0, 0, 0, 1 }
    };
    static const int32_t speed_div = 5;
    static uint32_t start_time = 0;

    if (start_time == 0)
      start_time = time;

    angle = ((time-start_time) / speed_div) % 360 * M_PI / 180.0;
    rotation[0][0] =  cos(angle);
    rotation[0][2] =  sin(angle);
    rotation[2][0] = -sin(angle);
    rotation[2][2] =  cos(angle);

    glViewport(0, 0, width, height);

    glUniformMatrix4fv(rotation_uniform, 1, GL_FALSE,
           (GLfloat *) rotation);

    glClearColor(0.0, 0.0, 0.0, 0.5);
    glClear(GL_COLOR_BUFFER_BIT);

    glVertexAttribPointer(pos, 2, GL_FLOAT, GL_FALSE, 0, verts);
    glVertexAttribPointer(col, 3, GL_FLOAT, GL_FALSE, 0, colors);
    glEnableVertexAttribArray(pos);
    glEnableVertexAttribArray(col);

    glDrawArrays(GL_TRIANGLES, 0, 3);

    glDisableVertexAttribArray(pos);
    glDisableVertexAttribArray(col);
  }
}
