#include "../detail/ast_prelude.hpp"
#include <iostream>
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
  const std::string cn = "Constant";
  const std::string pn = "Parameter Declaration";
  explore([&](ASTNodeBase *t) {
    if (t->what() == vn || t->what() == pn) {
      auto tname = std::string(t->token->content);
      if (local_symbols.find(tname) == local_symbols.end())
        local_symbols[tname] = local_symbols.size();
    } else if (t->what() == cn) {
      auto cname = std::string(t->token->content);
      if (constants.find(cname) == constants.end()) {
        constants[cname] = constants.size();
      }
    }
    return true;
  });
}
void FunctionAST::print_tree(std::ostream &o, int depth) const {
  auto indent = std::string(' ', depth);
  o << indent << what() << std::endl;
  if (!local_symbols.empty()) {
    o << indent << " "
      << "Variables:[\n";
    for (const auto &[p, i] : local_symbols) {
      o << indent << " " << i << "='" << p << "',\n";
    }
    o << indent << "]\n";
  }
  if (!constants.empty()) {
    o << indent << "constants:[\n";
    for (const auto &[p, i] : constants) {
      o << indent << " " << i << "='" << p << "',\n";
    }
    o << indent << "]\n";
  }
  for (const auto &child : children)
    child->print_tree(o, depth + 1);
}