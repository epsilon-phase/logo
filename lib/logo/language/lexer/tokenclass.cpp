#include "./tokenclass.hpp"
using namespace logo::language::tokens;

TokenClass classify_token(TokenType t) {
  using tc = TokenClass;
  TokenClass result = tc::no_class;
  if (t == Equal)
    result |= tc::assigns;
  if (t == Plus || t == Minus || t == Times || t == Divide || t == Exponent)
    result |= tc::operates | tc::assigns;
  if (t == LogicalOr || t == LogicalAnd || t == Greater || t == GreaterEqual ||
      t == IsEqual || t == Lesser || t == LesserEqual || t == NotEqual)
    result |= tc::operates;
  if (t == EndDo || t == EndIf || t == EndFunc)
    result |= tc::control_flow_end;
  if (t == If || t == While || t == Do || t == For || t == TokenType::Function)
    result |= tc::control_flow_start;
  return result;
}