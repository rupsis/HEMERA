#ifndef COMMON_H
#define COMMON_H

#include <cmath>
#include <cstdlib>
#include <limits>
#include <memory>

// Usings
using std::make_shared;
using std::shared_ptr;
using std::sqrt;

const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

inline double degrees_to_radians(double degrees) {
  return degrees * pi / 180.0;
}

inline double random_double() {
  // Return a random real in [0,1).
  return rand() / (RAND_MAX + 1.0);
}

inline double random_double(double min, double max) {
  // Return a random real in [min,max).
  return min + (max - min) * random_double();
}

inline int random_int(int min, int max) {
  // Returns a random integer in [min, max]
  return int(random_double(min, max + 1));
}

#include "interval.h"
#include "ray.h"
#include "vec3.h"

#endif