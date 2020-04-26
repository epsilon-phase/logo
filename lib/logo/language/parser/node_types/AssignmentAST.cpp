#include "../detail/ast_prelude.hpp"
ParseResult<AssignmentAST> AssignmentAST::parse(TokenStreamIterator start) {
  auto result = std::make_unique<AssignmentAST>();
  auto lhs = LValueAST::parse(start);
  if (!lhs.has_value())
    FAIL;
  auto &[lside, npos] = lhs.value();
  result->add_child(std::move(lside));
  start = npos;
  if (start->type != Equal)
    FAIL;
  start++;
  auto rhs = RValueAST::parse(start);
  if (!rhs.has_value())
    FAIL;
  auto &[rside, fpos] = rhs.value();
  result->add_child(std::move(rside));
  start = fpos;
  return Succeed<AssignmentAST>(result, start);
}