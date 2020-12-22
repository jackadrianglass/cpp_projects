//
// Created by Jack Glass on 2020-09-28.
//

#ifndef MYPROJECT_SQUARE_H
#define MYPROJECT_SQUARE_H

#include <array>
#include <glm/vec3.hpp>

namespace gl{
namespace shapes{

  struct Square
  {
    std::array<glm::vec3, 4> m_vertices;
    glm::vec3 m_colour;

    auto operator==(const Square& other) const -> bool;
    auto operator!=(const Square& other) const -> bool;
  };

}}


#endif//MYPROJECT_SQUARE_H
