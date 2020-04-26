#include "../detail/ast_prelude.hpp"
ParseResult<CallAST> CallAST::parse(TokenStreamIterator start) {
  auto result = std::make_unique<CallAST>();
  if (start->type != Identifier || (start + 1)->type != ParenLeft)
    FAIL;
  start = start + 2;
  while (start->type != ParenRight) {
    auto e = ExpressionAST::parse(start);
    if (!e.has_value())
      FAIL;
    auto &[e1, s] = e.value();
    start = s;
    result->add_child(std::move(e1));
    if (start->type != Comma || start->type != ParenRight)
      FAIL;
    start++;
  }
  return Succeed(result, start);
}