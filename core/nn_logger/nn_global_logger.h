#pragma once
#include "nn_logger.h"
#if defined(ENABLE_LOG) && ENABLE_LOG != 0
// nn_logger的唯一名字，通过这里修改
using namespace nn::log;
#define NN_LOGGER_NAME "NERUAL_NET"
#define NN_TRACE(fmt, ...) L_TRACE(NN_LOGGER_NAME, fmt, ##__VA_ARGS__)
#define NN_DEBUG(fmt, ...) L_DEBUG(NN_LOGGER_NAME, fmt, ##__VA_ARGS__)
#define NN_INFO(fmt, ...) L_INFO(NN_LOGGER_NAME, fmt, ##__VA_ARGS__)
#define NN_WARN(fmt, ...) L_WARN(NN_LOGGER_NAME, fmt, ##__VA_ARGS__)
#define NN_ERROR(fmt, ...) L_ERROR(NN_LOGGER_NAME, fmt, ##__VA_ARGS__)
#define NN_CRITICAL(fmt, ...) L_CRITICAL(NN_LOGGER_NAME, fmt, ##__VA_ARGS__)
#define NN_CLOSE() L_CLOSE(NN_LOGGER_NAME)
#define NN_OPEN(l) L_OPEN(NN_LOGGER_NAME, l)
#define NN_OPEN_DEFAULT() L_OPEN_DEFAULT(NN_LOGGER_NAME)
#define NN_SET_LEVEL(l) L_SET_LEVEL(NN_LOGGER_NAME, l)
#define NN_ADD_FILESINK(filename, max_file_size, max_files) L_ADD_FILESINK(NN_LOGGER_NAME, filename, max_file_size, max_files)
#else
#define NN_TRACE(...)
#define NN_DEBUG(...)
#define NN_INFO(...)
#define NN_WARN(...)
#define NN_ERROR(...)
#define NN_CRITICAL(...)
#define NN_CLOSE(...)
#define NN_OPEN(...)
#define NN_OPEN_DEFAULT(...)
#define NN_SET_LEVEL(...)
#define NN_ADD_FILESINK(...)
#endif

// #ifndef ENABLE_LOG_CHECKED
// #define ENABLE_LOG_CHECKED
// #define STRINGIFY(x) #x
// #define TOSTRING(x) STRINGIFY(x)
// #if defined(ENABLE_LOG)
//     #pragma message("========== ENABLE_LOG STATUS ==========")
//     #pragma message("ENABLE_LOG is defined")
//     #pragma message("ENABLE_LOG value = " TOSTRING(ENABLE_LOG))
//     #pragma message("=====================================")
// #else
//     #pragma message("========== ENABLE_LOG STATUS ==========")
//     #pragma message("ENABLE_LOG is NOT defined")
//     #pragma message("=====================================")
// #endif
// #endif // ENABLE_LOG_CHECKED

