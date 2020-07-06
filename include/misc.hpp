#ifndef BARELYFUNCTIONAL_MISC_HPP
#define BARELYFUNCTIONAL_MISC_HPP

#include <type_traits>
#include <optional>

namespace Barely {

template<template<class...> class Template, class Specialisation>
struct IsSpecialisationOf : std::false_type {};

template<template<class...> class Template, class... Args>
struct IsSpecialisationOf<Template, Template<Args...>> : std::true_type {};

template<class T>
using IsOptional = IsSpecialisationOf<std::optional, T>;

template<class T>
auto constexpr isOptional = IsOptional<T>::value;

}    // namespace Barely

#endif    // BARELYFUNCTIONAL_MISC_HPP
