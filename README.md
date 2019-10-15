# asr::util::Logger

ログ出力クラス

[![Build Status](https://travis-ci.org/asura/logger.svg?branch=master)](https://travis-ci.org/asura/logger)
 [![Coverage Status](https://coveralls.io/repos/github/asura/logger/badge.svg?branch=master)](https://coveralls.io/github/asura/logger?branch=master)

* [設計ドキュメント](https://asura.github.io/logger/design.pdf)
* [Doxygenドキュメント](https://asura.github.io/logger/)
* [テスト仕様](https://asura.github.io/logger/test_spec.pdf)

## requirement

* CMake v3.13以降
* C++11 compiler
  * 以下でテスト
    * g++ 5.4.0 @ Ubuntu 16.04
    * g++ 7.3.1 @ CentOS 7.6
    * clang++ 10.0.1 @ macOS Mojave 10.14.6
* clang-tidy (optional)

## cmake options

|オプション名|説明|デフォルト値|
|-|-|-|
|`COVERAGE`|テストカバレッジ取得|`OFF`|
|`USE_CLANG_TIDY`|`clang-tidy`を使った静的解析|`OFF`|
