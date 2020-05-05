#pragma once
#include "./stack.hpp"
#include "./vm.hpp"
#include <functional>
namespace logo {
  namespace vm {
    std::function<ProgramState(ProgramState &)> opcodes[64];
    //! Containment namespace for bytecode names
    namespace bytecodes {
      enum bytecode_ids {
        // Arithmetic
        Add,
        Sub,
        Mult,
        Div,
        Pow,
        // Logic ops
        LogOr,
        LogAnd,
        LogNot,
        // Comparison
        //! The less than (or equals!) operator
        /**
         * Takes the form (1) A B
         * If the comparison is false, then it skips over the next instruction
         *
         * So
         * ```
         * If a>b then
         *   print(a)
         * endif
         * ```
         * ```
         * turns into
         * 00 LT 1 R0 R1
         * 01 JMP 0 05 ;The 0 is unused
         * 02 GET_GLOBAL R3 K1
         * 03 MOVE R4 R1
         * 04 CALL R3 2 1
         * 05 RETURN R0 1
         * ```
         * */
        LT,
        //! The Less than or equal operator `>=`
        /**
         * @see LT for how this works
         * */
        LE,
        EQ,
        NEQ,
        //! Is the value truthy??
        Test,
        // Control Flow
        //! The jump instruction
        Jump,
        //! The call instruction
        /**
         * Format = @see Bytecode::normal
         *
         * Call (register) (Number of arguments+1) (Number of variables
         * assigned+1)
         *
         * The specified Register determines the first register to
         * use, which is the function that is called. The arguments to the
         * function are loaded into the registers that follow it and used by the
         * resulting call
         *
         * Reading over the source code, it appears that the third
         * argument specifies how many values are expected(+1)
         *
         * Further experimentation suggests that the range of results is stored
         * over the registers used in the call.
         * ```lua
         * function ab(a,b,c)
         *   return a,b,c
         * end
         * local a,b,c
         * a,b,c=ab(1,2,3)
         * ```
         * becomes something like:
         * ```
         * GET_GLOBAL R3 K0
         * LOADK R4 K1
         * LOADK R5 K2
         * LOADK R6 K3
         * CALL R3 4 4 ;R3,R4,R5=R3(R4,R5,R6)
         * ```
         * */
        Call,
        //! The return instruction
        /**
         * Format is @see LargeOp
         * Lua specifies it as [Register]+length of registers to return
         * So, `RETURN R1 5` returns R1 through R5(It appears that returning
         * with 1 is returning nil)
         * */
        Return,
        // Load instructions
        //! Possibly the only load instruction that uses the 'normal' format
        LoadBool,
        //! Load constant
        LoadK,
        //! Move one register's contents to another
        /**
         * Two argument register, So the @see Bytecode::largeop format, but the
         * second argument is just a register that is copied into the first
         * */
        Move,
      };
    }
    //! Bytecode that hasn't been through register reassignment
    //! Too large to use directly
    struct ByteCodeCandidate {
      using uint = uint32_t;
      union {
        struct {
          uint opcode;
          uint dest;
          uint op1;
          uint op2;
        } normal;
        struct {
          uint opcode;
          uint dest;
          uint addr;
        } largeop;
      };
    };
    /**
     * This is the bytecode structure. It is heavily informed by the way that
     * Lua works 😸️
     * */
    struct Bytecode {

      union {
        //! The more common opcode format
        struct {
          //! The opcode field, same for both formats of instruction
          unsigned int opcode : 6;
          //! The destination field, same for both instructions
          unsigned int dest : 8;
          //! The first operend, if `op1&(1<<9)` then it is a constant rather
          //! than a register
          unsigned int op1 : 9;
          //! The second operend, if `op1&(1<<9)` then it is a constant rather
          //! than a register
          unsigned int op2 : 9;
        } normal;
        //! The large op format, for things that need more than 256 options
        //! Or an address space large enough to write a program that *might even
        //! be useful*
        struct {
          //! The opcode field, same for both formats of instruction
          unsigned int opcode : 6;
          //! Unused for jump instructions, often used for constant loading
          //! beyond the usual 256 limit
          unsigned int dest : 8;
          //! the constant index for load instructions or the address to jump to
          //! on the execution of the jump instruction
          signed int addr : 18;
        } largeop;
      };
      //! Produce a human readable representation of the instruction
      std::string to_string() const;
    };
  } // namespace vm
} // namespace logo