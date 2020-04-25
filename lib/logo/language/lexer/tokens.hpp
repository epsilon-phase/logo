#pragma once
#include <string>
#include <string_view>
#include <variant>
namespace logo {
  namespace language {
    /**
     * Containment namespace for the token types(They're quite messy and many)
     **/
    namespace tokens {
      //! The types of token a token might be
      enum TokenType {
        //! indicates the start of a function declaration
        Function,
        //! indicates the end of a function declaration
        EndFunc,
        //! indicates an if statement token
        If,
        //! the border between the boolean expression of the if statement and
        //! the block it encloses
        Then,
        //! The else block token
        Else,
        //! indicates the end of an if block
        EndIf,
        //! indicates a variable declaration
        Variable,
        //! an identifier or user-permitted name
        Identifier,
        //! The start of a while loop
        While,
        //! the start of a do loop
        Do,
        //! The end of a loop
        EndDo,
        //! the start of a for loop
        For,
        //! A left parenthesis
        ParenLeft,
        //! a right parenthesis
        ParenRight,
        //! a comma
        Comma,
        //! A comment
        Comment,
        //! a string literal
        String,
        //! a numeric constant
        Number,
        //! a return statement
        Return,
        //! a plus sign '+'
        Plus,
        //! an increment operator '++'
        Increment,
        //! a subtraction operator
        Minus,
        //! a decrement operator
        Decrement,
        //! a multiplication operator
        Times,
        //! a division operator
        Divide,
        //! the modulo operator '%'
        Modulo,
        //! the exponentiation operator '^'
        Exponent,
        //! The semicolon, the statement terminator
        Semicolon,
        //! the assignment/comparison operator
        Equal,
        //! lesser than
        Lesser,
        //! greater than
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
       * A container for the relevant information for a token such as the text
       * contained within and the type of token.
       *
       * It does not own the string, so ensure that it lives only so long as
       * what it was lexed from
       *
       * For that purpose, consider @ref logo::language::TranslationUnit :)
       * */
      struct Token {
        //! The type of the token
        TokenType type;
        TokenType GetType() const { return type; }
        //! The portion of the input that comprises the token
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
