#pragma once
#include "../lexer/lexer.hpp"
#include <memory>
#include <unordered_map>
#include <vector>
namespace logo {
  namespace language {
    /**
     * The parser and related machinery
     * */
    namespace parser {
      /**
       * Alias for the hashmap for symbol names
       * */
      using SymbolMap = std::unordered_map<std::string, int>;
      template <typename T>
      using ParseResult =
          std::optional<std::tuple<std::unique_ptr<T>, TokenStreamIterator>>;
      //! Base class for the abstract syntax tree
      struct ASTNodeBase {
        virtual ~ASTNodeBase() {}
        ASTNodeBase *parent;
        SymbolMap local_symbols;
        //! The token that starts off this tree
        logo::language::tokens::Token *token = nullptr;
        std::vector<std::unique_ptr<ASTNodeBase>> children;
        virtual const char *what() const = 0;
        void print_tree(std::ostream &, int depth) const;
        // virtual void compile() const = 0;
      };
      //! Parameter declaration node
      struct ParamDecAST : public ASTNodeBase {
        virtual ~ParamDecAST() {}
        static ParseResult<ParamDecAST> parse(TokenStreamIterator start);
        virtual const char *what() const { return "Parameter Declaration"; }
      };
      //! Parameter list node
      struct ParameterAST : public ASTNodeBase {
        virtual ~ParameterAST() {}
        static ParseResult<ParameterAST> parse(TokenStreamIterator start);
        virtual const char *what() const { return "Parameter"; }
      };
      //! Function node
      struct FunctionAST : public ASTNodeBase {
        virtual ~FunctionAST() {}
        static ParseResult<FunctionAST> parse(TokenStreamIterator start);
        virtual const char *what() const { return "Function"; }
      };
      std::unique_ptr<ASTNodeBase>
      ParseToplevel(std::shared_ptr<TranslationUnit> &);
    } // namespace parser
  }   // namespace language
} // namespace logo