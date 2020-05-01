#include "../detail/ast_prelude.hpp"

ParseResult<ArrayAccessAST> ArrayAccessAST::parse(TokenStreamIterator start) {
  auto result = std::make_unique<ArrayAccessAST>();
  if (start->type != Identifier) {
    FAIL;
  }
  // Limited utility here, since it can't handle indirection, afaik
  // But it's better to have a shitty system than a completely lacking one.
  result->token = &(*start);
  start++;
  if (start->type != BracketLeft)
    FAIL;
  while (start.remaining() && start->type == BracketLeft) {
    auto pstart = start;
    pstart++;
    auto expr = ExpressionAST::parse(pstart);
    if (!expr.has_value())
      FAIL;
    auto &[ex_ast, ns] = expr.value();
    result->add_child(std::move(ex_ast));
    if (ns->type != BracketRight)
      FAIL;
    start = ++ns;
  }
  return Succeed(result, start);
}