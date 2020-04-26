#include "../ast.hpp"
#include "../detail/ast_prelude.hpp"
#include <iostream>
ParseResult<VariableDecAST> VariableDecAST::parse(TokenStreamIterator start) {
  auto result = std::make_unique<VariableDecAST>();

  if (start->type != Variable) {
    std::cerr << "Expected Variable, got " << TokenToString(start->type)
              << std::endl;
    FAIL;
  }
  start++;
  while (start->type != Semicolon) {
    if (start->type != Identifier)
      FAIL;
    auto c = std::make_unique<VariableNameAST>();
    c->token = &(*start);
    std::cerr << "Got variable " << start->content << std::endl;
    result->add_child(std::move(c));
    start++;
    if (start->type == Comma)
      start++;
  }
  return std::make_optional(std::tuple(std::move(result), start));
}