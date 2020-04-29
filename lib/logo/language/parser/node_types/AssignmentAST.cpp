#include "../detail/ast_prelude.hpp"
#include <cassert>
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
  if (start->type != Equal)
    start++;
  start++;
  auto rhs = RValueAST::parse(start);
  if (!rhs.has_value())
    FAIL;
  auto &[rside, fpos] = rhs.value();
  if (rside->children.size() != result->children[0]->children.size())
    FAIL;
  if (compoundType->type != Equal) {
    auto c2 = LValueAST::parse(start_copy);
    auto [copy, _] = std::move(c2.value());

    for (int i = 0; i < copy->children.size(); i++) {
      auto &c = copy->children[i];
      auto &r = rside->children[i];
      switch (compoundType->type) {
      case Plus:
      case Minus: {
        std::unique_ptr<AddSub> s = std::make_unique<AddSub>();
        s->token = compoundType;
        s->add_child(std::move(c));
        s->add_child(std::move(r));
        rside->children[i] = std::move(s);
      } break;
      case Times:
      case Divide:
      case Modulo: {
        std::unique_ptr<MultDiv> s = std::make_unique<MultDiv>();
        s->token = compoundType;
        s->add_child(std::move(c));
        s->add_child(std::move(r));
        rside->children[i] = std::move(s);
      } break;
      case Exponent: {
        std::unique_ptr<ExponentExpr> s = std::make_unique<ExponentExpr>();
        s->token = compoundType;
        s->add_child(std::move(c));
        s->add_child(std::move(r));
        rside->children[i] = std::move(s);
      } break;
      default:
        FAIL;
      }
    }
  }
  result->add_child(std::move(rside));
  start = fpos;
  return Succeed<AssignmentAST>(result, start);
}