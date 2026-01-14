# 文件名: AddAllConfig.cmake
if(NOT DEFINED __ADDALLCONFIGCMAKE__)
    set(__ADDALLCONFIGCMAKE__ TRUE)
# 函数名字: add_all_config
# 变量: INPUT_DIR OUTPUT_DIR
# 使用: add_all_config("${CMAKE_CURRENT_SOURCE_DIR}","${CMAKE_BINARY_DIR}/${dir_name}")
# 作用: 为当前路径下的所有配置文件执行configure_file命令
function(add_all_config INPUT_DIR OUTPUT_DIR)
    # 扫描所有 .in 文件
    file(GLOB IN_FILES "${INPUT_DIR}/*.in")

    foreach(IN_FILE ${IN_FILES})
        # 提取文件名（带扩展名）
        get_filename_component(FILENAME ${IN_FILE} NAME)

        # 如果以 ".in" 结尾，去掉它
        string(REGEX REPLACE "\\.in$" "" OUT_BASE_NAME "${FILENAME}")
        # 输出文件路径
        set(OUT_FILE "${OUTPUT_DIR}/${OUT_BASE_NAME}")

        # 调用 configure_file
        configure_file(
            "${IN_FILE}"
            "${OUT_FILE}"
            ESCAPE_QUOTES
        )
        message(STATUS "⚙️ Config                  : " ${IN_FILE})
        message(STATUS "📁 Save                    : " ${OUT_FILE})
    endforeach()
endfunction()

endif()