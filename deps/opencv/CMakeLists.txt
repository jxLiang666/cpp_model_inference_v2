# 手动指定OpenCV的安装路径
set(OpenCV_DIR "${CMAKE_CURRENT_SOURCE_DIR}/lib/cmake/opencv4")

find_package(OpenCV REQUIRED PATHS ${OpenCV_DIR})

message(STATUS "OpenCV版本: ${OpenCV_VERSION}")
message(STATUS "OpenCV头文件路径: ${OpenCV_INCLUDE_DIRS}")
message(STATUS "OpenCV库文件: ${OpenCV_LIBRARIES}")

add_library(opencv INTERFACE)
target_link_libraries(opencv INTERFACE ${OpenCV_LIBRARIES})
target_include_directories(opencv INTERFACE ${OpenCV_INCLUDE_DIRS})