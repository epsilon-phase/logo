#pragma once
#include "bytecode.hpp"
namespace logo {
  namespace vm {
    struct FunctionCandidate {
      std::vector<Number> constants;
      std::vector<ByteCodeCandidate> bytecode;
    };
    struct Function {
      std::vector<Number> constants;
      std::vector<Bytecode> bytecode;
    };
  } // namespace vm
} // namespace logo