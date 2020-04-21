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
        Else,
        EndIf,
        Identifier,
        While,
        Do,
        DoWhile,
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
        Minus,
        Times,
        Divide,
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
      struct Token {
        TokenType type;
        TokenType GetType() const { return type; }
        std::string_view content;
      };
    } // namespace tokens
  }   // namespace language
} // namespace logo