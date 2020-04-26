#include "lexer.hpp"
#include <cctype>
using namespace logo;
using namespace language;
TranslationUnit logo::language::LexString(const std::string &s) {
  TranslationUnit result;
  result.contents = s;
  lex2(result);
  return result;
}
std::shared_ptr<TranslationUnit> language::shared_lex(const std::string &s) {
  auto r = std::make_shared<TranslationUnit>();
  r->contents = s;
  lex2(*r);
  return r;
}
void TranslationUnit::add_token(tokens::TokenType t, const char *start,
                                size_t length) {
  tokens.emplace_back(tokens::Token{t, std::string_view(start, length)});
}
TokenStreamIterator TranslationUnit::begin() {
  return TokenStreamIterator{true, shared_from_this(), 0};
}
TokenStreamIterator TranslationUnit::end() {
  return TokenStreamIterator{true, shared_from_this(), tokens.size()};
}
static inline bool case_insensitive_equals(const std::string &s,
                                           const std::string_view &sv) {
  auto i = s.cbegin();
  auto j = sv.cbegin();
  // Apparently this is considered necessary for
  // the prevention of timing attacks
  //
  // It is broadly unnecessary otherwise, but we felt like
  // seeing what it would feel like to write it
#ifdef CONSTANT_TIME_COMPARE
  bool can_be_equal = true;
#endif
  while (*i != 0 && *j != 0 && i != s.cend() && j != sv.cend()) {
#ifdef CONSTANT_TIME_COMPARE
    can_be_equal &= std::tolower(*i) != std::tolower(*j);
#else
    if (std::tolower(*i) != std::tolower(*j))
      return false;
#endif
    i++;
    j++;
  }
#ifdef CONSTANT_TIME_COMPARE
  return can_be_equal;
#else
  return i == s.cend() && j == sv.cend();
#endif
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
    return IsEqual;
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
  if (sv == "%")
    return Modulo;
  return Unknown;
}