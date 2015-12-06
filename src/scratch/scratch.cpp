#include <scratch/scratch.hpp>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#include <projection/shader.hpp>

using namespace std;

namespace scratch {

  void scratch::setup()
  {
    program_helper = make_unique<projection::program_helper>("./src/scratch/vert.glsl", "./src/scratch/frag.glsl");
    program = program_helper->program();

    /*
      if you bind attribute names to locations (glBindAttribLocation), do it before linking
    */
    program_helper->link();
    /*
      if you get attribute/uniform locations (glGetAttribLocation), do it after linking
    */

    glDepthMask(true);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glEnable(GL_STENCIL_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glUseProgram(program);
    glGenBuffers(1, vbos);
  }

  void scratch::draw()
  {
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    const GLfloat vertices[] = {
      -0.50f, 0.50f,
      -0.25f,-0.50f,
       0.50f, 0.50f,
       0.25f,-0.50f
    };

    glViewport(0, 0, width(), height());

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glBindBuffer(GL_ARRAY_BUFFER, vbos[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    auto a_position = glGetAttribLocation(program, "a_position");

    glVertexAttribPointer(a_position, 2, GL_FLOAT, GL_FALSE, 0, (const void *)0);
    glEnableVertexAttribArray(a_position);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
  }
}
