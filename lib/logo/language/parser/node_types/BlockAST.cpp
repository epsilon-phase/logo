#include "../ast.hpp"
#include "../detail/ast_prelude.hpp"
#include <iostream>
ParseResult<BlockAST> BlockAST::parse(TokenStreamIterator start) {
  std::unique_ptr<BlockAST> result = std::make_unique<BlockAST>();
  while (true) {

    std::unique_ptr<ASTNodeBase> current;
    {
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