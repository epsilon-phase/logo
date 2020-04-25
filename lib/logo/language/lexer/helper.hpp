#pragma once
#include "./lexer.hpp"
/**
 * \file
 * Contains the declarations for functions used by the handwritten lexer
 * */
namespace logo {
  namespace language {

    /**
     * Helper functions and structures that do not really belong in the
     * external interface
     * */
    namespace helper {
      enum lexer_state {
        Normal,
        Ident,
        Operator,
        NumberS,
        ToEnd,
        Comment,
        DString,
        SString,
        Inappropriate
      };
      /**
       * A structure that holds the state of the lexer
       * */
      struct lexer {
        //! The character currently being consumed
        const char *p;
        //! The end of the input
        const char *pe;
        //! The start of the current token
        const char *start;
        //! The start of the line
        const char *line_start;
        //! The current state(unused mostly)
        lexer_state current;
        //! The line number
        int line;
      };
      /**
       * Consume whitespace until the lexer is no longer positioned on a
       * whitespace character.
       * @param tu The translation unit(unused by this one)
       * @param lx The starting lexer state
       * @returns the new state of the lexer
       */
      lexer consume_whitespace(TranslationUnit &, const lexer &);
      /**
       * Consume a string until its end
       * @param tu the translation unit modified
       * @param lx The lexer state at the start of the function
       * @returns the (potentially) modified lexer state
       */
      lexer consume_string(TranslationUnit &, const lexer &);
      /**
       * Consume a number if it's there, otherwise don't
       * @param tu the translation unit modified
       * @param lx the lexer state at the start of the function
       * @returns the (potentially) modified lexer state
       */
      lexer consume_number(TranslationUnit &, const lexer &);
      /**
       * Consume an identifier, converting it to a keyword if it matches one.
       * @param tu the translation unit modified
       * @param lx the lexer state at the start of the function
       * @returns the (potentially) modified lexer state
       *
       * */
      lexer consume_identifier(TranslationUnit &, const lexer &);
      /**
       * Consume an operator, opting for the longest match
       * @param tu the translation unit modified
       * @param lx the lexer state at the start of the function
       * @returns the (potentially) modified lexer state
       */
      lexer consume_operator(TranslationUnit &, const lexer &);
      /**
       * Consume a miscellaneous single character
       * @param tu the translation unit modified
       * @param lx the lexer state at the start of the function
       * @returns the (potentially) modified lexer state
       * */
      lexer consume_misc(TranslationUnit &, const lexer &);
      /**
       * Consume a comment.
       * @param tu the translation unit modified
       * @param lx the lexer state at the start of the function
       * @returns the (potentially) modified lexer state
       **/
      lexer consume_comment(TranslationUnit &, const lexer &);
    } // namespace helper
  }   // namespace language
} // namespace logo