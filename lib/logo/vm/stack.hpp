#pragma once
#include <variant>
#include <string>
#include <vector>
#include <optional>
#include <unordered_map>
namespace logo{
    namespace vm{
        using String=std::string;
        using Number=double;
        struct Array;
        /**
         * Evidently necessary in order to support recursive variants
         */
        template<typename T> struct recursive_wrapper{
            recursive_wrapper(T t_){t.emplace_back(std::move(t_));}
            operator const T&() const {return t.front;}
            std::optional<T> t;
        };
        using stackcell = std::variant<String,Number,recursive_wrapper<Array>>;
        /**
         * Structure providing array support to the vm
         * */
        struct Array{
            std::vector<stackcell> data;
            void setItem(size_t index,const stackcell &sc);
        };
        /**
         * this 'logo' implementation is stack based. It has a variable file for each function's scope and it maintains it for at *least* that much 
         * */
        struct Stack{
            Stack(size_t variable_count);
            std::vector<stackcell> contents;
            std::vector<stackcell> variables;
        };
    }
}
