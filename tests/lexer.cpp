#include "catch2.hpp"
#include <charconv>
#include <iostream>
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

TEST_CASE("Numbers are properly parsed", "[lexer]") {
  using namespace logo::language;
  const std::string testcase = "3.5 -5.1 1 ";
  auto lexed = shared_lex(testcase);
  WHEN("Numbers are parsed") {
    THEN("They should have the correct number of tokens") {
      REQUIRE(lexed->tokens.size() == 3);
    }
    THEN("They should have the number token type") {
      for (const auto &i : lexed->tokens) {
        REQUIRE(i.type == tokens::Number);
      }
    }
    THEN("They should have the correct parsing") {
      double a;
      a = std::strtod(lexed->tokens[0].content.data(), nullptr);
      REQUIRE(a == 3.5);
      a = std::strtod(lexed->tokens[1].content.data(), nullptr);
      REQUIRE(a == -5.1);
      a = std::strtod(lexed->tokens[2].content.data(), nullptr);
      REQUIRE(a == 1);
    }
  }
}
TEST_CASE("Keywords are parsed", "[lexer]") {
  const std::string func = "function endfunc ";
  WHEN("The Function Keywords are parsed") {
    auto lx = shared_lex(func);
    if (lx->tokens.size() != 2) {
      for (const auto &i : lx->tokens) {
        PRINT_OUT(<< i.content << " = " << TokenToString(i.type) << std::endl);
      }
    }
    THEN("The correct number is found") {
      REQUIRE(lx->tokens.size() == 2);
      THEN("They are correctly identified") {
        REQUIRE(lx->tokens[0].type == Function);
        REQUIRE(lx->tokens[1].type == EndFunc);
      }
    }
  }
  const std::string control_flow =
      "while for do endwhile endfor if then else endif";
  WHEN("The control flow keywords are parsed") {
    auto lx = shared_lex(control_flow);
    THEN("They are all found") {
      if (lx->tokens.size() != 9) {
#ifdef TEST_PRINTING
        for (const auto &i : lx->tokens) {
          std::cout << "'" << i.content << "'" << TokenToString(i.type)
                    << std::endl;
        }
#endif
      }
      REQUIRE(lx->tokens.size() == 9);
    }
    THEN("They are correctly identified") {
      const TokenType correct_ids[] = {While, For,  Do,   EndDo, EndDo,
                                       If,    Then, Else, EndIf};

      for (int i = 0; i < 8; i++) {
        if (lx->tokens[i].type != correct_ids[i]) {
#ifdef TEST_PRINTING
          std::cout << "'" << lx->tokens[i].content << "' "
                    << lx->tokens[i].type << std::endl;
#endif
        }
        REQUIRE(lx->tokens[i].type == correct_ids[i]);
      }
    }
  }
  WHEN("The operators are parsed") {
    const std::string operator_test =
        " ++ -- + - * / ^ || && > < >= <= == != =";
    const tokens::TokenType desired[] = {
        Increment,   Decrement, Plus,       Minus,   Times,  Divide,
        Exponent,    LogicalOr, LogicalAnd, Greater, Lesser, GreaterEqual,
        LesserEqual, IsEqual,   NotEqual,   Equal};
    auto lx = shared_lex(operator_test);
    THEN("They are all found") { REQUIRE(lx->tokens.size() == 16); }
    THEN("They are correctly identified") {
      for (int i = 0; i < 15; i++) {
        if (lx->tokens[i].type != desired[i]) {
#ifdef TEST_PRINTING
          std::cout << "'" << lx->tokens[i].content << "' Misidentified as "
                    << tokens::TokenToString(lx->tokens[i].type) << std::endl
                    << "Try looking in lib/logo/language/lexer/lexer2.rl or "
                       "lib/logo/language/lexer/lexer.cpp"
                    << std::endl;
#endif
        }
        REQUIRE(lx->tokens[i].type == desired[i]);
      }
    }
  }
}
TEST_CASE("Identifier parsing", "[lexer]") {
  WHEN("Identifiers are parsed") {
    const std::string identifier_test = "hello there how are you";
    auto lx = shared_lex(identifier_test);
    THEN("The correct number is found") { REQUIRE(lx->tokens.size() == 5); }
    THEN("They are the expected ones") {
      REQUIRE(lx->tokens[0].content == "hello");
      REQUIRE(lx->tokens[1].content == "there");
      REQUIRE(lx->tokens[2].content == "how");
      REQUIRE(lx->tokens[3].content == "are");
      REQUIRE(lx->tokens[4].content == "you");
    }
  }
  WHEN("Identifiers and operators mix properly") {
    const std::string identifier_operator_test = "hello ++";
    auto lx = shared_lex(identifier_operator_test);
    THEN("They are all found") { REQUIRE(lx->tokens.size() == 2); }
    THEN("They work as expected") {
      REQUIRE(lx->tokens[0].content == "hello");
      REQUIRE(lx->tokens[1].type == tokens::Increment);
    }
  }
  WHEN("Identifiers and keywords mix") {
    const std::string identifier_keyword_test = "function hello";
    auto lx = shared_lex(identifier_keyword_test);
    THEN("They are all found") { REQUIRE(lx->tokens.size() == 2); }
    THEN("They are correctly identified") {
      REQUIRE(lx->tokens[0].type == Function);
      REQUIRE(lx->tokens[1].type == Identifier);
    }
  }
}
TEST_CASE("Parenthesis tests", "[lexer]") {
  WHEN("parenthesis are parsed") {
    const std::string parenthesis_test = "()";
    auto lx = shared_lex(parenthesis_test);
    THEN("They are found") { REQUIRE(lx->tokens.size() == 2); }
    THEN("They are correct") {
      REQUIRE(lx->tokens[0].type == ParenLeft);
      REQUIRE(lx->tokens[1].type == ParenRight);
    }
  }
}
TEST_CASE("String tests", "[lexer]") {
  WHEN("Strings contain words that might be identifiers") {
    const std::string string_test =
        "\"strings exist forever, they never die! What fun times we'll have "
        "while we're here huh?\"";
    auto lx = shared_lex(string_test);
    THEN("There's only one token found") { REQUIRE(lx->tokens.size() == 1); }
    THEN("Quotes are included") {
      REQUIRE(lx->tokens[0].content.front() == '"');
      REQUIRE(lx->tokens[0].content.back() == '"');
    }
  }
  WHEN("Strings are using single quotes") {
    const std::string single_quote_test = "'hi'";
    auto lx = shared_lex(single_quote_test);
    THEN("Only one string is found") { REQUIRE(lx->tokens.size() == 1); }
    THEN("Quotes are still included") {
      REQUIRE(lx->tokens[0].content.front() == '\'');
      REQUIRE(lx->tokens[0].content.back() == '\'');
    }
  }
  WHEN("Strings contain escaped quotes") {
    const std::string escaped_quotes = "\"hi :3 \\\"\"";
    auto lx = shared_lex(escaped_quotes);
    THEN("It doesn't terminate early") { REQUIRE(lx->tokens.size() == 1); }
  }
}