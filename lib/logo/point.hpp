#pragma once
#include "detail/Epsilon.hpp"
namespace logo {

  struct Point {
    float x, y;
    float magnitude() const;
    static Point interpolate(float t, const Point &a, const Point &b);
    /**
     * Adds another point elementwise to this one
     * @param b the other point
     * @returns The previous value
     * */
    Point operator+=(Point);
    /**
     * Subtracts another point elementwise to this one
     * @param b the other point
     * @returns the previous value
     **/
    Point operator-=(Point);
    /**
     * Compares two points within a bound of precision specified by
     * logo::__detail::EPSILON
     * @param b The point being compared to
     * @returns True if the points are sufficiently similar, false otherwise
     * */
    bool operator==(Point b) const;
  };
  Point operator+(Point, Point);
  Point operator-(Point, Point);
  Point operator*(Point, float);
  Point operator/(Point, float);

} // namespace logo