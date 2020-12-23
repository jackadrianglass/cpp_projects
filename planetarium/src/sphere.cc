//
// Created by Jack Glass on 2020-11-26.
//

#include "mesh_point.h"
#include <glm/glm.hpp>
#include <cmath>
#include "sphere.h"

auto generate_sphere(float radius) -> std::vector<gl::MeshPoint>;
auto generate_face_list(gsl::span<const gl::MeshPoint> vertex_list) -> std::vector<glm::ivec3>;

Sphere::Sphere()
  : m_vertices{generate_sphere(1.0f)}
  , m_faceList{generate_face_list(m_vertices)}
{

}

auto Sphere::vertices() const -> std::vector<gl::MeshPoint>
{
  return m_vertices;
}

auto Sphere::faceList() const -> std::vector<glm::ivec3>
{
  return m_faceList;
}

auto generate_sphere(float radius) -> std::vector<gl::MeshPoint>
{
  const auto gen_point = [radius](const auto phi, const auto theta) -> gl::MeshPoint{
      const auto x = static_cast<float>(theta) / static_cast<float>(360);
      const auto y = static_cast<float>(phi + 90) / static_cast<float>(180);

      const auto p = glm::radians(phi);
      const auto t = glm::radians(theta);
      const auto vert = glm::vec3{
        radius * std::cos(p) * std::cos(t),
        radius * std::sin(p),
        radius * std::cos(p) * std::sin(t)
      };
       return {vert,
         glm::vec2{x, y},
        glm::normalize(vert)
      };
  };

  auto result = std::vector<gl::MeshPoint>{};

  // theta is traversing around the circle
  // phi is traversing from top to bottom
  for(float theta = 0; theta <= 360; theta += 15.0f) {
    for(float phi = 90; phi >= -90; phi -= 15.0f) {
      result.emplace_back(gen_point(phi, theta));
    }
  }
  return result;
}
auto generate_face_list(gsl::span<const gl::MeshPoint> vertex_list) -> std::vector<glm::ivec3>
{
  //!todo make this not a magic number
  constexpr auto width = 13;
  const auto size = vertex_list.size();

  // triangles in the mesh
  const auto get_close = [width, size](const auto idx) -> glm::ivec3 {
    if(idx + width >= size){
      return {idx, idx + width - size + 1, idx + 1};
    }
    return {idx, idx + width + 1, idx + 1};
  };

  const auto get_far = [width, size](const auto idx) -> glm::ivec3 {
    if(idx + width + 1 >= size){
      return {idx, idx + width - size, idx + width - size + 1};
    }
    return {idx, idx + width, idx + width + 1};
  };

  const auto get_end = [width, size](const auto idx) -> glm::ivec3 {
     if(idx + width >= size){
       return {idx, idx + width - size, idx + 1};
     }
     return {idx, idx + width, idx + 1};
  };

  auto result = std::vector<glm::ivec3>{};

  for(auto idx = 0; idx < vertex_list.size(); idx++){
    if((idx + 1) % width == 0) continue;
    if(idx % width == 0){
      result.emplace_back(get_close(idx));
    }
    if((idx + 2) % width == 0){
      result.emplace_back(get_end(idx));
    }
    else {
      result.emplace_back(get_close(idx));
      result.emplace_back(get_far(idx));
    }
  }
  return result;
}