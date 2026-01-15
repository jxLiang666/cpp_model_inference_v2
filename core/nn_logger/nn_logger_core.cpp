#include "spdlog/spdlog.h"
#include "nn_logger_core.h"
namespace nn {
namespace log {
    namespace log_core {
        ///< logger全局静态变量声明
        std::unordered_map< std::string, std::shared_ptr< spdlog::logger > > Logger::logger_umap_;
        std::mutex                                                           Logger::logger_mutex_;

        std::shared_ptr< spdlog::logger > Logger::getLogger(const std::string &_logger_name) {
            static LoggerHelper helper;
            // std::shared_ptr< spdlog::logger > logger = createLogger(_logger_name);
            // return logger;
            return createLogger(_logger_name);
        }
        std::shared_ptr< spdlog::logger > Logger::createLogger(const std::string &_logger_name) {
            std::lock_guard< std::mutex > lock(logger_mutex_);
            auto                          it = logger_umap_.find(_logger_name);
            if (it != logger_umap_.end()) {
                return it->second;
            } else {
                auto threadPool = spdlog::thread_pool();
                auto console_sink = std::make_shared< spdlog::sinks::ansicolor_stdout_sink_mt >();
                console_sink->set_level(spdlog::level::trace);

                std::vector< spdlog::sink_ptr > sinks{console_sink};

                auto logger = std::make_shared< spdlog::async_logger >(_logger_name,
                                                                       sinks.begin(),
                                                                       sinks.end(),
                                                                       threadPool,
                                                                       spdlog::async_overflow_policy::block);
                spdlog::register_logger(logger);
                logger->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%n] [%l] [%t] %v%$");
                logger->flush_on(spdlog::level::err);
                spdlog::flush_every(std::chrono::seconds(1));
                logger->set_level(spdlog::level::level_enum::trace);
                logger->trace("({}) logger init done", _logger_name);

                logger_umap_.emplace(_logger_name, logger);
                return logger;
            }
        }

        void Logger::addFilesink(const std::shared_ptr< spdlog::logger > &_logger, const std::string &_filename, size_t _max_file_size, size_t _max_files) {
            if (!_logger) return;

            try {
                // 防止重复添加
                for (auto &sink : _logger->sinks()) {
                    auto file_sink = std::dynamic_pointer_cast< spdlog::sinks::rotating_file_sink_mt >(sink);
                    if (file_sink && file_sink->filename() == _filename) {
                        return;
                    }
                }

                std::filesystem::path path(_filename);
                if (path.has_parent_path()) {
                    std::filesystem::create_directories(path.parent_path());
                }

                auto file_sink = std::make_shared< spdlog::sinks::rotating_file_sink_mt >(
                    _filename,       // 日志文件名称
                    _max_file_size,  // 单个文件最大大小 5MB
                    _max_files       // 备份文件数量（0表示不保留备份）
                );
                file_sink->set_level(spdlog::level::trace);
                file_sink->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%n] [%l] [%t] %v%$");

                _logger->sinks().push_back(file_sink);
                _logger->trace("({}) File sink added: {}", _logger->name(), _filename);

            } catch (const spdlog::spdlog_ex &ex) {
                std::cerr << "addFileSink failed: " << ex.what() << std::endl;
            }
        }
    }  // namespace log_core
}  // namespace log
}  // namespace nn