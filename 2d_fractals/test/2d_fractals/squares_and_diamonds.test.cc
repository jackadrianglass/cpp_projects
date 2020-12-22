//
// Created by Jack Glass on 2020-09-28.
//

#include <catch2/catch.hpp>
#include "squares_and_diamonds.h"

TEST_CASE("Squares and Diamonds computed", "[2d fractals]")
{
  SECTION("Can calculate a square within square"){
    const auto input = gl::shapes::Square{
      {
        glm::vec3{-1.0f, 0.0f, 0.0f},
        glm::vec3{0.0f, -1.0f, 0.0f},
        glm::vec3{1.0f, 0.0f, 0.0f},
        glm::vec3{0.0f, 1.0f, 0.0f}
      },
      glm::vec3{}
    };
    const auto expect = gl::shapes::Square{
      {
        glm::vec3{-0.5f, -0.5f, 0.0f},
        glm::vec3{0.5f, -0.5f, 0.0f},
        glm::vec3{0.5f, 0.5f, 0.0f},
        glm::vec3{-0.5f, 0.5f, 0.0f}
      },
      glm::vec3{}
    };

    REQUIRE(fractals::calc_midpoint_square(input, {}) == expect);
  }
}