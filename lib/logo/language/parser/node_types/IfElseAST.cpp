#include "../detail/ast_prelude.hpp"
#include <iostream>
ParseResult<IfElseAST> IfElseAST::parse(TokenStreamIterator start) {
  auto result = std::make_unique<IfElseAST>();
  if (start->type != If)
    FAIL;
  result->token = &*start;
  auto expr = ExpressionAST::parse(start + 1);
  if (!expr.has_value())
    FAIL;
  auto [condition, n] = std::move(expr.value());
  result->add_child(std::move(condition));
  start = n;
  if (start->type != Then)
    FAIL;
  start++;
  auto block = BlockAST::parse(start);
  if (!block.has_value()) {
    std::cerr << "Failed to obtain BLOCK" << std::endl;
    FAIL;
  }
  auto [b, post_block] = std::move(block.value());
  result->add_child(std::move(b));
  start = post_block;
  if (start->type != Else && start->type != EndIf) {
    std::cerr << "Failed to get endif/else, " << TokenToString(start->type)
              << std::endl;
    FAIL;
  }
  if (start->type == Else) {
    auto else_block = BlockAST::parse(start + 1);
    if (!else_block.has_value())
      FAIL;
    auto [eb, post_else] = std::move(else_block.value());
    result->add_child(std::move(eb));
    start = post_else;
  }
  if (start->type != EndIf)
    FAIL;
  start++;
  return Succeed(result, start);
}