#include "lexer.hpp"
#include <cctype>
using namespace logo;
using namespace language;
TranslationUnit logo::language::LexString(const std::string &s) {
  TranslationUnit result;
  result.contents = s;
  lex(result);
  return result;
}
static bool case_insensitive_equals(const std::string &s,
                                    const std::string_view &sv) {
  auto i = s.cbegin();
  auto j = sv.cbegin();
  while (i != s.cend() && j != sv.cend()) {
    if (std::tolower(*i) != std::tolower(*j))
      return false;
    i++;
    j++;
  }
  return true;
}
tokens::TokenType __detail::identify_keyword(const std::string_view &sv) {
  using namespace tokens;
  const auto cse = case_insensitive_equals;
  // Love to have no better way :)
  if (cse("function", sv))
    return Function;
  if (cse("endfunc", sv))
    return EndFunc;
  if (cse("while", sv))
    return While;
  if (cse("for", sv))
    return For;
  if (cse("endfor", sv))
    return EndDo; // Maybe EndFor later if it becomes necessary?
  if (cse("if", sv))
    return If;
  if (cse("endif", sv))
    return EndIf;
  if (cse("do", sv))
    return Do;
  if (cse("endwhile", sv))
    return EndDo;
  if (cse("enddo", sv))
    return EndDo;
  if (cse("return", sv))
    return Return;
  if (cse("variable", sv))
    return Variable;
  return Unknown;
}