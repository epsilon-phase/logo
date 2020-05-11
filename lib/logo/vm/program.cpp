#include "./program.hpp"
#include "./function.hpp"
#include "./gc.hpp"
#include "./stack.hpp"
#include <iostream>
namespace logo {
  namespace vm {
    void Program::Sweep() {
      std::vector<int32_t> to_erase;
      for (auto &[id, array] : array_heap) {
        if (array->Sweep(*this)) {
          this->available_arrays.emplace_back(std::move(array));
          to_erase.push_back(id);
        } else
          array->marked = false;
      }
      for (auto id : to_erase)
        array_heap.erase(id);
      to_erase.clear();
      for (auto &[id, string] : string_heap) {
        if (string->Sweep(*this)) {
          this->available_strings.push_back(std::move(string));
          to_erase.push_back(id);
        } else {
          string->marked = false;
        }
      }
      for (auto id : to_erase)
        string_heap.erase(id);
    }
    Number Program::acquire_array() {
      Number r;
      if (this->available_arrays.empty()) {
        auto id = acquire_next_array_id();
        r.setArray(id);
        array_heap[id] = std::make_unique<Array>();
        return r;
      } else {
        auto id = acquire_next_array_id();
        array_heap[id] = std::move(available_arrays.back());
        available_arrays.pop_back();
        r.setArray(id);
        return r;
      }
    }
    Number Program::acquire_string() {
      Number r;
      auto id = acquire_next_string_id();
      r.setString(id);
      if (this->available_strings.empty()) {
        r.setString(id);
        string_heap[id] = std::make_unique<String>();
      } else {
        string_heap[id] = std::move(available_strings.back());
        available_strings.pop_back();
      }
      return r;
    }
    size_t Program::getProgramCounter() const { return pc.back(); }
    int32_t Program::acquire_next_array_id() {
      while (array_heap.find(last_array_id) != array_heap.end())
        last_array_id++;
      return last_array_id;
    }
    int32_t Program::acquire_next_string_id() {
      while (string_heap.find(last_string_id) != string_heap.end())
        last_string_id++;
      return last_string_id;
    }
    void Program::setProgramCounter(size_t pc) { this->pc.back() = pc; }
    Bytecode Program::getCurrentInstruction(unsigned int levelsup) const {
      auto idx = pc.size() - 1;
      auto st = current;
      while (levelsup > 0 && idx > 0) {
        st = st->parent;
        idx--;
      }
      auto p = pc[idx];
      return st->environment->bytecode[p];
    }
    void Program::popStack() {
      stack *tmp = current;
      current = current->parent;
      pc.pop_back();
      delete tmp;
    }
  } // namespace vm
} // namespace logo