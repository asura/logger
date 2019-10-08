#include "asr/util/logger/LoggerFactory.h"

#include "asr/util/logger/Spdlog.h"

namespace asr
{
namespace util
{
namespace logger
{
std::unique_ptr<ILogger>
LoggerFactory::generate(
    const Config& the_config)
{
    if (the_config.data().logger == "spdlog")
    {
        const auto& spdlog_config = the_config.data().spdlog;

        assert(spdlog_config.get() != nullptr);  // Configクラスでチェックしているため、nullにはならない

        return std::unique_ptr<ILogger>(
            new Spdlog(*spdlog_config));
    }

    assert(false);  // Configクラスでチェックしているため、ここには来ない
}
}  // namespace logger
}  // namespace util
}  // namespace asr
