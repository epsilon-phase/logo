#pragma once
#include <cstddef>
namespace logo {
  namespace vm {
    struct Program;
    class GarbageCollected {
    public:
      virtual ~GarbageCollected() {}
      //! Determine what is reachable
      virtual void Mark(Program &) = 0;
      //! Collect what isn't :)
      /**
       * @param p The program instance
       * @returns If the object was collected
       * */
      virtual bool Sweep(Program &) = 0;

      bool marked = false;
    };
    struct String : public GarbageCollected {
      virtual ~String() {}
      char *data;
      size_t length;
      virtual void Mark(Program &);
      //! Collect the garbage
      virtual bool Sweep(Program &);
    };
  } // namespace vm
} // namespace logo