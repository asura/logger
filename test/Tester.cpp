#define CATCH_CONFIG_RUNNER
#include <gmock/gmock.h>

#include <catch.hpp>

int main(int the_argc, char** the_argv) noexcept
{
    try
    {
        ::testing::GTEST_FLAG(throw_on_failure) = true;
        ::testing::InitGoogleMock(&the_argc, the_argv);

        const int result = Catch::Session().run(the_argc, the_argv);

        return result;
    }
    catch (...)
    {
        return -1;
    }
}
