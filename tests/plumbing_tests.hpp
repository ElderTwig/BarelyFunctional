#ifndef BARELYFUNCTIONAL_PLUMBING_TESTS_HPP
#define BARELYFUNCTIONAL_PLUMBING_TESTS_HPP

#include "plumbing.hpp"
#include "testUtils.hpp"

#include <catch2/catch.hpp>

#include <type_traits>
#include <string_view>

TEST_CASE(
        "ID::operator() is an identity function overload set "
        "for the function objects it wraps",
        "[ID][ID::operator()]")
{
    auto constexpr foo = [](int) -> std::string_view {
        return "integer";
    };
    auto constexpr bar = [](double) -> std::string_view {
        return "double";
    };
    auto constexpr fun = []() -> std::string_view {
        return "nothing";
    };
    auto constexpr bun = [](auto) -> std::string_view {
        return "deduced";
    };

    auto constexpr a = Brly::ID{foo, bar, fun, bun};
    auto constexpr b = Brly::ID{bun, foo};
    auto constexpr c = Brly::ID{bun};

    STATIC_REQUIRE(a(5) == "integer");
    STATIC_REQUIRE(a(5.0) == "double");
    STATIC_REQUIRE(a() == "nothing");
    STATIC_REQUIRE(a(true) == "deduced");

    STATIC_REQUIRE(b(5) == "integer");
    STATIC_REQUIRE(b(5.0) == "deduced");

    STATIC_REQUIRE(c(2) == "deduced");
}

TEST_CASE(
        "ID operator| composes left and right function objects",
        "[ID][ID::operator|]")
{
    SECTION("An empty ID on the left constructs ID{right}")
    {
        auto constexpr foo = [i = 5] {
            return i;
        };

        auto constexpr a = Brly::ID{} | foo;
        auto constexpr b = Brly::ID{foo};

        STATIC_REQUIRE(std::is_same_v<decltype(a), decltype(b)>);
    }

    auto constexpr foo = [](auto counter) {
        return counter;
    };

    auto constexpr bar = [](auto const counter) {
        return counter;
    };

    auto constexpr bun = [](auto&& counter) {
        return std::forward<decltype(counter)>(counter);
    };

    auto constexpr idFoo       = Brly::ID{foo};
    auto constexpr idFooBar    = idFoo | bar;
    auto constexpr idFooBarBun = idFoo | bar | bun;

    SECTION("Non-empty ID on the left constructs a callable "
            "that forwards any arguments to the left invocable "
            "and calls the right invocable with the result of "
            "that expression")
    {
        STATIC_REQUIRE(idFoo(Counter{}).copyConstructed == 0);
        STATIC_REQUIRE(idFoo(Counter{}).moveConstructed == 1);

        STATIC_REQUIRE(idFooBar(Counter{}).copyConstructed == 1);
        STATIC_REQUIRE(idFooBar(Counter{}).moveConstructed == 2);

        STATIC_REQUIRE(idFooBarBun(Counter{}).copyConstructed == 1);
        STATIC_REQUIRE(idFooBarBun(Counter{}).moveConstructed == 3);
    }

    SECTION("Anonymous lambdas compose the same as named lambdas "
            "if they are closureless")
    {
        auto constexpr anonIdFooBarBun =
                Brly::ID{[](auto counter) {
                    return counter;
                }}
                |
                [](auto const counter) {
                    return counter;
                }
                | [](auto&& counter) {
                      return std::forward<decltype(counter)>(counter);
                  };

        STATIC_REQUIRE(
                idFooBarBun(Counter{}).copyConstructed
                == anonIdFooBarBun(Counter{}).copyConstructed);

        STATIC_REQUIRE(
                idFooBarBun(Counter{}).moveConstructed
                == anonIdFooBarBun(Counter{}).moveConstructed);
    }
}

#endif    // BARELYFUNCTIONAL_PLUMBING_TESTS_HPP
