//
// Created by Jack Glass on 2020-09-29.
//

#include "koch_snowflake.h"
#include "fractal_math.h"

using namespace fractals;
using gl::shapes::Triangle;

auto fractals::koch_snowflake(Triangle triangle, int iter) -> std::vector<glm::vec3>
{
  std::vector<glm::vec3> points{};
  points.push_back(triangle.A);
  koch_snowflake_algo(points, triangle.A, triangle.C, iter);
  points.push_back(triangle.C);
  koch_snowflake_algo(points, triangle.C, triangle.B, iter);
  points.push_back(triangle.B);
  koch_snowflake_algo(points, triangle.B, triangle.A, iter);

  return points;
}

auto fractals::koch_snowflake_algo(std::vector<glm::vec3> &points, glm::vec3 A, glm::vec3 B, int iter) -> void
{
  if(iter <= 0) return;

  const auto point1 = midpoint(A, B, 1.0f/3.0f);
  const auto point3 = midpoint(A, B, 2.0f/3.0f);
  const auto point2 = point1 + rotate_2d(point3 - point1, 60);

  iter--;

  // Ensure that the vertices are in the correct order for drawing
  koch_snowflake_algo(points, A, point1, iter);
  points.push_back(point1);
  koch_snowflake_algo(points, point1, point2, iter);
  points.push_back(point2);
  koch_snowflake_algo(points, point2, point3, iter);
  points.push_back(point3);
  koch_snowflake_algo(points, point3, B, iter);
}