#ifndef BARELYFUNCTIONAL_MISC_HPP
#define BARELYFUNCTIONAL_MISC_HPP

#include <type_traits>
#include <variant>
#include <optional>

namespace Barely {

template<class = void>
struct IsOptional : std::false_type {};

template<class T>
struct IsOptional<std::optional<T>> : std::true_type {};

template<class Optional>
auto constexpr isOptional = IsOptional<Optional>::value;

template<class... Invocables>
struct Overload : Invocables... {
    using Invocables::operator()...;
};

template<class... Invocables>
Overload(Invocables...) -> Overload<Invocables...>;

}    // namespace Barely

#endif    // BARELYFUNCTIONAL_MISC_HPP