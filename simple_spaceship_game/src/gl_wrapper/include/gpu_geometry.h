//
// Created by Jack Glass on 2020-09-23.
//

#ifndef MYPROJECT_GPU_GEOMETRY_H
#define MYPROJECT_GPU_GEOMETRY_H

#include <gsl/span>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include "vertex_array.h"
#include "vertex_buffer.h"

namespace gl {
  class GpuGeometry
  {
  public:
    GpuGeometry();
    auto bind() const -> void;
    auto setVertices(gsl::span<const glm::vec3> vertices) -> void;
    auto setTexCoord(gsl::span<const glm::vec2> tex_coords) -> void;
//    auto setColours(gsl::span<const glm::vec3> colours) -> void;
//    auto setColours(const glm::vec3& colour, size_t times) -> void;

  private:
    VertexArray m_vao;
    VertexBuffer m_vertex_buffer;
    VertexBuffer m_texture_coord_buffer;
//    VertexBuffer m_colour_buffer;
  };
}

#endif//MYPROJECT_GPU_GEOMETRY_H
