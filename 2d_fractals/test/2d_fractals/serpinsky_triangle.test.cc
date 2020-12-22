//
// Created by Jack Glass on 2020-09-23.
//
#include <catch2/catch.hpp>
#include "serpinsky_triangle.h"

using namespace fractals;

TEST_CASE("Serpinsky triangle computed", "[2d fractals]")
{
  // This is the default triangle that will be working with
  constexpr auto A = glm::vec3{-0.75f, -0.75f, 0.0f};
  constexpr auto B = glm::vec3{0.75f, -0.75f, 0.0f};
  constexpr auto C = glm::vec3{0.0f, 0.75f, 0.0f};

  constexpr auto midpoint = [](auto first, auto second){
    return 0.5f * first + 0.5f * second;
  };

  SECTION("Basic case is correct"){
    const auto expect = CpuGeometry{{A, B, C}, {}};

    REQUIRE(serpinsky_triangle(0) == expect);
  }
  SECTION("Add one iteration"){
    constexpr auto AC = midpoint(A, C);
    constexpr auto AB = midpoint(A, B);
    constexpr auto BC = midpoint(B, C);
    const auto expect = CpuGeometry{
      {
        A, AB, AC,
        AB, B, BC,
        BC, C, AC,
      },
      {}
    };

    REQUIRE(serpinsky_triangle(1) == expect);
  }

}