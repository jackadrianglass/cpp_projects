//
// Created by Jack Glass on 2020-11-14.
//

#include "3d_surface_algorithms.h"
#include "colours.h"
#include <glm/glm.hpp>
#include <algorithm>

auto mdl::calcRevolvedSurface(gsl::span<const gl::Point> curve, int subdivisions) -> std::vector<gl::Point>
{
  if(curve.empty()) return {};

  auto result = std::vector<gl::Point>();
  result.reserve(curve.size() * subdivisions);

  const auto step = glm::radians(360.0f) / static_cast<float>(subdivisions);

  const auto min_point = std::min_element(std::begin(curve),
                                          std::end(curve),
                                          [](const auto& a, const auto& b){
                                            return a.pos.x < b.pos.x;
                                          });
  const auto min_x = min_point->pos.x;

  for(auto i = 0; i < subdivisions; i++)
  {
    glm::vec3 colour;
    if(i % 2 == 0){
      colour = crimson;
    } else {
      colour = blue;
    }
    for(const auto& point : curve){
      const auto angle = step * i;
      // need to remember to translate everything so that the min x value is
      // at the origin, then translate back
      const auto pos = glm::vec3{
        (point.pos.x - min_x) * cos(angle) + min_x,
        point.pos.y,
        (point.pos.x - min_x) * sin(angle)
      };
      result.push_back({pos, colour});
    }
  }
  return result;
}

auto mdl::calculateIndices(gsl::span<const gl::Point> vertices, unsigned int row_w, CurveType type) -> std::vector<unsigned int>
{
  if(vertices.empty()) return {};

  auto result = std::vector<unsigned int>();
  const auto size = static_cast<unsigned int>(vertices.size());

  // triangles have 3 vertices so always push in 3s
  const auto push3back = [](auto& container, auto a, auto b, auto c){
         container.push_back(a);
         container.push_back(b);
         container.push_back(c);
  };

  for(auto idx = 0u; idx < size; idx++){
    if((idx + 1) % row_w == 0){
      continue;
    }


    unsigned int row_down;
    if(row_w > idx) {
      if(type == CurveType::closed){
        row_down = idx + size - row_w;
        push3back(result, idx, idx + 1, row_down);
      }
    }
    else{
      row_down = idx - row_w;
      push3back(result, idx, idx + 1, row_down);
    }

    unsigned int row_up = row_w + idx;
    if(row_up >= size){
      row_up -= size;
      if(type == CurveType::closed) {
       push3back(result, idx, idx + 1, row_up + 1);
      }
    }
    else {
      push3back(result, idx, idx + 1, row_up + 1);
    }
  }
  return result;
}