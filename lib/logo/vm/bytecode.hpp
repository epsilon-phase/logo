#pragma once
#include "./stack.hpp"
#include "./vm.hpp"
#include <functional>
namespace logo {
  namespace vm {
    std::function<ProgramState(ProgramState &)> functions[64];
    struct Bytecode {
      union {
        struct {
          unsigned int opcode : 6;
          unsigned int dest : 8;
          unsigned int op1 : 9, op2 : 9;
        } normal;
        struct {
          unsigned int opcode : 6;
          unsigned int dest : 8;
          signed int addr : 18;
        } beeg;
      };
    };
  } // namespace vm
} // namespace logo