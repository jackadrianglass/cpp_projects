//
// Created by Jack Glass on 2020-11-11.
//
#include "2d_curve_algorithms.h"
#include "point.h"
#include <cmath>

constexpr auto green = glm::vec3{0.0f, 1.0f, 0.0f};
auto decasteljau(gsl::span<const gl::Point> ctrl_points, int subdivisions)-> std::vector<gl::Point>
{
  if(subdivisions <= 0.0f) {
    throw std::exception("Invalid step. Needs to be between 0 and 1");
  }
  if(ctrl_points.empty()) return {};
  if(ctrl_points.size() == 1) return {ctrl_points[0]};

  auto result = std::vector<gl::Point>{};
  auto tmp = std::vector<gl::Point>();
  const auto step = 1.0f / static_cast<float>(subdivisions);

  for(const auto& point: ctrl_points){
    tmp.push_back({point.pos, green});
  }

  const auto degree = static_cast<float>(ctrl_points.size());
  for(float u = 0.0f; u <= 1.0f; u += step){
    for(auto i = 1; i < degree; i++) {
      for(auto j = 0; j < degree - i; j++) {
        tmp[j].pos = (1.0f - u) * tmp[j].pos + u * tmp[j + 1].pos;
      }
    }
    result.push_back(tmp[0]);
  }
  result.push_back({ctrl_points.rbegin()->pos, green});
  return result;
}

auto chaikin(gsl::span<const gl::Point> ctrl_points, int subdivisions) -> std::vector<gl::Point>
{
  auto num = static_cast<int>(ctrl_points.size());
  if(ctrl_points.empty()) return {};
  if(num == 1) return {{ctrl_points[0].pos, green}};
  const auto total_points = static_cast<int>(std::pow(2, subdivisions) * (num - 2) + 2);

  auto vec1 = std::vector<gl::Point>(total_points, {{0.0f, 0.0f, 0.0f}, green});
  auto vec2 = std::vector<gl::Point>(total_points, {{0.0f, 0.0f, 0.0f}, green});

  for(auto i = 0; i < num; i++) {
    vec1[i].pos = ctrl_points[i].pos;
  }

  for(auto i = 0; i < subdivisions; i++) {
    if(i % 2 == 0){
      chaikin_impl(vec1, vec2, num);
    }
    else {
      chaikin_impl(vec2, vec1, num);
    }
    num = 2 * (num - 2) + 2;
  }

  if(subdivisions % 2 == 0) {
    return vec1;
  }
  return vec2;
}

auto chaikin_impl(const std::vector<gl::Point>& points, std::vector<gl::Point>& out, int num_elements) -> void
{
  out[0].pos = points[0].pos;
  out[1].pos = 0.5f * points[0].pos + 0.5f * points[1].pos;

  for(auto i = 1; i < num_elements - 2; i++){
    out[2*i  ].pos = 0.75f * points[i].pos + 0.25f * points[i+1].pos;
    out[2*i+1].pos = 0.25f * points[i].pos + 0.75f * points[i+1].pos;
  }

  auto idx = 2* num_elements - 4;
  out[idx].pos = 0.5f * points[num_elements - 2].pos + 0.5f * points[num_elements - 1].pos;
  out[2* num_elements - 3].pos = points[num_elements - 1].pos;
}