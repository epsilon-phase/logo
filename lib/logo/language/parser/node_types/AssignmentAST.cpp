#include "../detail/ast_prelude.hpp"
static bool is_assignment(TokenStreamIterator s) {
  return s->type == Equal ||
         ((s + 1)->type == Equal &&
          (s->type == Plus || s->type == Times || s->type == Divide ||
           s->type == Minus || s->type == Exponent || s->type == Modulo));
}

ParseResult<AssignmentAST> AssignmentAST::parse(TokenStreamIterator start) {
  auto result = std::make_unique<AssignmentAST>();
  auto lhs = LValueAST::parse(start);
  auto start_copy = start;
  if (!lhs.has_value())
    FAIL;
  auto &[lside, npos] = lhs.value();

  result->add_child(std::move(lside));
  start = npos;
  Token *compoundType = &(*start);
  if (!is_assignment(start))
    FAIL;

  start++;
  auto rhs = RValueAST::parse(start);
  if (!rhs.has_value())
    FAIL;
  auto &[rside, fpos] = rhs.value();
  if (compoundType->type != Equal) {
    auto c2 = LValueAST::parse(start_copy);
    auto [copy, _] = std::move(c2.value());
    switch (compoundType->type) {
    case Plus:
    case Minus: {
      std::unique_ptr<AddSub> s = std::make_unique<AddSub>();
      s->token = compoundType;
      s->add_child(std::move(copy));
      s->add_child(std::move(rside));
    } break;
    case Times:
    case Divide:
    case Modulo: {
      std::unique_ptr<MultDiv> s = std::make_unique<MultDiv>();
      s->token = compoundType;
      s->add_child(std::move(copy));
      s->add_child(std::move(rside));
    } break;
    case Exponent: {
      std::unique_ptr<ExponentExpr> s = std::make_unique<ExponentExpr>();
      s->token = compoundType;
      s->add_child(std::move(copy));
      s->add_child(std::move(rside));
    } break;
    default:
      FAIL;
    }
  } else
    result->add_child(std::move(rside));
  start = fpos;
  return Succeed<AssignmentAST>(result, start);
}