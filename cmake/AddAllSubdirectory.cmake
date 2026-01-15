# 文件名: AddAllSubdirectory.cmake
if(NOT DEFINED __ADDALLSUBDIRECTORYCMAKE__)
    set(__ADDALLSUBDIRECTORYCMAKE__ TRUE)
# 函数名字: add_all_subdirectory
# 变量: SRC_DIR
# 使用: add_all_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}")
# 作用: 为当前路径下的所有文件夹执行add_subdirectory命令
function(add_all_subdirectory SRC_DIR)
    # 获取 SRC_DIR 下所有内容（文件和文件夹）
    file(GLOB children RELATIVE ${SRC_DIR} ${SRC_DIR}/*)
    string(REPEAT "#" 50 separator_line) # 定义分隔符长度
    message(STATUS "${separator_line}")
    message(STATUS "📌 Current CMakeLists path : " ${SRC_DIR}/CMakeLists.txt)
    foreach(child ${children})
        # 构建完整路径
        set(child_path "${SRC_DIR}/${child}")

        # 判断是否是文件夹
        if(IS_DIRECTORY ${child_path})
            # 检查子目录是否包含 CMakeLists.txt
            get_filename_component(dir_name "${child_path}" NAME)
            string(REGEX REPLACE "[-.]" "_" dir_name "${dir_name}")

            # 动态生成 option
            set(default_value ON)  # 默认开启
            option(${dir_name} "Add ${dir_name} or not" ${default_value})

            # 打印结果
            message(STATUS "${dir_name} = ${${dir_name}}")
            if(EXISTS "${child_path}/CMakeLists.txt")
                if(${dir_name})
                message(STATUS "📂 Adding subdirectory     : ${child_path}")
                # message(STATUS "📂 Subdirectory dir name   : ${dir_name}")
                add_subdirectory(${child_path})
                else()
                    message(STATUS "Skipping directory (${dir_name} OFF): ${child_path}")
                endif()
            else()
                message(WARNING "⚠️ Skipping directory (no CMakeLists.txt found): ${child_path}")
            endif()
        endif()
    endforeach()
    message(STATUS "${separator_line}")
endfunction()
endif()