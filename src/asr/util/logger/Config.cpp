#include "asr/util/logger/Config.h"

#include <iostream>
#include <sstream>

namespace asr
{
namespace util
{
namespace logger
{
Config::Config(std::istream& the_istream)
    : m_data(
          YAML::Load(
              std::string(
                  (std::istreambuf_iterator<char>(the_istream)),
                  std::istreambuf_iterator<char>())))
{
}

Config::TopLevel::TopLevel(const YAML::Node& the_node)
{
    logger = the_node["logger"].as<std::string>();

    if (logger == "spdlog")
    {
        spdlog.reset(new Config::Spdlog(the_node["spdlog"]));
    }
    else
    {
        std::ostringstream oss;
        oss << "未対応のロガー(" << logger << ")";
        throw std::runtime_error(oss.str());
    }
}

Config::Spdlog::Spdlog(const YAML::Node& the_node)
{
    name = the_node["name"].as<std::string>();

    const YAML::Node& node_sinks = the_node["sinks"];
    if (!node_sinks.IsDefined())
    {
        throw std::runtime_error("sinks指定なし");
    }

    for (const auto& node_sink : node_sinks)
    {
        Config::Spdlog::Sink spdlog_sink(node_sink);

        sinks.push_back(std::move(spdlog_sink));
    }
}

Config::Spdlog::Sink::Sink(const YAML::Node& the_node)
{
    type = the_node["type"].as<std::string>();

    level = the_node["level"].as<std::string>();

    const YAML::Node& node_pattern = the_node["pattern"];
    if (node_pattern.IsDefined())
    {
        pattern.reset(new std::string);
        *pattern = node_pattern.as<std::string>();
    }

    const YAML::Node& node_ident = the_node["ident"];
    if (node_ident.IsDefined())
    {
        ident.reset(new std::string);
        *ident = node_ident.as<std::string>();
    }
}

}  // namespace logger
}  // namespace util
}  // namespace asr
