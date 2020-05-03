add_executable(logo_test main.cpp turtle.cpp lexer.cpp tokens.cpp lexer2.cpp parser.cpp vm.cpp)
if(BUILD_BENCHMARKS)
  target_compile_definitions(logo_test PRIVATE CATCH_CONFIG_ENABLE_BENCHMARKING=1)
endif()
target_link_libraries(logo_test logo)
target_include_directories(logo_test PRIVATE ${CMAKE_SOURCE_DIR}/lib)
add_test(logo_test ${CMAKE_CURRENT_BINARY_DIR}/logo_test)
add_executable(parse_tester parsetester.cpp)
target_link_libraries(parse_tester logo)
target_include_directories(parse_tester PRIVATE ${CMAKE_SOURCE_DIR}/lib)
if(BUILD_WITH_COVERAGE)
  setup_target_for_coverage_gcovr_html(
          NAME coverage                 
          EXECUTABLE ctest -j ${n_cores} # Executable in PROJECT_BINARY_DIR
          DEPENDENCIES
              logo_test)
  message("Building with coverage!!")
endif()
if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
  target_compile_options(logo_test PUBLIC "-fno-omit-frame-pointer" "-fsanitize=address")
  target_link_libraries(logo_test "-fno-omit-frame-pointer" "-fsanitize=address")
  target_link_libraries(parse_tester "-fno-omit-frame-pointer" "-fsanitize=address")
endif()