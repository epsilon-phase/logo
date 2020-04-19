#pragma once
namespace logo{
  namespace __detail{
    /// Correctness bounds for floating point equality comparisons
    /// Since we still haven't gotten around to that one paper about
    /// "what all computer scientists should know about floating point".
    const float EPSILON = 1E-6f;
  }
  struct Point {
      float x,y;
      float magnitude()const;
      static Point interpolate(float t,const Point &a,const Point& b);
      Point operator+=(Point);
      Point operator-=(Point);    
  };
  Point operator+(Point,Point);
  Point operator-(Point,Point);
  Point operator*(Point,float);
  Point operator/(Point,float);
  bool operator==(Point a,Point b);
}