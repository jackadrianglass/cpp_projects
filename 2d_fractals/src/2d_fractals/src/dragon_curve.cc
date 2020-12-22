//
// Created by Jack Glass on 2020-09-29.
//

#include "dragon_curve.h"
#include "fractal_math.h"
#include <cmath>
#include <glm/geometric.hpp>

using namespace fractals;

auto fractals::dragon_curve(glm::vec3 A, glm::vec3 B, int iter) -> std::vector<glm::vec3>
{
  std::vector<glm::vec3> points;
  points.push_back(A);
  dragon_curve_algo(points, A, B, iter, Dir::Neg);
  points.push_back(B);

  return points;
}

auto fractals::dragon_curve_algo(std::vector<glm::vec3> &points, glm::vec3 A, glm::vec3 B, int iter, Dir direction) -> void
{
  // Compute once
  static const auto neg_dir = deg_to_rad(315);
  static const auto pos_dir = deg_to_rad(45);

  if(iter <= 0) return;
  const auto AB = B - A;
  const auto theta = angle_from_x_axis(AB);
  const auto calc_c = [](auto& A, auto& B, auto theta, auto rads){
       const auto length_AC = glm::distance(A, B) * cos(rads);
       return A + glm::vec3{
         length_AC * cos(theta + rads),
         length_AC * sin(theta + rads),
         0.0f
       };
  };
  glm::vec3 C;
  if(direction == Dir::Pos){
    C = calc_c(A, B, theta, pos_dir);
  } else {
    C = calc_c(A, B, theta, neg_dir);
  }


  iter--;
  //caller adds point A
  dragon_curve_algo(points, A, C, iter, Dir::Neg);
  points.push_back(C);
  dragon_curve_algo(points, C, B, iter, Dir::Pos);
  //caller adds point B
}