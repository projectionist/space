#include <cmath>
#include <cstdlib>

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

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

    glGenBuffers(NUM_VBOS, vbos);
  }

  void scratch::spawn()
  {
    for(int i = 0; particles.size() < 1; i++) {
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

    vector<GLfloat> triangles;
    // vector<GLfloat> offsets;

    for(auto & p : particles) {
      triangles.push_back(0.0f);
      triangles.push_back(1.0f);
      triangles.push_back(p.u);

      triangles.push_back(-1.0f);
      triangles.push_back(-1.0f);
      triangles.push_back(p.u);

      triangles.push_back(1.0f);
      triangles.push_back(-1.0f);
      triangles.push_back(p.u);
    }

    GLfloat *triangles_ptr = &triangles[0];
    //GLfloat *offsets_ptr = &offsets[0];

    // Load the progression data
    glBindBuffer(GL_ARRAY_BUFFER, vbos[POSITION_VBO]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 3 * triangles.size(), triangles_ptr, GL_STATIC_DRAW);
    gl_check_error();

    auto a_position = glGetAttribLocation(program, "a_position");
    assert(a_position >= 0);

    glVertexAttribPointer(a_position, 1, GL_FLOAT, GL_FALSE, 0, 0);
    gl_check_error();

    glEnableVertexAttribArray(a_position);
    gl_check_error();

    // buffer vertex angle offsets
    // glBindBuffer(GL_ARRAY_BUFFER, vbos[OFFSET_VBO]);
    // glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * offsets.size(), offsets_ptr, GL_STATIC_DRAW);
    //
    // auto a_offset = glGetAttribLocation(program, "a_offset");
    // assert(a_offset >= 0);
    //
    // glVertexAttribPointer(a_offset, 1, GL_FLOAT, GL_FALSE, 0, 0);
    // assert(glGetError() == GL_NO_ERROR);
    //
    // glEnableVertexAttribArray(a_offset);
    // assert(glGetError() == GL_NO_ERROR);

    // model view projection processing
    glm::mat4 projection = glm::perspectiveFov(glm::radians(60.0f),
      width(), height(), 0.1f, 100.0f);

    glm::mat4 view = glm::lookAt(
      glm::vec3(0.0,0.0,-1.0), // Camera in World Space
      glm::vec3(0,0,0), // looking at the origin
      glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
    );

    glm::mat4 model = glm::scale(glm::vec3(0.1, 0.1, 0.1));

    glm::mat4 u_mvp = projection * view * model;

    glUniformMatrix4fv(glGetUniformLocation(program, "u_mvp"), 1, GL_FALSE, glm::value_ptr(u_mvp));

    glDrawArrays(GL_TRIANGLES, 0, particles.size());
  }
}
