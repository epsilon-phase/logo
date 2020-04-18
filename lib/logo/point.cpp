#include "logo.hpp"
#include <cmath>
#include "detail/interpolate.hpp"
namespace logo{
  Point operator+(Point a,Point b){
    return Point{a.x+b.x,a.y+b.y};
  }
  Point operator-(Point a, Point b){
    return Point{a.x-b.x,a.y-b.y};
  }
  Point operator*(Point a,float b){
    return Point{a.x*b,a.y*b};
  }
  
  Point operator/(Point a,float b){
    return Point{a.x/b,a.y/b};
  }
  float Point::magnitude()const{
    return std::sqrt(x*x+y*y);
  }
  Point Point::interpolate(float t,const Point &a,const Point& b){
    using namespace __detail;
    return Point{lerp(t,a.x,b.x),lerp(t,a.x,b.x)};
  }
  bool operator==(Point a,Point b){
    return a.x==b.x&&a.y==b.y;
  }
}