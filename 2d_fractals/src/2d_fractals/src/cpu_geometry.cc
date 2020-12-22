//
// Created by Jack Glass on 2020-09-28.
//
#include "cpu_geometry.h"

using namespace fractals;
auto CpuGeometry::operator==(const CpuGeometry& other) const -> bool{
  return vertices == other.vertices;
}