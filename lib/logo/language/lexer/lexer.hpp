#pragma once
#include "./tokens.hpp"
#include <memory>
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
    struct TokenStreamIterator;
    /**
     * A structure to hold a translation unit and the tokens that refer to the
     * string
     * */
    struct TranslationUnit
        : public std::enable_shared_from_this<TranslationUnit> {
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
      TokenStreamIterator begin();
      TokenStreamIterator end();
    };
    struct TokenStreamIterator {
      //! Indicates that the TSI should skip comments;
      bool skip_comments = true;
      std::shared_ptr<TranslationUnit> parent;
      size_t position;
      tokens::Token *operator->() const { return &parent->tokens[position]; }
      tokens::Token &operator*() const { return parent->tokens[position]; }
      bool operator==(const TokenStreamIterator &tsi) const {
        return tsi.parent == parent && position == tsi.position;
      }
      TokenStreamIterator &operator++() {
        this->position++;
        if (skip_comments) {
          skip_comment();
        }
        return *this;
      }
      TokenStreamIterator operator++(int) {
        auto t = *this;
        this->position++;
        if (skip_comments)
          skip_comment();
        return t;
      }
      TokenStreamIterator operator+(int i) const {
        auto r = *this;
        r.position += i;
        return r;
      }
      TokenStreamIterator operator+=(int i) {
        auto r = *this;
        this->position += i;
        return r;
      }
      bool remaining() const { return parent->tokens.size() > this->position; }

    private:
      void skip_comment() {
        while (remaining() &&
               parent->tokens[position].type == tokens::Comment) {
          position++;
        }
      }
    };
    /**
     * Lex a string, producing a translation unit :)
     * */
    TranslationUnit LexString(const std::string &s);
    std::shared_ptr<TranslationUnit> shared_lex(const std::string &s);
    void lex(TranslationUnit &);
    /**
     * The new hand-written lexer function
     * @param tu The translation unit being modified
     * */
    void lex2(TranslationUnit &);
  } // namespace language
} // namespace logo