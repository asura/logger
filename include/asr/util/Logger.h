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
struct ILoggingImpl;
}  // namespace logger

/// ログ出力
class Logger
{
public:
    /// ログレベル
    enum Level
    {
        TRC,  ///< 詳細な調査用。プロダクションコードには残さないこと。
        DBG,  ///< デバッグ用途
        INF,  ///< 正常時、ユーザ or 運用担当者に通知したいケース
        WRN,  ///< 処理は継続して良いが、後後のため通知するケース
        ERR,  ///< 異常発生時
        CRT   ///< 実行を即停止せざるを得ないケース
    };

private:
    std::unique_ptr<logger::ILoggingImpl> m_impl;

public:
    Logger() = delete;

    explicit Logger(std::unique_ptr<logger::ILoggingImpl>&& the_impl);

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
#define ASR_UTIL_LOGT(the_message)

/// デバッグレベルのログ
#define ASR_UTIL_LOGD

/// 通常レベルのログ
#define ASR_UTIL_LOGI

/// 警告レベルのログ
#define ASR_UTIL_LOGW

/// エラーレベルのログ
#define ASR_UTIL_LOGE

/// クリティカルレベルのログ
#define ASR_UTIL_LOGC

#endif  // ASR_UTIL_LOGGER_H
