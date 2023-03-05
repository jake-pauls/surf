#include <catch2/catch.hpp>

#pragma optimize("", off)

TEST_CASE("Sample")
{
    SECTION("Logic")
    {
        REQUIRE(2 + 2 == 4);
    }

    SECTION("Reason")
    {
        REQUIRE(true);
    }
}