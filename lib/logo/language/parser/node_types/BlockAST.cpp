#include "../ast.hpp"
#include "../detail/ast_prelude.hpp"
#include <iostream>
ParseResult<BlockAST> BlockAST::parse(TokenStreamIterator start) {
  std::unique_ptr<BlockAST> result = std::make_unique<BlockAST>();
  while (true) {
    if (start->type == Else || start->type == EndIf)
      break;
    std::unique_ptr<ASTNodeBase> current;
    if (start->type == If) {
      auto IfS = IfElseAST::parse(start);
      if (!IfS.has_value())
        FAIL;
      auto [If, s] = std::move(IfS.value());
      result->add_child(std::move(If));
      start = s;
    } else if (start->type == For) {
      auto For = ForLoopAST::parse(start);
      if (!For.has_value())
        FAIL;
      auto [F, s] = std::move(For.value());
      result->add_child(std::move(F));
      start = s;
    } else {

      auto r = StatementAST::parse(start);
      if (r.has_value()) {
        auto &[a, s] = r.value();
        current = std::move(a);
        start = s;
        result->add_child(std::move(current));
        continue;
      }
    }
    break;
  }
  std::cerr << "Found " << TokenToString(start->type) << std::endl;
  return std::make_optional(std::make_tuple(std::move(result), start));
}