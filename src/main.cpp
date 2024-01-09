#include "common.h"

#include "bvh.h"
#include "camera.h"
#include "color.h"
#include "hittable_list.h"
#include "material.h"
#include "sphere.h"
#include "triangle.h"

#include <chrono>

int main() {

  // World
  hittable_list world;

  // Random Spheres with materials
  auto ground_material = make_shared<lambertian>(color(0.5, 0.5, 0.5));
  world.add(make_shared<sphere>(point3(0, -1000, 0), 1000, ground_material));

  for (int a = -11; a < 11; a++) {
    for (int b = -11; b < 11; b++) {
      auto choose_mat = random_double();
      point3 center(a + 0.9 * random_double(), 0.2, b + 0.9 * random_double());

      if ((center - point3(4, 0.2, 0)).length() > 0.9) {
        shared_ptr<material> sphere_material;

        if (choose_mat < 0.7) {
          // diffuse
          auto albedo = color::random() * color::random();
          sphere_material = make_shared<lambertian>(albedo);
          world.add(make_shared<sphere>(center, 0.2, sphere_material));
        }
        else if (choose_mat < 0.92) {
          // metal
          auto albedo = color::random(0.5, 1);
          auto fuzz = random_double(0, 0.5);
          sphere_material = make_shared<metal>(albedo, fuzz);
          auto center2 = center + vec3(0, random_double(0, 0.5), 0);
          world.add(make_shared<sphere>(center, center2, 0.2, sphere_material));
        }
        else {
          // glass
          sphere_material = make_shared<dielectric>(1.5);
          world.add(make_shared<sphere>(center, 0.2, sphere_material));
        }
      }
    }
  }

  auto material1 = make_shared<dielectric>(1.5);
  world.add(make_shared<sphere>(point3(0, 1, 0), 1.0, material1));

  auto material2 = make_shared<lambertian>(color(0.4, 0.2, 0.1));
  world.add(make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));

  auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
  world.add(make_shared<sphere>(point3(4, 1, 0), 1.0, material3));

  world = hittable_list(make_shared<bvh_node>(world));

  camera camera;

  camera.aspect_ratio = 16.0 / 9.0;
  camera.image_width = 400;
  camera.samples_per_pixel = 100;
  camera.max_depth = 50;

  camera.vfov = 20;
  camera.lookfrom = point3(13, 2, 3);
  camera.lookat = point3(0, 0, 0);
  camera.vup = vec3(0, 1, 0);

  camera.defocus_angle = 0.8;
  camera.focus_dist = 11;

  // Get Time elapse for rendering image.
  std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
  camera.render(world);
  std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

  std::clog << "Time elapse = "
            << std::chrono::duration_cast<std::chrono::seconds>(end - begin).count() << "[s]"
            << std::endl;
}