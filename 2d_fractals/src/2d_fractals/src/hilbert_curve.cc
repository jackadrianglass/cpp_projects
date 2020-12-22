//
// Created by Jack Glass on 2020-09-30.
//

#include "hilbert_curve.h"
#include <array>
#include <cmath>

/*
Algorithm from http://blog.marcinchwedczuk.pl/iterative-algorithm-for-drawing-hilbert-curve
 and code inspired by https://www.youtube.com/watch?v=dSK-MW-zuAc
 */
auto fractals::hilbert_curve(int iter) -> std::vector<glm::vec3>{
  auto points = std::vector<glm::vec3>();
  const auto N = std::pow(2, iter);
  const auto total_points = N * N;
  const float line_length = 2.0f / N;
  const auto transformation = glm::vec3{line_length / 2 - 1, line_length / 2 - 1, 0};

  for(auto i = 0; i < total_points; i++) {
    auto point = hilbert_curve_algo(i, N);
    point = point * line_length;
    point += transformation;
    points.push_back(point);
  }

  return points;
}

auto fractals::hilbert_curve_algo(unsigned int idx, int N) -> glm::vec3
{
  static constexpr std::array<glm::vec3, 4> points = {
    glm::vec3{0, 0, 0},
    glm::vec3{0, 1, 0},
    glm::vec3{1, 1, 0},
    glm::vec3{1, 0, 0}
  };
  auto result = points[idx & 3];
  idx >>= 2;

  for(auto i = 4; i <= N; i *= 2) {
    const auto i2 = i / 2.0f;
    switch(idx & 3){
    case 0:
    {
      std::swap(result.x, result.y);
      break;
    }
    case 1:
    {
      result.y += i2;
      break;
    }
    case 2:
    {
      result.x += i2;
      result.y += i2;
      break;
    }
    case 3:
    {
      auto tmp = result.y;
      result.y = i2 - 1 - result.x;
      result.x = i2 - 1 - tmp;
      result.x += i2;
      break;
    }
    default:
      break;
    }
    idx >>= 2;
  }
  return result;
}
