#include "./tokenclass.hpp"
using namespace logo::language::tokens;

TokenClass logo::language::tokens::classify_token(TokenType t) {
  using tc = TokenClass;
  TokenClass result = tc::no_class;
  switch (t) {
  case Equal:
    result |= tc::assigns;
    break;
  case Plus:
  case Minus:
  case Times:
  case Divide:
  case Exponent:
  case Modulo:
    result |= tc::assigns;
  case LogicalAnd:
  case LogicalOr:
  case Greater:
  case GreaterEqual:
  case IsEqual:
  case Lesser:
  case LesserEqual:
  case NotEqual:
    result |= tc::operates;
    break;
  case EndIf:
  case EndDo:
  case EndFunc:
    result |= tc::control_flow_end;
    break;
  case For:
  case If:
  case Function:
  case While:
  case Then:
    result |= tc::control_flow_start;
    break;
  default:
    break;
  }
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