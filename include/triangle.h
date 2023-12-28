#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "hittable.h"
#include "vec3.h"

class triangle : public hittable {
 public:
  triangle(point3 _p0, point3 _p1, point3 _p2) : p0(_p0), p1(_p1), p2(_p2) {}

  bool hit(const ray &r, interval ray_t, hit_record &rec) const override {
    // TODO explain Cramer's rule here
    // TODO refactor into it's own method.
    float tval;
    float A = p0.x() - p1.x();
    float B = p0.y() - p1.y();
    float C = p0.z() - p1.z();

    float D = p0.x() - p2.x();
    float E = p0.y() - p2.y();
    float F = p0.z() - p2.z();

    float G = r.direction().x();
    float H = r.direction().y();
    float I = r.direction().z();

    float J = p0.x() - r.origin().x();
    float K = p0.y() - r.origin().y();
    float L = p0.z() - r.origin().z();

    float EIHF = E * I - H * F;
    float GFDI = G * F - D * I;
    float DHEG = D * H - E * G;

    float denominator = (A * EIHF + B * GFDI + C * DHEG);
    float beta = (J * EIHF + K * GFDI + L * DHEG) / denominator;

    if (beta <= 0.0f || beta >= 1.0f)
      return false;

    float AKJB = A * K - J * B;
    float JCAL = J * K - A * L;
    float BLKC = B * L - K * C;

    float gamma = (I * AKJB + H * JCAL + G * BLKC) / denominator;
    if (gamma <= 0.0f || beta + gamma >= 1.0f)
      return false;

    tval = -(F * AKJB + E * JCAL + D * BLKC) / denominator;

    if (ray_t.contains(tval)) {
      rec.t = tval;
      rec.normal = unit_vector(cross((p1 - p0), (p2 - p0)));
      vec3 outward_normal = unit_vector(cross((p1 - p0), (p2 - p0)));
      rec.set_face_normal(r, outward_normal);
      return true;
    }
    return false;
  }

 private:
  point3 p0;
  point3 p1;
  point3 p2;
};

#endif