#ifndef BARELYFUNCTIONAL_MAP_TESTS_HPP
#define BARELYFUNCTIONAL_MAP_TESTS_HPP

#include "uncurry.hpp"

#include <catch2/catch.hpp>

#include <string_view>

TEST_CASE(
        "Uncurry{overload set} models "
        "Uncurry :: (a -> b -> c... -> n) -> (a, b, c...) -> n",
        "[Uncurry]")
{
    auto constexpr uncurried =
            Barely::Uncurry{[](int, double, std::string_view text) {
                return text;
            }};

    auto constexpr tuple = std::make_tuple(1, 2.0, std::string_view{"..."});

    STATIC_REQUIRE(uncurried(tuple) == "...");
}

#endif    // BARELYFUNCTIONAL_MAP_TESTS_HPP
