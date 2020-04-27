#pragma once
#include "../ast.hpp"
#ifdef PARSER_DEBUGGING
#include <iostream>
#endif
/**
 * \file
 * This is just here to make it easy to make lots of files and then
 * not need to repeat the same code
 */
using namespace logo;
using namespace logo::language;
using namespace logo::language::parser;
using namespace logo::language::tokens;
#ifdef PARSER_DEBUGGING
#define FAIL                                                                   \
  {                                                                            \
    std::cerr << __FILE__ << " " << __LINE__ << " "                            \
              << TokenToString(start->type) << std::endl;                      \
    return std::nullopt;                                                       \
  }
#else
#define FAIL return std::nullopt
#endif
