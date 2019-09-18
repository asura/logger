include_directories(
    ${PROJECT_SOURCE_DIR}/include
)

add_library(
    asr_util_logger
    asr/util/Logger.cpp
    asr/util/logger/Spdlog.cpp
)

if (COVERAGE)
    target_compile_options(asr_util_logger PRIVATE --coverage)
    target_link_options(asr_util_logger PRIVATE --coverage)
endif()

target_link_libraries(
    asr_util_logger
    spdlog::spdlog
)
