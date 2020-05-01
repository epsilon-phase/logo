#pragma once
#include <memory>
#include <optional>
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>
namespace logo {
  namespace vm {
    using String = std::string;
    using Number = double;
    struct Array;
    using stackcell = std::variant<std::monostate, String, Number,
                                   std::optional<std::shared_ptr<Array>>>;

    bool operator==(const stackcell &, const stackcell &);
    bool operator!=(const stackcell &, const stackcell &);
    bool operator<(const stackcell &, const stackcell &);
    bool IsNumber(const stackcell &);
    bool IsString(const stackcell &);
    bool IsNull(const stackcell &);
    bool IsArray(const stackcell &);
    /**
     * Structure providing array support to the vm
     * */
    struct Array {
      std::vector<stackcell> data;
      void setItem(size_t index, const stackcell &sc);
    };
    /**
     * this 'logo' implementation is stack based. It has a variable file for
     * each function's scope and it maintains it for at *least* that much
     * */
    struct Stack {
      Stack();
      std::vector<stackcell> contents;
      std::array<stackcell, 256> variables;
      Stack *parent = nullptr;
    };
    struct ProgramState {
      std::vector<Stack> currentStack;
      std::vector<size_t> execution_ptr;
    };
  } // namespace vm
} // namespace logo
