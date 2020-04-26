#include "../detail/ast_prelude.hpp"
ParseResult<LValueAST> LValueAST::parse(TokenStreamIterator start) {
  auto result = std::make_unique<LValueAST>();
  while (start->type != Equal && start->type != Semicolon) {
    auto child = AtomAST::parse(start);
    if (start->type == Comma)
      start++;
  }
  return Succeed(result, start);
}