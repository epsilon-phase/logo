#include "../detail/ast_prelude.hpp"
#include <iostream>
ParseResult<ReturnAST> ReturnAST::parse(TokenStreamIterator start) {
  if (start->type != Return)
    FAIL;
  auto result = std::make_unique<ReturnAST>();
  result->token = &*start;
  start++;
  while (start.remaining() && start->type != Semicolon) {
    auto exp = ExpressionAST::parse(start);
    if (!exp.has_value())
      FAIL;
    auto [e, s] = std::move(exp.value());
    result->add_child(std::move(e));
    start = s;
    if (start.remaining() && start->type == Comma)
      start++;
    if (start.remaining() && start->type == Semicolon)
      break;
  }
  // if (start.remaining() && start->type != Semicolon) {
  //   auto rhs = RValueAST::parse(start);
  //   if (rhs.has_value()) {
  //     auto [r, start] = std::move(rhs.value());
  //     result->add_child(std::move(r));
  //   }
  // }
  return Succeed(result, start);
}