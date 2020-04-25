#pragma once
#include "./tokens.hpp"
#include <vector>

namespace logo {
  /**
   * The primary interface point of the lexer functionality in this project
   * */
  namespace language {
    namespace __detail {
      /**
       * Identify the given keyword
       * @param sv The string segment that is being investigated
       * @returns The keyword, or, otherwise Unknown
       **/
      tokens::TokenType identify_keyword(const std::string_view &);
      /**
       * Identify the given operator
       * @param sv the string segment that is being investigated
       * @returns the operator type, otherwise Unknown
       * */
      tokens::TokenType identify_operator(const std::string_view &);
    } // namespace __detail
    /**
     * A structure to hold a translation unit and the tokens that refer to the
     * string
     * */
    struct TranslationUnit {
      std::string contents;
      std::vector<tokens::Token> tokens;
      /**
       * Convenience function equivalent to
       * `tu.tokens.emplace_back(Token{type,std::string_view(p,l)});`
       * @param type The type of token to add
       * @param start The point in the string that the token starts
       * @param length the length of the token
       * */
      void add_token(tokens::TokenType, const char *, size_t length);
    };
    /**
     * Lex a string, producing a translation unit :)
     * */
    TranslationUnit LexString(const std::string &s);
    void lex(TranslationUnit &);
    /**
     * The new hand-written lexer function
     * @param tu The translation unit being modified
     * */
    void lex2(TranslationUnit &);
  } // namespace language
} // namespace logo