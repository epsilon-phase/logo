#include "../detail/ast_prelude.hpp"
#include <iostream>
ParseResult<LValueAST> LValueAST::parse(TokenStreamIterator start) {
  auto result = std::make_unique<LValueAST>();
  std::cerr << TokenToString(start->type) << std::endl;
  while (start->type != Equal && start->type != Semicolon) {
    auto child = AtomAST::parse(start);
    if (!child.has_value())
      FAIL;
    auto [c, s] = std::move(child.value());
    result->add_child(std::move(c));
    start = s;
    if (start->type == Comma)
      start++;
    else
      break;
  }
  return Succeed(result, start);
}