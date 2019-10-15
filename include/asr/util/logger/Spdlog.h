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
/**
 * @brief spdlogを用いてログ出力する
 * @details デフォルト構築では色付きstderr出力を行なう。
 * @details 設定を指定した構築も可能。
 */
class Spdlog : public ILogger
{
public:
    /**
     * @brief ログレベル・出力パターンをspdlogに設定するファンクタ
     * @note spdlogは設定後に出力パターンの内容を確認できない。単体テストで挙動を確認するため、ファンクタとして取り出し、モックとして使えるようにする。
     */
    struct Setters
    {
        virtual ~Setters() = default;

        /**
         * @brief ログレベル文字列を spdlog のログレベル enum に変換する
         * 
         * @param [in] the_level ログレベル文字列
         * @return 変換結果の enum 値
         */
        spdlog::level::level_enum convert_level(const std::string& the_level) const;

        /**
         * @brief ログレベルを設定する
         * 
         * @param the_sink 設定対象の spdlog::sinks::sink
         * @param the_level 設定したいログレベル文字列
         * @note モック化したいのでvirtualとする
         */
        virtual void set_level(spdlog::sinks::sink& the_sink, const std::string& the_level);

        /**
         * @brief ログ出力パターンを設定する
         * 
         * @param the_sink 設定対象の spdlog::sinks::sink
         * @param the_level 設定したいログ出力パターン文字列
         * @note モック化したいのでvirtualとする
         */
        virtual void set_pattern(spdlog::sinks::sink& the_sink, const std::string& the_pattern);
    };

private:
    std::shared_ptr<spdlog::logger> m_old_logger;  ///< 構築前のデフォルトロガーを保持しておく
    std::shared_ptr<Setters> m_setters;            ///< 設定指定構築時に指定された Setters

public:
    /**
     * @brief デフォルト構築
     * @details 色付きstderr出力を行なう。
     * @details デバッグ版ではdebugレベル以上、リリース版ではinfoレベル以上のログを出力する。
     */
    Spdlog();

    /**
     * @brief 設定を指定して構築
     * 
     * @param [in] the_config 構築用設定
     * @param [in] the_setters ログレベル・出力パターンをspdlogに設定するファンクタ
     */
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
