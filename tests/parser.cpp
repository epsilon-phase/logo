#include "./catch2.hpp"
#include <cstring>
#include <fstream>
#include <iostream>
#include <logo/language/parser/ast.hpp>
#include <memory>
using namespace logo::language;
using namespace logo::language::tokens;
using namespace logo::language::parser;
static void list_tokens(const logo::language::TranslationUnit &tu) {
  using namespace logo::language::tokens;
  for (const auto &i : tu.tokens)
    std::cout << "'" << i.content << "' " << TokenToString(i.type) << std::endl;
}
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
    THEN("IT has precisely one child") { REQUIRE(c->children.size() == 2); }
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
  WHEN("A function is parsed with a variable declaration") {
    const std::string func = "function variables()\n"
                             "variable a b c;\n"
                             "endfunc";
    auto lx = std::make_shared<TranslationUnit>(LexString(func));
    auto c = ParseToplevel(lx);
    THEN("It parses") { REQUIRE(c != nullptr); }
    THEN("It contains a block in the second slot") {
      REQUIRE(c->children[1]->what() == std::string("Block"));
      REQUIRE(c->children[1]->children[0]->children[0]->what() ==
              std::string("Variable declaration"));
      REQUIRE(c->children[1]->children[0]->children[0]->children.size() == 3);
    }
    c->print_tree(std::cout, 0);
  }
}
TEST_CASE("Expression tests", "[parser]") {
  WHEN("it consists of a single number") {
    std::string num = " 1.5 ";
    auto lx = std::make_shared<TranslationUnit>(LexString(num));
    auto ex = ExpressionAST::parse(lx->begin());
    THEN("It parses") { REQUIRE(ex.has_value()); }
    std::get<0>(ex.value())->print_tree(std::cerr, 0);
    auto [e, s] = std::move(ex.value());
    THEN("It has the expected sequence") {
      std::string correct[] = {"Expression", "AddSub", "MultDiv",
                               "ExponentExpr", "Atom"};
      for (auto [i, ptr] = std::tuple{0, (ASTNodeBase *)e.get()};
           ptr->children.size() > 0; ptr = ptr->children[0].get(), i++) {
        REQUIRE(correct[i] == ptr->what());
      }
    }
    e->collapse();
    THEN("Collapsed, it comes out to the correct sequence as well") {
      REQUIRE(e->what() == std::string("Expression"));
      REQUIRE(e->children[0]->what() == std::string("Atom"));
    }
  }
  WHEN("It consists of a simple arithmetic expression") {
    const std::string arith = " 1.5 + 4.6 ";
    auto lx = std::make_shared<TranslationUnit>(LexString(arith));
    auto ex = ExpressionAST::parse(lx->begin());
    THEN("It is parsed") { REQUIRE(ex.has_value()); }
    auto [e, _] = std::move(ex.value());

    THEN("It has two leaves") {
      REQUIRE(e->children[0]->children.size() == 2);
      REQUIRE(e->count_leaves() == 2);
    }
  }
  WHEN("A call is made") {
    const std::string call = "call(15)";
    for (int i = 0; i < call.size(); i++)
      std::cout << (int)call[i] << " ";
    std::cout << "\n";
    auto lx = shared_lex(call);

    list_tokens(*lx);
    REQUIRE(lx->tokens[0].content.data()[0] == lx->contents.c_str()[0]);
    auto ex = ExpressionAST::parse(lx->begin());
    THEN("It is parsed") { REQUIRE(ex.has_value()); }
  }
}