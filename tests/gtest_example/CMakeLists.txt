set(Deps
gtest_main
)
add_executable(gtest_ex ${CMAKE_CURRENT_SOURCE_DIR}/gtest_ex.cpp)
target_link_libraries(gtest_ex PRIVATE ${Deps})
gtest_discover_tests(gtest_ex)