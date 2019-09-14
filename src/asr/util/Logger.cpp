#include "asr/util/Logger.h"

#include "asr/util/logger/ILoggingImpl.h"

#include <sstream>

namespace asr
{
namespace util
{
Logger::Logger(std::unique_ptr<logger::ILoggingImpl>&& the_impl)
    : m_impl(std::move(the_impl))
{
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

#if 0
Logger& theLogger()
{
    static Logger logger;
    return logger;
}
#endif
}  // namespace util
}  // namespace asr
