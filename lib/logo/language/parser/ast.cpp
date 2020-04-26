#include "./ast.hpp"
#include <optional>
using namespace logo::language;
using namespace logo::language::parser;
using namespace logo::language::tokens;
#define FAIL return std::nullopt
ParseResult<ParamDecAST> ParamDecAST::parse(TokenStreamIterator start) {
  if (start->type != Identifier) {
    FAIL;
  }
  auto result = std::make_unique<ParamDecAST>();
  result->token = &(*start);
  return std::make_optional(std::make_tuple(std::move(result), ++start));
}
ParseResult<ParameterAST> ParameterAST::parse(TokenStreamIterator start) {
  if (start->type != ParenLeft)
    FAIL;
  start++;
  auto result = std::make_unique<ParameterAST>();
  // TODO Parse the parameters here :)
  while (start->type != ParenRight) {
    if (!start.remaining())
      FAIL;
    auto param = ParamDecAST::parse(start);
    if (param.has_value()) {
      auto &[p, npos] = param.value();
      result->children.emplace_back(std::move(p));
      start = npos;
      if (start->type != Comma && start->type != ParenRight)
        FAIL;
      if (start->type == Comma)
        start++;
    }
  }
  if (start->type != ParenRight)
    FAIL;
  start++;
  return std::make_optional(std::make_tuple(std::move(result), start));
}
ParseResult<FunctionAST> FunctionAST::parse(TokenStreamIterator start) {
  auto result = std::make_unique<FunctionAST>();

  if (start->type != Function || !start.remaining())
    FAIL;
  start++;
  if (start->type != Identifier) {
    FAIL;
  }
  result->token = &(*start++);
  auto p = ParameterAST::parse(start);
  if (p.has_value()) {
    auto &[parameters, npos] = p.value();
    result->children.emplace_back(std::move(parameters));
    start = npos;
  } else
    FAIL;
  if (start->type != EndFunc)
    FAIL;
  start++;
  return std::make_optional(std::make_tuple(std::move(result), start));
}
std::unique_ptr<ASTNodeBase>
parser::ParseToplevel(std::shared_ptr<TranslationUnit> &tu) {

  auto result = FunctionAST::parse(tu->begin());
  if (!result.has_value())
    return nullptr;
  auto &[i, pos] = result.value();
  return std::move(i);
}