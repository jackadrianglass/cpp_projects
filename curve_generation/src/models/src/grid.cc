//
// Created by Jack Glass on 2020-11-14.
//

#include <random>
#include <ctime>
#include <2d_curve_algorithms.h>
#include "3d_surface_algorithms.h"

#include "grid.h"
#include "colours.h"

auto init_xy(std::vector<gl::Point>& m_points, glm::vec2 bounds, float step) -> void;
auto init_xz(std::vector<gl::Point>& m_points, glm::vec2 bounds, float step) -> void;
auto init_yz(std::vector<gl::Point>& m_points, glm::vec2 bounds, float step) -> void;
auto rand(float max_val) -> float;

using namespace mdl;

Grid::Grid(glm::vec2 bounds, float step, mdl::Plane plane, Algorithm type, unsigned int seed)
  : m_type{type}
  , m_rowWidth{static_cast<size_t>((bounds.y + 1 - bounds.x) / step)}
{
  std::srand(seed);
  switch (plane) {
  case Plane::XY:
  {
    init_xy(m_points, bounds, step);
    break;
  }
  case Plane::XZ: {
    init_xz(m_points, bounds, step);
    break;
  }
  case Plane::YZ: {
    init_yz(m_points, bounds, step);
  }
  }
  size_t width{};
  if(m_type == Algorithm::bspline){
    width = initBspline();
  }
  else if(m_type == Algorithm::bezier){
    width = initBezier();
  }
  m_indices = mdl::calculateIndices(m_vertices, static_cast<unsigned int>(width));
}

auto Grid::initBezier() -> size_t
{
  constexpr auto step = 0.5f;
  constexpr auto width = static_cast<int>(1 / step) + 1;

  m_vertices = std::vector<gl::Point>(width * width, gl::Point{glm::vec3{0}, gold});
  auto tmp = std::vector<gl::Point>(m_rowWidth, {glm::vec3{}, gold});

  auto dc = [](std::vector<gl::Point> t, auto u, auto degree){
   for(auto i = 1; i < degree; i++){
     for(auto j = 0; j < degree - i; j++){
       t[j].pos = (1.0f - u) * t[j].pos + u * t[j + 1].pos;
     }
   }
   return t[0];
  };

  //iterating over columns first
  for(auto col_begin = 0; col_begin < m_rowWidth; col_begin++){
    for(auto i = 0; i < m_rowWidth; i++){
      auto idx = col_begin + (i * m_rowWidth);
      tmp[i].pos = m_points[idx].pos;
    }
    //actual bezier time
    for(float u = 0; u < 1.0f; u += step){
      auto i = static_cast<int>(width * u);
      auto index = col_begin + (i * width);
      auto val = dc(tmp, u, m_rowWidth);
      m_vertices[index] = val;
    }

    auto idx = col_begin + width * (width - 1);
    auto pos = tmp.rbegin()->pos;
    m_vertices[idx].pos = pos;
  }

  //iterate over row
  for(auto row = 0; row < width; row++){
    std::copy(std::begin(m_vertices) + (row * width),
              std::begin(m_vertices) + (row * width) + m_rowWidth,
              std::begin(tmp));
    //actual bezier time
    for(float u = 0; u < 1.0f; u += step){
      auto i = static_cast<int>(width * u);
      auto idx = i + (row * width);
      auto val = dc(tmp, u, m_rowWidth);
      m_vertices[idx] = val;
    }
    auto idx = row * width;
    m_vertices[idx].pos = tmp.rbegin()->pos;
  }
  return width;
}

auto Grid::initBspline() -> size_t
{
  auto divisions = 3;
  auto tmp = std::vector<gl::Point>(m_rowWidth);
  const auto width = static_cast<int>(std::pow(2, divisions)
  * (static_cast<double>(m_rowWidth) - 2) + 2);

  m_vertices = std::vector<gl::Point>(width * width);

  // Iterate over columns first
  for(auto col = 0; col < m_rowWidth; col++){
    for(auto i = 0; i < tmp.size(); i++){
      auto idx = col + (i * m_rowWidth);
      tmp[i].pos = m_points[idx].pos;
    }

    auto tmp2 = chaikin(tmp, divisions);
    for(auto i = 0; i < tmp2.size(); i++)
    {
      auto idx = col + (i * width);
      m_vertices[idx] = tmp2[i];
    }
  }

  // Iterate over rows next
  for(auto row = 0; row < width; row++){
    std::copy(std::begin(m_vertices) + (row * width),
    std::begin(m_vertices) + (row * width) + m_rowWidth,//intentionally use both widths
    std::begin(tmp));

    auto tmp2 = chaikin(tmp, divisions);
    std::copy(std::begin(tmp2),
              std::end(tmp2),
              std::begin(m_vertices) + (row * width));

  }
  return width;
}

auto rand(float max_val) -> float
{
  return (static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX))
          * max_val - max_val / 2.0f;
}

auto init_xy(std::vector<gl::Point>& m_points, glm::vec2 bounds, float step) -> void
{
//  const auto z_max = (bounds.y - bounds.x) / 4;
//  for(auto i = bounds.x; i <= bounds.y; i += step) {
//    for(auto j = bounds.x; j <= bounds.y; j += step) {
//      const auto z_val = rand(z_max);
//      m_points.push_back({{i, j, z_val}, slate_blue});
//    }
//  }
  for(auto i = bounds.x; i <= bounds.y; i += step) {
    for(auto j = bounds.x; j <= bounds.y; j += step) {
      m_points.push_back({{i, j, 0}, slate_blue});
    }
  }
}

auto init_xz(std::vector<gl::Point>& m_points, glm::vec2 bounds, float step) -> void
{
//  const auto y_max = (bounds.y - bounds.x) / 4;
//  for(auto i = bounds.x; i <= bounds.y; i += step) {
//    for(auto j = bounds.x; j <= bounds.y; j += step) {
////      const auto y_val = rand(y_max);
//      float y_val;
//      if(static_cast<int>(i) % 2 == 0 && static_cast<int>(j) % 2 == 0) y_val = 0.5f;
//      else y_val = -0.5;
//      m_points.push_back({{i, y_val, j}, slate_blue});
//    }
//  }
  for(auto i = bounds.x; i <= bounds.y; i += step) {
    for(auto j = bounds.x; j <= bounds.y; j += step) {
      m_points.push_back({{i, 0, j}, slate_blue});
    }
  }
}

auto init_yz(std::vector<gl::Point>& m_points, glm::vec2 bounds, float step) -> void
{
  const auto x_max = (bounds.y - bounds.x) / 4;
  for(auto i = bounds.x; i <= bounds.y; i += step) {
    for(auto j = bounds.x; j <= bounds.y; j += step) {
      const auto x_val = rand(x_max);
      m_points.push_back({{x_val, i, j}, slate_blue});
    }
  }
}

auto mdl::Grid::vertices() const -> std::vector<gl::Point>
{
  return m_vertices;
}

auto mdl::Grid::indices() const -> std::vector<unsigned int>
{
  return m_indices;
}
