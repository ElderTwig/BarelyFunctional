#ifndef BARELYFUNCTIONAL_MISC_TESTS_HPP
#define BARELYFUNCTIONAL_MISC_TESTS_HPP

#include "misc.hpp"

#include <catch2/catch.hpp>

#include <type_traits>
#include <utility>

TEST_CASE(
        "IsSpecialisationOf and its derivatives",
        "[IsSpecialisationOf][isSpecialisationOf][IsOptional][isOptional]")
{
    struct AType {};

    SECTION("IsSpecialisationOf::value is true if second argument "
            "is a template specialisation of the first")
    {
        STATIC_REQUIRE(Barely::isSpecialisationOf<
                       std::is_same,
                       std::is_same<AType, AType>>);

        STATIC_REQUIRE_FALSE(Barely::isSpecialisationOf<
                             std::invoke_result,
                             std::is_trivial<AType>>);
    }

    SECTION("IsOptional::value is true if argument is a "
            "template specialisation of std::optional")
    {
        STATIC_REQUIRE(Barely::isOptional<std::optional<AType>>);

        STATIC_REQUIRE_FALSE(Barely::isOptional<AType>);

        STATIC_REQUIRE_FALSE(Barely::isOptional<std::is_trivial<AType>>);
    }
}

TEST_CASE(
        "CopyIfTrivial_t is T or T& depending on if "
        "T is trivially copy constructible",
        "[CopyIfTrivial][CopyIfTrivial_t]")
{
    struct Trivial {
        int a;
    };

    static_assert(std::is_trivially_copy_constructible_v<Trivial>);

    struct NonTrivial {
        NonTrivial(NonTrivial const&)
        {
            auto* p = new int;
            delete p;
        }

        NonTrivial(NonTrivial&&) = default;

        NonTrivial&
        operator=(NonTrivial const&) = default;
        NonTrivial&
        operator=(NonTrivial&&) = default;

        ~NonTrivial() = default;
    };

    static_assert(!std::is_trivially_copy_constructible_v<NonTrivial>);

    STATIC_REQUIRE(std::is_same_v<Barely::CopyIfTrivial_t<Trivial>, Trivial>);
    STATIC_REQUIRE(
            std::is_same_v<Barely::CopyIfTrivial_t<NonTrivial>, NonTrivial&>);
}

#endif    // BARELYFUNCTIONAL_MISC_TESTS_HPP
