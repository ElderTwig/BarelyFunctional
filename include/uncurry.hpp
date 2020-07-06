#ifndef BARELYFUNCTIONAL_UNCURRY_HPP
#define BARELYFUNCTIONAL_UNCURRY_HPP

#include "misc.hpp"
#include "plumbing.hpp"

#include <type_traits>
#include <utility>
#include <tuple>

namespace Barely {

template<class... Invocables>
struct Uncurry : Invocables... {
    using Invocables::operator()...;
};

template<class... Invocables>
Uncurry(Invocables...) -> Uncurry<Invocables...>;

template<class... RightInvs>
constexpr auto
operator|(ID<>, Uncurry<RightInvs...>&& rightInvocable) noexcept
{
    return ID{[rightInvocable = std::forward<Uncurry<RightInvs...>>(
                       rightInvocable)](auto&& arg) {
        return std::apply(rightInvocable, std::forward<decltype(arg)>(arg));
    }};
}

template<class... LeftInvs, class... RightInvs>
constexpr auto
operator|(
        ID<LeftInvs...>&& leftInvocable,
        Uncurry<RightInvs...>&& rightInvocable) noexcept
{
    return ID{[rightInvocable =
                       std::forward<Uncurry<RightInvs...>>(rightInvocable),
               leftInvocable = std::forward<ID<LeftInvs...>>(leftInvocable)](
                      auto&&... args) {
        return std::apply(
                leftInvocable(std::forward<decltype(args)>(args)...),
                rightInvocable);
    }};
}

auto constexpr f = [] {
    return std::make_tuple(2, 3);
};
auto constexpr c = ID{} | Uncurry{[](auto a, auto b) {
                       return a + b;
                   }};
auto constexpr g = ID{f} | c;
auto constexpr a = c(std::make_tuple(1, 5));
auto constexpr b = g();

}    // namespace Barely

#endif    // BARELYFUNCTIONAL_UNCURRY_HPP