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
      template <typename T>
      /**
       * Convenience function to be called on a parse success
       * @param t The node
       * @param start The new position in the stream
       * @returns the ParseResult object 😸️
       * */
      ParseResult<T> Succeed(std::unique_ptr<T> &t, TokenStreamIterator start) {
        return std::make_optional(std::make_tuple(std::move(t), start));
      }
      //! Base class for the abstract syntax tree
      struct ASTNodeBase {
        virtual ~ASTNodeBase() {}
        //! The node one level closer to the root, or null if it *is* the root
        ASTNodeBase *parent;
        SymbolMap local_symbols;
        //! The token that starts off this tree
        logo::language::tokens::Token *token = nullptr;
        //! The immediate children of this AST node
        std::vector<std::unique_ptr<ASTNodeBase>> children;
        //! Descriptive text of the node type
        virtual const char *what() const = 0;
        //! Print the tree to the given stream.
        /**
         * @param o The output stream
         * @param depth The depth
         * */
        void print_tree(std::ostream &, int depth) const;
        /**
         * Move a child into the subtree, modifying \see ASTNodeBase::parent
         * */
        void add_child(std::unique_ptr<ASTNodeBase> &&);
        // virtual void compile() const = 0;
      };
      struct ConstantLiteralAST : public ASTNodeBase {
        virtual ~ConstantLiteralAST() {}
        virtual const char *what() const { return "Constant"; }
        static ParseResult<ConstantLiteralAST> parse(TokenStreamIterator start);
      };
      struct ArrayAccessAST : public ASTNodeBase {
        virtual ~ArrayAccessAST() {}
        virtual const char *what() const { return "Array access "; }
        static ParseResult<ArrayAccessAST> parse(TokenStreamIterator start);
      };
      struct AtomAST : public ASTNodeBase {
        virtual ~AtomAST() {}
        virtual const char *what() const { return "Atom"; }
        static ParseResult<AtomAST> parse(TokenStreamIterator start);
      };
      //! The base class for all binary operators.
      struct BinaryOpAST : public ASTNodeBase {
        virtual ~BinaryOpAST() {}
        virtual const char *what() const {
          return "BinaryOp(Override me plox)";
        }
      };
      struct AddSub : public BinaryOpAST {
        virtual ~AddSub() {}
        virtual const char *what() const { return "AddSub"; }
        static ParseResult<AddSub> parse(TokenStreamIterator start);
      };
      struct MultDiv : public BinaryOpAST {
        virtual ~MultDiv() {}
        virtual const char *what() const { return "MultDiv"; }
        static ParseResult<MultDiv> parse(TokenStreamIterator start);
      };
      struct ExponentExpr : public BinaryOpAST {
        virtual ~ExponentExpr() {}
        virtual const char *what() const { return "ExponentExpr"; }
        static ParseResult<ExponentExpr> parse(TokenStreamIterator start);
      };
      /**
       * \page Expression_Precedence
       * For more details see \see logo::language::parser::ExpressionAST
       *
       * Hierarchy:->
       * \dot
       * digraph G{
       *   Expression -> AddSub -> Multiplication -> Exponent -> Atom;
       *   AddSub -> AddSub [label="[+-]"];
       *   Multiplication -> Multiplication [label="('*'|'/'|'%')"];
       *   Exponent -> Exponent [label="^"];
       *   Atom -> {Constant,ArrayAccess,Variable,Call,Expression};
       * }
       * \enddot
       * \dot
       * digraph G{
       *   rankdir="LR";
       *   subgraph cluster_array{
       *     AVar -> lbr -> Atom -> rbr;
       *     lbr[label="["];
       *     rbr[label="]"];
       *   }
       * }
       * \enddot
       * */
      struct ExpressionAST : public ASTNodeBase {
        virtual ~ExpressionAST() {}
        virtual const char *what() const { return "Expression"; }
        static ParseResult<ExpressionAST> parse(TokenStreamIterator start);
      };
      //! Argument AST
      struct ArgumentAST : public ASTNodeBase {
        virtual ~ArgumentAST() {}
        virtual const char *what() const { return "Argument"; }
      };
      //! Function call ast
      struct CallAST : public ASTNodeBase {
        virtual ~CallAST() {}
        virtual const char *what() const { return "Call"; }
        static ParseResult<CallAST> parse(TokenStreamIterator start);
      };
      struct RValueAST : public ASTNodeBase {
        virtual ~RValueAST() {}
        virtual const char *what() const { return "RValue"; };
        static ParseResult<RValueAST> parse(TokenStreamIterator s);
      };
      /**
       * Ast type for the left value of an assignment
       * */
      struct LValueAST : public ASTNodeBase {
        virtual ~LValueAST() {}
        virtual const char *what() const { return "LValue"; }
        static ParseResult<LValueAST> parse(TokenStreamIterator start);
      };
      struct AssignmentAST : public ASTNodeBase {
        virtual ~AssignmentAST() {}
        virtual const char *what() const { return "Assignment"; }
        static ParseResult<AssignmentAST> parse(TokenStreamIterator start);
      };
      struct VariableNameAST : public ASTNodeBase {
        virtual ~VariableNameAST() {}
        virtual const char *what() const { return "Variable Name"; }
        static ParseResult<VariableNameAST> parse(TokenStreamIterator start);
      };
      struct VariableDecAST : public ASTNodeBase {
        virtual ~VariableDecAST() {}
        virtual const char *what() const { return "Variable declaration"; }
        static ParseResult<VariableDecAST> parse(TokenStreamIterator start);
      };
      struct StatementAST : public ASTNodeBase {
        virtual ~StatementAST() {}
        virtual const char *what() const { return "Statement"; }
        static ParseResult<StatementAST> parse(TokenStreamIterator start);
      };
      struct BlockAST : public ASTNodeBase {
        virtual ~BlockAST() {}
        static ParseResult<BlockAST> parse(TokenStreamIterator start);
        virtual const char *what() const { return "Block"; }
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