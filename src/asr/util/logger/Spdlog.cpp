#include "asr/util/logger/Spdlog.h"

#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/syslog_sink.h>

namespace asr
{
namespace util
{
namespace logger
{
Spdlog::Spdlog(
    const std::string& the_category_name)
    : m_old_logger(spdlog::default_logger())
{
    auto console_sink = std::make_shared<spdlog::sinks::stderr_color_sink_mt>();
#ifndef NDEBUG
    console_sink->set_level(spdlog::level::debug);
#else
    console_sink->set_level(spdlog::level::info);
#endif
    // ログレベル(1文字)、日時、メッセージ
    console_sink->set_pattern("%L, [%Y-%m-%d %H:%M:%S.%f] %^%v%$");

    auto syslog_sink = std::make_shared<spdlog::sinks::syslog_sink_mt>(the_category_name);
    syslog_sink->set_level(spdlog::level::warn);

    spdlog::set_default_logger(
        std::make_shared<spdlog::logger>(
            "multi_sink",
            spdlog::sinks_init_list(
                {console_sink,
                 syslog_sink})));
}

Spdlog::~Spdlog()
{
    spdlog::set_default_logger(m_old_logger);
}

void Spdlog::output(
    const Logger::Level the_log_level,
    const std::string& the_log_message)
{
    switch (the_log_level)
    {
    case Logger::Level::TRC:
        spdlog::trace(the_log_message);
        break;
    case Logger::Level::DBG:
        spdlog::debug(the_log_message);
        break;
    case Logger::Level::INF:
        spdlog::info(the_log_message);
        break;
    case Logger::Level::WRN:
        spdlog::warn(the_log_message);
        break;
    case Logger::Level::ERR:
        spdlog::error(the_log_message);
        break;
    case Logger::Level::CRT:
        spdlog::critical(the_log_message);
        break;
    }
}
}  // namespace logger
}  // namespace util
}  // namespace asr
