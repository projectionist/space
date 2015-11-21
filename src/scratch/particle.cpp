#include <cmath>
#include <cstdlib>
#include <cstdio>

#include <scratch/particle.hpp>

namespace scratch {
  const GLfloat particle::lifespan = 10.0;

  particle::particle(const GLfloat spawned)
  {
    this->spawned = spawned;
    this->offset = ((float) rand()) / ((float) RAND_MAX);
  }

  void particle::update(GLfloat elapsed)
  {
    auto age = elapsed - spawned;

    u = age / lifespan;
  }
}
