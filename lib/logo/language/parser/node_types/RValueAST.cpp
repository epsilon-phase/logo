#include "../detail/ast_prelude.hpp"
ParseResult<RValueAST> RValueAST::parse(TokenStreamIterator start) {
  auto result = std::make_unique<RValueAST>();
  while (start->type != Semicolon && start.remaining()) {
    auto expr = ExpressionAST::parse(start);
    if (!expr.has_value())
      FAIL;
    auto &[e, s] = expr.value();
    start = s;
    result->add_child(std::move(e));
    if (start->type == Comma)
      start++;
  }
  return Succeed(result, start);
}