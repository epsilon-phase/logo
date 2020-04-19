#include "line.hpp"
#include "detail/interpolate.hpp"
using namespace logo;
Line Line::interpolate(float fraction)const{
  Line a;
  a=*this;
  a.end=__detail::lerp(fraction,a.start,a.end);
  a.end_width=__detail::lerp(fraction,a.start_width,a.end_width);
  return a;
}