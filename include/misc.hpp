#ifndef BARELYFUNCTIONAL_MISC_HPP
#define BARELYFUNCTIONAL_MISC_HPP

#include <type_traits>
#include <variant>

namespace Barely {

template<class...>
struct Concat {};

template<template<class...> class Compound, class... Us>
struct Concat<Compound<Us...>> {
    using type = Compound<Us...>;
};

template<
        template<class...>
        class Compound,
        class... Us,
        class... Ts,
        class... Compounds>
struct Concat<Compound<Us...>, Compound<Ts...>, Compounds...> {
    using type = typename Concat<Compound<Us..., Ts...>, Compounds...>::type;
};

template<class... Compounds>
using Concat_t = typename Concat<Compounds...>::type;

}    // namespace Barely

#endif    // BARELYFUNCTIONAL_MISC_HPP