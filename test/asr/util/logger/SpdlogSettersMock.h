#ifndef ASR_UTIL_LOGGER_SPDLOGSETTERSMOCK_H
#define ASR_UTIL_LOGGER_SPDLOGSETTERSMOCK_H

#include "asr/util/logger/Spdlog.h"

#include <gmock/gmock.h>

namespace asr
{
namespace util
{
namespace logger
{
class SpdlogSettersMock : public Spdlog::Setters
{
public:
    MOCK_METHOD2(set_level, void(spdlog::sinks::sink&, const std::string&));
    MOCK_METHOD2(set_pattern, void(spdlog::sinks::sink&, const std::string&));
};
}  // namespace logger
}  // namespace util
}  // namespace asr

#endif  // ASR_UTIL_LOGGER_SPDLOGSETTERSMOCK_H
