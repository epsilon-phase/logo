
#include "./catch2.hpp"
#include <cmath> //For nan()
#include <iostream>
#include <logo/vm/stack.hpp>
namespace Catch {
  template <> struct StringMaker<logo::vm::Number> {
    static std::string convert(logo::vm::Number const &value) {
      if (value.isInt())
        return std::to_string(value.Integral.i);
      else
        return std::to_string(value.fp);
    }
  };
} // namespace Catch
std::ostream &operator<<(std::ostream &os, logo::vm::Number const &value) {
  if (value.isInt())
    os << value.Integral.i;
  else
    os << value.fp;
  return os;
}
TEST_CASE("Numbers") {
  using namespace logo::vm;
  Number a, b, c;
  WHEN("numbers are instantiated Declared") {
    THEN("They are of the right size") {
      REQUIRE(sizeof(Number) == sizeof(double));
    }
    THEN("And so are the doubles") { REQUIRE(sizeof(double) == 8); }
  }
  WHEN("Numbers are added") {

    THEN("They are of the appropriate format") {
      a.fp = std::nan("");
      REQUIRE(a.Integral.exponent == 0b11111111111);
      REQUIRE(a.isInt());
    }

    THEN("They are not converted to doubles except if necessary") {
      b = Number::FromInt(3);
      REQUIRE(b.isInt());
    }
    THEN("They are added properly") {
      c = Number::fromFloat(5.0);
      a = b + c;
      REQUIRE(b.Integral.i == 3);
      REQUIRE(c.fp == 5.0);
      REQUIRE(!a.isInt());
      REQUIRE(a.fp == 8.0);
      REQUIRE(a == 8);
    }
  }
  WHEN("Numbers are multiplied") {
    a = Number::FromInt(5);
    b = Number::FromInt(2);
    auto c = a * b;
    THEN("They are correct") { REQUIRE(c == 10); }
    THEN("They do not become doubles unnecessarily") { REQUIRE(c.isInt()); }
    b = Number::fromFloat(2.5);
    THEN("THey become floats as necessary") {
      c = a * b;
      REQUIRE(c == 12.5);
    }
  }
  WHEN("Numbers are too big to be integers") {
    a = Number::FromInt(1L << 53);
    REQUIRE_FALSE(a.isInt());
  }
}
logo::vm::Number fibonacci_iter(logo::vm::Number n) {
  using namespace logo::vm;
  Number a = Number::FromInt(1), b = a;
  for (Number i = Number::FromInt(1); i < n; i = i + Number::FromInt(1)) {

    Number c = a + b;
    a = b;
    b = c;
  }
  return b;
}
logo::vm::Number fibonacci_recur(logo::vm::Number n) {
  using namespace logo::vm;
  if (n == 0 || n == 1)
    return Number::FromInt(1);
  return fibonacci_recur(n - Number::FromInt(1)) +
         fibonacci_recur(n - Number::FromInt(2));
}
TEST_CASE("Fibonacci") {
  using namespace logo::vm;
  // If these don't work, then the number stuff is almost certainly broken
  REQUIRE(Number::FromInt(1) == fibonacci_iter(Number::FromInt(0)));
  REQUIRE(Number::FromInt(1) == fibonacci_iter(Number::FromInt(1)));
  REQUIRE(Number::FromInt(8) == fibonacci_iter(Number::FromInt(5)));
  REQUIRE(Number::FromInt(8) == fibonacci_recur(Number::FromInt(5)));
  // Most people probably don't give a damn about this if they're testing it
  // tbh
#ifdef CATCH_CONFIG_ENABLE_BENCHMARKING
  BENCHMARK("Number type fibonacci iterative (20)") {
    return fibonacci_iter(Number::FromInt(20));
  };
  BENCHMARK("Number type fibonacci recur(20)") {
    return fibonacci_recur(Number::FromInt(20));
  };
  BENCHMARK("Number type fibonacci iterative(30)") {
    return fibonacci_iter(Number::FromInt(30));
  };
#endif
}