#include "./catch2.hpp"
#include <cstring>
#include <iostream>
#include <logo/language/parser/ast.hpp>
#include <memory>
using namespace logo::language;
using namespace logo::language::tokens;
using namespace logo::language::parser;

TEST_CASE("Simple parses", "[parser]") {
  WHEN("Empty function is parsed") {
    const std::string func = "function hello() endfunc";
    auto lx = std::make_shared<TranslationUnit>(LexString(func));
    auto c = ParseToplevel(lx);
    if (c == nullptr) {
      std::cout << "Oh no! It's null!" << std::endl;
    }
    THEN("It provides a parse tree") { REQUIRE(c != nullptr); }
    THEN("It is indeed what it should be") {
      REQUIRE(c->what() == std::string("Function"));
    }
    THEN("IT has precisely one child") { REQUIRE(c->children.size() == 1); }
    THEN("That child is a parameter type") {
      REQUIRE(c->children[0]->what() == std::string("Parameter"));
    }
  }
  WHEN("A function with a parameter is parsed") {
    const std::string func = "function hello(who) endfunc";
    auto lx = std::make_shared<TranslationUnit>(LexString(func));
    auto c = ParseToplevel(lx);
    THEN("It is still parsed :)") { REQUIRE(c != nullptr); }
    THEN("The parameter shows up") {
      REQUIRE(c->children[0]->children.size() == 1);
    }
    c->print_tree(std::cout, 0);
  }
  WHEN("A function with many parameters is parsed") {
    const std::string func =
        "function hello(a1,a2,a3,a4,a5,a6,a7,a8,a9) endfunc";
    auto lx = std::make_shared<TranslationUnit>(LexString(func));
    auto c = ParseToplevel(lx);
    THEN("It is parsed") { REQUIRE(c != nullptr); }
    THEN("It has 9 parameter children") {
      REQUIRE(c->children[0]->children.size() == 9);
    }
    c->print_tree(std::cout, 0);
  }
}