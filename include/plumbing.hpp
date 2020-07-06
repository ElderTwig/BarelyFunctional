#ifndef BARELYFUNCTIONAL_PLUMBING_HPP
#define BARELYFUNCTIONAL_PLUMBING_HPP

#include "misc.hpp"
#include "map.hpp"

#include <type_traits>
#include <utility>
#include <variant>
#include <optional>
#include <tuple>
#include <array>

namespace Barely {

template<class... Invocables>
struct ID : Invocables... {
    using Invocables::operator()...;
};

template<class... Invocables>
ID(Invocables...) -> ID<Invocables...>;

template<class... Invocables>
struct Map : Invocables... {
    using Invocables ::operator()...;
};

template<class... Invocables>
Map(Invocables...) -> Map<Invocables...>;

template<class RightInvocable>
constexpr auto
operator|(ID<>, RightInvocable&& rightInvocable) noexcept
{
    return ID{std::forward<RightInvocable>(rightInvocable)};
}

template<class... LeftInvocable, class RightInvocable>
constexpr auto
operator|(
        ID<LeftInvocable...>&& leftInvocable,
        RightInvocable&& rightInvocable) noexcept
{
    return ID{[rightInvocable = std::forward<RightInvocable>(rightInvocable),
               leftInvocable  = std::forward<ID<LeftInvocable...>>(
                       leftInvocable)](auto&&... args) {
        return rightInvocable(
                leftInvocable(std::forward<decltype(args)>(args)...));
    }};
}

template<class... RightInvs>
constexpr auto
operator|(ID<>, Map<RightInvs...>&& rightInvocable) noexcept
{
    return ID{[rightInvocable = std::forward<Map<RightInvs...>>(
                       rightInvocable)](auto&&... args) {
        return map(std::forward<decltype(args)>(args)..., rightInvocable);
    }};
}

template<class... LeftInvs, class... RightInvs>
constexpr auto
operator|(
        ID<LeftInvs...>&& leftInvocable,
        Map<RightInvs...>&& rightInvocable) noexcept
{
    return ID{[rightInvocable = std::forward<Map<RightInvs...>>(rightInvocable),
               leftInvocable  = std::forward<ID<LeftInvs...>>(leftInvocable)](
                      auto&&... args) {
        return map(
                leftInvocable(std::forward<decltype(args)>(args)...),
                rightInvocable);
    }};
}

auto constexpr a  = ID{} | Map{[](auto a) {
                       return a * 2;
                   }};
auto constexpr aa = std::array{5, 2};
auto constexpr b  = a(aa);
auto constexpr c  = b[0];

}    // namespace Barely

#endif    // BARELYFUNCTIONAL_PACK_HPP
