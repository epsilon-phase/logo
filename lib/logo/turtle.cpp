#include "logo.hpp"
#include "detail/interpolate.hpp"
using namespace logo;
using namespace __detail;
template<> sf::Color logo::__detail::lerp<sf::Color>(float f,const sf::Color& a,const sf::Color&b){
  return sf::Color(lerp(f,a.r,b.r),lerp(f,a.g,b.g),lerp(f,a.b,b.b),lerp(f,a.a,b.a));
}
Turtle Turtle::interpolate(float t, const Turtle &a, const Turtle &b){
  return Turtle{
    lerp(t,a.Angle,b.Angle),
    lerp(t,a.Location,b.Location),
    a.PenDown,
    lerp(t,a.width,b.width),
    lerp(t,a.color,b.color)
  };
}