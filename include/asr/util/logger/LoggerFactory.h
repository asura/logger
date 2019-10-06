#ifndef ASR_UTIL_LOGGER_LOGGERFACTORY_H
#define ASR_UTIL_LOGGER_LOGGERFACTORY_H

#include "asr/util/logger/Config.h"
#include "asr/util/logger/ILogger.h"

#include <memory>

namespace asr
{
namespace util
{
namespace logger
{
struct LoggerFactory
{
    static std::unique_ptr<ILogger> generate(const Config& the_config);
};
}  // namespace logger
}  // namespace util
}  // namespace asr

#endif  // ASR_UTIL_LOGGER_LOGGERFACTORY_H
