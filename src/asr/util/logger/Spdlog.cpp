#include "asr/util/logger/Spdlog.h"

#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/syslog_sink.h>

#include <sstream>

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

Spdlog::Spdlog(
    const Config::Spdlog& the_config,
    std::shared_ptr<Setters> the_setters)
    : m_setters(the_setters)
{
    if (!the_setters)
    {
        throw std::runtime_error("Setters指定がnullptr");
    }

    const size_t n_sinks = the_config.sinks.size();

    switch (n_sinks)
    {
    case 0:
        throw std::runtime_error("sink未指定のため構築不可");
    case 1:
        spdlog::set_default_logger(
            std::make_shared<spdlog::logger>(
                the_config.name,
                generate_sink(the_config.sinks[0])));
        break;
    default:
        throw std::runtime_error("複数sinkは未対応");
    }

    assert(spdlog::default_logger().get() != nullptr);

    spdlog::default_logger()->set_level(
        m_setters->convert_level(the_config.level));
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
    case Logger::Level::VBS:
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
    case Logger::Level::FTL:
        spdlog::critical(the_log_message);
        break;
    }
}

std::shared_ptr<spdlog::logger>
Spdlog::logger()
{
    return spdlog::default_logger();
}

spdlog::sink_ptr
Spdlog::generate_sink(
    const Config::Spdlog::Sink& the_sink_config)
{
    if (the_sink_config.type == "stderr_color_sink_mt")
    {
        auto result = std::make_shared<spdlog::sinks::stderr_color_sink_mt>();
        assert(result.get() != nullptr);

        m_setters->set_level(*result, the_sink_config.level);

        if (the_sink_config.pattern)
        {
            m_setters->set_pattern(*result, *the_sink_config.pattern);
        }

        return result;
    }

    // 未対応のsink指定
    return nullptr;
}

spdlog::level::level_enum
Spdlog::Setters::convert_level(
    const std::string& the_level)
    const
{
    if (the_level == "trace")
    {
        return spdlog::level::trace;
    }

    if (the_level == "debug")
    {
        return spdlog::level::debug;
    }

    if (the_level == "info")
    {
        return spdlog::level::info;
    }

    if (the_level == "warn")
    {
        return spdlog::level::warn;
    }

    if (the_level == "error")
    {
        return spdlog::level::err;
    }

    if (the_level == "critical")
    {
        return spdlog::level::critical;
    }

    std::ostringstream oss;
    oss << "未対応のログレベル指定("
        << the_level
        << ")";
    throw std::runtime_error(oss.str());
}

void Spdlog::Setters::set_level(
    spdlog::sinks::sink& the_sink,
    const std::string& the_level)
{
    the_sink.set_level(convert_level(the_level));
}

void Spdlog::Setters::set_pattern(
    spdlog::sinks::sink& the_sink,
    const std::string& the_pattern)
{
    the_sink.set_pattern(the_pattern);
}

}  // namespace logger
}  // namespace util
}  // namespace asr
