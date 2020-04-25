cmake_minimum_required(VERSION 3.12..3.15)
project(logoaway VERSION 0.0.0
  DESCRIPTION "Lol, 'logo'"
  LANGUAGES C CXX)
include(CMakeDependentOption)
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake_modules)
find_package(RAGEL 6.10 REQUIRED)
# Make target_sources respect where the CMakeLists file is per
# https://crascit.com/2016/01/31/enhanced-source-file-handling-with-target_sources/
cmake_policy(SET CMP0076 NEW)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_POLICY_DEFAULT_CMP0069 NEW) #enable interprocedural optimization for all projects
set(BUILD_SHARED_LIBS OFF)
option(BUILD_TESTS "Build tests of logo runner" OFF)
option(BUILD_LOGO_DOCS "Build the docs" OFF)
option(BUILD_FUZZ "Build fuzzed executable" OFF)
option(BUILD_LOGO_GUI "Build Gui executable" OFF)

RAGEL_TARGET(logo2 ${CMAKE_CURRENT_SOURCE_DIR}/lib/logo/language/lexer/lexer2.rl
             ${CMAKE_SOURCE_DIR}/lib/logo/language/lexer/lexer2.cpp)
add_library(logo STATIC ${RAGEL_logo2_OUTPUTS})
if(BUILD_LOGO_GUI)
  add_subdirectory("extern/SFML")
  add_executable(logoaway)
  target_include_directories(logoaway PRIVATE ${CMAKE_SOURCE_DIR}/extern/SFML/include)
  target_link_libraries(logoaway sfml-graphics sfml-window logo)
  add_subdirectory(gui)
  target_include_directories(logoaway
    PRIVATE extern/SFML/include
            ${CMAKE_SOURCE_DIR}/lib
  )
endif()
if(BUILD_LOGO_DOCS)
  CMAKE_DEPENDENT_OPTION(BUILD_LATEX_DOCS "Build latex output" OFF
                         "BUILD_LOGO_DOCS" off)
  if(BUILD_LATEX_DOCS)
    set(DOXYGEN_GENERATE_LATEX ON)
  else()
    set(DOXYGEN_GENERATE_LATEX OFF)
  endif()
  find_package(Doxygen REQUIRED dot)
  doxygen_add_docs(logo_docs ${PROJECT_SOURCE_DIR}/lib ${PROJECT_SOURCE_DIR}/doc)
endif()
target_link_libraries(logo)



add_subdirectory(lib/logo)

if(BUILD_FUZZ)
  add_executable(logofuzz)
  target_link_libraries(logofuzz PRIVATE logo)
  target_include_directories(logofuzz PRIVATE ${CMAKE_SOURCE_DIR}/lib/logo)
  add_subdirectory(tests/fuzzing)
  add_compile_options("-fstack-protector-strong")
endif()
if(BUILD_TESTS)
  add_subdirectory(tests)
endif()