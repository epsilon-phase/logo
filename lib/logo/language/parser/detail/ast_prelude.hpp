#pragma once
#include "../ast.hpp"
/**
 * \file
 * This is just here to make it easy to make lots of files and then
 * not need to repeat the same code
 */
using namespace logo;
using namespace logo::language;
using namespace logo::language::parser;
using namespace logo::language::tokens;
#define FAIL return std::nullopt