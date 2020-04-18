#pragma once
#include <SFML/Graphics.hpp>
namespace logo{
  struct Point {
      float x,y;
      float magnitude()const;
      static Point interpolate(float t,const Point &a,const Point& b);
  };
  Point operator+(Point,Point);
  Point operator-(Point,Point);
  Point operator*(Point,float);
  
  Point operator/(Point,float);
  bool operator==(Point a,Point b);
  struct Turtle{
    float Angle;
    Point Location;
    bool PenDown;
    float width = 1.0f;
    sf::Color color = sf::Color::Black;
    // Find the state of the turtle at a specified fraction of the transition
    // between the two states
    static Turtle interpolate(float,const Turtle&,const Turtle&);
  };
}