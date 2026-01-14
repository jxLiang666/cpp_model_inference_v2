CMAKE_MINIMUM_REQUIRED(VERSION 3.16)

PROJECT("cpp_model_inference_v2" 
VERSION 0.0.1
LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17) # cpp标准
set(CMAKE_CXX_STANDARD_REQUIRED ON)  # 强制cpp标准
set(CMAKE_CXX_EXTENSIONS OFF) # 不使用GNU扩展
set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # 在build目录下生成一个 compile_commands.json 文件，记录所有编译命令
# set(CMAKE_BUILD_TYPE Debug) # 设置为Debug模式
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)   # 设置静态库全局输出目录 .a / .lib
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)   # 设置动态库全局输出目录 .so / .dll
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)   # 设置可执行文件全局输出目录 .exe / ELF

option(CPP_TMPL_EX "Build examples" ${EXAMPLES}) # 是否编译 examples
option(CPP_TMPL_TESTS "Build tests" ${TESTS}) # 是否编译 tests [确保googletest 是ON的状态才行]

add_subdirectory(./config)
add_subdirectory(./deps)
add_subdirectory(./src)

if (${CPP_TMPL_TESTS})
    enable_testing()
    include(GoogleTest)
    add_subdirectory(./tests)
else()
    message(STATUS "Skipping tests module")
endif() # if (${CPP_TMPL_TESTS})

if (${CPP_TMPL_EX})
    add_subdirectory(./examples)
else()
    message(STATUS "Skipping examples module")
endif() # if (${CPP_TMPL_EX})

