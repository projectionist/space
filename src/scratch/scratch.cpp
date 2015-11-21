#include <cmath>
#include <cassert>
#include <cstdlib>

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#include <projection/shader.hpp>

#include <scratch/scratch.hpp>

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

    glGenBuffers(VBO_COUNT, vbos);
  }

  void scratch::spawn()
  {
    for(int i = 0; i < 1000 && particles.size() < 1000000; i++) {
      particle p(elapsed());
      particles.push_back(p);
    }
  }

  void scratch::update()
  {
    spawn();

    for(auto & p : particles) {
      p.update(elapsed());
    }
  }

  void scratch::draw()
  {
    update();

    // Set the viewport
    glViewport(0, 0, width(), height());

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    // Clear the color buffer
    glClear(GL_COLOR_BUFFER_BIT);

    // enable alpha blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    vector<GLfloat> progressions;
    vector<GLfloat> offsets;

    for(auto & p : particles) {
      progressions.push_back(p.u);
      offsets.push_back(p.offset);
    }

    GLfloat *progressions_ptr = &progressions[0];
    GLfloat *offsets_ptr = &offsets[0];

    // Load the progression data
    glBindBuffer(GL_ARRAY_BUFFER, vbos[U_VBO]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * progressions.size(), progressions_ptr, GL_STATIC_DRAW);
    assert(glGetError() == GL_NO_ERROR);

    auto a_u = glGetAttribLocation(program, "a_u");
    assert(a_u >= 0);

    glVertexAttribPointer(a_u, 1, GL_FLOAT, GL_FALSE, 0, 0);
    assert(glGetError() == GL_NO_ERROR);

    glEnableVertexAttribArray(a_u);
    assert(glGetError() == GL_NO_ERROR);

    // buffer vertex angle offsets
    glBindBuffer(GL_ARRAY_BUFFER, vbos[OFFSET_VBO]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * offsets.size(), offsets_ptr, GL_STATIC_DRAW);

    auto a_offset = glGetAttribLocation(program, "a_offset");
    assert(a_offset >= 0);

    glVertexAttribPointer(a_offset, 1, GL_FLOAT, GL_FALSE, 0, 0);
    assert(glGetError() == GL_NO_ERROR);

    glEnableVertexAttribArray(a_offset);
    assert(glGetError() == GL_NO_ERROR);

    glDrawArrays(GL_POINTS, 0, particles.size());
  }
}
