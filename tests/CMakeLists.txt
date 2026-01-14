# test CMakeLists.txt
# 导入add_all_subdir_executables函数
include(${CMAKE_SOURCE_DIR}/cmake/AddAllSubdirectory.cmake)

# 为 tests 下的所有目录执行add_subdirectory
add_all_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}")
