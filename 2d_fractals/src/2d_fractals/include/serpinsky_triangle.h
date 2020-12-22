//
// Created by Jack Glass on 2020-09-23.
//

#ifndef MYPROJECT_SERPINSKY_TRIANGLE_H
#define MYPROJECT_SERPINSKY_TRIANGLE_H

#include "cpu_geometry.h"

namespace fractals {

  [[nodiscard]]
  auto serpinsky_triangle(int iter) -> CpuGeometry;

}


#endif//MYPROJECT_SERPINSKY_TRIANGLE_H
