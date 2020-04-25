#include "./tokens.hpp"
const std::string token_names[logo::language::tokens::Unknown + 1] = {
    "Function",   "EndFunc",   "If",           "Then",        "Else",
    "EndIf",      "Variable",  "Identifier",   "While",       "Do",
    "EndDo",      "For",       "ParenLeft",    "ParenRight",  "Comma",
    "Comment",    "String",    "Number",       "Return",      "Plus",
    "Increment",  "Minus",     "Decrement",    "Times",       "Divide",
    "Modulo",     "Exponent",  "Semicolon",    "Equal",       "IsEqual",
    "Lesser",     "Greater",   "GreaterEqual", "LesserEqual", "NotEqual",
    "LogicalAnd", "LogicalOr", "Not",          "Unknown"};
std::string logo::language::tokens::TokenToString(TokenType t) {
  return token_names[t];
}
