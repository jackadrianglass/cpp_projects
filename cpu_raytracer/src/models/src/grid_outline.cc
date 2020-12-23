//
// Created by Jack Glass on 2020-11-11.
//

#include "grid_outline.h"

static constexpr auto violet = glm::vec3{0.498f, 0.0f, 1.0f};

mdl::GridOutline::GridOutline(glm::vec2 bounds, float step, float level, mdl::Plane plane)
  : m_points()
{
  for(auto i = bounds.x; i <= bounds.y; i += step) {
    switch(plane){
      case mdl::Plane::XY: {
        m_points.push_back({{bounds.x, i, level}, violet});
        m_points.push_back({{bounds.y, i, level}, violet});

        m_points.push_back({{i, bounds.x, level}, violet});
        m_points.push_back({{i, bounds.y, level}, violet});
        break;
      }
      case mdl::Plane::XZ: {
        m_points.push_back({{bounds.x, level, i}, violet});
        m_points.push_back({{bounds.y, level, i}, violet});

        m_points.push_back({{bounds.x, i, level}, violet});
        m_points.push_back({{bounds.y, i, level}, violet});
        break;
      }
      case mdl::Plane::YZ: {
        m_points.push_back({{level, bounds.x, i}, violet});
        m_points.push_back({{level, bounds.y, i}, violet});

        m_points.push_back({{level, bounds.x, i}, violet});
        m_points.push_back({{level, bounds.y, i}, violet});
        break;
      }
    }
  }
}