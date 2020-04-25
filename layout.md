# Directory layout

* `extern` third party libraries brought in at compile time
* `lib` Library Modules
  * `lib/logo` The base logo library
    * `lib/logo/language` Sources related to parsing
      * `lexer/helper.hpp` Lexer helper functions
      * `lexer/lexer.cpp` Lexer implementation insensitive functions
      * `lexer/lexer3.cpp` The hand written lexer
      * `lexer/tokens.hpp` Token definitions
      * `lexer/tokens.cpp` Token implementation
  * `lib/logo/vm` Virtual machine sources
  * `detail` Implementation details that can be tucked away without
    an external interface
  * `lib/errors` directory for error handling shitz
* `tests` Test directory
* `doc` Documentation that doesn't properly fit inline with
  the code