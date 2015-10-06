#include <scratch/scratch.hpp>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#include <projection/shader.hpp>

using namespace std;

namespace scratch {

  void scratch::setup()
  {
    program = make_unique<projection::program>("./src/scratch/vert.glsl", "./src/scratch/frag.glsl");

    glBindAttribLocation(program->gl_program(), 0, "a_position");
    program->link();

    generate_bars();
  }

  void scratch::generate_bars()
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

    bars = vertices;
  }

  void scratch::draw(int width, int height, uint32_t)
  {
    glUseProgram(program->gl_program());

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    GLfloat *vertices = &bars[0];

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

    for (unsigned int i = 0; i < bars.size() / 3; i += 4) {
      glDrawArrays(GL_TRIANGLE_STRIP, i, 4);
    }
  }
}
