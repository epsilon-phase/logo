#pragma once
#include <cstddef>
#include <cstdint>
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
      String(const String &);
      String();
      String(const char *c);
      virtual ~String();
      char *data = nullptr;
      uint32_t length = 0, capacity = 0;
      void append(const char *);
      void append(const String &);
      void narrow(uint32_t start, int32_t length = -1);
      void resize(uint32_t size);
      String narrowTo(uint32_t s, int32_t len = -1) const;
      virtual void Mark(Program &);
      //! Collect the garbage
      virtual bool Sweep(Program &);
    };
    bool operator<(const String &, const String &);
    bool operator==(const String &, const String &);
    bool operator>(const String &, const String &);
    bool operator>=(const String &, const String &);
    bool operator<=(const String &, const String &);
  } // namespace vm
} // namespace logo