

#include "logo/language/lexer/lexer.hpp"

#include <cmath> //For nan()
#include <iostream>
#include <logo/vm/stack.hpp>
namespace logo {
  namespace vm {
    std::ostream &operator<<(std::ostream &o, const logo::vm::Number &r) {
      if (r.isNumber())
        o << r.fp;
      else if (r.isArray())
        o << "Array(" << r.addr << ")";
      else if (r.isString())
        o << "String(" << r.addr << ")";
      return o;
    }
  } // namespace vm
} // namespace logo
#include "./catch2.hpp"
TEST_CASE("Number boxing", "[vm]") {
  using namespace logo::vm;
  WHEN("Numbers are set to strings") {
    Number r;
    r.setString(0);
    THEN("It is not a number") { REQUIRE_FALSE(r.isNumber()); }
    THEN("It is not an array") { REQUIRE_FALSE(r.isArray()); }
    THEN("It is a string") { REQUIRE(r.isString()); }
  }
  WHEN("Numbers are set to arrays") {
    Number r;
    r.setArray(0);
    THEN("IT is not a number") { REQUIRE_FALSE(r.isNumber()); }
    THEN("It is not a string") { REQUIRE_FALSE(r.isString()); }
    THEN("It is an array") { REQUIRE(r.isArray()); }
  }
  WHEN("Numbers are just numbers") {
    Number r;
    r = 15;
    Number b = r + r;
    THEN("IT is known to be a number") { REQUIRE(r.isNumber()); }
    THEN("IT is equal to 15") { REQUIRE(r == 15); }
    THEN("15+15 is still 30") { REQUIRE((r + r) == 30.0); }
  }
}