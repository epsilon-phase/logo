#include "logo/logo.hpp"
#include "catch2.hpp"
TEST_CASE("Turtles are interpolated","[turtles]"){
  logo::Turtle a,b,c;
  a.Angle = 1.0;
  b.Angle = 2.0;
  a.color=sf::Color::White;
  b.color=sf::Color::Black;
  a.width=1.0;
  b.width=2.0;
  a.Location=logo::Point{0.0,0.0};
  b.Location=logo::Point{2.0,2.0};
  c=logo::Turtle::interpolate(0.5,a,b);
  REQUIRE(c.Angle==1.5);
  REQUIRE(c.Location==logo::Point{1.0,1.0});
  REQUIRE(c.color==sf::Color(127,127,127,255));
  REQUIRE(c.width==1.5);
}