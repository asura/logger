include_directories(
  ${PROJECT_SOURCE_DIR}/third_party/Catch2-2.9.2/single_include/catch2
  ${PROJECT_SOURCE_DIR}/third_party/googletest-release-1.8.1/googlemock/include
  ${PROJECT_SOURCE_DIR}/third_party/yaml-cpp-0.6.2/include
  ${PROJECT_SOURCE_DIR}/include
  ${PROJECT_SOURCE_DIR}/test
)

add_executable(
  Tester
  Tester.cpp
  CErrRedirecter.cpp
  CErrRedirecterTest.cpp
  asr/util/LoggerTest.cpp
  asr/util/logger/ConfigTest.cpp
  asr/util/logger/LoggerFactoryTest.cpp
  asr/util/logger/SpdlogTest.cpp
)

if (COVERAGE)
  target_compile_options(Tester PRIVATE --coverage)
  target_link_options(Tester PRIVATE --coverage)
endif()

target_link_libraries(
  Tester
  Catch2::Catch2
  gmock
  asr_util_logger
)

add_test(
  NAME Tester
  COMMAND Tester -r junit -o ${PROJECT_BINARY_DIR}/test_result_Tester.xml
)
