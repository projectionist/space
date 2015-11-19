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

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_STENCIL_TEST);
    // enable alpha blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glUseProgram(program);
  }

  void scratch::draw()
  {
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    GLfloat vertices[0];

    // Set the viewport
    glViewport(0, 0, width(), height());

    // Clear the color buffer
    glClear(GL_COLOR_BUFFER_BIT);

    // enable alpha blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    auto a_pos_location = glGetAttribLocation(program, "a_position");
    // Load the vertex data
    glVertexAttribPointer(a_pos_location, 2, GL_FLOAT, GL_FALSE, 0, vertices);
    glEnableVertexAttribArray(a_pos_location);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 0);
  }
}
