#include "catch2.hpp"
#include <iostream>
#include <logo/errors/syntaxexception.hpp>
#include <logo/language/lexer/lexer.hpp>
#include <string>
#ifdef TEST_PRINTING
#define PRINT_ERROR(X) std::cerr << X
#define PRINT_OUT(X) std::cout << X
#else
#define PRINT_ERROR(X)
#define PRINT_OUT(X)
#endif
using namespace logo::language;
using namespace logo::language::tokens;
static void list_tokens(const logo::language::TranslationUnit &);
TEST_CASE("lex2 can find anything", "[lex2]") {

  WHEN("a string is present in the input") {
    const std::string basic = "\"hi\"";
    auto tu = TranslationUnit();
    tu.contents = basic;
    try {
      lex2(tu);
    } catch (const logo::error::SyntaxException &se) {
      PRINT_ERROR(<< se.get_context() << std::endl);
      throw se;
    }
    if (tu.tokens.size() != 1)
      list_tokens(tu);
    THEN("A token is found") { REQUIRE(tu.tokens.size() == 1); }
    THEN("It is identified as a string") {
      REQUIRE(tu.tokens[0].type == String);
    }
  }
  WHEN("a number is present") {
    const std::string num = "12.23";
    auto tu = TranslationUnit();
    tu.contents = num;
    lex2(tu);
    THEN("They are found") { REQUIRE(tu.tokens.size() == 1); }
    THEN("It is identified") { REQUIRE(tu.tokens[0].type == Number); }
    THEN("It looks right") { REQUIRE(tu.tokens[0].content == "12.23"); }
  }
  WHEN("Multiple numbers are present") {
    const std::string num = "12.3 5.4e5";
    auto tu = TranslationUnit();
    tu.contents = num;
    lex2(tu);
    THEN("They are found") { REQUIRE(tu.tokens.size() == 2); }
    THEN("They are identified as numbers") {
      REQUIRE(tu.tokens[0].type == Number);
      REQUIRE(tu.tokens[1].type == Number);
    }
  }
  WHEN("Multiple numbers are present and have signs mixed with operators") {
    const std::string num = "12.3 - -13 - 5";
    auto tu = LexString(num);
    THEN("They are found") { REQUIRE(tu.tokens.size() == 5); }
    THEN("They are correctly identified") {
      REQUIRE(tu.tokens[0].type == Number);
      REQUIRE(tu.tokens[1].type == Minus);
      REQUIRE(tu.tokens[2].type == Number);
      REQUIRE(tu.tokens[3].type == Minus);
      REQUIRE(tu.tokens[4].type == Number);
    }
  }
  WHEN("There is an identifier") {
    const std::string ident = "hello";
    auto tu = TranslationUnit();
    tu.contents = ident;
    lex2(tu);
    THEN("It is found") { REQUIRE(tu.tokens.size() == 1); }
    THEN("It is labeled as such") { REQUIRE(tu.tokens[0].type == Identifier); }
  }
  WHEN("There are several identifiers") {
    const std::string ident = "hello world";
    auto tu = TranslationUnit();
    tu.contents = ident;
    lex2(tu);
    THEN("They are found") { REQUIRE(tu.tokens.size() == 2); }
    THEN("They are labeled as such") {
      REQUIRE(tu.tokens[0].type == Identifier);
      REQUIRE(tu.tokens[1].type == Identifier);
    }
    THEN("They appear right") {
      REQUIRE(tu.tokens[0].content == "hello");
      REQUIRE(tu.tokens[1].content == "world");
    }
  }
  WHEN("Operators are present") {
    const std::string operators = "hello+3";
    auto tu = TranslationUnit();
    tu.contents = operators;
    lex2(tu);
    THEN("They are found") { REQUIRE(tu.tokens.size() == 3); }
    THEN("They are correctly identified") {
      REQUIRE(tu.tokens[0].type == Identifier);
      REQUIRE(tu.tokens[1].type == Plus);
      REQUIRE(tu.tokens[2].type == Number);
    }
  }
  WHEN("Operators are present and weird") {
    const std::string operators = "hello+-3";
    auto tu = TranslationUnit();
    tu.contents = operators;
    lex2(tu);
    THEN("They are found") { REQUIRE(tu.tokens.size() == 3); }
    THEN("They are correctly identified") {
      REQUIRE(tu.tokens[0].type == Identifier);
      REQUIRE(tu.tokens[1].type == Plus);
      REQUIRE(tu.tokens[2].type == Number);
    }
  }
  WHEN("Keywords are present") {
    const std::string keywords = "while endwhile";
    auto tu = TranslationUnit();
    tu.contents = keywords;
    lex2(tu);
    THEN("They are found") { REQUIRE(tu.tokens.size() == 2); }
    THEN("They are correctly identified") {
      REQUIRE(tu.tokens[0].type == While);
      REQUIRE(tu.tokens[1].type == EndDo);
    }
  }
}
TEST_CASE("Comments", "[lex2]") {
  WHEN("Comments are present") {
    try {
      const std::string comment = "//This is fine\n"
                                  "//Right?";
      auto tu = LexString(comment);
      THEN("They are found") { REQUIRE(tu.tokens.size() == 2); }
      THEN("They are identified as comments") {
        REQUIRE(tu.tokens[0].type == Comment);
        REQUIRE(tu.tokens[1].type == Comment);
      }
      THEN("They appear correct") {
        REQUIRE(tu.tokens[0].content == "This is fine");
        REQUIRE(tu.tokens[1].content == "Right?");
      }
    } catch (logo::error::SyntaxException &s) {
      PRINT_OUT(<< s.get_context() << std::endl);
      throw s;
    }
  }
  WHEN("The comments are multiline") {
    try {
      const std::string comment = "/*This is cool huh\n"
                                  "Well, we think it is at least :) */";
      auto tu = LexString(comment);
      THEN("It is found") { REQUIRE(tu.tokens.size() == 1); }
      THEN("They are identified as comments") {
        REQUIRE(tu.tokens[0].type == Comment);
      }
    } catch (logo::error::SyntaxException &s) {
      PRINT_OUT(<< "On line " << s.line << ": " << s.get_context()
                << std::endl);
      throw s;
    }
  }
}
static void list_tokens(const logo::language::TranslationUnit &tu) {
  using namespace logo::language::tokens;
  for (const auto &i : tu.tokens)
    PRINT_OUT(<< "'" << i.content << "' " << TokenToString(i.type)
              << std::endl);
}