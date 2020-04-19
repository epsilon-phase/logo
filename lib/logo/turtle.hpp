#pragma once
#include "point.hpp"
#include <SFML/Graphics/Color.hpp>
namespace logo{
  struct Turtle{
    float Angle;
    Point Location=Point{0.0f,0.0f};
    bool pen_down;
    float width = 1.0f;
    sf::Color color = sf::Color::Black;
    /// Move the turtle length units forward
    /// @param length The length to move
    /// @returns The turtle moved after being forwards
    Turtle Forward(float length)const;
    Turtle Turn(float angle)const;
    /// Find the state of the turtle at a specified fraction of the transition
    /// between the two states
    static Turtle interpolate(float,const Turtle&,const Turtle&);
  };
  bool operator==(const Turtle&,const Turtle&);
}