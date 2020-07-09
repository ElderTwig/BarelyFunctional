#ifndef BARELYFUNCTIONAL_UNCURRY_HPP
#define BARELYFUNCTIONAL_UNCURRY_HPP

#include "misc.hpp"
#include "plumbing.hpp"

#include <type_traits>
#include <utility>
#include <tuple>

namespace Brly {

template<class... Invocables>
constexpr auto
uncurry(Invocables... invocables) noexcept
{
    return ID{[invocable = ID{std::move(invocables)...}](auto&& arg) {
        using Invocable = decltype(invocable);

        if constexpr(isTriviallyCopyConstructible<Invocable>) {
            return std::apply(
                    Invocable{invocable},
                    std::forward<decltype(arg)>(arg));
        }
        else {
            return std::apply(invocable, std::forward<decltype(arg)>(arg));
        }
    }};
}

}    // namespace Brly

#endif    // BARELYFUNCTIONAL_UNCURRY_HPP
