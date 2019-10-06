#include "asr/util/logger/LoggerFactory.h"
#include "asr/util/logger/Spdlog.h"

#include <catch2/catch.hpp>
#include <sstream>
#include <typeinfo>

TEST_CASE(
    "LoggerFactory::generate - Config",
    "[LoggerFactory][asr::util::logger]")
{
    SECTION("Config::loggerはspdlog")
    {
        // 以下の場合、Config構築に失敗 (例外発生) するため、テスト対象外
        // - sinks指定なし

        SECTION("Config::spdlogに適切な設定あり")
        {
            std::stringstream ss;
            ss << "logger: spdlog" << std::endl
               << "spdlog:" << std::endl
               << "  level: info" << std::endl
               << "  name: single_sink" << std::endl
               << "  sinks:" << std::endl
               << "    - type: stderr_color_sink_mt" << std::endl
               << "      level: error" << std::endl;

            asr::util::logger::Config doc(ss);

            WHEN("generateを呼ぶ")
            {
                const auto result = asr::util::logger::LoggerFactory::generate(doc);

                THEN("戻り値は非nullptr")
                {
                    const auto result_ptr = result.get();
                    REQUIRE(result_ptr != nullptr);

                    AND_THEN("Spdlogが返る")
                    {
                        CHECK(typeid(*result_ptr) == typeid(asr::util::logger::Spdlog));

                        AND_THEN("ログレベルは設定通り")
                        {
                            auto spdlog = dynamic_cast<asr::util::logger::Spdlog*>(result_ptr);
                            REQUIRE(spdlog != nullptr);

                            CHECK(spdlog->logger()->level() == spdlog::level::info);
                        }
                    }
                }
            }
        }
    }
}
