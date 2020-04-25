#pragma once
#include <exception>
#include <string>
#include <string_view>
namespace logo {
  namespace error {
    struct SyntaxException : public std::exception {
      std::string_view context;
      int line;
      std::string_view get_context() const { return context; }
      virtual const char *what() const noexcept { return "Syntax Error!"; }
      SyntaxException(int line, const char *line_start,
                      const char *current_point)
          : std::exception(), line(line) {
        context = std::string_view(line_start, current_point - line_start);
      }
    };
  } // namespace error
} // namespace logo