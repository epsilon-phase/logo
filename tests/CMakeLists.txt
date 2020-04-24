add_executable(logo_test main.cpp turtle.cpp lexer.cpp tokens.cpp)
target_link_libraries(logo_test logo)
target_include_directories(logo_test PRIVATE ${CMAKE_SOURCE_DIR}/lib)
