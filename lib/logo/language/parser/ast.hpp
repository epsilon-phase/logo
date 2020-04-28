#pragma once
#include "../lexer/lexer.hpp"
#include <functional>
#include <memory>
#include <unordered_map>
#include <vector>
// TODO Add booleanexpr, conditionalexpr and then the control flow AST stuff
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
        virtual bool collapsible() const { return false; }
        /**
         * Attempt to collapse the tree.
         *
         * 1. For each child node
         *   1. If it is not of a collapsible type, recurse into it
         *   2. Replace it with its only child
         *   3. If that child is collapsible the replace that with its only
         *      child. Repeat until no longer true
         *   4. Collapse that child
         * */
        void collapse();
        //! Print the tree to the given stream.
        /**
         * @param o The output stream
         * @param depth The depth
         * */
        void print_tree(std::ostream &, int depth) const;
        /**
         * Produce a graphviz file representing the syntax tree and print to the
         * stream
         *
         * @param f The stream
         * @param thisid The id for this node
         * @param The parent id
         * @returns The id for the next node
         * */
        int produce_dot(std::ostream &, int thisid = -1,
                        int parentid = -1) const;
        /**
         * Move a child into the subtree, modifying \see ASTNodeBase::parent
         * */
        void add_child(std::unique_ptr<ASTNodeBase> &&);
        size_t count_leaves() const;
        //! Count the number of nodes overall in the tree
        size_t tree_size() const;
        /**
         * Traverse the tree, calling a function on each node in pre-order
         * @param f The function to call
         * */
        bool explore(std::function<bool(ASTNodeBase *)>);
        bool is_leaf() const;

      private:
        //! returns true if the node has one child and is of a collapsible
        //! type,
        bool can_collapse() const;
        // virtual void compile() const = 0;
      };
      //! A literal present in the source file
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
        virtual bool collapsible() const { return true; }
        static ParseResult<AtomAST> parse(TokenStreamIterator start);
      };
      //! The base class for all binary operators.
      struct BinaryOpAST : public ASTNodeBase {
        virtual ~BinaryOpAST() {}
        virtual const char *what() const {
          return "BinaryOp(Override me plox)";
        }
        virtual bool collapsible() const { return true; }
        void collapse();
      };
      struct BooleanAST : public BinaryOpAST {
        virtual ~BooleanAST() {}
        virtual const char *what() const { return "BooleanExpr"; }
        static ParseResult<BooleanAST> parse(TokenStreamIterator start);
      };
      struct ComparisonAST : public BinaryOpAST {
        virtual ~ComparisonAST() {}
        virtual const char *what() const { return "ComparisonExpr"; }
        //! Parse it!
        static ParseResult<ComparisonAST> parse(TokenStreamIterator start);
      };
      struct AddSub : public BinaryOpAST {
        virtual ~AddSub() {}
        virtual const char *what() const { return "AddSub"; }
        //! Parse it!
        static ParseResult<AddSub> parse(TokenStreamIterator start);
      };
      struct MultDiv : public BinaryOpAST {
        virtual ~MultDiv() {}
        virtual const char *what() const { return "MultDiv"; }
        //! Parse it!
        static ParseResult<MultDiv> parse(TokenStreamIterator start);
      };
      struct ExponentExpr : public BinaryOpAST {
        virtual ~ExponentExpr() {}
        virtual const char *what() const { return "ExponentExpr"; }
        //! Parse it!
        static ParseResult<ExponentExpr> parse(TokenStreamIterator start);
      };
      struct ControlFlowAST : public ASTNodeBase {
        virtual ~ControlFlowAST(){};
        virtual const char *what() const {
          return "ControlFlow(Override me plox)";
        }
      };
      //! If else AST
      /**
       * Bytecode stuff:
       * 1. Condition Check
       * 2. If true, don't jump, otherwise go to 4 if there's an else block, 5
       *    otherwise
       * 3. BLOCK
       * 4. Else Block
       * 5. The end
       * */
      struct IfElseAST : public ControlFlowAST {
        virtual ~IfElseAST() {}
        virtual const char *what() const { return "If-else"; }
        static ParseResult<IfElseAST> parse(TokenStreamIterator start);
      };
      //! For loop
      /**
       * Bytecode stuffz:
       * 1. Initial Assignment
       * 2. Conditional check
       * 3. Jump to end if not
       * 4. The BLOCK
       * 5. Update assignment
       * 6. Jump to 2.
       * */
      struct ForLoopAST : public ControlFlowAST {
        virtual ~ForLoopAST() {}
        virtual const char *what() const { return "For loop"; }
        static ParseResult<ForLoopAST> parse(TokenStreamIterator start);
      };
      /**
       * Bytecode stuff:
       * 1. Check
       * 2. If false jump past 4
       * 3. BLOCK
       * 4. jump to 2
       * */
      struct WhileLoopAST : public ControlFlowAST {
        virtual ~WhileLoopAST() {}
        virtual const char *what() const { return "While loop"; }
        static ParseResult<WhileLoopAST> parse(TokenStreamIterator start);
      };
      /**
       * Bytecode stuff:
       * 1. BLOCK
       * 2. Check
       * 3. Jump to 1 if true
       * */
      struct DoWhileAST : public ControlFlowAST {
        virtual ~DoWhileAST() {}
        virtual const char *what() const { return "DoWhile"; }
        static ParseResult<DoWhileAST> parse(TokenStreamIterator start);
      };
      /**
       * \page Expression_Precedence
       * For more details see \see logo::language::parser::ExpressionAST
       *
       * Hierarchy:->
       * \dot
       * digraph G{
       *   Expression -> Boolean -> Comparison;
       *   Comparison -> AddSub -> Multiplication -> Exponent -> Atom;
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
      //! File node, File -> Function+
      struct FileAST : public ASTNodeBase {
        virtual ~FileAST() {}
        static ParseResult<FileAST> parse(TokenStreamIterator start);
        virtual const char *what() const { return "File"; }
      };
      /**
       * Parse the toplevel of the grammar, returning the AST.
       * @param tu The translationunit to parse
       * @param collapse Whether to collapse unnecessary nodes if possible
       * */
      std::unique_ptr<ASTNodeBase>
      ParseToplevel(std::shared_ptr<TranslationUnit> &, bool collapse = true);
    } // namespace parser
  }   // namespace language
} // namespace logo