#ifndef BARELYFUNCTIONAL_UNCURRY_TESTS_HPP
#define BARELYFUNCTIONAL_UNCURRY_TESTS_HPP

#include "uncurry.hpp"
#include "testUtils.hpp"

#include <catch2/catch.hpp>

#include <string_view>

TEST_CASE(
        "uncurry(overload set) models "
        "uncurry :: (a -> b -> c... -> n) -> (a, b, c...) -> n",
        "[Uncurry]")
{
    auto constexpr uncurry = Brly::uncurry(
            [](int, double, std::string_view text) { return text; });

    auto constexpr tuple = std::make_tuple(1, 2.0, std::string_view{"..."});

    STATIC_REQUIRE(uncurry(tuple) == "...");
}

#endif    // BARELYFUNCTIONAL_UNCURRY_TESTS_HPP
