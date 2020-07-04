#ifndef BARELYFUNCTIONAL_PLUMBING_TESTS_HPP
#define BARELYFUNCTIONAL_PLUMBING_TESTS_HPP

#include "plumbing.hpp"

#include <catch2/catch.hpp>

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
    /*
    auto constexpr cal = Barely::Entry() > [](int a) {
        return a;    // 1
    } > [](int a) {
        return a;    // 1
    } > [](int a) {
        return a;    // 1
    };

    cal(1);

    auto constexpr bb = [](auto&& a) {
        std::cout << typeid(decltype(a)).name() << "\n\n";

        return std::tuple<
                std::variant<int, double, Diag>,
                std::variant<int, double, Diag>>{
                std::forward<decltype(a)>(a),
                std::forward<decltype(a)>(a)};
    };

    auto constexpr cc =
            Barely::Entry() > [](auto&& a) -> std::variant<int, double, Diag> {
        return {std::forward<decltype(a)>(a)};
    } >= bb >= [=](auto&&... a) {
        (bb(std::forward<decltype(a)>(a)), ...);
    };

    cc(1);
    cc(1.0);
    cc(Diag());
    */

    auto constexpr option = Barely::Entry() > [](auto&& a) {
        std::cout << typeid(decltype(a)).name() << "\n\n";

        return std::optional<Diag>{std::nullopt};
    } >= [](auto&& a) {
        std::cout << typeid(decltype(a)).name() << "\n\n";

        return std::move(a);
    } > [](auto&& a) {
        std::cout << typeid(decltype(a)).name() << "\n\n";

        return std::optional{a};
    } >= [](auto&& a) {
        std::cout << typeid(decltype(a)).name() << "\n\n";

        return 5;
    };

    auto const o = option(Diag());

    std::cout << o.has_value() << '\n';
}

#endif    // BARELYFUNCTIONAL_PLUMBING_TESTS_HPP