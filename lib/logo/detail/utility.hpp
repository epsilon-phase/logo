#pragma once
#include <iostream>
namespace logo {
  namespace __detail {
    template <typename T>
    void print_info(std::ostream &o, const char *name = nullptr) {
      static bool hasprinted = false;
      if (hasprinted)
        return;
      if (name)
        o << "Type '" << name << "' information:\n";
      else
        o << "Unnamed Type information\n";
      o << "Size of type: " << sizeof(T) << "\nAlignment:" << alignof(T)
        << std::endl;
      hasprinted = true;
    }
  } // namespace __detail
} // namespace logo
