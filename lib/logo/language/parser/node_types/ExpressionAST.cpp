#include "../detail/ast_prelude.hpp"

ParseResult<ExpressionAST> ExpressionAST::parse(TokenStreamIterator start) {
  auto result = std::make_unique<ExpressionAST>();
  auto add = AddSub::parse(start);
  if (!add.has_value())
    FAIL;
  auto &[a, s] = add.value();
  result->add_child(std::move(a));
  return Succeed(result, start);
}