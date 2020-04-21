#include "color.hpp"

using namespace logo;
bool Color::operator==(const Color &c) const {
  return r == c.r && g == c.g && b == c.b && a == c.a;
}
const Color Color::Black = Color{0, 0, 0, 255};
const Color Color::White = Color{255, 255, 255, 255};
const Color Color::Blue = Color{0, 0, 255, 255};
const Color Color::Green = Color{0, 255, 0, 255};
const Color Color::Red = Color{255, 0, 0, 255};
const Color Color::Transparent = Color{0, 0, 0, 0};