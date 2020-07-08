#ifndef BARELYFUNCTIONAL_MISC_HPP
#define BARELYFUNCTIONAL_MISC_HPP

#include <type_traits>
#include <optional>

namespace Barely {

template<template<class...> class Template, class Specialisation>
struct IsSpecialisationOf : std::false_type {};

template<template<class...> class Template, class... Args>
struct IsSpecialisationOf<Template, Template<Args...>> : std::true_type {};

template<template<class...> class Template, class Specialisation>
auto constexpr isSpecialisationOf =
        IsSpecialisationOf<Template, Specialisation>::value;

template<class Optional>
using IsOptional = IsSpecialisationOf<std::optional, Optional>;

template<class Optional>
auto constexpr isOptional = IsOptional<Optional>::value;

template<class T, class = void>
struct CopyIfTrivial {
    using type = std::remove_reference_t<T>&;
};

template<class T>
struct CopyIfTrivial<
        T,
        std::enable_if_t<std::is_trivially_copy_constructible_v<
                std::remove_reference_t<T>>>> {
    using type = std::remove_reference_t<T>;
};

template<class T>
using CopyIfTrivial_t = typename CopyIfTrivial<T>::type;

}    // namespace Barely

#endif    // BARELYFUNCTIONAL_MISC_HPP
