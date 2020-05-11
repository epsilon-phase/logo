#pragma once
#include "./gc.hpp"
#include "./stack.hpp"
#include <memory>
#include <unordered_map>
namespace logo {
  namespace vm {
    struct Bytecode;
    struct Array;
    struct Program {
      stack *current;
      std::vector<size_t> pc;
      std::unordered_map<int32_t, std::unique_ptr<Array>> array_heap;
      std::unordered_map<int32_t, std::unique_ptr<String>> string_heap;
      int32_t last_array_id = 0, last_string_id = 0;
      std::vector<std::unique_ptr<Array>> available_arrays;
      std::vector<std::unique_ptr<String>> available_strings;
      void Sweep();
      Number acquire_string();
      Number acquire_array();
      size_t getProgramCounter() const;
      void setProgramCounter(size_t pc);
      void popStack();
      void pushStack(Function *);
      Bytecode getCurrentInstruction(unsigned int levelsup = 0) const;
      void dispatchInstruction();

    private:
      int32_t acquire_next_array_id();
      int32_t acquire_next_string_id();
    };
  } // namespace vm
} // namespace logo