#include <errors/syntaxexception.hpp>
#include <fstream>
#include <iostream>
#include <language/lexer/lexer.hpp>
#include <language/parser/ast.hpp>
#include <memory>
#include <streambuf>
int main(int argc, char **argv) {
  using namespace logo::language;
  std::ifstream t(argv[1]);
  std::string str((std::istreambuf_iterator<char>(t)),
                  std::istreambuf_iterator<char>());
  try {
    auto lx = std::make_shared<logo::language::TranslationUnit>(LexString(str));
    auto pt = parser::ParseToplevel(lx);
    for (const auto &i : lx->tokens) {
      std::cout << TokenToString(i.type) << std::endl;
    }
    if (pt != nullptr)
      pt->print_tree(std::cout, 0);
    else
      std::cerr << "Could not parse" << std::endl;
  } catch (std::exception &) { // We don't care about the syntax errors, those
                               // are signals it's working properly. :)
  }
}