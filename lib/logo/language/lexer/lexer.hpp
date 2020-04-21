#pragma once
#include "./tokens.hpp"
#include <vector>
namespace logo {
  namespace language {
    namespace __detail {
      tokens::TokenType identify_keyword(const std::string_view &);
      tokens::TokenType identify_operator(const std::string_view &);
    } // namespace __detail
    struct TranslationUnit {
      std::string contents;
      std::vector<tokens::Token> tokens;
    };
    TranslationUnit LexString(const std::string &s);
    void lex(TranslationUnit &);
  } // namespace language
} // namespace logo