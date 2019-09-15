#ifndef ASR_UTIL_LOGGER_SPDLOG_H
#define ASR_UTIL_LOGGER_SPDLOG_H

#include "asr/util/logger/ILoggingImpl.h"

#include <spdlog/spdlog.h>

namespace asr
{
namespace util
{
namespace logger
{
/// @brief spdlogを用いてログ出力する
/// @details 色付きstderr出力 および syslog出力を行なう。
/// syslogにはwarnレベル以上のログのみ出力する。
/// stderrには、デバッグ版ではdebugレベル以上、リリース版ではinfoレベル以上のログを出力する。
class Spdlog : public ILoggingImpl
{
    std::shared_ptr<spdlog::logger> m_old_logger;

public:
    Spdlog() = delete;

    /// @param [in] the_category_name syslog出力時のカテゴリ名称
    explicit Spdlog(const std::string& the_category_name);

    Spdlog(const Spdlog&) = delete;

    Spdlog(Spdlog&&) = delete;

    ~Spdlog() override;

    Spdlog& operator=(const Spdlog&) = delete;
    Spdlog& operator=(Spdlog&&) & = delete;

    /// @brief ログ出力
    /// @param [in] the_log_level 本ログのログレベル
    /// @param [in] the_log_message 出力したい文字列
    void output(const Logger::Level the_log_level,
                const std::string& the_log_message)
        override;
};
}  // namespace logger
}  // namespace util
}  // namespace asr

#endif  // ASR_UTIL_LOGGER_SPDLOG_H
