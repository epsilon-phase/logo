#include "../detail/ast_prelude.hpp"
ParseResult<FileAST> FileAST::parse(TokenStreamIterator start) {
  auto result = std::make_unique<FileAST>();
  while (start.remaining()) {
    auto f = FunctionAST::parse(start);
    if (!f.has_value()) {
      FAIL;
    }
    auto [func, s] = std::move(f.value());
    result->add_child(std::move(func));
    start = s;
  }
  return Succeed(result, start);
}