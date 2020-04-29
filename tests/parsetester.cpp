#include <filesystem>
#include <iostream>
#include <logo/language/lexer/lexer.hpp>
#include <logo/language/parser/ast.hpp>
using namespace logo::language;
using namespace logo::language::parser;
int main(int argc, char **argv) {
  std::string input = argv[1];
  auto lx = shared_lex(input);
  int z = 0;
  for (auto &i : lx->tokens) {
    std::cout << z++ << ":" << TokenToString(i.type) << " '" << i.content << "'"
              << std::endl;
  }
  auto p = logo::language::parser::ParseToplevel(lx, false);
  if (p == nullptr) {
    std::cerr << "Could not parse" << std::endl;
    return 1;
  } else
    p->print_tree(std::cout, 0);
  return 0;
}