#include "./bytecode.hpp"
#include "../errors/VMValueException.hpp"
#include "./function.hpp"
#include "./program.hpp"
#include <cmath>
#include <iostream>
namespace logo::vm {
  bool isConstant(uint32_t i) { return i & (1 << 8); }
  uint32_t getConstant(uint32_t i) { return i & 0xFF; }
  namespace bytecodes {
    inline std::tuple<Number, Number> getValues(stack &s, Bytecode i) {
      Number a, b;
      if (isConstant(i.normal.op1)) {
        a = s.environment->constants[getConstant(i.normal.op1)];
      } else {
        a = s.registers[i.normal.op1];
      }
      if (isConstant(i.normal.op2)) {
        b = s.environment->constants[getConstant(i.normal.op2)];
      } else {
        b = s.registers[i.normal.op2];
      }
      return std::make_tuple(a, b);
    }
    // Convenience method for grabbing numbers for
    // arithmetic instructions.
    inline std::tuple<Number, Number> getNumbers(Program &p, stack &s,
                                                 Bytecode i) {
      auto [a, b] = getValues(s, i);
      if (!a.isNumber() || !b.isNumber())
        throw logo::error::NotANumber(p.getProgramCounter());
      return std::make_tuple(a, b);
    }
    uint32_t add(Program &p, uint32_t pc, stack &s, Bytecode i) {
      auto [a, b] = getNumbers(p, s, i);
      auto c = a + b;
      s.registers[i.normal.dest] = c;
      return pc + 1;
    }
    uint32_t sub(Program &p, uint32_t pc, stack &s, Bytecode i) {
      auto [a, b] = getNumbers(p, s, i);
      s.registers[i.normal.dest] = a - b;
      return pc + 1;
    }
    uint32_t divide(Program &p, uint32_t pc, stack &s, Bytecode i) {
      auto [a, b] = getNumbers(p, s, i);
      s.registers[i.normal.dest] = a / b;
      return pc + 1;
    }
    uint32_t multiply(Program &p, uint32_t pc, stack &s, Bytecode i) {
      auto [a, b] = getNumbers(p, s, i);
      s.registers[i.normal.dest] = a * b;
      return pc + 1;
    }
    uint32_t exponentiate(Program &p, uint32_t pc, stack &s, Bytecode i) {
      auto [a, b] = getNumbers(p, s, i);
      s.registers[i.normal.dest].fp = std::pow(a.fp, b.fp);
      return pc + 1;
    }
    uint32_t ReturnFrom(Program &p, uint32_t pc, stack &s, Bytecode i) {
      stack *parent = p.current->parent;
      if (!parent) {
        // AAA!!!
        p.popStack();
        return pc;
      }
      // Get the immediate parent of the stack frame
      auto ip = p.getCurrentInstruction(1);
      int write_to = ip.normal.op1;
      // Copy them down
      for (int c = 0; c < write_to; c++) {
        parent->registers[ip.normal.dest + c] =
            s.registers[ip.largeop.dest + c];
      }
      p.popStack();
      return p.getProgramCounter() + 1;
    }
    uint32_t jump(Program &p, uint32_t pc, stack &s, Bytecode i) {
      return i.largeop.addr;
    }
    uint32_t call(Program &p, uint32_t pc, stack &s, Bytecode i) { return 0; }
    bool sameType(Number a, Number b) {
      if (a.isNumber())
        return b.isNumber();
      return a.type == b.type;
    }
    uint32_t less_than(Program &p, uint32_t pc, stack &s, Bytecode i) {
      pc += 1;
      auto [a, b] = getNumbers(p, s, i);
      return a.fp < b.fp;
    }
  } // namespace bytecodes
  void Program::dispatchInstruction() {
    using namespace bytecodes;
    auto &p = *this;
    auto pc = getProgramCounter();
    auto &s = *current;
    auto i = current->environment->bytecode[pc];
    switch (i.normal.opcode) {
    case Add:
      pc = add(p, pc, s, i);
      break;
    case Sub:
      pc = sub(p, pc, s, i);
      break;
    case Mult:
      pc = multiply(p, pc, s, i);
      break;
    case Div:
      pc = divide(p, pc, s, i);
      break;
    case Pow:
      pc = exponentiate(p, pc, s, i);
      break;
    case Return:
      pc = ReturnFrom(p, pc, s, i);
      break;
    case Jump:
      pc = jump(p, pc, s, i);
      break;
    }
    p.setProgramCounter(pc);
  }
} // namespace logo::vm