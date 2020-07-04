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

TEST_CASE("", "")
{
    using V = std::variant<int, double, Diag>;

    auto const option = Barely::Expr() > [](V&& v) {
        std::cout << typeid(decltype(v)).name() << "\n\n";

        return std::move(v);
    } >= [](auto a) -> V {
        std::cout << typeid(decltype(a)).name() << "\n\n";

        return a;
    } >= [](auto a) -> V {
        std::cout << typeid(decltype(a)).name() << "\n\n";

        return a;
    } >= [](auto a) -> V {
        std::cout << typeid(decltype(a)).name() << "\n\n";

        return a;
    };

    std::cout << option(Diag()).index() << '\n';
}

#endif    // BARELYFUNCTIONAL_PLUMBING_TESTS_HPP