#include "../../errors/syntaxexception.hpp"
#include "./helper.hpp"
#include "./lexer.hpp"
#include <exception>
#include <iostream>
#include <sstream>
using namespace logo::language;
using namespace logo::language::tokens;
using namespace logo::language::helper;
#define TOSS_ERROR(tu, l) toss_error(tu, l, __LINE__)

static void toss_error(const TranslationUnit &, const lexer &, int line);
static bool is_operator_candidate(char c);
static bool is_whitespace(char c);
/**
 * Returns true if the lexer is at the end of the input string
 * @param lexer The lexer
 * @returns if it is at the end
 * */
static bool is_at_end(const lexer &);

void logo::language::lex2(TranslationUnit &s) {
  lexer lex = {s.contents.c_str(),
               s.contents.c_str() + s.contents.size(),
               nullptr,
               s.contents.c_str(),
               Normal,
               1};
  const char *last_pos = s.contents.c_str();
  while (lex.p != lex.pe && *lex.p != '\0') {
    if (!is_at_end(lex))
      lex = consume_comment(s, lex);
    lex = consume_whitespace(s, lex);
    if (!is_at_end(lex))
      lex = consume_string(s, lex);
    if (!is_at_end(lex))
      lex = consume_whitespace(s, lex);
    if (!is_at_end(lex))
      lex = consume_operator(s, lex);
    if (!is_at_end(lex))
      lex = consume_whitespace(s, lex);
    if (!is_at_end(lex)) {
      lex = consume_number(s, lex);
      if (s.tokens.size() > 1 && s.tokens.back().type == Number &&
          s.tokens[s.tokens.size() - 2].type == Minus) {
        // TODO make this a little  bit more... introspective For example, it
        // should try to figure out if the third to last token might rely on
        // that subtraction
        auto new_num = s.tokens.back();
        s.tokens.pop_back();
        s.tokens.pop_back();
        new_num.content = std::string_view(new_num.content.data() - 1,
                                           new_num.content.size() + 1);
        s.tokens.push_back(new_num);
#ifdef DEBUG_LEXER
        std::cout << "Replaced sign and number with number" << std::endl;
#endif
      }
    }
    if (!is_at_end(lex))
      lex = consume_whitespace(s, lex);
    if (!is_at_end(lex))
      lex = consume_identifier(s, lex);
    if (!is_at_end(lex))
      lex = consume_misc(s, lex);

    if (last_pos != lex.p) {
      last_pos = lex.p;
    } else {
      TOSS_ERROR(s, lex);
    }
#ifdef DEBUG_LEXER
    std::cout << lex.pe - lex.p << " characters remain(currently at '" << *lex.p
              << "'" << std::endl;
#endif
  }
}
lexer helper::consume_whitespace(TranslationUnit &, const lexer &p) {
  lexer lx = p;
  while (is_whitespace(*lx.p) && lx.p != lx.pe) {
    if (*lx.p == '\n') {
      lx.line++;
      lx.line_start = lx.p + 1;
    }
    lx.p++;
  }
  return lx;
}
lexer_state determine_quote(const lexer &lx) {
  if (*lx.p == '"')
    return DString;
  if (*lx.p == '\'')
    return SString;
  return Inappropriate;
}
static bool can_follow_string(const lexer &l) {
  return is_whitespace(*l.p) || is_operator_candidate(*l.p) || is_at_end(l);
}
lexer helper::consume_string(TranslationUnit &tu, const lexer &l) {
  lexer lx = l;
  auto is_str = determine_quote(lx);
  if (is_str == Inappropriate)
    return lx;
  lx.current = is_str;
  char to_avoid = lx.current == SString ? '\'' : '"';
  lx.start = lx.p;
  lx.p++;
  while (*lx.p != to_avoid && lx.p != lx.pe) {
    // escape the quotes
    if (*lx.p == '\\' && *(lx.p + 1) == to_avoid) {
      lx.p += 2;
      continue;
    }
    lx.p++;
  }
  lx.p++;

  if (!can_follow_string(lx)) {
    if (lx.pe - lx.p < 0) {
      std::cout << "Jumped the end of the string" << std::endl;
    }
    TOSS_ERROR(tu, lx);
    return l;
  }
  std::string_view sv(lx.start, lx.p - lx.start);
  tu.tokens.emplace_back(Token{String, sv});

  lx.current = Normal;
  return lx;
}
static bool can_follow_number(const lexer &lx) {
  return is_operator_candidate(*lx.p) || is_whitespace(*lx.p) || is_at_end(lx);
}
lexer helper::consume_number(TranslationUnit &tu, const lexer &lex) {
  auto is_digit = [](char c) { return c >= '0' && c <= '9'; };
  auto is_sign = [](char c) { return c == '-' || c == '+'; };
  bool found_dot = false;
  bool found_first_digit = false;
  bool found_exponent = false;
  bool found_first_exponent_digit = false;
  lexer l = lex;
  auto is_appropriate = [&](char c) {
    if (!found_first_digit && is_sign(c)) {
      return true;
    }
    if (!found_dot && c == '.') {
      found_dot = true;
      return true;
    }
    if (is_digit(c)) {
      found_first_digit = true;
      found_first_exponent_digit = found_exponent;
      return true;
    }
    if (!found_exponent && found_first_digit && (c == 'e' || c == 'E')) {
      found_exponent = true;
      return true;
    }
    if (found_exponent && !found_first_exponent_digit && is_sign(c)) {
      return true;
    }
    return false;
  };

  // if (is_sign(*l.p) && !is_digit(*(l.p + 1))) {
  //   std::cout << "Not a number" << std::endl;
  //   return l;
  // } else if (!is_sign(*l.p) && !is_digit(*l.p)) {
  //   return l;
  // }
  if (!is_digit(*l.p))
    return l;
  l.start = l.p;
  while (is_appropriate(*l.p)) {
    l.p++;
  }
  if (!can_follow_number(l)) {
    TOSS_ERROR(tu, l);
    return lex;
  }
  tu.tokens.emplace_back(
      Token{Number, std::string_view(l.start, l.p - l.start)});
  return l;
}
static bool is_identifier_candidate(char c, bool first) {
  return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '$' ||
         c == '_' || (!first && c >= '0' && c <= '9');
}
static bool can_follow_identifier(const lexer &l) {
  return is_operator_candidate(*l.p) || is_whitespace(*l.p) || is_at_end(l);
}
lexer helper::consume_identifier(TranslationUnit &tu, const lexer &lx) {
  lexer l = lx;
  if (!is_identifier_candidate(*lx.p, true))
    return lx;
  l.current = Ident;
  l.start = l.p;
  while (is_identifier_candidate(*l.p, false)) {
    l.p++;
  }
  std::string_view sv(l.start, l.p - l.start);
  auto token_type = __detail::identify_keyword(sv);
  if (!can_follow_identifier(l))
    TOSS_ERROR(tu, l);
  if (token_type == Unknown)
    token_type = Identifier;
  tu.tokens.emplace_back(Token{token_type, sv});
  return l;
}
static bool can_follow_operator(const lexer &lx) {
  return is_whitespace(*lx.p) || is_identifier_candidate(*lx.p, true);
}
lexer helper::consume_operator(TranslationUnit &tu, const lexer &lx) {
  auto l = lx;
  if (!is_operator_candidate(*l.p))
    return lx;
  // Necessary because otherwise it will not capture it properly.
  if (*l.p == '/' && *(l.p + 1) == '/') {
    return consume_comment(tu, lx);
  }
  l.start = l.p;
  std::string_view best;
  tokens::TokenType t = Unknown;
  while (is_operator_candidate(*l.p)) {
    std::string_view sv(l.start, l.p - l.start + 1);
    // std::cout << sv << std::endl;
    TokenType temp = __detail::identify_operator(sv);
    if (temp != Unknown) {
      best = sv;
      t = temp;
    } else {
      sv = std::string_view(l.start, l.p - l.start + 2);
      temp = __detail::identify_operator(sv);
      if (temp != Unknown) {
        l.p++;
        best = sv;
        t = temp;
        continue;
      }
      break;
    }
    l.p++;
  }
  // std::cout << "Best operator candidate '" << best << "'" << std::endl;
  if (t == Unknown) {
    std::cout << "erroring at '" << best << "'"
              << " input at '" << *l.p << "'" << std::endl;
    TOSS_ERROR(tu, l);
    return lx;
  }
  tu.tokens.emplace_back(Token{t, best});
  return l;
}
static bool is_operator_candidate(char c) {
  return c == '=' || c == '/' || c == '*' || c == '+' || c == '&' || c == '|' ||
         c == '^' || c == '%' || c == '>' || c == '<' || c == '=' || c == '-' ||
         c == '!';
}
static bool is_whitespace(char c) {
  return c == ' ' || c == '\t' || c == '\n' || c == '\r';
}

static bool is_at_end(const lexer &l) { return l.p == l.pe || *l.p == '\0'; }
static TokenType which_misc(char c) {
  switch (c) {
  case '(':
    return ParenLeft;
  case ')':
    return ParenRight;
  case ',':
    return Comma;
  case ';':
    return Semicolon;
  default:
    return Unknown;
  }
}
lexer helper::consume_misc(TranslationUnit &tu, const lexer &lx) {
  lexer l = lx;
  auto candidate = which_misc(*l.p);
  if (candidate == Unknown)
    return lx;
  std::string_view sv(l.p, 1);
  l.p++;
  tu.tokens.emplace_back(Token{candidate, sv});
  return l;
}
static void toss_error(const TranslationUnit &, const lexer &lex, int line) {
  std::cerr << "Error from state " << lex.current << " at " << lex.pe - lex.p
            << " originates on line " << line << std::endl
            << std::string_view(lex.line_start, lex.p - lex.line_start)
            << std::endl;
  throw logo::error::SyntaxException(lex.line, lex.line_start, lex.p);
}
lexer helper::consume_comment(TranslationUnit &tu, const lexer &lx) {
  lexer l = lx;
  bool increment_again = false;
  bool capture = false;
  if (*l.p == '#' || (increment_again = (*l.p == '/' && *(l.p + 1) == '/'))) {
    l.start = l.p + 1 + increment_again;
    // TODO make it include locations for the comments and stuff.
    while (*l.p != '\n' && l.pe != l.p)
      l.p++;
    capture = true;
  }
  if (*l.p == '/' && *(l.p + 1) == '*') {
    l.p += 2;
    l.start = l.p;
    while (!(*(l.p - 1) == '*' && *l.p == '/')) {
      if (*l.p == '\n') {
        l.line++;
        l.line_start = l.p + 1;
      }
      if (l.p == l.pe) {
        TOSS_ERROR(tu, l);
      }
      l.p++;
    }
    capture = true;
  }
  if (capture) {
    tu.tokens.emplace_back(
        Token{TokenType::Comment, std::string_view(l.start, l.p - l.start)});
    if (l.p != l.pe)
      l.p++;
  }
  return l;
}