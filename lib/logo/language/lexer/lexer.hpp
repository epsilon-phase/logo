#pragma once
#include "./tokens.hpp"
#include <vector>
namespace logo {
  namespace language {
    struct TranslationUnit {
      std::string contents;
      std::vector<tokens::Token> tokens;
    };
    TranslationUnit LexString(const std::string &s);
    void lex(TranslationUnit &);
  } // namespace language
} // namespace logo