#include "../detail/ast_prelude.hpp"
ParseResult<ExponentExpr> ExponentExpr::parse(TokenStreamIterator start) {
  auto result = std::make_unique<ExponentExpr>();
  auto a1 = AtomAST::parse(start);
  if (!a1.has_value())
    FAIL;
  auto &[a, s] = a1.value();
  result->add_child(std::move(a));
  start = s;
  if (start->type == Exponent) {
    result->token = &(*start);
    auto a2 = ExponentExpr::parse(start + 1);
    if (!a2.has_value())
      FAIL;
    auto &[a, s] = a2.value();
    result->add_child(std::move(a));
    start = s;
  }
  return Succeed(result, start);
}