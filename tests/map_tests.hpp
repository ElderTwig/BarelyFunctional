#ifndef BARELYFUNCTIONAL_MAP_TESTS_HPP
#define BARELYFUNCTIONAL_MAP_TESTS_HPP

#include "map.hpp"

#include <catch2/catch.hpp>

#include <string_view>

TEST_CASE("map models ((a -> b), [a]) -> [b]", "[map]")
{
    auto constexpr foo = [](auto a) {
        return a * 2;
    };
    auto constexpr bar = [](auto) -> std::string_view {
        return "now a string_view";
    };

    SECTION("map on std::array")
    {
        auto constexpr array = std::array{2, 3, 4};

        auto constexpr result1 = Barely::map(foo, array);
        auto constexpr result2 = Barely::map(bar, array);

        for(auto i = 0Lu; i < array.size(); ++i) {
            REQUIRE(result1[i] == (array[i] * 2));
            REQUIRE(result2[i] == "now a string_view");
        }
    }
    SECTION("map on std::optional")
    {
        auto constexpr bun = [](auto) -> std::optional<std::string_view> {
            return std::optional{"opt of string_view"};
        };

        auto constexpr optional      = std::optional{1};
        auto constexpr emptyOptional = std::optional<int>{std::nullopt};

        STATIC_REQUIRE(Barely::map(foo, optional).has_value());
        STATIC_REQUIRE_FALSE(Barely::map(foo, emptyOptional).has_value());

        STATIC_REQUIRE(std::is_same_v<
                       decltype(Barely::map(bar, optional)),
                       std::optional<std::string_view>>);

        STATIC_REQUIRE(std::is_same_v<
                       decltype(Barely::map(bar, emptyOptional)),
                       std::optional<std::string_view>>);

        STATIC_REQUIRE(std::is_same_v<
                       decltype(Barely::map(bun, optional)),
                       std::optional<std::string_view>>);

        STATIC_REQUIRE(std::is_same_v<
                       decltype(Barely::map(bun, emptyOptional)),
                       std::optional<std::string_view>>);
    }
}

TEST_CASE(
        "Map{overload set} models "
        "Map :: (a -> b) -> [a] -> [b]",
        "[Map]")
{
    auto constexpr foo = [](int) -> std::optional<double> {
        return 2.0;
    };

    auto constexpr bar = [](double) -> std::string_view {
        return "double";
    };

    auto constexpr mapMap = Barely::Map{foo, bar};

    auto constexpr optInt    = std::optional{1};
    auto constexpr optDouble = std::optional<double>(std::nullopt);

    STATIC_REQUIRE(
            std::is_same_v<decltype(mapMap(optInt)), std::optional<double>>);
    STATIC_REQUIRE(*mapMap(optInt) == 2.0);

    STATIC_REQUIRE(std::is_same_v<
                   decltype(mapMap(optDouble)),
                   std::optional<std::string_view>>);
    STATIC_REQUIRE_FALSE(mapMap(optDouble).has_value());
}

#endif    // BARELYFUNCTIONAL_MAP_TESTS_HPP
