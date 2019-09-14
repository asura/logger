include_directories(
    ${PROJECT_SOURCE_DIR}/include
)

add_library(
    asr_util_logger
    asr/util/Logger.cpp
)

target_link_libraries(
    asr_util_logger
)
