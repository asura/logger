#include "CErrRedirecter.h"
#include "asr/util/logger/Spdlog.h"

#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/syslog_sink.h>

#include <catch2/catch.hpp>
#include <iostream>
#include <regex>

TEST_CASE(
    "Spdlog::Spdlog",
    "[asr][asr::util::logger][Spdlog]")
{
    WHEN("Spdlogを構築")
    {
        asr::util::logger::Spdlog sut("適当なカテゴリ名称文字列を渡します");

        THEN("デフォルトロガーが設定される")
        {
            auto logger = spdlog::default_logger();
            REQUIRE(logger);

            AND_THEN("ログ出力レベルはINFO")
            {
                CHECK(logger->level() == spdlog::level::info);
            }

            AND_THEN("名称はmulti_sink")
            {
                CHECK(logger->name() == "multi_sink");
            }

            AND_THEN("sinkは2つ設定される")
            {
                REQUIRE(logger->sinks().size() == 2);

                AND_THEN("先頭はstderrへの色付きマルチスレッドロガー")
                {
                    auto sink = logger->sinks().front();
                    REQUIRE(sink);

                    auto casted = std::dynamic_pointer_cast<
                        spdlog::sinks::stderr_color_sink_mt>(
                        sink);
                    REQUIRE(casted);

#ifndef NDEBUG
                    AND_THEN("ログ出力レベルはDEBUG")
                    {
                        CHECK(sink->level() == spdlog::level::debug);
#else
                    AND_THEN("ログ出力レベルはINFO")
                    {
                        CHECK(sink->level() == spdlog::level::info);
#endif

                        // 出力フォーマットを確認したいが
                        // フォーマッター取得メソッドがないので不可
                    }
                }

                AND_THEN("2つ目はsyslogへの出力")
                {
                    auto sink = logger->sinks().back();
                    REQUIRE(sink);

                    auto casted = std::dynamic_pointer_cast<
                        spdlog::sinks::syslog_sink_mt>(
                        sink);
                    REQUIRE(casted);

                    AND_THEN("ログ出力レベルはWARN")
                    {
                        CHECK(sink->level() == spdlog::level::warn);

                        // identの内容を確認したいが
                        // 取得メソッドがないので不可
                    }
                }
            }
        }
    }
}

TEST_CASE(
    "Spdlog::output",
    "[asr][asr::util::logger][Spdlog]")
{
    GIVEN("stderrをリダイレクトし、Spdlog構築")
    {
        CErrRedirecter redirect;
        asr::util::logger::Spdlog sut("カテゴリ名称");

        WHEN("全ケースで出力されるERRレベルでログ出力")
        {
            sut.output(asr::util::Logger::ERR, "メッセージ文字列");

            THEN("期待通りの文字列が出力されている")
            {
                REQUIRE(redirect.data() != nullptr);
                REQUIRE(redirect.size() > 0);
                INFO("str=" << redirect.data());

                std::regex re(
                    R"(^E, \[\d{4}-\d{2}-\d{2} \d{2}:\d{2}:\d{2}\.\d{6}\] メッセージ文字列\n$)");

                const auto match_result = std::regex_match(redirect.data(), re);

                CHECK(match_result == true);
            }
        }

        WHEN("全ケースで出力されるFTLレベルでログ出力")
        {
            sut.output(asr::util::Logger::FTL, "メッセージ文字列");

            THEN("期待通りの文字列が出力されている")
            {
                REQUIRE(redirect.data() != nullptr);
                REQUIRE(redirect.size() > 0);
                INFO("str=" << redirect.data());

                std::regex re(
                    R"(^C, \[\d{4}-\d{2}-\d{2} \d{2}:\d{2}:\d{2}\.\d{6}\] メッセージ文字列\n$)");

                const auto match_result = std::regex_match(redirect.data(), re);

                CHECK(match_result == true);
            }
        }
    }
}
