include_directories(
  ${PROJECT_SOURCE_DIR}/third_party/Catch2-2.9.2/single_include/catch2
  ${PROJECT_SOURCE_DIR}/third_party/googletest-release-1.8.1/googlemock/include
  ${PROJECT_SOURCE_DIR}/include
)

add_executable(
  Tester
  Tester.cpp
  CErrRedirecter.cpp
  CErrRedirecterTest.cpp
  LoggerTest.cpp
  SpdlogTest.cpp
)

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
