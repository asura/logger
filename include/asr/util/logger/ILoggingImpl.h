#ifndef ASR_UTIL_LOGGER_ILOGGINGIMPL_H
#define ASR_UTIL_LOGGER_ILOGGINGIMPL_H

#include "asr/util/Logger.h"

namespace asr
{
namespace util
{
namespace logger
{
/// ログ出力の詳細を担うインタフェース
struct ILoggingImpl
{
    ILoggingImpl() = default;
    ILoggingImpl(const ILoggingImpl&) = delete;
    ILoggingImpl(ILoggingImpl&&) = delete;

    virtual ~ILoggingImpl() = default;

    ILoggingImpl& operator=(const ILoggingImpl&) = delete;
    ILoggingImpl& operator=(ILoggingImpl&&) & = delete;

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

#endif  // ASR_UTIL_LOGGER_ILOGGINGIMPL_H
