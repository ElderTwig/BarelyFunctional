#ifndef BARELYFUNCTIONAL_UNCURRY_HPP
#define BARELYFUNCTIONAL_UNCURRY_HPP

#include "misc.hpp"
#include "plumbing.hpp"

#include <type_traits>
#include <utility>
#include <tuple>

namespace Barely {

template<class... Invocables>
struct Uncurry : ID<Invocables...> {
    constexpr Uncurry(Invocables... invocables) noexcept :
                ID<Invocables...>{std::move(invocables)...}
    {}

    template<class Arg>
    constexpr auto
    operator()(Arg&& arg) const noexcept
    {
        using Base = ID<Invocables...>;

        if constexpr(isTriviallyCopyConstructible<Base>) {
            return std::apply(
                    Base{static_cast<Base>(*this)},
                    std::forward<Arg>(arg));
        }
        else {
            return std::apply(static_cast<Base>(*this), std::forward<Arg>(arg));
        }
    }
};

template<class Invocable>
struct Uncurry<Invocable> : Invocable {
    template<class Arg>
    constexpr auto
    operator()(Arg&& arg) const noexcept
    {
        if constexpr(isTriviallyCopyConstructible<Invocable>) {
            return std::apply(
                    Invocable{static_cast<Invocable>(*this)},
                    std::forward<Arg>(arg));
        }
        else {
            return std::apply(
                    static_cast<Invocable>(*this),
                    std::forward<Arg>(arg));
        }
    }
};

template<class Invocable>
Uncurry(Invocable) -> Uncurry<Invocable>;

}    // namespace Barely

#endif    // BARELYFUNCTIONAL_UNCURRY_HPP
