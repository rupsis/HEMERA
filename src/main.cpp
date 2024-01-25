#include "common.h"

#include "bvh.h"
#include "camera.h"
#include "color.h"
#include "hittable_list.h"
#include "material.h"
#include "quad.h"
#include "sphere.h"
#include "texture.h"
#include "triangle.h"

#include <chrono>

void random_spheres() {
  // World
  hittable_list world;

  auto checker = make_shared<checker_texture>(0.32, color(0.2, 0.3, 0.1), color(0.9, 0.9, 0.9));
  world.add(make_shared<sphere>(point3(0, -1000, 0), 1000, make_shared<lambertian>(checker)));

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
  camera.background = color(0.70, 0.80, 1.00);

  camera.vfov = 20;
  camera.lookfrom = point3(13, 2, 3);
  camera.lookat = point3(0, 0, 0);
  camera.vup = vec3(0, 1, 0);

  camera.defocus_angle = 0.8;
  camera.focus_dist = 11;

  camera.render(world);
}

void two_spheres() {
  hittable_list world;

  auto checker = make_shared<checker_texture>(0.8, color(0.2, 0.3, 0.1), color(0.9, 0.9, 0.9));

  world.add(make_shared<sphere>(point3(0, -10, 0), 10, make_shared<lambertian>(checker)));
  world.add(make_shared<sphere>(point3(0, 10, 0), 10, make_shared<lambertian>(checker)));

  camera camera;

  camera.aspect_ratio = 16.0 / 9.0;
  camera.image_width = 400;
  camera.samples_per_pixel = 100;
  camera.max_depth = 50;
  camera.background = color(0.70, 0.80, 1.00);

  camera.vfov = 20;
  camera.lookfrom = point3(13, 2, 3);
  camera.lookat = point3(0, 0, 0);
  camera.vup = vec3(0, 1, 0);

  camera.defocus_angle = 0;

  camera.render(world);
}

void uv_sphere() {
  auto uv_texture = make_shared<image_texture>("../textures/uv_grid.png");
  auto uv_surface = make_shared<lambertian>(uv_texture);
  auto uv_sphere = make_shared<sphere>(point3(0, 0, 0), 2, uv_surface);

  camera camera;

  camera.aspect_ratio = 16.0 / 9.0;
  camera.image_width = 400;
  camera.samples_per_pixel = 100;
  camera.max_depth = 50;
  camera.background = color(0.70, 0.80, 1.00);

  camera.vfov = 20;
  camera.lookfrom = point3(0, 0, 12);
  camera.lookat = point3(0, 0, 0);
  camera.vup = vec3(0, 1, 0);

  camera.defocus_angle = 0;

  camera.render(hittable_list(uv_sphere));
}

void two_perlin_spheres() {
  hittable_list world;

  auto pertext = make_shared<noise_texture>(4);
  world.add(make_shared<sphere>(point3(0, -1000, 0), 1000, make_shared<lambertian>(pertext)));
  world.add(make_shared<sphere>(point3(0, 2, 0), 2, make_shared<lambertian>(pertext)));

  camera camera;

  camera.aspect_ratio = 16.0 / 9.0;
  camera.image_width = 400;
  camera.samples_per_pixel = 100;
  camera.max_depth = 50;
  camera.background = color(0.70, 0.80, 1.00);

  camera.vfov = 20;
  camera.lookfrom = point3(13, 2, 3);
  camera.lookat = point3(0, 0, 0);
  camera.vup = vec3(0, 1, 0);

  camera.defocus_angle = 0;

  camera.render(world);
}

void quads() {
  hittable_list world;

  // Materials
  auto left_red = make_shared<lambertian>(color(1.0, 0.2, 0.2));
  auto back_green = make_shared<lambertian>(color(0.2, 1.0, 0.2));
  auto right_blue = make_shared<lambertian>(color(0.2, 0.2, 1.0));
  auto upper_orange = make_shared<lambertian>(color(1.0, 0.5, 0.0));
  auto lower_teal = make_shared<lambertian>(color(0.2, 0.8, 0.8));

  // Quads
  world.add(make_shared<quad>(point3(-3, -2, 5), vec3(0, 0, -4), vec3(0, 4, 0), left_red));
  world.add(make_shared<quad>(point3(-2, -2, 0), vec3(4, 0, 0), vec3(0, 4, 0), back_green));
  world.add(make_shared<quad>(point3(3, -2, 1), vec3(0, 0, 4), vec3(0, 4, 0), right_blue));
  world.add(make_shared<quad>(point3(-2, 3, 1), vec3(4, 0, 0), vec3(0, 0, 4), upper_orange));
  world.add(make_shared<quad>(point3(-2, -3, 5), vec3(4, 0, 0), vec3(0, 0, -4), lower_teal));

  camera camera;

  camera.aspect_ratio = 1.0;
  camera.image_width = 400;
  camera.samples_per_pixel = 100;
  camera.max_depth = 50;
  camera.background = color(0.70, 0.80, 1.00);

  camera.vfov = 80;
  camera.lookfrom = point3(0, 0, 9);
  camera.lookat = point3(0, 0, 0);
  camera.vup = vec3(0, 1, 0);

  camera.defocus_angle = 0;

  camera.render(world);
}

void simple_light() {
  hittable_list world;

  auto pertext = make_shared<noise_texture>(4);
  world.add(make_shared<sphere>(point3(0, -1000, 0), 1000, make_shared<lambertian>(pertext)));
  world.add(make_shared<sphere>(point3(0, 2, 0), 2, make_shared<lambertian>(pertext)));

  auto difflight = make_shared<diffuse_light>(color(4, 4, 4));
  world.add(make_shared<sphere>(point3(0, 7, 0), 2, difflight));
  world.add(make_shared<quad>(point3(3, 1, -2), vec3(2, 0, 0), vec3(0, 2, 0), difflight));

  camera camera;

  camera.aspect_ratio = 1.0;
  camera.image_width = 400;
  camera.samples_per_pixel = 100;
  camera.max_depth = 50;
  camera.background = color(0, 0, 0);

  camera.vfov = 20;
  camera.lookfrom = point3(26, 3, 6);
  camera.lookat = point3(0, 2, 0);
  camera.vup = vec3(0, 1, 0);

  camera.defocus_angle = 0;

  camera.render(world);
}

void cornell_box() {
  hittable_list world;

  auto red = make_shared<lambertian>(color(0.65, 0.05, 0.05));
  auto white = make_shared<lambertian>(color(0.73, 0.73, 0.73));
  auto green = make_shared<lambertian>(color(0.12, 0.45, 0.15));
  auto light = make_shared<diffuse_light>(color(15, 15, 15));

  world.add(make_shared<quad>(point3(555, 0, 0), vec3(0, 555, 0), vec3(0, 0, 555), green));
  world.add(make_shared<quad>(point3(0, 0, 0), vec3(0, 555, 0), vec3(0, 0, 555), red));
  world.add(make_shared<quad>(point3(343, 554, 332), vec3(-130, 0, 0), vec3(0, 0, -105), light));
  world.add(make_shared<quad>(point3(0, 0, 0), vec3(555, 0, 0), vec3(0, 0, 555), white));
  world.add(make_shared<quad>(point3(555, 555, 555), vec3(-555, 0, 0), vec3(0, 0, -555), white));
  world.add(make_shared<quad>(point3(0, 0, 555), vec3(555, 0, 0), vec3(0, 555, 0), white));

  // Boxes
  shared_ptr<hittable> box1 = box(point3(0, 0, 0), point3(165, 330, 165), white);
  box1 = make_shared<rotate_y>(box1, 15);
  box1 = make_shared<translate>(box1, vec3(265, 0, 295));
  world.add(box1);

  shared_ptr<hittable> box2 = box(point3(0, 0, 0), point3(165, 165, 165), white);
  box2 = make_shared<rotate_y>(box2, -18);
  box2 = make_shared<translate>(box2, vec3(130, 0, 65));
  world.add(box2);

  camera camera;

  camera.aspect_ratio = 1.0;
  camera.image_width = 600;
  camera.samples_per_pixel = 200;
  camera.max_depth = 50;
  camera.background = color(0, 0, 0);

  camera.vfov = 40;
  camera.lookfrom = point3(278, 278, -800);
  camera.lookat = point3(278, 278, 0);
  camera.vup = vec3(0, 1, 0);

  camera.defocus_angle = 0;

  camera.render(world);
}

int main() {

  // Get Time elapse for rendering image.
  std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
  switch (7) {
    case 1:
      random_spheres();
      break;
    case 2:
      two_spheres();
      break;
    case 3:
      uv_sphere();
      break;
    case 4:
      two_perlin_spheres();
      break;
    case 5:
      quads();
      break;
    case 6:
      simple_light();
      break;
    case 7:
      cornell_box();
      break;
  }
  std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
  std::clog << "Time elapse = "
            << std::chrono::duration_cast<std::chrono::seconds>(end - begin).count() << "[s]"
            << std::endl;
}