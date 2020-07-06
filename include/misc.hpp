#ifndef BARELYFUNCTIONAL_MISC_HPP
#define BARELYFUNCTIONAL_MISC_HPP

#include <type_traits>
#include <variant>
#include <optional>
#include <tuple>

namespace Barely {

template<template<class...> class Template, class Specialisation>
struct IsSpecialisationOf : std::false_type {};

template<template<class...> class Template, class... Args>
struct IsSpecialisationOf<Template, Template<Args...>> : std::true_type {};

template<class T>
using IsOptional = IsSpecialisationOf<std::optional, T>;

template<class T>
auto constexpr isOptional = IsOptional<T>::value;

template<class T>
using IsTuple = IsSpecialisationOf<std::tuple, T>;

template<class T>
auto constexpr isTuple = IsTuple<T>::value;

template<class... Invocables>
struct Overload : Invocables... {
    using Invocables::operator()...;
};

template<class... Invocables>
Overload(Invocables...) -> Overload<Invocables...>;

template<typename T>
struct Tag {};

}    // namespace Barely

#endif    // BARELYFUNCTIONAL_MISC_HPP
