#ifndef BARELYFUNCTIONAL_VISIT_HPP
#define BARELYFUNCTIONAL_VISIT_HPP

#include "misc.hpp"
#include "plumbing.hpp"

#include <type_traits>
#include <utility>
#include <variant>

namespace Brly {

template<class... Invocables>
constexpr auto
visit(Invocables... invocables) noexcept
{
    return ID{[invocable = ID{std::move(invocables)...}](auto&& arg) {
        using Invocable = decltype(invocable);

        if constexpr(isTriviallyCopyConstructible<Invocable>) {
            return std::visit(
                    Invocable{invocable},
                    std::forward<decltype(arg)>(arg));
        }
        else {
            return std::visit(invocable, std::forward<decltype(arg)>(arg));
        }
    }};
}

}    // namespace Brly

#endif    // BARELYFUNCTIONAL_VISIT_HPP
