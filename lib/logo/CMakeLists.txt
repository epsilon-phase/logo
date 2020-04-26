
target_sources(logo PUBLIC logo.cpp point.cpp turtle.cpp color.cpp
                           vm/stack.cpp )
add_subdirectory(language)
target_sources(logo INTERFACE logo.hpp)
