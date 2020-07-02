#ifndef BARELYFUNCTIONAL_MISC_TESTS_HPP
#define BARELYFUNCTIONAL_MISC_TESTS_HPP

#include "misc.hpp"

#include <catch2/catch.hpp>

TEST_CASE("Concat concattenates compound types", "[static][Concat][Concat_t]")
{
    SECTION("std::variant")
    {
        using var1 = std::variant<int, double>;
        using var2 = std::variant<float, char>;

        using result = BarelyFunctional::Concat_t<var1, var2, var1>;

        STATIC_REQUIRE(std::is_same_v<
                       result,
                       std::variant<int, double, float, char, int, double>>);
    }

    SECTION("std::tuple")
    {
        using var1 = std::tuple<int, double>;
        using var2 = std::tuple<float, char>;

        using result = BarelyFunctional::Concat_t<var1, var2, var1>;

        STATIC_REQUIRE(std::is_same_v<
                       result,
                       std::tuple<int, double, float, char, int, double>>);
    }
}

#endif    // BARELYFUNCTIONAL_MISC_TESTS_HPP