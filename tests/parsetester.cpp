#include <filesystem>
#include <iostream>
#include <logo/language/lexer/lexer.hpp>
#include <logo/language/parser/ast.hpp>
using namespace logo::language;
using namespace logo::language::parser;
int main(int argc, char **argv) {
  std::string input = argv[1];
  auto lx = shared_lex(input);
  auto p = logo::language::parser::ParseToplevel(lx);
  if (p == nullptr) {
    std::cerr << "Could not parse" << std::endl;
    return 1;
  } else
    p->print_tree(std::cout, 0);
  return 0;
}