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

}    // namespace Barely

#endif    // BARELYFUNCTIONAL_MISC_HPP
