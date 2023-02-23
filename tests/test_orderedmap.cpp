#include <catch2/catch_test_macros.hpp>

extern "C"
{
    #include <libcmap.h>
}

TEST_CASE("Ordered map", "[orderedmap]") {

    SECTION("init") {
        orderedmap_t map;

        REQUIRE(orderedmap_init(&map) == 0);
        REQUIRE(orderedmap_destroy(&map) == 0);
    }
}
