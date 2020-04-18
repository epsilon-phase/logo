add_executable(logo_test main.cpp turtle.cpp)
target_link_libraries(logo_test logo)
target_include_directories(logo_test PRIVATE ${CMAKE_SOURCE_DIR}/extern/SFML/include ${CMAKE_SOURCE_DIR}/lib)
