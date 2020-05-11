#pragma once
#include <exception>
namespace logo {
  namespace error {
    struct VMValueException : public std::exception {
      size_t program_counter;
      virtual const char *what() const noexcept {
        return "VMValueException(override me)";
      }

    protected:
      VMValueException(size_t pc) : program_counter(pc) {}
    };
    struct NotAString : public VMValueException {
      NotAString(size_t pc) : VMValueException(pc) {}
      virtual const char *what() const noexcept { return "Not A string"; }
    };
    struct NotAnArray : public VMValueException {
      NotAnArray(size_t pc) : VMValueException(pc) {}
      virtual const char *what() const noexcept { return "Not an Array!"; }
    };
    struct IAmButASimpleNumber : public VMValueException {
      IAmButASimpleNumber(size_t pc) : VMValueException(pc) {}
      const char *what() const noexcept { return "I am but a simple number"; }
    };
    struct NotANumber : public VMValueException {
      NotANumber(size_t pc) : VMValueException(pc) {}
      const char *what() const noexcept { return "Not a number! >:("; }
    };
  } // namespace error
} // namespace logo