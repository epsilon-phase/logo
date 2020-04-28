#include "../detail/ast_prelude.hpp"
#include <iostream>
ParseResult<AtomAST> AtomAST::parse(TokenStreamIterator start) {
  auto result = std::make_unique<AtomAST>();
  if (start->type == ParenLeft) {
    auto expr = ExpressionAST::parse(start);
    if (!expr.has_value())
      FAIL;
    auto &[ex, s] = expr.value();

    result->add_child(std::move(ex));
    start = s;
    if (start->type != ParenRight)
      FAIL;
    start++;
  } else if ((start + 1)->type == BracketLeft) {
    auto array = ArrayAccessAST::parse(start);
    if (!array.has_value())
      FAIL;
    auto &[arr, s] = array.value();
    result->add_child(std::move(arr));
    start = s;
  } else if (start->type == Identifier && (start + 1)->type == ParenLeft) {
    auto c = CallAST::parse(start);
    if (!c.has_value()) {
      // std::cerr << "Call failed to match" << std::endl;
      FAIL;
    }
    auto &[call, s] = c.value();
    result->add_child(std::move(call));
    start = s;
  } else if (start->type == Identifier) {
    auto r = VariableNameAST::parse(start);
    if (!r.has_value())
      FAIL;
    auto &[nv, s] = r.value();
    result->add_child(std::move(nv));
    start = s;
  } else if (start->type == Number || start->type == String) {
    auto cl = ConstantLiteralAST::parse(start);
    auto &[c, s] = cl.value();
    result->add_child(std::move(c));
    start = s;
  } else {
  }
  return Succeed(result, start);
}