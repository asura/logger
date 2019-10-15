# asr::util::Logger

ログ出力クラス

[![Build Status](https://travis-ci.org/asura/logger.svg?branch=master)](https://travis-ci.org/asura/logger)
 [![Coverage Status](https://coveralls.io/repos/github/asura/logger/badge.svg?branch=master)](https://coveralls.io/github/asura/logger?branch=master)

* [Doxygenドキュメント](https://asura.github.io/logger/)

## requirement

* CMake v3.13以降
* C++11 compiler
  * 以下でテスト
    * g++ 5.4.0 @ Ubuntu 16.04
    * g++ 7.3.1 @ CentOS 7.6
* yaml-cpp
  * Ubuntuの場合
    * `apt-get install libyaml-cpp-dev`
  * CentOS7の場合
    * `yum install yaml-cpp-devel`
  * macosの場合
    * `brew install yaml-cpp`
* clang-tidy (optional)

## cmake options

|オプション名|説明|デフォルト値|
|-|-|-|
|`COVERAGE`|テストカバレッジ取得|`OFF`|
|`USE_CLANG_TIDY`|`clang-tidy`を使った静的解析|`OFF`|
