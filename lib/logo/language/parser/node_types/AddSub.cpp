#include "../detail/ast_prelude.hpp"
ParseResult<AddSub> AddSub::parse(TokenStreamIterator start) {
  auto result = std::make_unique<AddSub>();
  auto md = MultDiv::parse(start);
  if (!md.has_value())
    FAIL;
  auto &[m, s] = md.value();
  result->add_child(std::move(m));
  start = s;
  if (start.remaining())
    if (start->type == Plus || start->type == Minus) {
      result->token = &(*start);
      auto m2 = AddSub::parse(start + 1);
      auto &[mm, ss] = m2.value();
      result->add_child(std::move(mm));
      start = ss;
    }
  return Succeed(result, start);
}