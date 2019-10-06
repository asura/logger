#include "asr/util/Logger.h"

#include "asr/util/logger/Spdlog.h"

#include <sstream>

namespace asr
{
namespace util
{
Logger::Logger(std::unique_ptr<logger::ILogger>&& the_impl)
    : m_impl(std::move(the_impl))
{
    if (!m_impl)
    {
        throw std::runtime_error("LogginImpl was nullptr");
    }
}

void Logger::output(
    const Level the_log_level,
    const std::string& the_file_name,
    const int the_line_number,
    const std::string& the_function_name,
    const std::string& the_message)
{
    std::ostringstream oss;
    oss << the_file_name
        << ","
        << the_line_number
        << ","
        << the_function_name
        << ","
        << the_message;

    m_impl->output(the_log_level, oss.str());
}

Logger& theLogger()
{
    static Logger logger(
        std::unique_ptr<asr::util::logger::ILogger>(
            new asr::util::logger::Spdlog()));
    return logger;
}
}  // namespace util
}  // namespace asr
