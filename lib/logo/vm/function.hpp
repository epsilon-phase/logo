#pragma once
#include "bytecode.hpp"
namespace logo {
  namespace vm {
    struct FunctionCandidate {
      std::vector<stackcell> constants;
      std::vector<stackcell> variables;
      std::vector<ByteCodeCandidate> bytecode;
    };
  } // namespace vm
} // namespace logo