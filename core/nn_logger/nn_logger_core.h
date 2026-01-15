#pragma once
#include <string>
#include <iostream>
#include <vector>
#include <filesystem>
#include <unordered_map>
#include "spdlog/spdlog.h"
#include "spdlog/async.h"
#include "spdlog/sinks/rotating_file_sink.h"
#include "spdlog/fmt/bundled/printf.h"
namespace nn {
namespace log {
    namespace log_core {
        template < class loggerPtr, class... Args >
        void logPrintFmt(loggerPtr                 logger,
                         spdlog::level::level_enum level,
                         const char               *fmt,
                         Args &&...args) {
            try {
                if (logger && logger->should_log(level)) {
                    logger->log(level, "{}", fmt::sprintf(fmt, std::forward< Args >(args)...));
                }
            } catch (const std::exception &e) {
                std::cerr << __func__ << "invoke: " << "try logger->log() then catch error, which is " << e.what() << std::endl;
            } catch (...) {
                std::cerr << __func__ << "invoke: " << "try logger->log() then catch unknown error" << std::endl;
            }
        }
#define SPDLOG_LOGGER_PRINTF(logger, level, ...) logPrintFmt(logger, level, ##__VA_ARGS__)
        class Logger {
        public:
            ~Logger() = default;
            static std::shared_ptr< spdlog::logger > getLogger(const std::string &_logger_name);
            static void addFilesink(const std::shared_ptr< spdlog::logger > &_logger, const std::string &_filename, size_t _max_file_size, size_t _max_files);
            
        private:
            Logger() = delete;
            Logger(Logger &&) = delete;
            Logger(const Logger &) = delete;
            Logger &operator=(Logger &&) = delete;
            Logger &operator=(const Logger &) = delete;

            struct LoggerHelper {
                LoggerHelper() {
                    spdlog::init_thread_pool(1024, 2);
                    spdlog::flush_every(std::chrono::seconds(1));
                }
                ~LoggerHelper() {
                    spdlog::shutdown();
                }
            };

            static std::shared_ptr< spdlog::logger > createLogger(const std::string &_logger_name);

            

        private:
            static std::unordered_map< std::string, std::shared_ptr< spdlog::logger > > logger_umap_;
            static std::mutex                                                           logger_mutex_;
        };
    }  // namespace log_core
}  // namespace log
}  // namespace nn