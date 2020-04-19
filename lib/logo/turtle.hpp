#pragma once
#include "point.hpp"
#include <SFML/Graphics/Color.hpp>
namespace logo{
  struct Line;
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
    /// Turn the turtle
    /// @param angle The angle in radians
    /// @returns The newly turned turtle
    Turtle Turn(float angle)const;
    /// The same as Turn, but in degrees.
    /// @param Angle the angle, in degrees
    /// @returns the newly turned turtle
    Turtle TurnDeg(float angle)const;
    float AngleInDegrees()const;
    
    /// Find the state of the turtle at a specified fraction of the transition
    /// between the two states
    /// Special notes on behavior:
    /// * The pen_down variable of the interpolated turtle is taken from the
    ///   first one regardless of the fraction between the two
    ///
    /// @param t The fraction between the two turtles with
    ///          0.0 being the first one and 1.0 being the second
    /// @param a The first turtle, returned unchanged at t=0.0
    /// @param b The second turtle, returned unchanged at t=1.0
    static Turtle interpolate(float,const Turtle&,const Turtle&);
    
    /// Convenience method to make a line between two turtle states
    /// @param a The first turtle
    /// @param b The second turtle
    /// @returns The line computed between the two states.
    static Line TurtsToLine(const Turtle&,const Turtle&);
  };
  bool operator==(const Turtle&,const Turtle&);
}