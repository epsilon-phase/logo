#include "../detail/ast_prelude.hpp"

ParseResult<ArrayAccessAST> ArrayAccessAST::parse(TokenStreamIterator start) {
  auto result = std::make_unique<ArrayAccessAST>();
  if (start->type != Identifier) {
    FAIL;
  }
  result->token = &(*start);
  if (start->type != BracketLeft)
    FAIL;
  while (start->type == BracketLeft) {
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