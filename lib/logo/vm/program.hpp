#pragma once
#include "./gc.hpp"
#include "./stack.hpp"
#include <memory>
namespace logo {
  namespace vm {
    struct Array;
    struct Program {
      stack *current;
      std::vector<size_t> pc;
      std::vector<Array> array_heap;
      std::vector<String> string_heap;
      std::vector<size_t> available_arrays;
      std::vector<size_t> available_strings;
      void Sweep();
      Number &acquire_string();
      Number &acquire_array();
    };
  } // namespace vm
} // namespace logo