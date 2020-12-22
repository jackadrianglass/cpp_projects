//
// Created by Jack Glass on 2020-09-28.
//

#include <stdexcept>
#include "squares_and_diamonds.h"
#include "fractal_math.h"

using namespace fractals;
using gl::shapes::Square;
constexpr auto blue = glm::vec3{0.1f, 0.1f, 0.3f};
constexpr auto grey = glm::vec3{0.1f, 0.1f, 0.1f};

auto fractals::squares_and_diamonds(int iter) -> std::vector<Square>
{
  std::vector<Square> squares;
  squares.reserve(2*iter);
  squares.push_back({
    {
      glm::vec3{-0.75f, -0.75f, 0.0f},
      glm::vec3{0.75f, -0.75f, 0.0f},
      glm::vec3{0.75f, 0.75f, 0.0f},
      glm::vec3{-0.75f, 0.75f, 0.0f}
    },
    grey
  });
  squares.push_back(calc_midpoint_square(squares.back(), blue));

  for(auto i = 0; i < iter; i++){
    squares.push_back(calc_midpoint_square(squares.back(), grey));
    squares.push_back(calc_midpoint_square(squares.back(), blue));
  }
  return squares;
}

auto fractals::calc_midpoint_square(const Square &square, const glm::vec3& colour) -> Square
{
  const auto& vertices = square.m_vertices;
  if(vertices.size() != 4){
    throw std::runtime_error("A square need four m_vertices");
  }

  return Square{
    {
      midpoint(vertices[0], vertices[1]),
      midpoint(vertices[1], vertices[2]),
      midpoint(vertices[2], vertices[3]),
      midpoint(vertices[3], vertices[0])
    },
    colour
  };
}