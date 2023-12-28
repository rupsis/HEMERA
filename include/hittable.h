#ifndef HITTABLE_H
#define HITTABLE_H

#include "ray.h"

// Forward declaration to avoid circular reference issue
class material;

class hit_record {
 public:
  point3 p;
  vec3 normal;
  shared_ptr<material> mat;
  double t;
  bool front_face;

  // We're going to assume the normal opposes the incoming ray.
  void set_face_normal(const ray &r, const vec3 &outward_normal) {
    // Sets the hit record normal vector.
    // Note: `outward_normal` is assumed to have unit length

    front_face = dot(r.direction(), outward_normal) < 0;
    normal = front_face ? outward_normal : -outward_normal;
  }
};

class hittable {
 public:
  virtual ~hittable() = default;

  virtual bool hit(const ray &r, interval ray_t, hit_record &rec) const = 0;
};

#endif