#include <fstream>
#include <iostream>
#include <language/lexer/lexer.hpp>
#include <streambuf>
int main(int argc, char **argv) {
  using namespace logo::language;
  std::ifstream t(argv[1]);
  std::string str((std::istreambuf_iterator<char>(t)),
                  std::istreambuf_iterator<char>());
  auto lx = LexString(str);
  for (const auto &i : lx.tokens) {
    std::cout << TokenToString(i.type) << std::endl;
  }
}