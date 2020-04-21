#pragma once
#include "color.hpp"
#include "point.hpp"
namespace logo {
  /// The minimal definition of a line segment
  /// Contains nothing but the simple geometric necessities
  struct Line {
    Point start, end;
    Point PointAlong(float fraction) const;
    float slope() const;
  };
  /**
   * A line segment, but with additional information that might be useful
   * when drawing it, such as the start and end widths, and the colors at each
   * endpoint
   * */
  struct GraphicalLine : public Line {
    float start_width, end_width;
    Color start_color, end_color;
  };
} // namespace logo