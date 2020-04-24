#pragma once
#include <string>
#include <string_view>
#include <variant>
namespace logo {
  namespace language {
    namespace tokens {
      enum TokenType {
        Function,
        EndFunc,
        If,
        Then,
        Else,
        EndIf,
        Variable,
        Identifier,
        While,
        Do,
        EndDo,
        For,
        ParenLeft,
        ParenRight,
        Comma,
        Comment,
        String,
        Number,
        Return,
        Plus,
        Increment,
        Minus,
        Decrement,
        Times,
        Divide,
        Exponent,
        Semicolon,
        Equal,
        Lesser,
        Greater,
        GreaterEqual,
        LesserEqual,
        NotEqual,
        LogicalAnd,
        LogicalOr,
        Not,
        Unknown
      };
      /**
       * A container for the relevant information for a token such as the text contained within and the type of token.
       * 
       * It does not own the string, so ensure that it lives only so long as what it was lexed from
       * */
      struct Token {
        TokenType type;
        TokenType GetType() const { return type; }
        std::string_view content;
      };
      /**
       * Convert the token type into a more easily readable string
       * @param TokenType The kind of token
       * @returns The name of the token type
       **/
      std::string TokenToString(TokenType);
    } // namespace tokens
  }   // namespace language
} // namespace logo
