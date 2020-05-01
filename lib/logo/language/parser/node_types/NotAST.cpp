#include "../detail/ast_prelude.hpp"
ParseResult<NotAST> NotAST::parse(TokenStreamIterator start) {
  auto result = std::make_unique<NotAST>();
  if (start->type == Not) {
    result->token = &*start;
    start++;
  }
  // sigh
  if (start->type == Not) {
    auto n = NotAST::parse(start);
    if (!n.has_value())
      FAIL;
    auto [nested, s] = std::move(n.value());
    result->add_child(std::move(nested));
    start = s;
  } else {
    auto boolean = AtomAST::parse(start);
    if (!boolean.has_value())
      FAIL;
    auto [b, s] = std::move(boolean.value());
    start = s;
    result->add_child(std::move(b));
  }
  return Succeed(result, start);
}