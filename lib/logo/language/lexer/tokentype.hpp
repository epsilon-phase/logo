#pragma once
#include "tokens.hpp"
namespace logo {
  namespace language {
    namespace tokens {
      enum TokenClass {
        control_flow_start = 1,
        control_flow_end = 1 << 1,
        operates = 1 << 3,
        assigns = 1 << 4,
        raw_value = 1 << 5
      };
      TokenClass classify_token(TokenType t);
    } // namespace tokens
  }   // namespace language
} // namespace logo