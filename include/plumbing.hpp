#ifndef BARELYFUNCTIONAL_PLUMBING_HPP
#define BARELYFUNCTIONAL_PLUMBING_HPP

#include "misc.hpp"

#include <type_traits>
#include <utility>

namespace Barely {

template<class... Invocables>
struct ID : Invocables... {
    using Invocables::operator()...;
};

template<class... Invocables>
ID(Invocables...) -> ID<Invocables...>;

template<class RightInvocable>
constexpr auto
operator|(ID<>, RightInvocable rightInvocable) noexcept -> auto
{
    return ID{std::move(rightInvocable)};
}

template<class... LeftInvocable, class RightInvocable>
constexpr auto
operator|(
        ID<LeftInvocable...> leftInvocable,
        RightInvocable rightInvocable) noexcept
{
    return ID{[leftInvocable  = std::move(leftInvocable),
               rightInvocable = std::move(rightInvocable)](auto&&... args) {
        return rightInvocable(
                leftInvocable(std::forward<decltype(args)>(args)...));
    }};
}

}    // namespace Barely

#endif    // BARELYFUNCTIONAL_PACK_HPP
