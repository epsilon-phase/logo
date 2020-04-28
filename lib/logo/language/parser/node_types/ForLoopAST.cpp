#include "../detail/ast_prelude.hpp"
ParseResult<ForLoopAST> ForLoopAST::parse(TokenStreamIterator start) {
  auto result = std::make_unique<ForLoopAST>();
  REQSUCC(For);
  result->token = &*start;
  auto assignment = AssignmentAST::parse(start + 1);
  REQPARSE(assignment, ass);
  result->add_child(std::move(ass));
  REQSUCC(Semicolon);
  auto condition = ExpressionAST::parse(start + 1);
  REQPARSE(condition, cond);
  result->add_child(std::move(cond));
  REQSUCC(Semicolon);
  auto update = AssignmentAST::parse(start + 1);
  REQPARSE(update, up);
  result->add_child(std::move(up));
  auto block = BlockAST::parse(start + 1);
  REQPARSE(block, b);
  result->add_child(std::move(b));
  REQSUCC(EndDo);
  start++;
  return Succeed(result, start);
}