#include "gc.hpp"
#include <cstdlib>
#include <cstring>
namespace logo {
  namespace vm {
    using std::strcmp;
    String::String(const String &other) {
      resize(other.length);
      std::strncpy(data, other.data, other.length);
    }
    String::String(const char *c) {
      auto len = std::strlen(c);
      resize(len);
      strcpy(data, c);
      length = len;
    }
    String::String() {}
    String::~String() {
      if (data)
        free(data);
    }
    void String::append(const char *str) {
      if (str == nullptr || *str == '\0')
        return;
      size_t len = std::strlen(str);
      resize(length + len);
      if (length > 0) {
        std::strncpy(data + length, str, len + 1);
      } else {
        std::strncpy(data, str, len);
      }
      length += len;
      data[length] = 0;
    }
    void String::append(const String &s) {
      if (s.data == nullptr || s.data[0] == 0)
        return;
      resize(s.length + length);
      if (length > 0) {
        std::strncpy(data + length, s.data, s.length + 1);
      } else {
        std::strncpy(data, s.data, s.length);
      }
      length += s.length;
      data[length] = 0;
    }
    void String::resize(uint32_t size) {
      if (this->data == nullptr) {
        data = (char *)std::malloc(size + 1);
        data[size] = 0;
        capacity = size + 1;
        return;
      }
      if (capacity > size + 1) {
        data[size + 1] = 0;
        if (length > size)
          length = size;
      }
      if (capacity < size + 1) {
        data = (char *)std::realloc(data, size + 1);
        capacity = size + 1;
        data[size] = 0;
      }
    }
    void String::narrow(uint32_t start, int32_t len) {
      if (start > length) {
        length = 0;
        data[0] = 0;
        return;
      }
      if (len == -1 || len > start - length) {
        len = strlen(data + start);
      }
      std::memmove(data, data + start, len);
      length = len;
      data[len] = 0;
    }
    String String::narrowTo(uint32_t start, int32_t len) const {
      auto s = std::strlen(data + start);
      if (len == -1 || len > s)
        len = s;
      String c;
      c.length = len;
      c.resize(len);
      strncpy(c.data, data + start, len + 1);
      return c;
    }
    uint32_t min_size(const String &a, const String &b) {
      return a.length > b.length ? b.length : a.length;
    }
    bool operator<(const String &a, const String &b) {
      return strcmp(a.data, b.data) < 0;
    }
    bool operator==(const String &a, const String &b) {
      return strcmp(a.data, b.data) == 0;
    }
    bool operator>(const String &a, const String &b) {
      return strcmp(a.data, b.data) > 0;
    }
    bool operator>=(const String &a, const String &b) {
      auto i = strcmp(a.data, b.data);
      return i == 0 || i > 0;
    }
    bool operator<=(const String &a, const String &b) {
      auto i = strcmp(a.data, b.data);
      return i == 0 || i < 0;
    }
  } // namespace vm
} // namespace logo