#include "CErrRedirecter.h"

#include <catch2/catch.hpp>

TEST_CASE(
    "CErrRedirecterによってstderrの参照先が変更される",
    "[CErrRedirecter]")
{
    auto old = stderr;

    {
        CErrRedirecter sut;

        CHECK(sut.old() == old);
        CHECK(stderr != old);
        CHECK(stderr == sut.fp());
    }

    CHECK(stderr == old);
}

TEST_CASE(
    "CErrRedirecter後、cerrへの出力は特に変更されない",
    "[CErrRedirecter]")
{
    auto old = std::cerr.rdbuf();

    CErrRedirecter sut;

    CHECK(old == std::cerr.rdbuf());
}

TEST_CASE(
    "CErrRedirecter後、stderrの出力先が内部バッファへと変更される",
    "[CErrRedirecter]")
{
    CErrRedirecter sut;
    CHECK(stderr == sut.fp());
    // note: 書き込み前はsut.data()にアクセスしないこと。
    // まだバッファが作られていないので。

    const std::string test("テスト");
    const auto written_size = fwrite(
        test.c_str(),
        sizeof(char),
        test.length(),
        stderr);
    CHECK(written_size == test.length());
    fflush(stderr);

    CHECK(sut.data() == test);
}

TEST_CASE(
    "CErrRedirecter連続利用",
    "[CErrRedirecter]")
{
    {
        CErrRedirecter sut;
        const std::string test("テスト");
        const auto written_size = fwrite(
            test.c_str(),
            sizeof(char),
            test.length(),
            stderr);
        fflush(stderr);
        CHECK(written_size == test.length());
    }
    {
        CErrRedirecter sut;
        const std::string test("テスト");
        const auto written_size = fwrite(
            test.c_str(),
            sizeof(char),
            test.length(),
            stderr);
        fflush(stderr);
        CHECK(written_size == test.length());
    }
}
