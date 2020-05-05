#include "../catch2.hpp"
#include <iostream>
#include <logo/detail/utility.hpp>
#include <logo/vm/gc.hpp>
TEST_CASE("Strings", "[vm]") {
  using namespace logo::vm;
  String c;
  logo::__detail::print_info<String>(std::cout, "String");
  WHEN("A const char* is appended to the string") {
    c.append("Hello");
    THEN("The capacity is right") { REQUIRE(c.capacity == 6); }
    THEN("The length is correct") { REQUIRE(c.length == 5); }
    THEN("The last character is null") { REQUIRE(c.data[c.capacity - 1] == 0); }
    WHEN("It is appended again") {
      c.append(" there");
      THEN("The length is right") { REQUIRE(c.length == 5 + 6); }
      THEN("The capacity is also right") { REQUIRE(c.capacity == 5 + 6 + 1); }
      WHEN("It is appended a third time") {
        const char *q = "ack ack ack";
        c.append(q);
        THEN("The length is right") { REQUIRE(c.length == 5 + 6 + strlen(q)); }
      }
    }
  }
  WHEN("A logo::vm::String is appended to the string") {
    c.append("Hello");
    String q;
    q.append(" there");
    c.append(q);
    REQUIRE(q.data != c.data);
    THEN("The capacity is right") { REQUIRE(c.capacity == 5 + 6 + 1); }
    THEN("The length is correct") { REQUIRE(c.length == 5 + 6); }
    THEN("It's what you expect") {
      REQUIRE(strcmp(c.data, "Hello there") == 0);
    }
  }
  WHEN("The string is narrowed") {
    c.append("hello there");
    WHEN("It is from the start") {
      c.narrow(0, 5);
      THEN("It contains only hello") {
        REQUIRE(std::strcmp(c.data, "hello") == 0);
      }
    }
    WHEN("It is from the 6th character") {
      c.narrow(6, -1);
      THEN("It contains 'there'") {
        REQUIRE(std::strcmp(c.data, "there") == 0);
      }
      THEN("The capacity remains the same") { REQUIRE(c.capacity == 12); }
    }
    WHEN("It is nondestructively narrowed") {
      auto q = c.narrowTo(6);
      THEN("It also produces the same result") {
        REQUIRE(strcmp(q.data, "there") == 0);
      }
    }
  }
}