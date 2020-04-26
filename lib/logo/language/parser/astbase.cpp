#include "ast.hpp"
#include <ostream>
using namespace logo::language;
using namespace logo::language::parser;
using namespace logo::language::tokens;
void ASTNodeBase::print_tree(std::ostream &o, int depth) const {
  for (int i = 0; i < depth; i++)
    o << "  ";
  o << what();
  if (token) {
    o << " " << token->content;
  }
  o << std::endl;
  for (const auto &i : children)
    i->print_tree(o, depth + 1);
}