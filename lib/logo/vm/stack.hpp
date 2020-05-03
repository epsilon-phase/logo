#pragma once
#include "../detail/Epsilon.hpp"
#include <memory>
#include <optional>
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>
namespace logo {
  namespace language {
    namespace tokens {
      struct Token;
    }
  } // namespace language
  namespace vm {
    using String = std::string;
    /**
     * NaN-boxed number. Maybe it works, maybe it's trash.
     **/
    struct Number {
      Number();
      //! Implicit promotion is an asshole
      static Number fromInt(int64_t);
      static Number FromInt(int64_t i) { return fromInt(i); }
      static Number fi(int64_t i) { return FromInt(i); }
      static Number fromFloat(double);
      static Number ff(double i) { return fromFloat(i); }
      Number(const Number &a) {
        if (a.isInt()) {
          setInt();
          Integral.i = a.Integral.i;
        } else
          fp = a.fp;
      }
      union {
        double fp;

        struct {
#ifdef __BIG_ENDIAN__
          unsigned int sign : 1;
          unsigned int exponent : 11;
          int64_t i : 52;
#else
          int64_t i : 52;
          unsigned int exponent : 11;
          unsigned int sign : 1;
#endif
        } Integral;
      };
      bool isInt() const;
      void setInt();
      Number &operator-=(const Number &);
      Number &operator+=(const Number &);
      Number &operator/=(const Number &);
      Number &operator*=(const Number &);
      /**
       * Returns true if it's within the range of which this can store integers
       * perfectly
       * @param a The integer to test
       * @returns If the integer can fit within 52 bits
       * */
      static bool inIntRange(int64_t);
    };
    struct Array;
    using stackcell = std::variant<std::monostate, String, Number,
                                   std::optional<std::shared_ptr<Array>>>;
    Number operator+(Number, Number);
    Number operator-(Number, Number);
    Number operator/(Number, Number);
    Number operator*(Number, Number);
    inline bool operator<(Number a, Number b) {
      if (a.isInt()) {
        if (b.isInt())
          return a.Integral.i < b.Integral.i;
        return a.Integral.i < b.fp;
      } else {
        if (b.isInt())
          return a.fp < b.Integral.i;
        return a.fp < b.fp;
      }
    }

    template <typename T>
    typename std::enable_if<std::is_arithmetic<T>::value, bool>::type
    operator<(Number a, T b) {
      if (a.isInt())
        return a.Integral.i < b;
      return a.fp < b;
    }
    template <typename T>
    typename std::enable_if<std::is_arithmetic<T>::value, bool>::type
    operator>(Number a, T b) {
      if (a.isInt())
        return a.Integral.i > b;
      return a.fp > b;
    }
    template <typename T>
    typename std::enable_if<std::is_integral<T>::value, bool>::type
    operator==(Number a, T b) {
      if (a.isInt())
        return a.Integral.i == b;
      return a.fp == b;
    }
    template <typename T>
    typename std::enable_if<std::is_floating_point<T>::value, bool>::type
    operator==(Number a, T b) {
      double diff = (a.isInt() ? a.Integral.i : a.fp) - b;
      return diff >= -logo::__detail::EPSILON ||
             diff <= logo::__detail::EPSILON;
    }
    template <typename T> bool operator>=(Number a, T b) {
      return a > b || a == b;
    }
    template <typename T> bool operator<=(Number a, T b) {
      return a < b || a == b;
    }
    bool operator==(Number a, Number b);
    bool operator==(const stackcell &, const stackcell &);
    bool operator!=(const stackcell &, const stackcell &);
    bool operator<(const stackcell &, const stackcell &);
    bool IsNumber(const stackcell &);
    bool IsString(const stackcell &);
    bool IsNull(const stackcell &);
    bool IsArray(const stackcell &);
    stackcell FromToken(const logo::language::tokens::Token &);
    /**
     * Structure providing array support to the vm
     * */
    struct Array {
      std::vector<stackcell> data;
      void setItem(size_t index, const stackcell &sc);
    };
    /**
     * this 'logo' implementation is stack based. It has a variable file for
     * each function's scope and it maintains it for at *least* that much
     * */
    struct Stack {
      Stack();
      std::vector<stackcell> contents;
      std::array<stackcell, 256> variables;
      Stack *parent = nullptr;
    };
    struct ProgramState {
      std::vector<Stack> currentStack;
      std::vector<size_t> execution_ptr;
    };
  } // namespace vm
} // namespace logo
