#include "./stack.hpp"
#include "../detail/Epsilon.hpp"
using namespace logo::vm;
Stack::Stack() {}
void Array::setItem(size_t index, const stackcell &sc) {
  if (data.size() < index) {
    data.resize(index + 1);
  }
  data[index] = sc;
}
bool logo::vm::IsNumber(const stackcell &s) { return s.index() == 2; }
bool logo::vm::IsString(const stackcell &s) { return s.index() == 1; }
bool logo::vm::IsNull(const stackcell &s) { return s.index() == 0; }
namespace logo {
  namespace vm {
    bool operator==(const stackcell &a, const stackcell &b) {
      using namespace std;
      if (a.index() != b.index())
        return false;
      switch (a.index()) {
      case 0:
        return false;
      case 1:
        return std::get<std::string>(a) == get<std::string>(b);
      case 2: {
        double diff = get<double>(a) - get<double>(b);
        return diff >= -logo::__detail::EPSILON &&
               diff <= logo::__detail::EPSILON;
      }; break;
      case 3: {
        const auto &v1 = get<std::optional<std::shared_ptr<Array>>>(a),
                   &v2 = get<std::optional<std::shared_ptr<Array>>>(b);
        if (!v1.has_value())
          return !v2.has_value();
        if (!v2.has_value())
          return false;
        const auto &a1 = v1.value(), &a2 = v2.value();
        if (a1->data.size() != a2->data.size())
          return false;
        for (unsigned int i = 0; i < a1->data.size(); i++)
          if (!(a1->data[i] == a2->data[i]))
            return false;
        return true;
      } break;
      default:
        // How did you get down here?!?!
        return false;
      }
    }
    bool operator<(const stackcell &a, const stackcell &b) {
      using std::get;
      if (a.index() != b.index())
        return a.index() < b.index();
      switch (a.index()) {
      case 0:
        return false;
      case 1:
        return get<double>(a) < get<double>(b);
      case 2:
        return get<std::string>(a) < get<std::string>(b);
      case 3: {
        const auto &v1 = get<std::optional<std::shared_ptr<Array>>>(a),
                   &v2 = get<std::optional<std::shared_ptr<Array>>>(b);
        if (!v1.has_value() || !v2.has_value())
          return false;
        const auto &a1 = v1.value(), &a2 = v2.value();
        for (size_t i = 0; i < a1->data.size() && i < a2->data.size(); i++)
          if (a1->data[i] != a2->data[i])
            return a1->data[i] < a2->data[i];
        return true;
      } break;
      default:
        return false;
      }
    }
    bool operator!=(const stackcell &a, const stackcell &b) {
      return !(a == b);
    }
  } // namespace vm
} // namespace logo