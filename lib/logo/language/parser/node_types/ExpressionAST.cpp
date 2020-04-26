#include "../detail/ast_prelude.hpp"

ParseResult<ExpressionAST> ExpressionAST::parse(TokenStreamIterator start) {
  auto result = std::make_unique<ExpressionAST>();

  return Succeed(result, start);
}