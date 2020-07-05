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

constexpr auto
doStuff() noexcept
{
    return 5.0;
}

constexpr auto
doOtherStuff() noexcept
{
    return 6.0;
}

TEST_CASE("", "")
{
    struct Close {};
    struct Forward {};

    using Actions = std::variant<Close, Forward>;

    auto constexpr a = [](auto&& a) {
        std::cout << __PRETTY_FUNCTION__;
        if constexpr(std::is_same_v<Diag, decltype(a)>) {
            return doStuff();
        }
        else {
            return doOtherStuff();
        }
    } |= Barely::End{};

    auto const i = a.unwrap(std::optional<Diag>{Diag()});
}

#endif    // BARELYFUNCTIONAL_PLUMBING_TESTS_HPP