#ifndef ASR_UTIL_LOGGER_H
#define ASR_UTIL_LOGGER_H

#include <memory>  // unique_ptr
#include <string>

namespace asr
{
/// 特定ドメインに属さないクラス・メソッド群の名前空間
namespace util
{
namespace logger
{
struct ILogger;
}  // namespace logger

/// @brief ログ出力
/// サンプルコード
/// @code
/// #include "asr/util/Logger.h"
///
/// void foo()
/// {
///     ASR_UTIL_LOGI("OK");
/// }
/// @endcode
class Logger
{
public:
    /// ログレベル
    enum Level
    {
        VBS,  ///< 詳細な調査用。プロダクションコードには残さないこと。
        DBG,  ///< デバッグ用途
        INF,  ///< 正常時、ユーザ or 運用担当者に通知したいケース
        WRN,  ///< 処理は継続して良いが、後後のため通知するケース
        ERR,  ///< 異常発生時
        FTL   ///< 実行を即停止せざるを得ないケース
    };

private:
    std::unique_ptr<logger::ILogger> m_impl;

public:
    Logger() = delete;

    explicit Logger(std::unique_ptr<logger::ILogger>&& the_impl);

    /// @brief ログ出力
    /// @param [in] the_log_level 本ログのログレベル
    void output(const Level the_log_level,
                const std::string& the_file_name,
                const int the_line_number,
                const std::string& the_function_name,
                const std::string& the_message);
};

Logger& theLogger();
}  // namespace util
}  // namespace asr

/// トレースレベルのログ
#define ASR_UTIL_LOGV(the_message) asr::util::theLogger().output(asr::util::Logger::VBS, __FILE__, __LINE__, __func__, the_message)

/// デバッグレベルのログ
#define ASR_UTIL_LOGD(the_message) asr::util::theLogger().output(asr::util::Logger::DBG, __FILE__, __LINE__, __func__, the_message)

/// 通常レベルのログ
#define ASR_UTIL_LOGI(the_message) asr::util::theLogger().output(asr::util::Logger::INF, __FILE__, __LINE__, __func__, the_message)

/// 警告レベルのログ
#define ASR_UTIL_LOGW(the_message) asr::util::theLogger().output(asr::util::Logger::WRN, __FILE__, __LINE__, __func__, the_message)

/// エラーレベルのログ
#define ASR_UTIL_LOGE(the_message) asr::util::theLogger().output(asr::util::Logger::ERR, __FILE__, __LINE__, __func__, the_message)

/// クリティカルレベルのログ
#define ASR_UTIL_LOGF(the_message) asr::util::theLogger().output(asr::util::Logger::FTL, __FILE__, __LINE__, __func__, the_message)

#endif  // ASR_UTIL_LOGGER_H
