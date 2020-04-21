#pragma once
namespace logo {
  struct Point;
  namespace __detail {
    /// Compute the linear interpolation between two objects
    /// @param f The fraction
    /// @param a The first object
    /// @param b The second object
    /// @return The interpolation specified by f between a and b
    template <typename T> T lerp(float f, const T &a, const T &b) {
      return a + (b - a) * f;
    }
  } // namespace __detail

} // namespace logo