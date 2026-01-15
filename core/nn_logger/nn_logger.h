#pragma once
#include <algorithm>
#include "nn_logger_core.h"
namespace nn {
namespace log {
#if defined(ENABLE_LOG) && ENABLE_LOG != 0
/**
 * @brief 输出 TRACE 级别日志，自动包含文件名和行号
 * @param name logger名字
 * @param fmt 格式化字符串（支持 printf 风格，如 "%d, %s"）
 * @param ... 可变参数，与 fmt 中的占位符对应（如 int, const char*）
 * @example LTRACE("用户 %s 访问了接口，耗时 %d ms", "Alice", 42);
 */
#define L_TRACE(name, fmt, ...)                            \
    log_core::SPDLOG_LOGGER_PRINTF(log_core::Logger::getLogger(name), \
                         spdlog::level::trace,             \
                         "%s(%d): " fmt "",                \
                         __FILE__,                         \
                         __LINE__,                         \
                         ##__VA_ARGS__)
/**
 * @brief 输出 DEBUG 级别日志，自动包含文件名和行号
 * @param name logger名字
 * @param fmt 格式化字符串（支持 printf 风格，如 "%d, %s"）
 * @param ... 可变参数，与 fmt 中的占位符对应（如 int, const char*）
 */
#define L_DEBUG(name, fmt, ...)                            \
    log_core::SPDLOG_LOGGER_PRINTF(log_core::Logger::getLogger(name), \
                         spdlog::level::debug,             \
                         "%s(%d): " fmt "",                \
                         __FILE__,                         \
                         __LINE__,                         \
                         ##__VA_ARGS__)
/**
 * @brief 输出 INFO 级别日志，自动包含文件名和行号
 * @param name logger名字
 * @param fmt 格式化字符串（支持 printf 风格，如 "%d, %s"）
 * @param ... 可变参数，与 fmt 中的占位符对应（如 int, const char*）
 */
#define L_INFO(name, fmt, ...)                             \
    log_core::SPDLOG_LOGGER_PRINTF(log_core::Logger::getLogger(name), \
                         spdlog::level::info,              \
                         "%s(%d): " fmt "",                \
                         __FILE__,                         \
                         __LINE__,                         \
                         ##__VA_ARGS__)
/**
 * @brief 输出 WARN 级别日志，自动包含文件名和行号
 * @param name logger名字
 * @param fmt 格式化字符串（支持 printf 风格，如 "%d, %s"）
 * @param ... 可变参数，与 fmt 中的占位符对应（如 int, const char*）
 */
#define L_WARN(name, fmt, ...)                             \
    log_core::SPDLOG_LOGGER_PRINTF(log_core::Logger::getLogger(name), \
                         spdlog::level::warn,              \
                         "%s(%d): " fmt "",                \
                         __FILE__,                         \
                         __LINE__,                         \
                         ##__VA_ARGS__)
/**
 * @brief 输出 ERROR 级别日志，自动包含文件名和行号
 * @param name logger名字
 * @param fmt 格式化字符串（支持 printf 风格，如 "%d, %s"）
 * @param ... 可变参数，与 fmt 中的占位符对应（如 int, const char*）
 */
#define L_ERROR(name, fmt, ...)                            \
    log_core::SPDLOG_LOGGER_PRINTF(log_core::Logger::getLogger(name), \
                         spdlog::level::err,               \
                         "%s(%d): " fmt "",                \
                         __FILE__,                         \
                         __LINE__,                         \
                         ##__VA_ARGS__)
/**
 * @brief 输出 CRITICAL 级别日志，自动包含文件名和行号
 * @param name logger名字
 * @param fmt 格式化字符串（支持 printf 风格，如 "%d, %s"）
 * @param ... 可变参数，与 fmt 中的占位符对应（如 int, const char*）
 */
#define L_CRITICAL(name, fmt, ...)                         \
    log_core::SPDLOG_LOGGER_PRINTF(log_core::Logger::getLogger(name), \
                         spdlog::level::critical,          \
                         "%s(%d): " fmt "",                \
                         __FILE__,                         \
                         __LINE__,                         \
                         ##__VA_ARGS__)
/**
 * @brief 关闭日志输出
 * @param name logger名字
 */
#define L_CLOSE(name) log_core::Logger::getLogger(name)->set_level(spdlog::level::off);

/**
 * @brief 开启并设置日志输出功能及等级
 *
 * 该函数用于启用日志系统并设置输出等级，只有等级大于等于设定值的日志才会被输出。
 * 若日志系统未初始化，此函数会自动初始化默认日志配置（控制台输出）。
 *
 * @param name logger名字
 * @param l 日志等级（范围：0~5，超出范围会被自动截断到有效区间）
 *              0: TRACE - 最详细的调试信息，包含程序执行的每一步细节（仅开发环境使用）
 *              1: DEBUG - 调试信息，用于跟踪程序流程和变量状态（开发/测试环境）
 *              2: INFO  - 常规运行信息，记录程序正常执行的关键节点（生产环境默认）
 *              3: WARN  - 警告信息，标识可能存在的异常情况但不影响程序继续运行
 *              4: ERROR - 错误信息，标识功能执行失败但程序可继续运行
 *              5: CRITICAL - 严重错误信息，标识可能导致程序崩溃或数据丢失的致命问题
 *
 *
 * @note 1. 若传入的等级超出[0,5]范围，会被自动调整为最近的边界值（0或5）
 *       2. 调用此函数前若日志已处于开启状态，会覆盖之前的等级设置
 *       3. 日志等级越高，输出的日志数量越少（CRITICAL仅输出最严重的错误）
 *
 *
 * @example
 *       // 开启INFO级别日志（输出INFO及更高级别的日志）
 *       enable_log(2);
 *
 *       // 开启DEBUG级别日志（输出DEBUG、INFO、WARN、ERROR、CRITICAL）
 *       enable_log(1);
 */
#define L_OPEN(name, l) L_SET_LEVEL(name, l)


/**
 * @brief 开启日志输出
 * @param name logger名字
 * @note 默认日志等级0，即TRACE
 *
 */
#define L_OPEN_DEFAULT(name) L_OPEN(name, 0)
/**
 * @brief 设置日志输出级别
 *
 * 该宏用于安全地设置日志系统的输出级别，仅当日志器实例存在时才执行操作。
 * 会自动将输入级别限制在有效范围内，避免设置无效值导致的异常。
 *
 * @param name logger名字
 * @param l 期望设置的日志级别（整数）
 *          有效范围：0 ~ spdlog::level::off对应的整数值
 *          对应关系通常为：
 *              0: trace   - 最详细的调试信息
 *              1: debug   - 调试信息
 *              2: info    - 一般运行信息
 *              3: warn    - 警告信息
 *              4: error   - 错误信息
 *              5: critical- 严重错误信息
 *              6: off     - 关闭所有日志输出（默认上限值）
 *
 * @note 1. 内部使用std::clamp进行边界处理，超出范围时会自动截断到最近的有效边界
 *       2. 会先检查日志器实例是否存在，避免空指针操作
 *       3. 级别值越高，输出的日志数量越少（off级别无任何输出）
 *       4. 依赖spdlog库的level_enum枚举类型，确保编译环境中已正确包含相关头文件
 *
 * @example
 *       // 设置为INFO级别（只输出info及更高级别的日志）
 *       L_SET_LEVEL(2);
 *
 *       // 传入超出范围的值，会被自动截断为off级别
 *       L_SET_LEVEL(10);  // 等效于设置为spdlog::level::off
 */
#define L_SET_LEVEL(name, l) log_core::Logger::getLogger(name)->set_level(static_cast< spdlog::level::level_enum >(std::clamp(l, 0, static_cast< int >(spdlog::level::off))));

/**
 * @brief 对日志增加输入到文件中去
 * @param name logger名字
 * @param filename 存储文件路径
 * @param max_file_size 文件可存储的大小
 * @param max_files 文件数量
 * @example
 *       L_ADD_FILESINK("NN", ./.log, 5*1024*1024, 0)
 */
#define L_ADD_FILESINK(name, filename, max_file_size, max_files) \
    log_core::Logger::addFilesink(log_core::Logger::getLogger(name), filename, max_file_size, max_files)

#else
#define L_TRACE(...)
#define L_DEBUG(...)
#define L_INFO(...)
#define L_WARN(...)
#define L_ERROR(...)
#define L_CRITICAL(...)
#define L_CLOSE(...)
#define L_OPEN(...)
#define L_OPEN_DEFAULT(...)
#define L_SET_LEVEL(...)
#define L_ADD_FILESINK(...)
#endif
}  // namespace log
}  // namespace nn