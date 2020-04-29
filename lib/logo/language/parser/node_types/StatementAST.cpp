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

  } else if (start->type == Return) {
    auto r = ReturnAST::parse(start);
    if (!r.has_value())
      FAIL;
    auto [ret, s] = std::move(r.value());
    start = s;
    result->add_child(std::move(ret));
  } else if (start->type == Identifier) {
    if ((start + 1)->type == ParenLeft) {
      auto call = CallAST::parse(start);
      if (!call.has_value()) {
        FAIL;
      } else {
        auto &[c, s] = call.value();
        result->add_child(std::move(c));
        start = s;
      }
    } else if ((start + 1)->type == Equal || (start + 2)->type == Equal) {
      auto assign = AssignmentAST::parse(start);
      if (!assign.has_value())
        FAIL;
      auto [ass, n] = std::move(assign.value());
      result->add_child(std::move(ass));
      start = n;
    }
  } else {
    FAIL;
  }
  if (start->type != Semicolon) {
    std::cerr << "Expected semicolon, got " << TokenToString(start->type)
              << std::endl;
    FAIL;
  }
  start++;
  return std::make_optional(std::make_tuple(std::move(result), start));
}