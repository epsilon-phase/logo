#pragma once
#include <cstddef>
namespace logo {
  /// Because any decent cross-backend code needs this because...
  /// ... Well, it's important.
  struct Color {
    // Byte, surprisingly, isn't what we want
    unsigned char r, g, b, a;
    bool operator==(const Color &b) const;
    static const Color Black;
    static const Color White;
    static const Color Blue;
    static const Color Transparent;
    static const Color Magenta;
    static const Color Red;
    static const Color Green;
  };
} // namespace logo