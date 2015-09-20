#include <math.h>
#include <cstdio>
#include <cstdlib>
#include <cassert>
#include <scratch/scratch.hpp>
#include <GLES2/gl2.h>

namespace scratch {

  static const char *vert_shader_text =
    "uniform mat4 rotation;\n"
    "attribute vec4 pos;\n"
    "attribute vec4 color;\n"
    "varying vec4 v_color;\n"
    "void main() {\n"
    "  gl_Position = rotation * pos;\n"
    "  v_color = color;\n"
    "}\n";

  static const char *frag_shader_text =
    "precision mediump float;\n"
    "varying vec4 v_color;\n"
    "void main() {\n"
    "  gl_FragColor = v_color;\n"
    "}\n";

  GLuint scratch::create_shader(const char *source, GLenum shader_type)
  {
    GLuint shader;
    GLint status;

    shader = glCreateShader(shader_type);
    assert(shader != 0);

    glShaderSource(shader, 1, (const char **) &source, NULL);
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

  void scratch::initialize()
  {
    GLuint program;
    GLuint frag, vert;
    GLint status;

    frag = create_shader(frag_shader_text, GL_FRAGMENT_SHADER);
    vert = create_shader(vert_shader_text, GL_VERTEX_SHADER);

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

    gl.pos = 0;
    gl.col = 1;

    glBindAttribLocation(program, gl.pos, "pos");
    glBindAttribLocation(program, gl.col, "color");
    glLinkProgram(program);

    gl.rotation_uniform =
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
