#include "../detail/ast_prelude.hpp"
ParseResult<VariableNameAST> VariableNameAST::parse(TokenStreamIterator start) {
  if (start->type != Identifier)
    FAIL;
  auto result = std::make_unique<VariableNameAST>();
  result->token = &(*start);
  start++;
  return Succeed(result, start);
}