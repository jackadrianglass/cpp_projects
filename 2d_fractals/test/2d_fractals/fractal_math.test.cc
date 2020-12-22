//
// Created by Jack Glass on 2020-09-29.
//

#include <Catch2/catch.hpp>
#include "fractal_math.h"

using namespace fractals;

TEST_CASE("Mid Point Calculations", "[2d fractals]"){
   const auto A = glm::vec3{0.5, 1.0, 0.0};
   const auto B = glm::vec3{1.0, 0.5, 0.0};

   REQUIRE(midpoint(A, B) == glm::vec3{0.75, 0.75, 0.0});
   const auto result = midpoint(A, B, 0.75);
   REQUIRE( result == glm::vec3{0.5 + (0.75)*0.5, 1.0 + (0.75)*(-0.5), 0.0});
}

TEST_CASE("Vector Rotation Calculations", "[2d fractals]"){
  const auto A = glm::vec3{1.0f, 0.0f, 0.0f};
  const auto result = rotate_2d(A, 90);
  REQUIRE(result.x == Approx(0.0f).margin(0.0001));
  REQUIRE(result.y == Approx(1.0f).epsilon(0.001));
  REQUIRE(result.z == 0.0f);
}

TEST_CASE("Vector angle calculation", "[2d fractals]") {
  REQUIRE(angle_from_x_axis({1.0f, 0.0f, 0.0f}) == Approx(0.0f));
  REQUIRE(angle_from_x_axis({0.0f, 1.0f, 0.0f}) == Approx(deg_to_rad(90.0f)));
  REQUIRE(angle_from_x_axis({0.0f, -1.0f, 0.0f}) == Approx(deg_to_rad(270.0f)));
  REQUIRE(angle_from_x_axis({-1.0f, 0.0f, 0.0f}) == Approx(deg_to_rad(180.0f)));
}