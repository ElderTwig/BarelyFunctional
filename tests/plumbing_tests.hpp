#ifndef BARELYFUNCTIONAL_PLUMBING_TESTS_HPP
#define BARELYFUNCTIONAL_PLUMBING_TESTS_HPP

#include "plumbing.hpp"

#include <catch2/catch.hpp>

#include <iostream>

struct Counter {
    int copyConstructed = 0;
    int moveConstructed = 0;

    Counter(Counter const& c) :
                copyConstructed(c.copyConstructed + 1),
                moveConstructed(c.moveConstructed)
    {
        copyConstructed++;
    }

    Counter(Counter&& c) :
                copyConstructed(c.copyConstructed),
                moveConstructed(c.moveConstructed + 1)
    {}

    Counter&
    operator=(Counter const&) = delete;

    Counter&
    operator=(Counter&&) = delete;

    ~Counter() = default;
};

TEST_CASE("Barely::Inv interface", "[Inv]")
{
    SECTION("Inv can be constructed by a generic function object")
    {
        auto constexpr lambda = [] {
        };

        struct {
            constexpr auto
            operator()()
            {}
        } s;

        auto constexpr inv1 = Barely::Inv{lambda};
        auto constexpr inv2 = Barely::Inv{lambda};
    }

    SECTION("You can call")
    {}
}

#endif    // BARELYFUNCTIONAL_PLUMBING_TESTS_HPP