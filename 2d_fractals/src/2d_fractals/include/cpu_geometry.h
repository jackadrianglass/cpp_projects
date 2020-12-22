//
// Created by Jack Glass on 2020-09-28.
//

#ifndef MYPROJECT_CPU_GEOMETRY_H
#define MYPROJECT_CPU_GEOMETRY_H
#include <vector>
#include <glm/vec3.hpp>

namespace fractals {

  class CpuGeometry {
  public:
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> colours;

    auto operator==(const CpuGeometry& other) const -> bool;
  };

}
#endif//MYPROJECT_CPU_GEOMETRY_H
