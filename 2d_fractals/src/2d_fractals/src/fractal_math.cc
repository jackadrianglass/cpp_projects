//
// Created by Jack Glass on 2020-09-28.
//

#include "fractal_math.h"
#include <cmath>

auto fractals::midpoint(glm::vec3 first, glm::vec3 second) -> glm::vec3 {
  return 0.5f * (first + second);
}

auto fractals::midpoint(glm::vec3 first, glm::vec3 second, float frac_distance_from_first) -> glm::vec3
{
  return first + frac_distance_from_first * (second - first);
}

auto fractals::rotate_2d(glm::vec3 vec, float deg) -> glm::vec3
{
  const auto rads = deg_to_rad(deg);
  return {
    vec.x * std::cos(rads) - vec.y * std::sin(rads),
    vec.x * std::sin(rads) + vec.y * std::cos(rads),
    0.0f
  };
}

auto fractals::angle_from_x_axis(glm::vec3 vec) -> float
{
  static const auto rad_180 = deg_to_rad(180);
  static const auto rad_360 = deg_to_rad(360);
  const auto intermediate = std::abs(std::atan(vec.y / vec.x));
  // Go by quadrants
  if(vec.x >= 0 && vec.y >= 0){
    return intermediate;
  }else if(vec.x < 0 && vec.y >= 0){
    return rad_180 - intermediate;
  }else if(vec.x < 0 && vec.y < 0){
    return rad_180 + intermediate;
  } else {
    return rad_360 - intermediate;
  }
}

auto fractals::deg_to_rad(float deg) -> float
{
  static const auto pi_to_rad = std::acos(0.0f) * 2.0f / 180.0f;
  return deg * pi_to_rad;
}