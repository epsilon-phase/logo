#include "../detail/ast_prelude.hpp"

ParseResult<MultDiv> MultDiv::parse(TokenStreamIterator start) {
  auto result = std::make_unique<MultDiv>();
  auto ex = ExponentExpr::parse(start);
  if (!ex.has_value())
    FAIL;
  auto &[e, s] = ex.value();
  result->add_child(std::move(e));
  start = s;
  if (start.remaining())
    if (start->type == Divide || start->type == Times ||
        start->type == Modulo) {
      result->token = &(*start);
      auto ex2 = MultDiv::parse(start + 1);
      if (!ex2.has_value())
        FAIL;
      auto &[e2, s2] = ex2.value();
      result->add_child(std::move(e2));
      start = s2;
    }
  return Succeed(result, start);
}