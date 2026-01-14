# config CMakeLists.txt
get_filename_component(dir_name ${CMAKE_CURRENT_SOURCE_DIR} NAME) # dir_name是文件夹名字即config
add_library(${dir_name} INTERFACE)
target_include_directories(${dir_name} INTERFACE ${CMAKE_BINARY_DIR}/config)

include(${CMAKE_SOURCE_DIR}/cmake/AddAllConfig.cmake)
add_all_config("${CMAKE_CURRENT_SOURCE_DIR}" "${CMAKE_BINARY_DIR}/${dir_name}")