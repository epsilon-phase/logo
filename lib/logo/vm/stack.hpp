#pragma once
#include "../detail/Epsilon.hpp"
#include "./gc.hpp"
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
    enum class ValueType {
      Array,
      String,
      None,
    };
    struct Array;
    /**
     * NaN-boxed number. Maybe it works, maybe it's trash.
     **/
    struct Number {
      Number();
      Number(const Number &a) { fp = a.fp; }
      Number(double d) : fp(d) {}
      union {
        double fp;

        struct {
#ifdef __BIG_ENDIAN__
          unsigned int sign : 1;
          unsigned int exponent : 11;
          ValueType type : 4;
          int64_t addr : 48;
#else
          int64_t addr : 48;
          ValueType type : 4;
          unsigned int exponent : 11;
          unsigned int sign : 1;
#endif
        };
      };
      bool isNumber() const;
      bool isArray() const;
      bool isString() const;
      bool isNull() const;
      void setArray(unsigned int);
      void setString(unsigned int);
      void setNull();
      Array &resolveArray(Program &);
      String &resolveString(Program &);
      Number &operator-=(const Number &);
      Number &operator+=(const Number &);
      Number &operator/=(const Number &);
      Number &operator*=(const Number &);
    };
    Number operator+(Number, Number);
    Number operator-(Number, Number);
    Number operator/(Number, Number);
    Number operator*(Number, Number);
    bool operator<(Number, Number);
    bool operator>(Number, Number);
    bool operator>=(Number, Number);
    bool operator<=(Number, Number);
    bool operator==(Number a, Number b);
    struct Program;
    /**
     * Not really a stack, more a register file.
     * */
    struct stack : public GarbageCollected {
      stack *parent = nullptr;
      std::array<Number, 256> registers;
      std::vector<Number> constants;
      virtual void Mark(Program &);
      virtual bool Sweep(Program &);
    };
    /**
     * An array object.
     * */
    struct Array : public GarbageCollected {
      virtual ~Array() {}
      virtual void Mark(Program &);
      virtual bool Sweep(Program &);
      std::vector<Number> contents;
    };
  } // namespace vm
} // namespace logo
