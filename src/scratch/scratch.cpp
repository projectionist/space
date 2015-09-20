#include <math.h>
#include <scratch/scratch.hpp>

namespace scratch {
  void scratch::initialize()
  {
    
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

    glUniformMatrix4fv(gl.rotation_uniform, 1, GL_FALSE,
           (GLfloat *) rotation);

    glClearColor(0.0, 0.0, 0.0, 0.5);
    glClear(GL_COLOR_BUFFER_BIT);

    glVertexAttribPointer(gl.pos, 2, GL_FLOAT, GL_FALSE, 0, verts);
    glVertexAttribPointer(gl.col, 3, GL_FLOAT, GL_FALSE, 0, colors);
    glEnableVertexAttribArray(gl.pos);
    glEnableVertexAttribArray(gl.col);

    glDrawArrays(GL_TRIANGLES, 0, 3);

    glDisableVertexAttribArray(gl.pos);
    glDisableVertexAttribArray(gl.col);
  }
}
