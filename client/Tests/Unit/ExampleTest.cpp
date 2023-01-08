#include <catch2/catch.hpp>

#include <surf/surf.h>

#pragma optimize("", off)

TEST_CASE("Reason")
{
    REQUIRE(surf_BridgeTestReturn());
}