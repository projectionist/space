#include <math.h>

#include <scratch/particle.hpp>

namespace scratch {
  void particle::update(GLfloat elapsed)
  {
    u = elapsed;
  }
}
