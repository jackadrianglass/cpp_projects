//
// Created by Jack Glass on 2020-11-26.
//

#pragma once
#include <vector>
#include <string>
#include <glm/vec3.hpp>
#include <gsl/span>
#include "mesh_point.h"

class Sphere
{
private:
  std::vector<gl::MeshPoint> m_vertices;
  std::vector<glm::ivec3> m_faceList;

public:
  Sphere();

  auto vertices() const -> std::vector<gl::MeshPoint>;
  auto faceList() const -> std::vector<glm::ivec3>;
};
