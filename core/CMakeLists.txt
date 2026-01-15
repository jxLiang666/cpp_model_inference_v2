# core CMakeLists.txt
function(auto_add_sources_and_includes TARGET DIR)
    # 递归收集源文件
    file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS
        ${DIR}/*.cpp
        ${DIR}/*.c
    )

    # 把源文件加到 target
    target_sources(${TARGET} PRIVATE ${SRC_FILES})

    # 递归收集所有子目录
    file(GLOB CHILD_DIRS LIST_DIRECTORIES true ${DIR}/*)
    foreach(subdir ${CHILD_DIRS})
        if(IS_DIRECTORY ${subdir})
            target_include_directories(${TARGET} PUBLIC ${subdir})
        endif()
    endforeach()
endfunction()

add_library(nerual_net SHARED)  # 先创建空库

auto_add_sources_and_includes(nerual_net ${CMAKE_CURRENT_SOURCE_DIR})

set(Deps
onnxruntime
opencv
spdlog
)

target_link_libraries(nerual_net PUBLIC ${Deps})

target_compile_definitions(nerual_net PUBLIC 
    ENABLE_LOG=1
)
