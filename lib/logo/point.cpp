#include "point.hpp"
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
  Point Point::operator+=(Point c){
    Point tmp = *this;
    this->x=c.x+x;
    this->y=c.y+y;
    return tmp;
  }
  Point Point::operator-=(Point b){
    Point tmp = *this;
    (*this)=(*this)-b;
    return tmp;
  }
  float Point::magnitude()const{
    return std::sqrt(x*x+y*y);
  }
  Point Point::interpolate(float t,const Point &a,const Point& b){
    using namespace __detail;
    return Point{lerp(t,a.x,b.x),lerp(t,a.x,b.x)};
  }
  
  bool operator==(Point a,Point b){
    using namespace __detail;
    float dx = a.x-b.x,
          dy = a.y-b.y;
    return EPSILON >=dx && -EPSILON<=dx &&
           EPSILON >=dy && -EPSILON<=dy;
  }
}