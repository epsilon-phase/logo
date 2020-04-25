
target_sources(logo PUBLIC logo.cpp point.cpp turtle.cpp color.cpp
                           language/lexer/lexer.cpp language/lexer/tokens.cpp
                           vm/stack.cpp language/lexer/lexer3.cpp)

target_sources(logo INTERFACE logo.hpp)
