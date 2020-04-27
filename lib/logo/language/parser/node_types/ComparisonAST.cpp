#include "../detail/ast_prelude.hpp"
ParseResult<ComparisonAST> ComparisonAST::parse(TokenStreamIterator start) {
  auto result = std::make_unique<ComparisonAST>();
  auto c1 = AddSub::parse(start);
  if (!c1.has_value())
    FAIL;
  {
    auto [fp, nstart] = std::move(c1.value());
    result->add_child(std::move(fp));
    start = nstart;
  }
  if (start->type == Greater || start->type == IsEqual ||
      start->type == GreaterEqual || start->type == Lesser ||
      start->type == LesserEqual || start->type == NotEqual) {
    result->token = &(*start);
    auto c2 = ComparisonAST::parse(start + 1);
    if (!c2.has_value())
      FAIL;
    auto [fp, nstart] = std::move(c2.value());
    result->add_child(std::move(fp));
    start = nstart;
  }
  return Succeed(result, start);
}