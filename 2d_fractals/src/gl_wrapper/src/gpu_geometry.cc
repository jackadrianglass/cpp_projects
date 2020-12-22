//
// Created by Jack Glass on 2020-09-23.
//

#include <vector>
#include "gpu_geometry.h"

using namespace gl;

GpuGeometry::GpuGeometry()
  : m_vao()
  , m_vertex_buffer(0, 3, GL_FLOAT)
  , m_colour_buffer(1, 3, GL_FLOAT)
{}

auto GpuGeometry::bind() const -> void
{
  m_vao.bind();
}

auto GpuGeometry::setVertices(gsl::span<const glm::vec3> vertices) -> void
{
  m_vertex_buffer.uploadData(
    sizeof(glm::vec3) * vertices.size(),
    vertices.data(),
    GL_STATIC_DRAW
    );
}

auto GpuGeometry::setColours(gsl::span<const glm::vec3> colours) -> void
{
  m_colour_buffer.uploadData(
    static_cast<GLsizeiptr>(sizeof(glm::vec3) * colours.size()),
    colours.data(),
    GL_STATIC_DRAW
    );
}

auto GpuGeometry::setColours(const glm::vec3& colour, size_t times) -> void
{
  std::vector<glm::vec3> colours{};
  colours.reserve(times);
  for(auto i = 0; i < times; i++){
    colours.emplace_back(colour);
  }

  setColours(colours);
}
