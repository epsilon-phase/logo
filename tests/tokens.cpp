#include "logo/language/lexer/tokens.hpp"
#include "catch2.hpp"
TEST_CASE("Token names are accurate","lexer"){
    using namespace logo::language::tokens;
    REQUIRE("Unknown"==TokenToString(Unknown));
}

