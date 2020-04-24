#include "lexer.hpp"
#include <cctype>
using namespace logo;
using namespace language;
TranslationUnit logo::language::LexString(const std::string &s) {
  TranslationUnit result;
  result.contents = s;
  if (result.contents.back() != ' ' || result.contents.back() != '\t')
    result.contents.push_back(' ');
  lex(result);
  return result;
}
static bool case_insensitive_equals(const std::string &s,
                                    const std::string_view &sv) {
  auto i = s.cbegin();
  auto j = sv.cbegin();
  while (*i != 0 && *j != 0 && i != s.cend() && j != sv.cend()) {
    if (std::tolower(*i) != std::tolower(*j))
      return false;
    i++;
    j++;
  }
  return i == s.cend() && j == sv.cend();
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
  if (cse("then", sv))
    return Then;
  if (cse("else", sv))
    return Else;
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
tokens::TokenType __detail::identify_operator(const std::string_view &sv) {
  using namespace tokens;
  if (sv == "==")
    return Equal;
  if (sv == "=")
    return Equal;
  if (sv == "!=")
    return NotEqual;
  if (sv == ">=")
    return GreaterEqual;
  if (sv == "<=")
    return LesserEqual;
  if (sv == ">")
    return Greater;
  if (sv == "<")
    return Lesser;
  if (sv == "&&")
    return LogicalAnd;
  if (sv == "||")
    return LogicalOr;
  if (sv == "!")
    return Not;
  if (sv == "++")
    return Increment;
  if (sv == "--")
    return Decrement;
  if (sv == "+")
    return Plus;
  if (sv == "-")
    return Minus;
  if (sv == "*")
    return Times;
  if (sv == "/")
    return Divide;
  if (sv == "^")
    return Exponent;
  return Unknown;
}