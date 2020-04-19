#pragma once
#include "turtle.hpp"
#include "line.hpp"
#include <vector>
namespace logo{
  struct State{
    Turtle current;
    std::vector<Line> paths;
  };
}