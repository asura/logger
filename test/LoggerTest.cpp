#include "asr/util/Logger.h"
#include "asr/util/logger/ILoggingImpl.h"

#include <gmock/gmock.h>
using namespace ::testing;

#include <catch2/catch.hpp>

namespace asr
{
namespace util
{
class LoggingMock : public logger::ILoggingImpl
{
public:
    MOCK_METHOD2(output, void(const Logger::Level, const std::string&));
};
}  // namespace util
}  // namespace asr

TEST_CASE(
    "Loggerを構築し、outputを呼ぶ",
    "[asr][asr::util][Logger]")
{
    // ILoggingImplから派生させたモッククラスをインスタンス化
    auto mock = new asr::util::LoggingMock;

    // モッククラスにて依存性注入
    std::unique_ptr<asr::util::logger::ILoggingImpl> doc(mock);
    asr::util::Logger sut(std::move(doc));

    // モックの挙動を定義
    EXPECT_CALL(
        *mock,
        output(
            asr::util::Logger::INF,
            "file_name,123,function_name,error_message"))
        .Times(1);

    // 定義どおり呼ばれるか?
    sut.output(
        asr::util::Logger::INF,
        "file_name",
        123,
        "function_name",
        "error_message");

    SUCCEED();
}
