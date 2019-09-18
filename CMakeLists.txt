cmake_minimum_required(VERSION 3.6 FATAL_ERROR)

project(
  asr_util_logger
  VERSION 1.0.0
  LANGUAGES CXX
)

# C++規格
set(CMAKE_CXX_STANDARD 11)

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

# ビルドコマンドを出力 (要 CMake 2.8.5)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

#######################################
# clang-tidyを実行させる (要 CMake 3.6)
set(CMAKE_CXX_CLANG_TIDY "clang-tidy;-header-filter=${CMAKE_SOURCE_DIR}/include")

#######################################
# CTest有効化
enable_testing()

# テストカバレッジ : デフォルトはOFF
SET(COVERAGE OFF CACHE BOOL "Coverage")

#######################################
# GTest無効化
add_definitions(-DINSTALL_GTEST=OFF)

#######################################
# 本体
add_subdirectory(third_party/Catch2-2.9.2)
add_subdirectory(third_party/googletest-release-1.8.1)
add_subdirectory(third_party/spdlog-1.3.1)
add_subdirectory(docs)
add_subdirectory(src)
add_subdirectory(test)
