#include "../catch2.hpp"
#include <iostream>
#include <logo/vm/gc.hpp>
#include <logo/vm/program.hpp>
#include <logo/vm/stack.hpp>
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
  }
}