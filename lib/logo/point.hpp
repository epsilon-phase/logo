#pragma once
namespace logo {
  namespace __detail {
    /// Correctness bounds for floating point equality comparisons
    /// Since we still haven't gotten around to that one paper about
    /// "what all computer scientists should know about floating point".
    const float EPSILON = 1E-6f;
  } // namespace __detail
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