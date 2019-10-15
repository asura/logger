#include "RuntimeErrorMatcher.h"
#include "asr/util/logger/Config.h"

TEST_CASE(
    "Config",
    "[asr::util::logger::Config]")
{
    SECTION("logger指定なし")
    {
        SECTION("別キーあり")
        {
            std::stringstream ss;
            ss << "aaa: bbb" << std::endl;

            WHEN("Configを構築")
            {
                THEN("例外発生")
                {
                    REQUIRE_THROWS_MATCHES(
                        asr::util::logger::Config(ss),
                        std::runtime_error,
                        RuntimeErrorMatcher("logger指定なし"));
                }
            }
        }

        SECTION("非オブジェクト")
        {
            std::stringstream ss;
            ss << "" << std::endl;

            WHEN("Configを構築")
            {
                THEN("例外発生")
                {
                    REQUIRE_THROWS_MATCHES(
                        asr::util::logger::Config(ss),
                        std::runtime_error,
                        RuntimeErrorMatcher("トップレベルノードが非オブジェクト"));
                }
            }
        }
    }

    SECTION("spdlog指定")
    {
        SECTION("正常系")
        {
            SECTION("単一sink指定")
            {
                std::stringstream ss;
                ss << "logger: spdlog" << std::endl
                   << "spdlog:" << std::endl
                   << "  level: warn" << std::endl
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

                        CHECK(spdlog.level == "warn");

                        CHECK(spdlog.name == "single_sink");

                        REQUIRE(spdlog.sinks.size() == 1);

                        CHECK(spdlog.sinks[0].type == "stderr_color_sink_mt");
                        CHECK(spdlog.sinks[0].level == "debug");
                    }
                }
            }

            SECTION("複数sink指定")
            {
                std::stringstream ss;
                ss << "logger: spdlog" << std::endl
                   << "spdlog:" << std::endl
                   << "  level: warn" << std::endl
                   << "  name: multi_sink" << std::endl
                   << "  sinks:" << std::endl
                   << "    - type: stderr_color_sink_mt" << std::endl
                   << "      level: debug" << std::endl
                   << "      pattern: AbCd" << std::endl
                   << "    - type: syslog_sink_mt" << std::endl
                   << "      level: warn" << std::endl
                   << "      ident: IDENT" << std::endl;

                WHEN("Configを構築")
                {
                    asr::util::logger::Config sut(ss);

                    THEN("設定値を正常にパースできている")
                    {
                        const auto& top_level = sut.data();

                        CHECK(top_level.logger == "spdlog");

                        REQUIRE(top_level.spdlog.get() != nullptr);
                        const auto& spdlog = *top_level.spdlog;

                        CHECK(spdlog.level == "warn");

                        CHECK(spdlog.name == "multi_sink");

                        REQUIRE(spdlog.sinks.size() == 2);

                        CHECK(spdlog.sinks[0].type == "stderr_color_sink_mt");
                        CHECK(spdlog.sinks[0].level == "debug");
                        REQUIRE(spdlog.sinks[0].pattern.get() != nullptr);
                        CHECK(*spdlog.sinks[0].pattern == "AbCd");
                        REQUIRE(spdlog.sinks[0].ident.get() == nullptr);

                        CHECK(spdlog.sinks[1].type == "syslog_sink_mt");
                        CHECK(spdlog.sinks[1].level == "warn");
                        REQUIRE(spdlog.sinks[1].pattern.get() == nullptr);
                        REQUIRE(spdlog.sinks[1].ident.get() != nullptr);
                        CHECK(*spdlog.sinks[1].ident == "IDENT");
                    }
                }
            }
        }

        SECTION("異常系")
        {
            SECTION("sinkにlevel指定なし")
            {
                std::stringstream ss;
                ss << "logger: spdlog" << std::endl
                   << "spdlog:" << std::endl
                   << "  level: warn" << std::endl
                   << "  name: single_sink" << std::endl
                   << "  sinks:" << std::endl
                   << "    - type: stderr_color_sink_mt" << std::endl;

                WHEN("Configを構築")
                {
                    THEN("例外発生")
                    {
                        REQUIRE_THROWS_MATCHES(
                            asr::util::logger::Config(ss),
                            std::runtime_error,
                            RuntimeErrorMatcher("spdlog.sinks.level指定なし"));
                    }
                }
            }

            SECTION("sinkにtype指定なし")
            {
                std::stringstream ss;
                ss << "logger: spdlog" << std::endl
                   << "spdlog:" << std::endl
                   << "  level: warn" << std::endl
                   << "  name: single_sink" << std::endl
                   << "  sinks:" << std::endl
                   << "    - aaa: bbb" << std::endl;

                WHEN("Configを構築")
                {
                    THEN("例外発生")
                    {
                        REQUIRE_THROWS_MATCHES(
                            asr::util::logger::Config(ss),
                            std::runtime_error,
                            RuntimeErrorMatcher("spdlog.sinks.type指定なし"));
                    }
                }
            }

            SECTION("sinkに下位ノードなし")
            {
                std::stringstream ss;
                ss << "logger: spdlog" << std::endl
                   << "spdlog:" << std::endl
                   << "  level: warn" << std::endl
                   << "  name: single_sink" << std::endl
                   << "  sinks:" << std::endl;

                WHEN("Configを構築")
                {
                    THEN("例外発生")
                    {
                        REQUIRE_THROWS_MATCHES(
                            asr::util::logger::Config(ss),
                            std::runtime_error,
                            RuntimeErrorMatcher("spdlog.sinksノードが非配列"));
                    }
                }
            }

            SECTION("sinkに非オブジェクトの下位ノード")
            {
                std::stringstream ss;
                ss << "logger: spdlog" << std::endl
                   << "spdlog:" << std::endl
                   << "  level: warn" << std::endl
                   << "  name: single_sink" << std::endl
                   << "  sinks:" << std::endl
                   << "    - a" << std::endl;

                WHEN("Configを構築")
                {
                    THEN("例外発生")
                    {
                        REQUIRE_THROWS_MATCHES(
                            asr::util::logger::Config(ss),
                            std::runtime_error,
                            RuntimeErrorMatcher("spdlog.sinks[]ノードが非オブジェクト"));
                    }
                }
            }

            SECTION("sink指定なし")
            {
                std::stringstream ss;
                ss << "logger: spdlog" << std::endl
                   << "spdlog:" << std::endl
                   << "  level: warn" << std::endl
                   << "  name: single_sink" << std::endl;

                WHEN("Configを構築")
                {
                    THEN("例外発生")
                    {
                        REQUIRE_THROWS_MATCHES(
                            asr::util::logger::Config(ss),
                            std::runtime_error,
                            RuntimeErrorMatcher("spdlog.sinks指定なし"));
                    }
                }
            }

            SECTION("name指定なし")
            {
                std::stringstream ss;
                ss << "logger: spdlog" << std::endl
                   << "spdlog:" << std::endl
                   << "  level: warn" << std::endl
                   << "  aaa: bbb";

                WHEN("Configを構築")
                {
                    THEN("例外発生")
                    {
                        REQUIRE_THROWS_MATCHES(
                            asr::util::logger::Config(ss),
                            std::runtime_error,
                            RuntimeErrorMatcher("spdlog.name指定なし"));
                    }
                }
            }

            SECTION("level指定なし")
            {
                std::stringstream ss;
                ss << "logger: spdlog" << std::endl
                   << "spdlog:" << std::endl
                   << "  aaa: bbb";

                WHEN("Configを構築")
                {
                    THEN("例外発生")
                    {
                        REQUIRE_THROWS_MATCHES(
                            asr::util::logger::Config(ss),
                            std::runtime_error,
                            RuntimeErrorMatcher("spdlog.level指定なし"));
                    }
                }
            }

            SECTION("spdlogに非オブジェクトの下位ノード")
            {
                std::stringstream ss;
                ss << "logger: spdlog" << std::endl
                   << "spdlog:" << std::endl
                   << "  - a" << std::endl;

                WHEN("Configを構築")
                {
                    THEN("例外発生")
                    {
                        REQUIRE_THROWS_MATCHES(
                            asr::util::logger::Config(ss),
                            std::runtime_error,
                            RuntimeErrorMatcher("spdlogノードが非オブジェクト"));
                    }
                }
            }

            SECTION("spdlog指定なし")
            {
                std::stringstream ss;
                ss << "logger: spdlog" << std::endl;

                WHEN("Configを構築")
                {
                    THEN("例外発生")
                    {
                        REQUIRE_THROWS_MATCHES(
                            asr::util::logger::Config(ss),
                            std::runtime_error,
                            RuntimeErrorMatcher("spdlog指定なし"));
                    }
                }
            }
        }
    }

    SECTION("非spdlog指定")
    {
        std::stringstream ss;
        ss << "logger: bbb" << std::endl;

        WHEN("Configを構築")
        {
            THEN("例外発生")
            {
                REQUIRE_THROWS_MATCHES(
                    asr::util::logger::Config(ss),
                    std::runtime_error,
                    RuntimeErrorMatcher("未対応のロガー(bbb)"));
            }
        }
    }
}
