//
// Created by Jack Glass on 2020-09-23.
//

#include "serpinsky_triangle.h"
#include "triangle.h"
#include "fractal_math.h"

using namespace fractals;
using gl::shapes::Triangle;

constexpr auto starting_triangle = Triangle{
  glm::vec3{-0.75f, -0.75f, 0.0f},
  glm::vec3{0.75f, -0.75f, 0.0f},
  glm::vec3{0.0f, 0.75f, 0.0f},
  glm::vec3{0.05f, 0.05f, 0.05f}
};

auto serpinsky_algo(std::vector<Triangle>& triangles, const Triangle& triangle, int iter) -> void
{
  if(iter > 0){
    const auto AB = midpoint(triangle.A, triangle.B);
    const auto AC = midpoint(triangle.A, triangle.C);
    const auto BC = midpoint(triangle.B, triangle.C);

    const auto gen_new_color = [](auto colour){
      return 0.05f * (1.0f - colour) + colour;
    };
    const auto& c = triangle.colour;
    auto colour1 = glm::vec3{gen_new_color(c.x), c.y, c.z};
    auto colour2 = glm::vec3{c.x, gen_new_color(c.y), c.z};
    auto colour3 = glm::vec3{c.x, c.y, gen_new_color(c.z)};

    serpinsky_algo(triangles, { triangle.A, AB, AC, colour1 }, iter - 1);
    serpinsky_algo(triangles, { AB, triangle.B, BC, colour2 }, iter - 1);
    serpinsky_algo(triangles, { BC, triangle.C, AC, colour3 }, iter - 1);
  } else {
    triangles.push_back(triangle);
  }
}

[[nodiscard]]
auto to_cpu_geom(const std::vector<Triangle>& triangles) -> CpuGeometry
{
  CpuGeometry geom{};
  for(auto& triangle : triangles) {
    geom.vertices.emplace_back(triangle.A);
    geom.vertices.emplace_back(triangle.B);
    geom.vertices.emplace_back(triangle.C);
    geom.colours.emplace_back(triangle.colour);
    geom.colours.emplace_back(triangle.colour);
    geom.colours.emplace_back(triangle.colour);
  }

  return geom;
}

[[nodiscard]]
auto fractals::serpinsky_triangle(int iter) -> CpuGeometry
{
  std::vector<Triangle> triangles{};
  serpinsky_algo(triangles, starting_triangle, iter);
  return to_cpu_geom(triangles);
}