
#ifndef BARELYFUNCTIONAL_UNCURRY_HPP
#define BARELYFUNCTIONAL_UNCURRY_HPP

#include "misc.hpp"
#include "plumbing.hpp"

#include <type_traits>
#include <utility>
#include <variant>

namespace Barely {

template<class... Invocables>
struct Visit : Invocables... {
    using Invocables::operator()...;
};

template<class... Invocables>
Visit(Invocables...) -> Visit<Invocables...>;

template<class... RightInvs>
constexpr auto
operator|(ID<>, Visit<RightInvs...> rightInvocable)
{
    return ID{[rightInvocable = std::move(rightInvocable)](auto&& arg) {
        return std::visit(rightInvocable, std::forward<decltype(arg)>(arg));
    }};
}

template<class... LeftInvs, class... RightInvs>
constexpr auto
operator|(ID<LeftInvs...> leftInvocable, Visit<RightInvs...> rightInvocable)
{
    return ID{[leftInvocable  = std::move(leftInvocable),
               rightInvocable = std::move(rightInvocable)](auto&&... args) {
        return std::visit(
                rightInvocable,
                leftInvocable(std::forward<decltype(args)>(args)...));
    }};
}

}    // namespace Barely

#endif    // BARELYFUNCTIONAL_UNCURRY_HPP
