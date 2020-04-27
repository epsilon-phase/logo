#include "../detail/ast_prelude.hpp"
ParseResult<BooleanAST> BooleanAST::parse(TokenStreamIterator start) {
  auto result = std::make_unique<BooleanAST>();
  auto e1 = ComparisonAST::parse(start);
  if (!e1.has_value()) {
    FAIL;
  }
  auto [e, s] = std::move(e1.value());
  result->add_child(std::move(e));
  start = s;
  if (start->type == LogicalAnd || start->type == LogicalOr) {
    result->token = &(*start);
    auto e2 = BooleanAST::parse(start + 1);
    if (!e2.has_value())
      FAIL;
    auto [ev, s2] = std::move(e2.value());
    result->add_child(std::move(ev));
    start = s2;
  }
  return Succeed(result, start);
}