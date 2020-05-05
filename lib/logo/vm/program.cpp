#include "./program.hpp"
#include "./gc.hpp"
#include "./stack.hpp"
namespace logo {
  namespace vm {
    void Program::Sweep() {
      for (int i = 0; i < array_heap.size(); i++) {
        auto &array = array_heap[i];
        if (array.Sweep(*this)) {
          this->available_arrays.push_back(i);
        }
      }
      for (int i = 0; i < string_heap.size(); i++) {
        auto &string = string_heap[i];
        if (string.Sweep(*this))
          this->available_strings.push_back(i);
      }
    }
    Array &Program::acquire_array() {
      if (this->available_arrays.empty())){
        array_heap.push_back(Array());
      }
    }
  } // namespace vm
} // namespace logo