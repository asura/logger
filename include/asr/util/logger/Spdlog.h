#ifndef ASR_UTIL_LOGGER_SPDLOG_H
#define ASR_UTIL_LOGGER_SPDLOG_H

#include "asr/util/logger/Config.h"
#include "asr/util/logger/ILogger.h"

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
class Spdlog : public ILogger
{
public:
    struct Setters
    {
        virtual ~Setters() = default;

        spdlog::level::level_enum convert_level(const std::string& the_level) const;

        /// @note モック化したいのでvirtualとする
        virtual void set_level(spdlog::sinks::sink& the_sink, const std::string& the_level);

        /// @note モック化したいのでvirtualとする
        virtual void set_pattern(spdlog::sinks::sink& the_sink, const std::string& the_pattern);
    };

private:
    std::shared_ptr<spdlog::logger> m_old_logger;
    std::shared_ptr<Setters> m_setters;

public:
    Spdlog();

    /// @param [in] the_config 構築用設定
    explicit Spdlog(const Config::Spdlog& the_config, std::shared_ptr<Setters> the_setters = std::make_shared<Setters>());

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

    std::shared_ptr<spdlog::logger> logger();

private:
    spdlog::sink_ptr generate_sink(const Config::Spdlog::Sink& the_sink_config);
};
}  // namespace logger
}  // namespace util
}  // namespace asr

#endif  // ASR_UTIL_LOGGER_SPDLOG_H
