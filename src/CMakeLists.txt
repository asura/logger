include_directories(
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/third_party/yaml-cpp-0.6.2/include
    )

add_library(
    asr_util_logger
    asr/util/Logger.cpp
    asr/util/logger/Config.cpp
    asr/util/logger/LoggerFactory.cpp
    asr/util/logger/Spdlog.cpp
)

target_link_libraries(
    asr_util_logger
    spdlog::spdlog
    yaml-cpp
)
