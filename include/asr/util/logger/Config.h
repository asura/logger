#ifndef ASR_UTIL_LOGGER_CONFIG_H
#define ASR_UTIL_LOGGER_CONFIG_H

#include <yaml-cpp/yaml.h>

#include <istream>
#include <memory>  // std::unique_ptr
#include <string>
#include <vector>

namespace asr
{
namespace util
{
namespace logger
{
class Config
{
public:
    struct Spdlog
    {
        struct Sink
        {
            std::string type;
            std::string level;
            std::unique_ptr<std::string> pattern;
            std::unique_ptr<std::string> ident;

            Sink() = delete;
            explicit Sink(const YAML::Node& node);
            Sink(const Sink&) = delete;
            Sink(Sink&& the_other) = default;
        };

        std::string level;
        std::string name;
        std::vector<Sink> sinks;

        Spdlog() = delete;
        explicit Spdlog(const YAML::Node& node);
    };

    struct TopLevel
    {
        std::string logger;
        std::unique_ptr<Spdlog> spdlog;

        TopLevel() = delete;
        explicit TopLevel(const YAML::Node& node);
    };

private:
    TopLevel m_data;

public:
    Config() = delete;
    explicit Config(std::istream& the_istream);

    const TopLevel& data() const { return m_data; }
};
}  // namespace logger
}  // namespace util
}  // namespace asr

#endif  // ASR_UTIL_LOGGER_CONFIG_H
