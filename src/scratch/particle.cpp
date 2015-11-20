#include <cmath>
#include <cstdlib>
#include <cstdio>

#include <scratch/particle.hpp>

namespace scratch {
  particle::particle()
  {
    offset = ((float) rand()) / ((float) RAND_MAX);
  }

  void particle::update(GLfloat elapsed)
  {
    double integral;
    double fraction = modf(elapsed, &integral);

    u = fraction;
  }
}
