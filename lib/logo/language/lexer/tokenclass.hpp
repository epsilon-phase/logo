#pragma once
#include "tokens.hpp"
namespace logo {
  namespace language {
    namespace tokens {
      namespace __detail {
        // Taken from
        // http://blog.bitwigglers.org/using-enum-classes-as-type-safe-bitmasks/
        template <typename Enum> struct EnableBitmaskedOperators {
          static const bool enable = false;
        };
#define ENABLE_BITMASKED_OPS(x)                                                \
  template <>                                                                  \
  struct logo::language::tokens::__detail::EnableBitmaskedOperators<x> {       \
    static const bool enable = true;                                           \
  }
      } // namespace __detail
      /**
       * Bitmasked enumerator for the semantics that tokens have.
       * */
      enum class TokenClass : unsigned {
        none = 0,
        //! No class, probably not a good thing if this is returned
        no_class = 0,
        //! Signifies the start of a control block
        control_flow_start = 1,
        //! Signifies the end of a control block
        control_flow_end = 1 << 1,
        //! signifies that this will produce a value
        operates = 1 << 3,
        //! Signifies that the token may be used to assign
        assigns = 1 << 4,
        //! Signifies that the value is a constant or variable so far as the
        //! lexer cares
        raw_value = 1 << 5,

      };
      ENABLE_BITMASKED_OPS(TokenClass);
      template <typename Enum>
      typename std::enable_if<
          std::is_enum<Enum>::value &&
              __detail::EnableBitmaskedOperators<Enum>::enable,
          Enum>::type
      operator|(Enum a, Enum b) {
        static_assert(std::is_enum<Enum>::value, "Type is not an enum");
        using underlying = typename std::underlying_type<Enum>::type;
        return static_cast<Enum>(static_cast<underlying>(a) |
                                 static_cast<underlying>(b));
      }
      template <typename Enum>
      typename std::enable_if<
          std::is_enum<Enum>::value &&
              __detail::EnableBitmaskedOperators<Enum>::enable,
          Enum>::type
      operator&(Enum a, Enum b) {
        using underlying = typename std::underlying_type<Enum>::type;
        return static_cast<Enum>(static_cast<underlying>(a) &
                                 static_cast<underlying>(b));
      }
      template <typename Enum>
      typename std::enable_if<
          std::is_enum<Enum>::value &&
              __detail::EnableBitmaskedOperators<Enum>::enable,
          Enum>::type
      operator^(Enum a, Enum b) {
        using underlying = typename std::underlying_type<Enum>::type;
        return static_cast<Enum>(static_cast<underlying>(a) ^
                                 static_cast<underlying>(b));
      }
      template <typename Enum>
      typename std::enable_if<
          std::is_enum<Enum>::value &&
              __detail::EnableBitmaskedOperators<Enum>::enable,
          Enum>::type &
      operator|=(Enum &a, Enum b) {
        using underlying = typename std::underlying_type<Enum>::type;
        a = a | b;
        return a;
      }
      template <typename Enum>
      typename std::enable_if<
          std::is_enum<Enum>::value &&
              __detail::EnableBitmaskedOperators<Enum>::enable,
          Enum>::type &
      operator&=(Enum &a, Enum b) {
        using underlying = typename std::underlying_type<Enum>::type;
        a = a & b;
        return a;
      }
      template <typename Enum>
      typename std::enable_if<
          std::is_enum<Enum>::value &&
              __detail::EnableBitmaskedOperators<Enum>::enable,
          Enum>::type &
      operator^=(Enum &a, Enum b) {
        using underlying = typename std::underlying_type<Enum>::type;
        a = a ^ b;
        return a;
      }
      template <typename Enum>
      typename std::enable_if<
          std::is_enum<Enum>::value &&
              __detail::EnableBitmaskedOperators<Enum>::enable,
          bool>::type
      any(Enum e) {
        return e != Enum::none;
      }
      TokenClass classify_token(TokenType t);
    } // namespace tokens
  }   // namespace language
} // namespace logo