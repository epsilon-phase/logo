
target_sources(logo PUBLIC logo.cpp point.cpp turtle.cpp color.cpp)
add_subdirectory(language)
add_subdirectory(vm)
target_sources(logo INTERFACE logo.hpp)
