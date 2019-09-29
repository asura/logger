#include "asr/util/logger/Config.h"

#include <catch2/catch.hpp>
#include <sstream>

TEST_CASE(
    "Config",
    "[asr::util::logger::Config]")
{
    SECTION("正常系 - spdlog指定 - 単一sink指定")
    {
        std::stringstream ss;
        ss << "logger: spdlog" << std::endl
           << "spdlog:" << std::endl
           << "  name: single_sink" << std::endl
           << "  sinks:" << std::endl
           << "    - type: stderr_color_sink_mt" << std::endl
           << "      level: debug" << std::endl;

        WHEN("Configを構築")
        {
            asr::util::logger::Config sut(ss);

            THEN("設定値を正常にパースできている")
            {
                const auto& top_level = sut.data();

                CHECK(top_level.logger == "spdlog");

                REQUIRE(top_level.spdlog.get() != nullptr);
                const auto& spdlog = *top_level.spdlog;

                CHECK(spdlog.name == "single_sink");

                REQUIRE(spdlog.sinks.size() == 1);

                CHECK(spdlog.sinks[0].type == "stderr_color_sink_mt");
                CHECK(spdlog.sinks[0].level == "debug");
            }
        }
    }

    SECTION("正常系 - spdlog指定 - 複数sink指定")
    {
        std::stringstream ss;
        ss << "logger: spdlog" << std::endl
           << "spdlog:" << std::endl
           << "  name: multi_sink" << std::endl
           << "  sinks:" << std::endl
           << "    - type: stderr_color_sink_mt" << std::endl
           << "      level: debug" << std::endl
           << "    - type: syslog_sink_mt" << std::endl
           << "      level: warn" << std::endl;

        WHEN("Configを構築")
        {
            asr::util::logger::Config sut(ss);

            THEN("設定値を正常にパースできている")
            {
                const auto& top_level = sut.data();

                CHECK(top_level.logger == "spdlog");

                REQUIRE(top_level.spdlog.get() != nullptr);
                const auto& spdlog = *top_level.spdlog;

                CHECK(spdlog.name == "multi_sink");

                REQUIRE(spdlog.sinks.size() == 2);

                CHECK(spdlog.sinks[0].type == "stderr_color_sink_mt");
                CHECK(spdlog.sinks[0].level == "debug");

                CHECK(spdlog.sinks[1].type == "syslog_sink_mt");
                CHECK(spdlog.sinks[1].level == "warn");
            }
        }
    }

    SECTION("正常系 - spdlog指定 - sink指定なし")
    {
        std::stringstream ss;
        ss << "logger: spdlog" << std::endl
           << "spdlog:" << std::endl
           << "  name: single_sink" << std::endl;

        WHEN("Configを構築")
        {
            THEN("例外発生")
            {
                REQUIRE_THROWS_AS(
                    asr::util::logger::Config(ss),
                    std::runtime_error);
            }
        }
    }

    SECTION("正常系 - spdlog指定 - name指定なし")
    {
        std::stringstream ss;
        ss << "logger: spdlog" << std::endl
           << "spdlog:" << std::endl;

        WHEN("Configを構築")
        {
            THEN("例外発生")
            {
                REQUIRE_THROWS_AS(
                    asr::util::logger::Config(ss),
                    YAML::InvalidNode);
            }
        }
    }

    SECTION("正常系 - spdlog指定 - spdlog指定なし")
    {
        std::stringstream ss;
        ss << "logger: spdlog" << std::endl;

        WHEN("Configを構築")
        {
            THEN("例外発生")
            {
                REQUIRE_THROWS_AS(
                    asr::util::logger::Config(ss),
                    YAML::InvalidNode);
            }
        }
    }
}
