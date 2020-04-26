#include "../ast.hpp"
#include "../detail/ast_prelude.hpp"
#include <iostream>
ParseResult<StatementAST> StatementAST::parse(TokenStreamIterator start) {
  auto result = std::make_unique<StatementAST>();
  if (start->type == Variable) {
    auto vd = VariableDecAST::parse(start);
    if (vd.has_value()) {
      auto &[va, s] = vd.value();
      start = s;
      result->add_child(std::move(va));
    }

  } else {
    return std::nullopt;
  }
  if (start->type != Semicolon) {
    std::cerr << "Expected semicolon, got " << TokenToString(start->type)
              << std::endl;
    FAIL;
  }
  start++;
  return std::make_optional(std::make_tuple(std::move(result), start));
}