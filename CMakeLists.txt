cmake_minimum_required(VERSION 3.12..3.15)
project(logoaway VERSION 0.0.0
  DESCRIPTION "Lol, logo"
  LANGUAGES C CXX)
# Make target_sources respect where the CMakeLists file is per
# https://crascit.com/2016/01/31/enhanced-source-file-handling-with-target_sources/
cmake_policy(SET CMP0076 NEW)
set(CMAKE_POLICY_DEFAULT_CMP0069 NEW) #enable interprocedural optimization for all projects
set(BUILD_SHARED_LIBS OFF)
option(BUILD_TESTS "Build tests of logo runner" OFF)
add_subdirectory("extern/SFML")
add_library(logo STATIC)
add_executable(logoaway)
target_include_directories(logo PRIVATE ${CMAKE_SOURCE_DIR}/extern/SFML/include)
target_link_libraries(logo sfml-graphics)
target_link_libraries(logoaway sfml-graphics sfml-window logo)
target_include_directories(logoaway
  PRIVATE extern/SFML/include
          ${CMAKE_SOURCE_DIR}/lib
)
add_subdirectory(lib/logo)
add_subdirectory(gui)
if(BUILD_TESTS)
  add_subdirectory(tests)
endif()