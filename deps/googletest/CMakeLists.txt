get_filename_component(dir_name ${CMAKE_CURRENT_SOURCE_DIR} NAME)

include(${CMAKE_SOURCE_DIR}/cmake/AddAllSubdirectory.cmake)

# 为 example 下的所有目录执行add_subdirectory
add_all_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}")

# enable_testing()
# gtest_main
# (GoogleTest)
# gtest_discover_tests()