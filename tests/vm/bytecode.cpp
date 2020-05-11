#include "../catch2.hpp"
#include <logo/vm/bytecode.hpp>
TEST_CASE("Structural considerations", "[bytecode]") {
  using namespace logo::vm;
  Bytecode c;
  WHEN("The address object is negative") {
    c.largeop.addr = -1;
    THEN("It is actually negative :)") { REQUIRE(c.largeop.addr < 0); }
  }
  WHEN("The operend objects are constants") {
    c.normal.op1 = 1 << 8 | 5;
    c.normal.op2 = 1 << 8 | 9;
    THEN("They are identified as constants") {
      REQUIRE(isConstant(c.normal.op1));
    }
    THEN("They are correctly decoded") {
      REQUIRE(getConstant(c.normal.op1) == 5);
      REQUIRE(getConstant(c.normal.op2) == 9);
    }
  }
  WHEN("The opcode is assigned") {
    c.normal.opcode = bytecodes::Add;
    Bytecode d;
    d.largeop.opcode = bytecodes::Sub;
    THEN("It is the same regardless of format") {
      REQUIRE(c.largeop.opcode == bytecodes::Add);
      REQUIRE(d.normal.opcode == bytecodes::Sub);
    }
  }
}
