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
template <typename T> std::unique_ptr<T> unwrap(ParseResult<T> &r) {
  auto [b, _] = std::move(r.value());
  return std::move(b);
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
    auto lx = shared_lex(num);
    auto ex = ExpressionAST::parse(lx->begin());
    THEN("It parses") { REQUIRE(ex.has_value()); }
    // std::get<0>(ex.value())->print_tree(std::cerr, 0);
    auto [e, s] = std::move(ex.value());
    e->print_tree(std::cerr, 0);
    THEN("It has the expected sequence") {
      std::string correct[] = {"Expression",     "Not AST", "BooleanExpr",
                               "ComparisonExpr", "AddSub",  "MultDiv",
                               "ExponentExpr",   "Atom"};
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
TEST_CASE("For Loop", "[parser]") {
  WHEN("A simple for loop is present") {
    const std::string forloop = "for i=0;i<15;i=i+1 do\n enddo";
    auto lx = shared_lex(forloop);
    auto parsed = ForLoopAST::parse(lx->begin());
    THEN("It is parsed") { REQUIRE(parsed.has_value()); }
    auto [forl, _] = std::move(parsed.value());
    THEN("It contains the expected descendants") {

      const std::string expected[] = {"Assignment", "Expression", "Assignment",
                                      "Block"};
      for (int index = 0; index < forl->children.size(); index++)
        REQUIRE(expected[index] == forl->children[index]->what());
    }
    THEN("The leaves are correct") {
      const std::string expected[] = {"i", "0", "i", "15", "i", "i", "1", ""};
      int index = 0;
      forl->explore([&expected, &index](ASTNodeBase *t) {
        if (t->is_leaf() && t->token) {
          REQUIRE(expected[index] == t->token->content);
          index++;
        }
        return true;
      });
    }
  }
  WHEN("It is present from the toplevel") {
    const std::string forFunc = "function forloop()\n"
                                "for a=0;a<15;a=a+1 do\n"
                                "print(a);\n"
                                "enddo\n"
                                "endfunc";
    auto lx = shared_lex(forFunc);
    auto parsed = ParseToplevel(lx);
    THEN("It is parsed") { REQUIRE(parsed != nullptr); }
    parsed->print_tree(std::cerr, 0);
  }
}
TEST_CASE("Multiple assignment", "[parser]") {
  WHEN("Two things are assigned") {
    std::string c = "a,b=1,2";
    auto lx = shared_lex(c)->begin();
    auto parsed = AssignmentAST::parse(lx);
    THEN("It is parsed") { REQUIRE(parsed.has_value()); }
    auto p = unwrap(parsed);
    p->collapse();
    p->print_tree(std::cout, 0);
    THEN("The types and contents are correct") {
      std::string correct[4][2] = {{"Variable Name", "a"},
                                   {"Variable Name", "b"},
                                   {"Constant", "1"},
                                   {"Constant", "2"}};
      int index = 0;
      p->explore([&index, &correct](ASTNodeBase *t) {
        if (t->is_leaf()) {
          REQUIRE(t->what() == correct[index][0]);
          REQUIRE(t->token->content == correct[index][1]);
          index++;
        }
        return true;
      });
    }
  }
  WHEN("Two things are compound-assigned") {
    std::string c = "a,b+=1,1";
    auto lx = shared_lex(c);
    auto parsed = AssignmentAST::parse(lx->begin());
    THEN("It is parsed") { REQUIRE(parsed.has_value()); }
    auto p = unwrap(parsed);
    p->print_tree(std::cout, 0);
    p->collapse();
    THEN("It is correct") {
      const char *vn = "Variable Name";
      const std::string correct[][2] = {{vn, "a"}, {vn, "b"},
                                        {vn, "a"}, {"Constant", "1"},
                                        {vn, "b"}, {"Constant", "1"}};
      int index = 0;
      p->explore([&index, &correct](ASTNodeBase *t) {
        if (t->is_leaf()) {
          REQUIRE(correct[index][0] == t->what());
          REQUIRE(correct[index][1] == t->token->content);
          index++;
        }
        return true;
      });
    }
  }
  WHEN("The compound assignments are unbalanced") {
    const std::string c = "a,b=1";
    auto lx = shared_lex(c);
    THEN("It does not parse") {
      auto parsed = AssignmentAST::parse(lx->begin());
      if (parsed.has_value()) {
        std::get<0>(parsed.value())->print_tree(std::cerr, 0);
      }
      REQUIRE(!parsed.has_value());
    }
  }
  WHEN("An assignment lacks an right hand value") {
    const std::string c = "a=;";
    auto lx = shared_lex(c);
    THEN("It does not parse") {
      auto parsed = AssignmentAST::parse(lx->begin());
      REQUIRE_FALSE(parsed.has_value());
    }
  }
}
TEST_CASE("Return expressions", "[parser]") {

  WHEN("It is in a simple expression") {
    const std::string r = "function a() return 1,2,3,4,5; endfunc";
    auto lx = shared_lex(r);
    auto parsed = ParseToplevel(lx);
    THEN("It is parsed") { REQUIRE(parsed != nullptr); }
  }
  WHEN("It is tested independently") {
    const std::string r = "return 1,2,3,4";
    auto lx = shared_lex(r);
    auto ret = ReturnAST::parse(lx->begin());
    THEN("It is parsed") { REQUIRE(ret.has_value()); }
    std::get<0>(ret.value())->print_tree(std::cout, 0);
  }
  WHEN("It is tested independently without an expression") {
    const std::string r = "return";
    auto lx = shared_lex(r);
    auto ret = ReturnAST::parse(lx->begin());
    THEN("It is parsed") { REQUIRE(ret.has_value()); }
    std::get<0>(ret.value())->print_tree(std::cout, 0);
  }

  WHEN("IT has no expression") {
    const std::string r = "function a() return;endfunc";
    auto lx = shared_lex(r);
    auto parsed = ParseToplevel(lx);
    THEN("It is parsed") { REQUIRE(parsed != nullptr); }
  }
  WHEN("It has an invalid expression") {
    const std::string r = "function a() return %;endfunc";
    auto lx = shared_lex(r);
    auto parsed = ParseToplevel(lx);
    THEN("It is not parsed") { REQUIRE(parsed == nullptr); }
  }
}