#include "./util.hpp"

#include "../catch2.hpp"

#include <iostream>
#include <logo/errors/VMValueException.hpp>
#include <logo/vm/function.hpp>
#include <logo/vm/gc.hpp>
#include <logo/vm/program.hpp>
#include <logo/vm/stack.hpp>
namespace Catch {
  template <> struct StringMaker<logo::vm::String> {
    static std::string convert(logo::vm::String const &value) {
      return std::string(value.data);
    }
  };
} // namespace Catch
TEST_CASE("Program", "[vm]") {
  using namespace logo::vm;
  Program p;
  WHEN("An array is acquired") {
    auto num = p.acquire_array();
    THEN("The number is appropriately tagged") {
      REQUIRE(num.isArray());
      auto addr = num.addr;
      REQUIRE(addr == 0);
    }
    THEN("The array resolves properly") {
      auto &array = num.resolveArray(p);
      // This will at least test... that it's at a valid address
      REQUIRE(array.contents.empty());
    }
  }
  WHEN("A string is acquired") {
    auto num = p.acquire_string();
    THEN("The string is appropriately tagged") {
      REQUIRE(num.isString());
      // The program is empty :)
      REQUIRE(num.addr == 0);
    }
    THEN("It is empty") {
      auto &str = num.resolveString(p);
      REQUIRE(str.length == 0);
      // It might not be a *new* string with no capacity
      REQUIRE(str.capacity >= 0);
    }
  }
  WHEN("A program has references") {
    auto a = p.acquire_array(), b = p.acquire_array(), c = p.acquire_string(),
         d = p.acquire_string();
    p.current = new stack();
    p.current->registers[0] = a;
    p.current->registers[1] = c;
    THEN("The two strings have different indices") {
      REQUIRE(c.addr != d.addr);
    }
    p.current->Mark(p);
    THEN("The accessible string is marked") {
      REQUIRE(c.resolveString(p).marked);
      std::cout << c.addr << "," << d.addr << std::endl;
      REQUIRE_FALSE(d.resolveString(p).marked);
    }
    p.current->Sweep(p);
    p.Sweep();
    THEN("The program has a single available array for reuse") {
      REQUIRE(p.available_arrays.size() == 1);
    }
    THEN("It has a single available string for reuse") {
      REQUIRE(p.available_strings.size() == 1);
    }
    d = p.acquire_string();
    REQUIRE(p.available_strings.empty());
    WHEN("The string is added to a living array") {
      auto &array = a.resolveArray(p);
      array.contents.push_back(d);
      THEN("Prior to collection, the string is alive") {
        REQUIRE(p.string_heap.find(d.addr) != p.string_heap.end());
      }
      p.current->Mark(p);
      p.Sweep();
      THEN("The string persists") {
        REQUIRE_FALSE(a.resolveArray(p).contents.empty());
        REQUIRE(p.string_heap.find(d.addr) != p.string_heap.end());
        REQUIRE(p.available_strings.empty());
      }
    }
    WHEN("A string is added to a dead array") {
      auto e = p.acquire_string();
      b = p.acquire_array();
      auto &array = b.resolveArray(p);
      array.contents.push_back(e);
      p.current->Mark(p);
      p.Sweep();
      THEN("The string is freed") {
        REQUIRE(p.available_strings.size() == 2);
        REQUIRE(p.string_heap.find(e.addr) == p.string_heap.end());
      }
    }
    // TODO add testing for collecting constants(that is, it doesn't happen)
  }
}
TEST_CASE("boxing exceptions", "[vm]") {
  using namespace logo::vm;
  Program p;
  // Necessary because there must be program counter to keep the code quick
  p.pc.push_back(0);
  Number c = 1.5;
  WHEN("It isn't an array or string") {
    THEN("It throws an IAmASimpleNumber exception") {
      REQUIRE_THROWS_AS(c.resolveArray(p), logo::error::IAmButASimpleNumber);
    }
  }
  WHEN("It isn't an array") {
    c.setString(5);
    REQUIRE_THROWS_AS(c.resolveArray(p), logo::error::NotAnArray);
  }
  WHEN("It isn't a string") {
    c.setArray(5);
    REQUIRE_THROWS_AS(c.resolveString(p), logo::error::NotAString);
  }
}
logo::vm::Bytecode getNormal(logo::vm::bytecodes::bytecode_ids op,
                             uint32_t dest, uint32_t op1, uint32_t op2) {
  logo::vm::Bytecode c;
  c.normal.opcode = op;
  c.normal.dest = dest;
  c.normal.op1 = op1;
  c.normal.op2 = op2;
  return c;
}
TEST_CASE("Simple bytecode interpretation", "[vm]") {
  using namespace logo::vm;
  Program p;
  auto s = new stack();
  auto f = new Function();
  const unsigned int const_tag = 1 << 8;
  f->constants.push_back(Number(1));
  f->constants.push_back(Number(3));
  f->bytecode.push_back(
      getNormal(bytecodes::Add, 1, (1 << 8) | 0, (1 << 8) | 0));
  f->bytecode.push_back(getNormal(bytecodes::Sub, 0, (1 << 8) | 1, 1));
  f->bytecode.push_back(getNormal(bytecodes::Mult, 0, 1, 1));
  f->bytecode.push_back(getNormal(bytecodes::Pow, 0, const_tag | 1, 1));
  f->bytecode.push_back(getNormal(bytecodes::Div, 0, 0, const_tag | 1));
  s->environment = f;
  p.current = s;
  p.pc.push_back(0);
  WHEN("a few things are opened") {
    p.dispatchInstruction();
    THEN("It adds when adding") { REQUIRE(s->registers[1] == 2); }
    p.dispatchInstruction();
    THEN("It subtracts when subtracting") { REQUIRE(s->registers[0] == 1); }
    p.dispatchInstruction();
    THEN("It multiplies") { REQUIRE(s->registers[0] == 4); }
    p.dispatchInstruction();
    THEN("It exponentiates") { REQUIRE(s->registers[0] == 9); }
    p.dispatchInstruction();
    THEN("It divides") { REQUIRE(s->registers[0] == 3); }
  }
  delete s;
  delete f;
}