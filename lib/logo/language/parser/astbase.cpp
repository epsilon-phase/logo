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
void ASTNodeBase::add_child(std::unique_ptr<ASTNodeBase> &&nb) {
  nb->parent = this;
  children.emplace_back(std::move(nb));
}
void ASTNodeBase::collapse() {
  for (size_t index = 0; index < children.size(); index++) {
    auto *child = children[index].get();
    if (!child->can_collapse()) {
      // Descend the chain then
      child->collapse();
    } else {
      while (child->can_collapse()) {
        children[index] = std::move(child->children[0]);
        child = children[index].get();
      }
      child->collapse();
    }
  }
}
size_t ASTNodeBase::tree_size() const {
  if (children.empty())
    return 1;
  size_t sum = 1;
  // Hopefully the trees shall not be so deep that this recursion
  // should be an issue.
  // Of course, we're sure that won't hold up against a sufficiently determined
  // user
  for (const auto &i : children) {
    sum += i->tree_size();
  }
  return sum;
}
bool ASTNodeBase::can_collapse() const {
  return collapsible() && children.size() == 1;
}
int ASTNodeBase::produce_dot(std::ostream &f, int thisid, int parentid) const {
  if (thisid == -1) {
    f << "Digraph G{\n";
    thisid++;
  }
  int successor = thisid + 1;
  if (parentid != -1)
    f << parentid << "->" << thisid << ";" << std::endl;
  f << thisid << "[label=\"" << what() << "\"];\n";
  for (const auto &child : children) {
    successor = child->produce_dot(f, successor, thisid);
  }
  if (thisid == 0)
    f << "}";
  return successor + 1;
}
size_t ASTNodeBase::count_leaves() const {
  if (children.empty())
    return 1;
  size_t r = 0;
  for (const auto &c : children)
    r += c->count_leaves();
  return r;
}