#include "turtle.hpp"
#include "line.hpp"
#include "detail/interpolate.hpp"
#include <cmath>
using namespace logo;
using namespace __detail;
template<> sf::Color logo::__detail::lerp<sf::Color>(float f,const sf::Color& a,const sf::Color&b){
  return sf::Color(lerp(f,a.r,b.r),lerp(f,a.g,b.g),lerp(f,a.b,b.b),lerp(f,a.a,b.a));
}

Turtle Turtle::interpolate(float t, const Turtle &a, const Turtle &b){
  return Turtle{
    lerp(t,a.Angle,b.Angle),
    lerp(t,a.Location,b.Location),
    a.pen_down,
    lerp(t,a.width,b.width),
    lerp(t,a.color,b.color)
  };
}
Line Turtle::TurtsToLine(const Turtle& a,const Turtle& b){
  Line result;
  result.start = a.Location;
  result.end = b.Location;
  result.start_width=a.width;
  result.end_width=b.width;
  return result;
}
Turtle Turtle::Forward(float length)const{
  Point delta={std::cos(Angle)*length,std::sin(Angle)*length};
  Turtle l=*this;
  l.Location+=delta;
  return l;
}
Turtle Turtle::Turn(float angle)const{
  Turtle l=*this;
  l.Angle+=angle;
  return l;
}
Turtle Turtle::TurnDeg(float angle)const{
  return Turn((angle*M_PIf32)/2.0f);
}
float Turtle::AngleInDegrees()const{
  return (180.0/M_PIf32)*Angle;
}
bool logo::operator==(const Turtle& a,const Turtle& b){
  return a.Angle    == b.Angle    &&
         a.Location == b.Location &&
         a.pen_down == b.pen_down &&
         a.width    == b.width    &&
         a.color    == b.color;
}