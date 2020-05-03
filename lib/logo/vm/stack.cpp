#include "./stack.hpp"
#include "../detail/Epsilon.hpp"
#include "../language/lexer/tokens.hpp"
#include <iostream>
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
    Number::Number() {}
    bool Number::inIntRange(int64_t a) {
      return a < (1L << 52) && a > -(1L << 52);
    }
    Number &Number::operator*=(const Number &a) {
      if (isInt()) {
        if (a.isInt()) {
          int64_t res = a.Integral.i * Integral.i;
          if (!inIntRange(res))
            this->fp = res;
          else
            Integral.i = res;
        } else {
          fp = Integral.i * a.fp;
        }
      } else {
        if (a.isInt())
          fp *= a.Integral.i;
        else
          fp *= a.fp;
      }
      return *this;
    }
    Number &Number::operator+=(const Number &b) {
      if (isInt()) {
        if (b.isInt()) {
          int64_t res = b.Integral.i + Integral.i;
          if (!inIntRange(res))
            fp = res;
          else
            Integral.i = res;
        }
      } else {
        if (b.isInt())
          fp += b.Integral.i;
        else
          fp += b.fp;
      }
      return *this;
    }
    bool Number::isInt() const { return Integral.exponent == 0b11111111111; }
    void Number::setInt() { Integral.exponent = 0b11111111111; }
    Number Number::fromFloat(double a) {
      Number result;
      result.fp = a;
      return result;
    }
    Number Number::fromInt(int64_t a) {
      Number result;
      if (!inIntRange(a)) {
        result.fp = a;
      } else {
        result.setInt();
        result.Integral.i = a;
      }
      return result;
    }
    Number operator+(Number a, Number b) {
      Number result;
      if (a.isInt()) {
        if (b.isInt()) {
          int64_t res = a.Integral.i + b.Integral.i;
          if (Number::inIntRange(res)) {
            result.setInt();
            result.Integral.i = a.Integral.i + b.Integral.i;
          } else
            result.fp = res;
        } else {
          result.fp = a.Integral.i + b.fp;
        }
      } else {
        result.fp = a.fp;
        if (b.isInt())
          result.fp += b.Integral.i;
        else
          result.fp += b.fp;
      }

      return result;
    }
    Number operator-(Number a, Number b) {
      Number result;
      if (a.isInt()) {
        if (b.isInt()) {
          result.setInt();
          result.Integral.i = a.Integral.i - b.Integral.i;
        } else {
          result.fp = a.Integral.i - b.fp;
        }
      } else {
        result.fp = a.fp;
        if (b.isInt())
          result.fp -= b.Integral.i;
        else
          result.fp -= b.fp;
      }
      return result;
    }
    Number operator/(Number a, Number b) {
      Number result;
      if (a.isInt()) {
        if (b.isInt()) {
          result.setInt();
          result.Integral.i = a.Integral.i / b.Integral.i;
        } else {
          result.fp = a.Integral.i / b.fp;
        }
      } else {
        result.fp = a.fp;
        if (b.isInt())
          result.fp /= b.Integral.i;
        else
          result.fp /= b.fp;
      }
      return result;
    }
    Number operator*(Number a, Number b) {
      Number result;
      if (a.isInt()) {
        if (b.isInt()) {
          result.setInt();
          result.Integral.i = a.Integral.i * b.Integral.i;
        } else {
          result.fp = a.Integral.i * b.fp;
        }
      } else {
        result.fp = a.fp;
        if (b.isInt())
          result.fp *= b.Integral.i;
        else
          result.fp *= b.fp;
      }
      return result;
    }
    bool operator==(Number a, Number b) {
      if (a.isInt()) {
        if (b.isInt())
          return a.Integral.i == b.Integral.i;
        double diff = b.fp - a.Integral.i;
        return diff >= -__detail::EPSILON && diff <= __detail::EPSILON;
      } else {
        double diff = a.fp;
        if (b.isInt())
          diff -= b.Integral.i;
        else
          diff -= b.fp;
        return diff >= -__detail::EPSILON && diff <= __detail::EPSILON;
      }
    }
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
        Number diff = get<Number>(a) - get<Number>(b);
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
        return get<Number>(a) < get<Number>(b);
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
    stackcell FromToken(const logo::language::tokens::Token &t) {
      using namespace logo::language::tokens;
      if (t.type == TokenType::Number) {
        double d = strtod(t.content.data(), nullptr);
        Number r;
        if (int64_t(d) == d) {

          r.setInt();
          r.Integral.i = d;
        } else {
          r.fp = d;
        }
        return r;
      } else if (t.type == TokenType::String) {
        // Narrow it because the token includes the quotes
        return std::string(&t.content[1], &t.content.back() - &t.content[1]);
      }
      return std::monostate();
    }

  } // namespace vm
} // namespace logo