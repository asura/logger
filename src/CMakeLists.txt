include_directories(
    ${PROJECT_SOURCE_DIR}/include
    ${CMAKE_BINARY_DIR}/include
)

add_library(
    asr_util_logger
    asr/util/Logger.cpp
    asr/util/logger/Config.cpp
    asr/util/logger/LoggerFactory.cpp
    asr/util/logger/Spdlog.cpp
)

add_dependencies(
  asr_util_logger
  yaml_cpp_project
)

if (COVERAGE)
    target_compile_options(asr_util_logger PRIVATE --coverage)
    target_link_options(asr_util_logger PRIVATE --coverage)
endif()

target_link_libraries(
    asr_util_logger
    spdlog::spdlog
    yaml-cpp
)
