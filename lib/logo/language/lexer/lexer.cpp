#include "lexer.hpp"
using namespace logo;
using namespace language;
TranslationUnit logo::language::LexString(const std::string &s) {
  TranslationUnit result;
  result.contents = s;
  lex(result);
  return result;
}