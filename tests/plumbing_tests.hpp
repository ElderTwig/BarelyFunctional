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

#endif    // BARELYFUNCTIONAL_PLUMBING_TESTS_HPP