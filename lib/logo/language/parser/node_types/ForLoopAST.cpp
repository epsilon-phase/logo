#include "../detail/ast_prelude.hpp"
#include <iostream>
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
  cond->print_tree(std::cerr, 0);
  result->add_child(std::move(cond));
  std::cerr << "Left on " << TokenToString(start->type) << std::endl;
  REQSUCC(Semicolon);

  auto update = AssignmentAST::parse(start + 1);
  REQPARSE(update, up);
  result->add_child(std::move(up));
  REQSUCC(Do);
  auto block = BlockAST::parse(start + 1);
  REQPARSE(block, b);
  result->add_child(std::move(b));
  REQSUCC(EndDo);
  start++;
  return Succeed(result, start);
}