#include "CErrRedirecter.h"
#include "asr/util/Logger.h"
#include "asr/util/logger/ILogger.h"
#include "asr/util/logger/Spdlog.h"

#include <gmock/gmock.h>
using namespace ::testing;

#include <catch2/catch.hpp>
#include <regex>

namespace asr
{
namespace util
{
class LoggingMock : public logger::ILogger
{
public:
    MOCK_METHOD2(output, void(const Logger::Level, const std::string&));
};
}  // namespace util
}  // namespace asr

TEST_CASE(
    "Loggerを構築(異常系)",
    "[asr][asr::util][Logger]")
{
    REQUIRE_THROWS_AS(
        asr::util::Logger(nullptr),
        std::runtime_error);
}

TEST_CASE(
    "Loggerを構築し、outputを呼ぶ",
    "[asr][asr::util][Logger]")
{
    // ILoggerから派生させたモッククラスをインスタンス化
    auto mock = new asr::util::LoggingMock;

    // モッククラスにて依存性注入
    std::unique_ptr<asr::util::logger::ILogger> doc(mock);
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

TEST_CASE(
    "Logger::output",
    "[asr][asr::util][Logger]")
{
    GIVEN("stderrをリダイレクト")
    {
        CErrRedirecter redirect;
        CHECK(redirect.data() == nullptr);
        CHECK(redirect.size() == 0);

        asr::util::Logger sut(
            std::unique_ptr<asr::util::logger::ILogger>(
                new asr::util::logger::Spdlog("test")));

        WHEN("VBSレベルのログ出力")
        {
            sut.output(
                asr::util::Logger::VBS,
                __FILE__,
                __LINE__,
                __func__,
                "trace");

#if 1
            THEN("レベル設定により標準エラー出力に出力されない")
            {
                CHECK(redirect.size() == 0);
            }
#else
            THEN("標準エラー出力に期待通りの出力あり")
            {
                INFO("str=" << redirect.data());

                std::regex re(
                    R"(^T, \[\d{4}-\d{2}-\d{2} \d{2}:\d{2}:\d{2}\.\d{6}\] .+LoggerTest\.cpp,\d+,.+,trace\n$)");

                const auto match_result = std::regex_match(redirect.data(), re);

                CHECK(match_result == true);
            }
#endif
        }

        WHEN("DBGレベルのログ出力")
        {
            sut.output(
                asr::util::Logger::DBG,
                __FILE__,
                __LINE__,
                __func__,
                "DEBUG");

#ifndef NDEBUG
            THEN("レベル設定により標準エラー出力に出力されない")
            {
                CHECK(redirect.size() == 0);
            }
#else
            THEN("標準エラー出力に期待通りの出力あり")
            {
                INFO("str=" << redirect.data());

                std::regex re(
                    R"(^D, \[\d{4}-\d{2}-\d{2} \d{2}:\d{2}:\d{2}\.\d{6}\] .+LoggerTest\.cpp,\d+,.+,DEBUG\n$)");

                const auto match_result = std::regex_match(redirect.data(), re);

                CHECK(match_result == true);
            }
#endif
        }

        WHEN("INFレベルのログ出力")
        {
            sut.output(
                asr::util::Logger::INF,
                __FILE__,
                __LINE__,
                __func__,
                "OK");

            THEN("標準エラー出力に期待通りの出力あり")
            {
                REQUIRE(redirect.size() > 0);
                INFO("str=" << redirect.data());

                std::regex re(
                    R"(^I, \[\d{4}-\d{2}-\d{2} \d{2}:\d{2}:\d{2}\.\d{6}\] .+LoggerTest\.cpp,\d+,.+,OK\n$)");

                const auto match_result = std::regex_match(redirect.data(), re);

                CHECK(match_result == true);
            }
        }

        WHEN("WRNレベルのログ出力")
        {
            sut.output(
                asr::util::Logger::WRN,
                __FILE__,
                __LINE__,
                __func__,
                "警告");

            THEN("標準エラー出力に期待通りの出力あり")
            {
                REQUIRE(redirect.size() > 0);
                INFO("str=" << redirect.data());

                std::regex re(
                    R"(^W, \[\d{4}-\d{2}-\d{2} \d{2}:\d{2}:\d{2}\.\d{6}\] .+LoggerTest\.cpp,\d+,.+,警告\n$)");

                const auto match_result = std::regex_match(redirect.data(), re);

                CHECK(match_result == true);
            }
        }

        WHEN("ERRレベルのログ出力")
        {
            sut.output(
                asr::util::Logger::ERR,
                __FILE__,
                __LINE__,
                __func__,
                "Error");

            THEN("標準エラー出力に期待通りの出力あり")
            {
                REQUIRE(redirect.size() > 0);
                INFO("str=" << redirect.data());

                std::regex re(
                    R"(^E, \[\d{4}-\d{2}-\d{2} \d{2}:\d{2}:\d{2}\.\d{6}\] .+LoggerTest\.cpp,\d+,.+,Error\n$)");

                const auto match_result = std::regex_match(redirect.data(), re);

                CHECK(match_result == true);
            }
        }

        WHEN("FTLレベルのログ出力")
        {
            sut.output(
                asr::util::Logger::FTL,
                __FILE__,
                __LINE__,
                __func__,
                "致命的");

            THEN("標準エラー出力に期待通りの出力あり")
            {
                REQUIRE(redirect.size() > 0);
                INFO("str=" << redirect.data());

                std::regex re(
                    R"(^C, \[\d{4}-\d{2}-\d{2} \d{2}:\d{2}:\d{2}\.\d{6}\] .+LoggerTest\.cpp,\d+,.+,致命的\n$)");

                const auto match_result = std::regex_match(redirect.data(), re);

                CHECK(match_result == true);
            }
        }
    }
}

TEST_CASE(
    "theLogger",
    "[asr][asr::util][Logger]")
{
    auto& logger = asr::util::theLogger();

    SUCCEED();
}

// theLoggerからの出力テストは実施しない。
// spdlog::sinks::stderr_color_sink_mtが内部にstderrを保持しているため
// (=> ansicolor_sink::target_file_)
// テストの順序によって、リダイレクトが期待通りに動作しない。
