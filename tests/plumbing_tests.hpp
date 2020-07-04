#ifndef BARELYFUNCTIONAL_PLUMBING_TESTS_HPP
#define BARELYFUNCTIONAL_PLUMBING_TESTS_HPP

#include "plumbing.hpp"

#include <catch2/catch.hpp>

#include <optional>
#include <type_traits>
#include <iostream>
#include <typeinfo>

struct Diag {
    Diag(Diag const&)
    {
        std::cerr << "Copy constructor\n";
    }

    Diag(Diag&&)
    {
        std::cerr << "Move constructor\n";
    }

    Diag()
    {
        std::cerr << "Default constructor\n";
    }

    Diag&
    operator=(Diag const&)
    {
        std::cerr << "Copy assignment\n";
        return *this;
    }

    Diag&
    operator=(Diag&&)
    {
        std::cerr << "Move assignment\n";
        return *this;
    }
};

auto
doStuff()
{}

auto
doOtherStuff()
{}

TEST_CASE("", "")
{
    struct Close {};
    struct Forward {};

    using Actions = std::variant<Close, Forward>;

    auto const option = Barely::Expr() >= [](auto a) {
        if constexpr(std::is_same_v<Close, decltype(a)>) {
            return doStuff();
        }
        else {
            return doOtherStuff();
        }
    };
}

#endif    // BARELYFUNCTIONAL_PLUMBING_TESTS_HPP