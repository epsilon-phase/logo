#pragma once
#include "point.hpp"

namespace logo {
  struct Line {
    Point start, end;
    float start_width, end_width;
    Line interpolate(float fraction) const;
  };
} // namespace logo