#include "gc.hpp"
#include "./program.hpp"
#include "./stack.hpp"
namespace logo {
  namespace vm {
    void String::Mark(Program &p) { this->marked = true; }
    bool String::Sweep(Program &p) {
      if (marked)
        return false;
      delete[] this->data;
      data = nullptr;
      this->length = 0;
      this->marked = false;
      return true;
    }
  } // namespace vm
} // namespace logo