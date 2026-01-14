get_filename_component(dir_name ${CMAKE_CURRENT_SOURCE_DIR} NAME) # dir_name是文件夹名字即 spdlog
add_library(${dir_name} INTERFACE)
target_include_directories(${dir_name} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})