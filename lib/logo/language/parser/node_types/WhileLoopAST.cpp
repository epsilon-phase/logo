#include "../detail/ast_prelude.hpp"
ParseResult<WhileLoopAST> WhileLoopAST::parse(TokenStreamIterator start) {
  auto result = std::make_unique<WhileLoopAST>();
  if (start->type != While)
    FAIL;
  auto condition = ExpressionAST::parse(start + 1);
  if (!condition.has_value())
    FAIL;
  result->add_child(std::move(std::get<0>(condition.value())));
  start = std::get<1>(condition.value());
  if (start->type != Do)
    FAIL;
  auto B = BlockAST::parse(start + 1);
  if (!B.has_value())
    FAIL;
  result->add_child(std::move(std::get<0>(B.value())));
  start = std::get<1>(B.value());
  if (start->type != EndDo)
    FAIL;
  start++;
  return Succeed(result, start);
}