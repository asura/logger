cmake_minimum_required(VERSION 3.13 FATAL_ERROR)

project(
  asr_util_logger
  VERSION 1.0.0
  LANGUAGES CXX
)

# C++規格
set(CMAKE_CXX_STANDARD 11)

#######################################
# オプション指定
option(USE_CLANG_TIDY "clang-tidyを使って静的解析する" OFF)
option(COVERAGE "テストカバレッジ取得" OFF)

#######################################
# ビルド・タイプ指定(Debug or Releaseに限定)
if(CMAKE_BUILD_TYPE)
  if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    # AddressSanitizer
    set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-fsanitize=address")
  elseif(${CMAKE_BUILD_TYPE} STREQUAL "Release")
    #
  else()
    message(FATAL_ERROR "変数CMAKE_BUILD_TYPEにはDebugかReleaseを指定してください")
  endif()
else()
  message(FATAL_ERROR "変数CMAKE_BUILD_TYPEが定義されていません")
endif()

#######################################
# 外部パッケージ
find_package(yaml-cpp)
if(NOT yaml-cpp_FOUND)
  find_package(PkgConfig REQUIRED)
  pkg_check_modules(yaml-cpp yaml-cpp REQUIRED)
endif()

#######################################
# コンパイラに応じた警告設定
if(MSVC)
  add_compile_options(/W4 /WX)
else()
  add_compile_options(
    -Wall -Wextra
    -Wnon-virtual-dtor   # warn the user if a class with virtual functions has a non-virtual destructor.
                         # This helps catch hard to track down memory errors
    -Wold-style-cast     # warn for c-style casts
    -Wcast-align         # warn for potential performance problem casts
    -Wunused             # warn on anything being unused
    -Woverloaded-virtual # warn if you overload (not override) a virtual function
    -Wpedantic           # warn if non-standard C++ is used
    -Wconversion         # warn on type conversions that may lose data
    -Werror              # 警告をエラーとして扱う
  )
endif()

# ビルドコマンドを出力 (要 CMake 2.8.5)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

#######################################
# clang-tidyを実行させる (要 CMake 3.6)
if (USE_CLANG_TIDY)
  # clang-tidyの存在確認
  find_program(CLANG_TIDY_EXE clang-tidy)
  if(NOT CLANG_TIDY_EXE)
    message(FATAL_ERROR "clang-tidyが必要です")
  endif()

  set(CMAKE_CXX_CLANG_TIDY "clang-tidy;-header-filter=${CMAKE_SOURCE_DIR}/include")
endif()

#######################################
# CTest有効化
enable_testing()

# テストカバレッジ

if(COVERAGE)
  add_custom_target(
    coverage_init
    ALL
    COMMAND find ${PROJECT_BINARY_DIR} -name "*.gcda" -exec rm {} +
  )

  add_custom_target(
    coverage
    COMMAND mkdir -p ${PROJECT_BINARY_DIR}/coverage
    COMMAND gcovr -s -r ${PROJECT_SOURCE_DIR} --html --html-details -o ${PROJECT_BINARY_DIR}/coverage/coverage.html ${PROJECT_BINARY_DIR}/src
  )
endif()

#######################################
# GTest無効化
add_definitions(-DINSTALL_GTEST=OFF)
add_definitions(-DYAML_CPP_BUILD_TOOLS=OFF)

#######################################
# 本体
add_subdirectory(third_party/Catch2-2.9.2)
add_subdirectory(third_party/googletest-release-1.8.1)
add_subdirectory(third_party/spdlog-1.3.1)
add_subdirectory(docs)
add_subdirectory(src)
add_subdirectory(test)
