#include "line.hpp"
#include "detail/interpolate.hpp"
using namespace logo;
float Line::slope() const { return (end.y - start.y) / (end.x - start.x); }
Point Line::PointAlong(float fraction) const {
  Point p = start + (end - start) * fraction;
  return p;
}
