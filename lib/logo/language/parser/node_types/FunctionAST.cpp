#include "../detail/ast_prelude.hpp"
ParseResult<FunctionAST> FunctionAST::parse(TokenStreamIterator start) {
  auto result = std::make_unique<FunctionAST>();

  if (!start.remaining() || start->type != Function)
    FAIL;
  start++;
  if (start->type != Identifier) {
    FAIL;
  }
  result->token = &(*start++);
  auto p = ParameterAST::parse(start);
  if (p.has_value()) {
    auto &[parameters, npos] = p.value();
    result->add_child(std::move(parameters));
    start = npos;
  } else
    FAIL;
  auto r = BlockAST::parse(start);
  if (r.has_value()) {
    auto &[a, s] = r.value();
    result->add_child(std::move(a));
    start = s;
  }
  if (start->type != EndFunc)
    FAIL;
  start++;
  return std::make_optional(std::make_tuple(std::move(result), start));
}
void FunctionAST::finish() {
  const std::string vn = "Variable Name";
  explore([&](ASTNodeBase *t) {
    if (t->what() == vn) {
      auto tname = std::string(t->token->content);
      if (local_symbols.find(tname) == local_symbols.end())
        local_symbols[tname] = local_symbols.size();
    }
    return true;
  });
}