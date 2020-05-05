#include "./stack.hpp"
#include "./program.hpp"

#include "../detail/Epsilon.hpp"
#include "../language/lexer/tokens.hpp"
#include <iostream>
using namespace logo::vm;

namespace logo {
  namespace vm {
    Number::Number() {}

    void Number::setNull() {
      this->exponent = 0b11111111111;
      this->type = ValueType::None;
      this->addr = 0;
    }
    bool Number::isNumber() const { return exponent != 0b11111111111; }
    bool Number::isArray() const {
      return !isNumber() && type == ValueType::Array;
    }
    bool Number::isString() const {
      return !isNumber() && type == ValueType::String;
    }
    bool Number::isNull() const {
      return !isNumber() && type == ValueType::None;
    }
    void Number::setArray(unsigned int addr) {
      exponent = 0b11111111111;
      addr = addr;
      type = ValueType::Array;
    }
    void Number::setString(unsigned int addr) {
      exponent = 0b11111111111;
      addr = addr;
      type = ValueType::String;
    }
    Number operator+(Number a, Number b) {
      Number c = 0;
      c.fp = a.fp + b.fp;
      return c;
    }
    Number operator-(Number a, Number b) {
      Number c;
      c.fp = a.fp - b.fp;
      return c;
    }
    Number operator/(Number a, Number b) {
      Number c;
      c.fp = a.fp / b.fp;
      return c;
    }
    Number operator*(Number a, Number b) {
      Number c;
      c.fp = a.fp * b.fp;
      return c;
    }
    Number &Number::operator-=(const Number &b) {
      *this = *this - b;
      return *this;
    }
    Number &Number::operator+=(const Number &b) {
      *this = *this / b;
      return *this;
    }
    Number &Number::operator/=(const Number &b) {
      *this = *this / b;
      return *this;
    }
    Number &Number::operator*=(const Number &b) {
      *this = *this * b;
      return *this;
    }
    bool operator<(Number a, Number b) { return a.fp < b.fp; }
    bool operator>(Number a, Number b) { return a.fp > b.fp; }
    bool operator>=(Number a, Number b) { return a.fp >= b.fp; }
    bool operator<=(Number a, Number b) { return a.fp <= b.fp; }
    bool operator==(Number a, Number b) {
      double diff = a.fp - b.fp;
      return diff >= -__detail::EPSILON && diff <= __detail::EPSILON;
    }
    void stack::Mark(Program &p) {
      marked = true;
      for (auto &i : registers) {
        if (!i.isNumber() && !i.isNull()) {
          if (i.type == ValueType::String) {
            if (!p.array_heap[i.addr].marked)
              p.string_heap[i.addr].Mark(p);
          } else if (i.type == ValueType::Array) {
            if (!p.array_heap[i.addr].marked)
              p.array_heap[i.addr].Mark(p);
          }
        }
      }
      if (parent != nullptr) {
        parent->Mark(p);
      }
    }
    bool stack::Sweep(Program &p) { return false; }
    void Array::Mark(Program &p) {
      marked = true;
      for (auto &i : contents) {
        if (not(i.isNumber() || i.isNull())) {
          if (i.type == ValueType::Array) {
            if (!p.array_heap[i.addr].marked)
              p.array_heap[i.addr].Mark(p);
          } else if (i.type == ValueType::String) {
            if (!p.string_heap[i.addr].marked)
              p.string_heap[i.addr].Mark(p);
          }
        }
      }
    }
    bool Array::Sweep(Program &p) {
      if (marked)
        return false;
      this->contents.resize(0);
      return true;
    }
  } // namespace vm
} // namespace logo