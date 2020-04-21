#pragma once
#include "line.hpp"
#include "turtle.hpp"
#include <vector>
namespace logo {
  struct State {
    /// The current turtle.
    Turtle current;
    std::vector<Line> paths;
  };
} // namespace logo