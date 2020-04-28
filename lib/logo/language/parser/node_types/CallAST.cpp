#include "../detail/ast_prelude.hpp"
#include <iostream>
ParseResult<CallAST> CallAST::parse(TokenStreamIterator start) {
  auto result = std::make_unique<CallAST>();
  if (start->type != Identifier || (start + 1)->type != ParenLeft) {
    std::cerr << "Initial tokens failed" << std::endl;
    FAIL;
  }
  result->token = &(*start);
  start = start + 2;
  while (start->type != ParenRight) {
    auto e = ExpressionAST::parse(start);
    if (!e.has_value()) {
      // std::cerr << "Failed to match expression" << std::endl;
      FAIL;
    }
    auto &[e1, s] = e.value();
    start = s;
    result->add_child(std::move(e1));
    if (start->type != Comma && start->type != ParenRight) {
      // std::cerr << "Failed to match on " << TokenToString(start->type)
      // << std::endl;
      FAIL;
    }
    if (start->type != ParenRight)
      start++;
  }
  start++;
  return Succeed(result, start);
}