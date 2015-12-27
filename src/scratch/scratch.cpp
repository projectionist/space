#include <cmath>
#include <cstdlib>

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
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

    glDepthMask(true);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glEnable(GL_STENCIL_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glUseProgram(program);

    glGenBuffers(NUM_VBOS, vbos);

    spawn();
  }

  void scratch::spawn()
  {
    for(int i = 0; particles->size() < 10000; i++) {
      particle p(elapsed());
      particles->push_back(p);
    }
  }

  void scratch::update()
  {
    for(auto & p : *particles) {
      p.update(elapsed());
    }
  }

  void scratch::draw()
  {
    glViewport(0, 0, width(), height());
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    update();

    auto points = make_unique<vector<GLfloat>>();

    for(auto & p : *particles) {
      points->push_back(p.x);
      points->push_back(p.y);
      points->push_back(p.z);
    }

    auto points_ptr = points->data();

    // Load the data
    glBindBuffer(GL_ARRAY_BUFFER, vbos[POSITION_VBO]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * points->size(), points_ptr, GL_STATIC_DRAW);
    check_gl_error();

    auto a_position = glGetAttribLocation(program, "a_position");
    check_gl_error();

    glVertexAttribPointer(a_position, 3, GL_FLOAT, GL_FALSE, 0, (const void *) 0);
    check_gl_error();

    glEnableVertexAttribArray(a_position);
    check_gl_error();

    // world camera and projection positioning
    glm::mat4 model = glm::translate(glm::vec3(-0.5f));

    glm::mat4 view = glm::lookAt(
      glm::vec3(0.0,0.0,0.0), // Camera in World Space
      glm::vec3(0,0,1.0),        // Looking at the origin
      glm::vec3(0,1,0)         // Head is up
    );

    glm::mat4 projection = glm::perspectiveFov(glm::radians(92.0f),
      width(), height(), 0.1f, 100.0f);

    glUniformMatrix4fv(glGetUniformLocation(program, "u_mvp"), 1, GL_FALSE, glm::value_ptr(projection * view * model));

    glDrawArrays(GL_POINTS, 0, particles->size());
  }
}
