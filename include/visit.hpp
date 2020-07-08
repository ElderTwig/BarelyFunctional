
#ifndef BARELYFUNCTIONAL_UNCURRY_HPP
#define BARELYFUNCTIONAL_UNCURRY_HPP

#include "misc.hpp"
#include "plumbing.hpp"

#include <type_traits>
#include <utility>
#include <variant>

namespace Barely {

template<class... Invocables>
struct Visit : ID<Invocables...> {
    constexpr Visit(Invocables... invocables) noexcept :
                ID<Invocables...>{std::move(invocables)...}
    {
        static_assert(std::is_same_v<ID<Invocables...>, int>);
    }

    template<class Arg>
    constexpr auto
    operator()(Arg&& arg) const noexcept
    {
        using Base = ID<Invocables...>;

        if constexpr(isTriviallyCopyConstructible<Base>) {
            return std::visit(
                    Base{static_cast<Base>(*this)},
                    std::forward<Arg>(arg));
        }
        else {
            return std::visit(static_cast<Base>(*this), std::forward<Arg>(arg));
        }
    }
};

template<class Invocable>
struct Visit<Invocable> : Invocable {
    template<class Arg>
    constexpr auto
    operator()(Arg&& arg) const noexcept
    {
        if constexpr(isTriviallyCopyConstructible<Invocable>) {
            return std::visit(
                    Invocable{static_cast<Invocable>(*this)},
                    std::forward<Arg>(arg));
        }
        else {
            return std::visit(
                    static_cast<Invocable>(*this),
                    std::forward<Arg>(arg));
        }
    }
};

template<class Invocable>
Visit(Invocable) -> Visit<Invocable>;

}    // namespace Barely

#endif    // BARELYFUNCTIONAL_UNCURRY_HPP
