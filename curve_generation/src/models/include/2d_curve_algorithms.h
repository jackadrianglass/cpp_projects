//
// Created by Jack Glass on 2020-11-11.
//

#pragma once
#include <vector>
#include <gsl/span>
#include "point.h"

auto decasteljau(gsl::span<const gl::Point> ctrl_points, int subdivisions)-> std::vector<gl::Point>;
auto chaikin(gsl::span<const gl::Point> ctrl_points, int subdivisions) -> std::vector<gl::Point>;
auto chaikin_impl(const std::vector<gl::Point>& points, std::vector<gl::Point>& out, int num_elements) -> void;

