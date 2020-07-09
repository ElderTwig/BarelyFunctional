#ifndef BARELYFUNCTIONAL_VISIT_TESTS_HPP
#define BARELYFUNCTIONAL_VISIT_TESTS_HPP

#include "visit.hpp"

#include <catch2/catch.hpp>

#include <string_view>

TEST_CASE(
        "visit(overload set) models "
        "visit :: (a | b | c... -> common) -> std::variant<a, b, c...> -> common",
        "[Visit]")
{
    auto constexpr visit = Brly::visit(
            [](int) -> std::string_view { return "int"; },
            [](double) -> std::string_view { return "double"; },
            [](auto) -> std::string_view { return "deduced"; });

    struct AType {};

    using variant = std::variant<int, AType, double>;

    STATIC_REQUIRE(visit(variant{1}) == "int");
    STATIC_REQUIRE(visit(variant{2.0}) == "double");
    STATIC_REQUIRE(visit(variant{AType{}}) == "deduced");
}

#endif    // BARELYFUNCTIONAL_VISIT_TESTS_HPP
