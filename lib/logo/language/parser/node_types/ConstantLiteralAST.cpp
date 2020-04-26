#include "../detail/ast_prelude.hpp"
ParseResult<ConstantLiteralAST>
ConstantLiteralAST::parse(TokenStreamIterator start) {
  if (start->type != String && start->type != Number)
    FAIL;
  auto result = std::make_unique<ConstantLiteralAST>();
  result->token = &(*start);
  start++;
  return Succeed(result, start);
}