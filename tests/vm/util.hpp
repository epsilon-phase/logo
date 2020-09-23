#include <logo/vm/gc.hpp>
#include <ostream>

namespace logo {
  namespace vm {
    std::ostream &operator<<(std::ostream &s, String const &v) {
      s << v.data;
      return s;
    }
  } // namespace vm
} // namespace logo