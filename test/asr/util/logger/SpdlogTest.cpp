#include "CErrRedirecter.h"
#include "RuntimeErrorMatcher.h"
#include "asr/util/logger/Spdlog.h"
#include "asr/util/logger/SpdlogSettersMock.h"

#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/syslog_sink.h>

#include <catch2/catch.hpp>
#include <iostream>
#include <regex>

using ::testing::_;

TEST_CASE(
    "Spdlog::Spdlog",
    "[asr][asr::util::logger][Spdlog]")
{
    WHEN("Spdlogを構築")
    {
        asr::util::logger::Spdlog sut;

        THEN("デフォルトロガーが設定される")
        {
            auto logger = spdlog::default_logger();
            REQUIRE(logger);

            AND_THEN("ログ出力レベルはINFO")
            {
                CHECK(logger->level() == spdlog::level::info);
            }

            AND_THEN("名称はsingle_sink")
            {
                CHECK(logger->name() == "single_sink");
            }

            AND_THEN("sinkは1つだけ設定される")
            {
                REQUIRE(logger->sinks().size() == 1);

                AND_THEN("stderrへの色付きマルチスレッドロガー")
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
            }
        }
    }
}

TEST_CASE(
    "Spdlog::Spdlog - Config::Spdlog",
    "[asr][asr::util::logger][Spdlog]")
{
    GIVEN("未対応のsinkを指定してConfig::Spdlogを構築")
    {
        std::stringstream ss;
        ss << "level: info" << std::endl
           << "name: single_sink" << std::endl
           << "sinks:" << std::endl
           << "  - type: AbCd" << std::endl
           << "    level: error" << std::endl
           << "    pattern: AbCd" << std::endl;

        YAML::Node yaml_node;

        INFO("yaml=" << ss.str());
        REQUIRE_NOTHROW(yaml_node = YAML::Load(ss.str()));

        asr::util::logger::Config::Spdlog spdlog_config(yaml_node);

        WHEN("Settersに有効な値を設定してSpdlogを構築")
        {
            auto mock = std::make_shared<asr::util::logger::SpdlogSettersMock>();
            REQUIRE(mock.get() != nullptr);

            REQUIRE_THROWS_MATCHES(
                asr::util::logger::Spdlog(
                    spdlog_config,
                    std::dynamic_pointer_cast<asr::util::logger::Spdlog::Setters>(mock)),
                std::runtime_error,
                RuntimeErrorMatcher("未対応のsink (AbCd)"));
        }
    }

    GIVEN("1sinkのConfig::Spdlogを構築")
    {
        std::stringstream ss;
        ss << "level: info" << std::endl
           << "name: single_sink" << std::endl
           << "sinks:" << std::endl
           << "  - type: stderr_color_sink_mt" << std::endl
           << "    level: error" << std::endl
           << "    pattern: AbCd" << std::endl;

        YAML::Node yaml_node;

        INFO("yaml=" << ss.str());
        REQUIRE_NOTHROW(yaml_node = YAML::Load(ss.str()));

        asr::util::logger::Config::Spdlog spdlog_config(yaml_node);

        WHEN("Settersにmockを設定してSpdlogを構築")
        {
            auto mock = std::make_shared<asr::util::logger::SpdlogSettersMock>();
            REQUIRE(mock.get() != nullptr);

            EXPECT_CALL(
                *mock,
                set_level(_, "error"))
                .Times(1);

            EXPECT_CALL(
                *mock,
                set_pattern(_, "AbCd"))
                .Times(1);

            asr::util::logger::Spdlog sut(
                spdlog_config,
                std::dynamic_pointer_cast<asr::util::logger::Spdlog::Setters>(mock));

            SUCCEED();
        }

        WHEN("Settersに有効な値を設定してSpdlogを構築")
        {
            auto setters = std::make_shared<asr::util::logger::Spdlog::Setters>();

            asr::util::logger::Spdlog sut(
                spdlog_config,
                std::dynamic_pointer_cast<asr::util::logger::Spdlog::Setters>(setters));

            THEN("デフォルトロガーが設定される")
            {
                auto logger = spdlog::default_logger();
                REQUIRE(logger);

                AND_THEN("ログ出力レベルはINFO")
                {
                    CHECK(logger->level() == spdlog::level::info);
                }

                AND_THEN("名称はsingle_sink")
                {
                    CHECK(logger->name() == "single_sink");
                }

                AND_THEN("sinkは1つだけ設定される")
                {
                    REQUIRE(logger->sinks().size() == 1);

                    AND_THEN("stderrへの色付きマルチスレッドロガー")
                    {
                        auto sink = logger->sinks().front();
                        REQUIRE(sink);

                        auto casted = std::dynamic_pointer_cast<
                            spdlog::sinks::stderr_color_sink_mt>(
                            sink);
                        REQUIRE(casted);

                        AND_THEN("ログ出力レベルはERROR")
                        {
                            CHECK(sink->level() == spdlog::level::err);

                            // 出力フォーマットを確認したいが
                            // フォーマッター取得メソッドがないので不可
                        }
                    }
                }
            }
        }

        WHEN("Settersにnullptrを設定してSpdlogを構築")
        {
            THEN("例外発生")
            {
                REQUIRE_THROWS_MATCHES(
                    asr::util::logger::Spdlog(spdlog_config, nullptr),
                    std::runtime_error,
                    RuntimeErrorMatcher("Setters指定がnullptr"));
            };
        }
    }

    GIVEN("2sinksのConfig::Spdlogを構築")
    {
        std::stringstream ss;
        ss << "level: info" << std::endl
           << "name: single_sink" << std::endl
           << "sinks:" << std::endl
           << "  - type: stderr_color_sink_mt" << std::endl
           << "    level: error" << std::endl
           << "    pattern: AbCd" << std::endl
           << "  - type: stdout_sink_mt" << std::endl
           << "    level: warn" << std::endl
           << "    pattern: eee" << std::endl;

        YAML::Node yaml_node;

        REQUIRE_NOTHROW(yaml_node = YAML::Load(ss.str()));

        asr::util::logger::Config::Spdlog spdlog_config(yaml_node);

        WHEN("Settersに有効な値を設定してSpdlogを構築")
        {
            auto mock = std::make_shared<asr::util::logger::SpdlogSettersMock>();
            REQUIRE(mock.get() != nullptr);

            THEN("例外発生")
            {
                REQUIRE_THROWS_MATCHES(
                    asr::util::logger::Spdlog(
                        spdlog_config,
                        std::dynamic_pointer_cast<asr::util::logger::Spdlog::Setters>(mock)),
                    std::runtime_error,
                    RuntimeErrorMatcher("複数sinkは未対応"));
            };
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
        asr::util::logger::Spdlog sut;

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

TEST_CASE(
    "Spdlog::Setters::set_level",
    "[asr][asr::util::logger][Spdlog]")
{
    asr::util::logger::Spdlog::Setters sut;

    GIVEN("新規sinkを作成")
    {
        spdlog::sinks::stderr_color_sink_mt sink;

        WHEN("正常系")
        {
            struct Data
            {
                std::string level_string;
                spdlog::level::level_enum expected_level;
            };

            const auto value = GENERATE(
                values<Data>({
                    {"trace", spdlog::level::trace},
                    {"debug", spdlog::level::debug},
                    {"info", spdlog::level::info},
                    {"warn", spdlog::level::warn},
                    {"error", spdlog::level::err},
                    {"critical", spdlog::level::critical},
                }));

            sut.set_level(sink, value.level_string);

            THEN("ログレベルが適切に設定される")
            {
                INFO("level=" << value.level_string);
                CHECK(sink.level() == value.expected_level);
            }
        }

        WHEN("異常系 - サポート外のレベル文字列を指定")
        {
            THEN("例外発生")
            {
                REQUIRE_THROWS_MATCHES(
                    sut.set_level(sink, "サポート外"),
                    std::runtime_error,
                    RuntimeErrorMatcher("未対応のログレベル指定(サポート外)"));
            }
        }
    }
}
