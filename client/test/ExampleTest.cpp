#include <catch2/catch.hpp>

#include <surf/surf.h>

/*
 * Disable optimize in unit tests for more verbose traces/errors
 */
#pragma optimize("", off)

TEST_CASE("Reason")
{
    REQUIRE(surf_BridgeTestReturn());
}