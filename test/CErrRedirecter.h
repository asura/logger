#ifndef CERRREDIRECTER_H
#define CERRREDIRECTER_H

#include <iostream>
#include <mutex>

/// @brief stderrへの出力を文字列バッファで受けるようリダイレクトさせるクラス
/// spdlogの標準エラー出力ログクラスの出力をテストするために作成
class CErrRedirecter
{
    std::mutex m_mutex;
    std::lock_guard<std::mutex> m_locker;
    FILE* m_fp;
    char* m_buffer;
    size_t m_size;
    FILE* m_old;

public:
    CErrRedirecter();
    ~CErrRedirecter();

    const char* const data() const { return m_buffer; }
    const size_t size() const { return m_size; }

    /// テスト用途
    FILE* fp() const { return m_fp; }

    /// テスト用途
    FILE* old() const { return m_old; }
};

#endif  // CERRREDIRECTER_H
