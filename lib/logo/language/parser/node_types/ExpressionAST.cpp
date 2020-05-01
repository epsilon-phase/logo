#include "../../lexer/tokenclass.hpp"
#include "../detail/ast_prelude.hpp"
ParseResult<ExpressionAST> ExpressionAST::parse(TokenStreamIterator start) {
  auto c = classify_token(start->type);
  if (any(c & TokenClass::control_flow_start) ||
      any(c & TokenClass::control_flow_end))
    FAIL;
  auto result = std::make_unique<ExpressionAST>();
  auto add = BooleanAST::parse(start);
  if (!add.has_value())
    FAIL;
  auto &[a, s] = add.value();
  result->add_child(std::move(a));
  start = s;
  return Succeed(result, start);
}