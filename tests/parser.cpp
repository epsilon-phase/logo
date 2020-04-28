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
  }
}
TEST_CASE("Assignment tests", "[parser]") {
  WHEN("A simple assignment is made 'a=1+2'") {
    const std::string assignment = "a=1+2";
    auto ass = shared_lex(assignment);
    auto parse = AssignmentAST::parse(ass->begin());
    THEN("It is parsed") { REQUIRE(parse.has_value()); }
    auto [tree, _] = std::move(parse.value());
    THEN("The tree has the correct number of leaves(3)") {
      REQUIRE(tree->count_leaves() == 3);
    }
    tree->collapse();
    tree->print_tree(std::cout, 0);
    THEN("it produces the right tree") {
      tree->explore([](ASTNodeBase *t) {
        if (t->is_leaf()) {
          REQUIRE(t->what() == std::string("Variable Name"));
          return false;
        }
        return true;
      });
    }
    THEN("The leaves are in the correct order") {
      std::vector<std::string> leaves = {"Variable Name", "Constant",
                                         "Constant"};
      auto iter = leaves.cbegin();
      tree->explore([&iter](ASTNodeBase *node) {
        if (!node->children.empty())
          return true;
        REQUIRE(*(iter++) == node->what());
        return true;
      });
    }
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
      std::string correct[] = {"Expression", "BooleanExpr", "ComparisonExpr",
                               "AddSub",     "MultDiv",     "ExponentExpr",
                               "Atom"};
      for (auto [i, ptr] = std::tuple{0, (ASTNodeBase *)e.get()};
           ptr->children.size() > 0; ptr = ptr->children[0].get(), i++) {
        REQUIRE(correct[i] == ptr->what());
      }
    }
    e->collapse();
    THEN("Collapsed, it comes out to the correct sequence as well") {
      REQUIRE(e->what() == std::string("Expression"));
      REQUIRE(e->children[0]->what() == std::string("Constant"));
    }
  }
  WHEN("It consists of a simple arithmetic expression") {
    const std::string arith = " 1.5 + 4.6 ";
    auto lx = std::make_shared<TranslationUnit>(LexString(arith));
    auto ex = ExpressionAST::parse(lx->begin());
    THEN("It is parsed") { REQUIRE(ex.has_value()); }
    auto [e, _] = std::move(ex.value());
    e->collapse();
    THEN("It has two leaves") { REQUIRE(e->count_leaves() == 2); }
    THEN("The two leaves are labelled properly") {
      e->explore([](ASTNodeBase *t) {
        if (t->is_leaf())
          REQUIRE(t->what() == std::string("Constant"));
        return true;
      });
    }
  }
  WHEN("A call is made") {
    const std::string call = "call(15)";
    auto lx = shared_lex(call);
    REQUIRE(lx->tokens[0].content.data()[0] == lx->contents.c_str()[0]);
    auto ex = ExpressionAST::parse(lx->begin());
    THEN("It is parsed") { REQUIRE(ex.has_value()); }
    auto [e, _] = std::move(ex.value());
    auto pre_collapse = e->tree_size();
    e->collapse();
    e->print_tree(std::cout, 0);
  }
}
TEST_CASE("If statement", "[parser]") {
  WHEN("An if statement alone is tested") {
    const std::string s = "if 5 then\n"
                          "a=a+1;\nendif";
    auto lx = shared_lex(s);
    auto p = IfElseAST::parse(lx->begin());
    REQUIRE(p.has_value());
    auto [If, _] = std::move(p.value());
    If->collapse();
    If->print_tree(std::cout, 0);
  }
  WHEN("An if statement is there") {
    const std::string s = "function hello()\n"
                          "if 5 then\n"
                          "a=a+1;\n"
                          "else\n"
                          "a=a-2;\n"
                          "endif\n"
                          "endfunc";
    auto lx = shared_lex(s);

    auto p = ParseToplevel(lx);
    REQUIRE(p != nullptr);
    p->collapse();
    p->print_tree(std::cout, 0);
  }
}