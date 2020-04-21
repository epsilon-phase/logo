#include "catch2.hpp"
#include "logo/logo.hpp"
#include <cmath>
TEST_CASE("Turtles are interpolated", "[turtles]") {
  logo::Turtle a, b, c;
  a.Angle = 1.0;
  b.Angle = 2.0;
  a.color = logo::Color::White;
  b.color = logo::Color::Black;
  a.width = 1.0;
  b.width = 2.0;
  a.Location = logo::Point{0.0, 0.0};
  b.Location = logo::Point{2.0, 2.0};
  c = logo::Turtle::interpolate(0.5, a, b);
  SECTION("Interpolated Values") {
    REQUIRE(c.Angle == 1.5);
    REQUIRE(c.Location == logo::Point{1.0, 1.0});
    REQUIRE(c.color == logo::Color{127, 127, 127, 255});
    REQUIRE(c.width == 1.5);
  }
  SECTION("Arbitrary Behavior") { REQUIRE(c.pen_down); }
}
TEST_CASE("Turtles are nondestructive", "[turtles]") {
  // They're just,,, so,,, peaceful
  logo::Turtle a, b, c;
  a.Location = {0, 0};
  c = a;
  WHEN("Turtles are moved forward") {
    b = a.Forward(5.0);
    THEN("The original object is not altered") { REQUIRE(a == c); }
  }
  WHEN("Turtles are turned and moved in a unit") {
    b = a.Turn(1.5f).Forward(5);
    THEN("The original object remains the same") { REQUIRE(a == c); }
  }
}
TEST_CASE("Turtles are moved correctly", "[turtles]") {
  logo::Turtle a, b, c, d;
  a.Location = {0, 0};
  // Just zero out the locations
  b = a;
  c = a;
  a.Angle = 0.0;
  b.Angle = M_PI / 2.0f;
  WHEN("Turtles are moved forwards when in the first quadrant") {
    c = a.Forward(1.0);
    d = b.Forward(1.0);
    THEN("They end up in the right place") {
      REQUIRE(c.Location == logo::Point{1.0, 0.0});
      REQUIRE(d.Location == logo::Point{0.0, 1.0});
    }
  }

  WHEN("Turtles are moved forwards facing the second quadrant") {
    const float a_angle = M_PIf32,       // 180 degrees
        b_angle = 3.0f * M_PIf32 / 4.0f; // 135 degrees
    a.Angle = a_angle;
    b.Angle = b_angle;
    c = a.Forward(1.0); // Should be (-1.0,0.0);
    d = b.Forward(1.0); // Should be something like
    const float length = 1.0, c_theta = M_PIf32 * 1.5;
    logo::Point c_ref = logo::Point{std::cos(a_angle) * length,
                                    std::sin(a_angle) * length},
                d_ref = logo::Point{std::cos(b_angle) * length,
                                    std::sin(b_angle) * length};
    THEN("They still end up in the right spots") {
      REQUIRE(c.Location == logo::Point{-1.0f, 0.0f});
      REQUIRE(c.Location == c_ref);
      REQUIRE(d.Location == d_ref);
    }
  }
}
