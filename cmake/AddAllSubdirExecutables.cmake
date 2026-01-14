# 文件名: AddAllSubdirExecutables.cmake
if(NOT DEFINED __ADDALLSUBDIREXECUTABLESCMAKE__)
    set(__ADDALLSUBDIREXECUTABLESCMAKE__ TRUE)
# 函数名字: add_all_subdir_executables
# 变量: SRC_DIR
# 使用: add_all_subdir_executables("${CMAKE_CURRENT_SOURCE_DIR}")
# 作用: 为当前路径下的所有cpp文件创建可执行文件
function(add_all_subdir_executables SRC_DIR)
    file(GLOB_RECURSE all_srcs CONFIGURE_DEPENDS "${SRC_DIR}/*.cpp")

    string(REPEAT "#" 50 separator_line) # 定义分隔符长度
    string(REPEAT "*" 50 inner_line) # 定义内部分隔符长度
    message(STATUS "${separator_line}")
    message(STATUS "📌 Current CMakeLists path : " ${SRC_DIR}/CMakeLists.txt)
    if(NOT all_srcs)
        message(WARNING "⚠️ No source files found in ${SRC_DIR}!")
        return()
    endif()
    file(RELATIVE_PATH src_rel_dir ${CMAKE_SOURCE_DIR} ${SRC_DIR}) # 相较于根目录的文件夹名字
    foreach(v ${all_srcs})
        # 得到相对路径，相对于 SRC_DIR
        file(RELATIVE_PATH relative_path ${SRC_DIR} ${v}) 

        # target 名：文件名（不带扩展名）
        get_filename_component(target_name ${relative_path} NAME_WE) # NAME带后缀 NAME_WE不带后缀

        # 添加可执行文件
        add_executable(${target_name} ${v})

        # 输出目录：保持原有的子目录结构
        get_filename_component(subdir ${relative_path} DIRECTORY)
        set(output_dir ${CMAKE_BINARY_DIR}/${src_rel_dir}/${subdir})
        set_target_properties(${target_name} PROPERTIES    
            RUNTIME_OUTPUT_DIRECTORY ${output_dir}
            OUTPUT_NAME ${target_name}
        )

        # 打印信息
        message(STATUS "${inner_line}")
        message(STATUS "📂 Relative path           : " ${relative_path})
        message(STATUS "🎯 Target name             : " ${target_name})
        message(STATUS "📁 Output directory        : " ${output_dir})
        message(STATUS "🚀 Executable path         : " ${output_dir}${target_name})
        message(STATUS "${inner_line}")
    endforeach()
    message(STATUS "${separator_line}")
endfunction()
endif()
