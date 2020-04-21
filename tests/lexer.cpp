#include "catch2.hpp"
#include <charconv>
#include <iostream>
#include <logo/language/lexer/lexer.hpp>
#include <string>
TEST_CASE("Numbers are properly parsed", "[lexer]") {
  using namespace logo::language;
  const std::string testcase = "3.5 -5.1 1 ";
  auto lexed = LexString(testcase);
  WHEN("Numbers are parsed") {
    THEN("They should have the correct number of tokens") {
      REQUIRE(lexed.tokens.size() == 3);
    }
    THEN("They should have the number token type") {
      for (const auto &i : lexed.tokens) {
        REQUIRE(i.type == tokens::Number);
      }
    }
    THEN("They should have the correct parsing") {
      double a;
      a = std::strtod(lexed.tokens[0].content.data(), nullptr);
      REQUIRE(a == 3.5);
      a = std::strtod(lexed.tokens[1].content.data(), nullptr);
      REQUIRE(a == -5.1);
      a = std::strtod(lexed.tokens[2].content.data(), nullptr);
      REQUIRE(a == 1);
    }
  }
}