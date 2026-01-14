# deps/onnxruntime/CMakeLists.txt
# 手动指定onnxruntime的安装路径
set(onnxruntime_DIR "${CMAKE_CURRENT_SOURCE_DIR}/onnxruntime-linux-x64-1.22.0/lib/cmake/onnxruntime")

# 查找onnxruntime库
find_package(onnxruntime REQUIRED PATHS ${onnxruntime_DIR})
if(NOT TARGET onnxruntime::onnxruntime)
    message(FATAL_ERROR "can not find onnxruntime::onnxruntime target, please check ONNX Runtime config!")
else()
    message(STATUS "onnxruntime version: ${onnxruntime_VERSION}")
endif()

add_library(onnxruntime INTERFACE)
target_link_libraries(onnxruntime INTERFACE onnxruntime::onnxruntime)