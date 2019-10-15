#ifndef ASR_UTIL_LOGGER_ILOGGER_H
#define ASR_UTIL_LOGGER_ILOGGER_H

#include "asr/util/Logger.h"

namespace asr
{
namespace util
{
namespace logger
{
/// ログ出力の詳細を担うインタフェース
struct ILogger
{
    ILogger() = default;
    ILogger(const ILogger&) = delete;
    ILogger(ILogger&&) = delete;

    virtual ~ILogger() = default;

    ILogger& operator=(const ILogger&) = delete;
    ILogger& operator=(ILogger&&) & = delete;

    /// @brief ログ出力
    /// @param [in] the_log_level 本ログのログレベル
    /// @param [in] the_log_message 出力したい文字列
    /// @note 派生クラスで要定義
    virtual void output(const Logger::Level the_log_level,
                        const std::string& the_log_message) = 0;
};
}  // namespace logger
}  // namespace util
}  // namespace asr

#endif  // ASR_UTIL_LOGGER_ILOGGER_H
