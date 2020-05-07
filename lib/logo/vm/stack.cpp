#include "./stack.hpp"
#include "../detail/Epsilon.hpp"
#include "../errors/VMValueException.hpp"
#include "../language/lexer/tokens.hpp"
#include "./function.hpp"
#include "./program.hpp"
#include <iostream>
using namespace logo::vm;

namespace logo {
  namespace vm {

    Number::Number() {}
    void Number::setNan() { exponent = 0b11111111111; }
    void Number::setNull() {
      setNan();
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
      setNan();
      this->addr = addr;
      type = ValueType::Array;
    }
    void Number::setString(unsigned int addr) {
      setNan();
      this->addr = addr;
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
      *this = *this + b;
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
    Array &Number::resolveArray(Program &p) {
      if (isNumber())
        throw(logo::error::IAmButASimpleNumber(p.getProgramCounter()));
      else if (!isArray())
        throw(logo::error::NotAnArray(p.getProgramCounter()));
      return *p.array_heap[addr];
    }
    String &Number::resolveString(Program &p) {
      if (isNumber())
        throw logo::error::IAmButASimpleNumber(p.getProgramCounter());
      if (!isString())
        throw logo::error::NotAString(p.getProgramCounter());
      return *p.string_heap[addr];
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
            auto &s = i.resolveString(p);
            if (!s.marked)
              s.Mark(p);
          } else if (i.type == ValueType::Array) {
            auto &s = i.resolveArray(p);
            if (!s.marked)
              s.Mark(p);
          }
        }
      }
      // TODO this doesn't make sense to put here
      if (environment)
        for (auto &i : environment->constants) {
          if (!i.isNumber()) {
            if (i.isArray()) {
              auto &a = i.resolveArray(p);
              if (!a.marked)
                a.Mark(p);
            } else if (i.isString()) {
              auto &s = i.resolveString(p);
              if (!s.marked)
                s.Mark(p);
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
          if (i.isArray()) {
            auto &s = i.resolveArray(p);
            if (!s.marked)
              s.Mark(p);
          } else if (i.isString()) {
            auto &s = i.resolveString(p);
            if (!s.marked)
              s.Mark(p);
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