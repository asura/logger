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
    if (!the_node.IsMap())
    {
        throw std::runtime_error("トップレベルノードが非オブジェクト");
    }

    const auto& logger_node = the_node["logger"];
    if (!logger_node.IsDefined())
    {
        throw std::runtime_error("logger指定なし");
    }

    logger = the_node["logger"].as<std::string>();

    if (logger == "spdlog")
    {
        const auto& spdlog_node = the_node["spdlog"];
        if (!spdlog_node.IsDefined())
        {
            throw std::runtime_error("spdlog指定なし");
        }

        spdlog.reset(new Config::Spdlog(spdlog_node));
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
    if (!the_node.IsMap())
    {
        throw std::runtime_error("spdlogノードが非オブジェクト");
    }

    const auto& level_node = the_node["level"];
    if (!level_node.IsDefined())
    {
        throw std::runtime_error("spdlog.level指定なし");
    }

    level = level_node.as<std::string>();

    const auto& name_node = the_node["name"];
    if (!name_node.IsDefined())
    {
        throw std::runtime_error("spdlog.name指定なし");
    }

    name = name_node.as<std::string>();

    const YAML::Node& node_sinks = the_node["sinks"];
    if (!node_sinks.IsDefined())
    {
        throw std::runtime_error("spdlog.sinks指定なし");
    }

    if (!node_sinks.IsSequence())
    {
        throw std::runtime_error("spdlog.sinksノードが非配列");
    }

    for (const auto& node_sink : node_sinks)
    {
        Config::Spdlog::Sink spdlog_sink(node_sink);

        sinks.push_back(std::move(spdlog_sink));
    }
}

Config::Spdlog::Sink::Sink(const YAML::Node& the_node)
{
    if (!the_node.IsMap())
    {
        throw std::runtime_error("spdlog.sinks[]ノードが非オブジェクト");
    }

    const auto& type_node = the_node["type"];
    if (!type_node.IsDefined())
    {
        throw std::runtime_error("spdlog.sinks.type指定なし");
    }

    type = type_node.as<std::string>();

    const auto& level_node = the_node["level"];
    if (!level_node.IsDefined())
    {
        throw std::runtime_error("spdlog.sinks.level指定なし");
    }

    level = level_node.as<std::string>();

    const auto& pattern_node = the_node["pattern"];
    if (pattern_node.IsDefined())
    {
        pattern.reset(new std::string);
        *pattern = pattern_node.as<std::string>();
    }

    const YAML::Node& ident_node = the_node["ident"];
    if (ident_node.IsDefined())
    {
        ident.reset(new std::string);
        *ident = ident_node.as<std::string>();
    }
}

}  // namespace logger
}  // namespace util
}  // namespace asr
