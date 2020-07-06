#ifndef BARELYFUNCTIONAL_MAP_HPP
#define BARELYFUNCTIONAL_MAP_HPP

#include "misc.hpp"
#include "plumbing.hpp"

#include <type_traits>
#include <optional>
#include <utility>
#include <array>

namespace Barely {

template<class T, class Invocable>
constexpr auto
map(std::optional<T> const& optional, Invocable&& invocable)
{
    using InvReturnT = std::invoke_result_t<Invocable, T>;

    if constexpr(isOptional<InvReturnT>) {
        if(optional.has_value()) {
            return invocable(*optional);
        }
        return InvReturnT{std::nullopt};
    }
    else {
        if(optional.has_value()) {
            return std::optional<InvReturnT>{
                    std::in_place,
                    invocable(*optional)};
        }
        return std::optional<InvReturnT>{std::nullopt};
    }
}

template<class T, class Invocable>
constexpr auto
map(std::optional<T>&& optional, Invocable&& invocable)
{
    using InvReturnT = std::invoke_result_t<Invocable, T>;

    if constexpr(isOptional<InvReturnT>) {
        if(optional.has_value()) {
            return invocable(*optional);
        }
        return InvReturnT{std::nullopt};
    }
    else {
        if(optional.has_value()) {
            return std::optional<InvReturnT>{
                    std::in_place,
                    invocable(*optional)};
        }
        return std::optional<InvReturnT>{std::nullopt};
    }
}

template<class T, size_t length, class Invocable>
constexpr auto
map(std::array<T, length> const& array, Invocable&& invocable)
{
    using ReturnT = decltype(invocable(array[0]));
    auto newArray = std::array<ReturnT, length>{};

    for(auto i = 0Lu; i < length; ++i) {
        newArray[i] = invocable(array[i]);
    }

    return newArray;
}

template<class T, size_t length, class Invocable>
constexpr auto
map(std::array<T, length>&& array, Invocable&& invocable)
{
    using ReturnT = decltype(invocable(array[0]));
    auto newArray = std::array<ReturnT, length>{};

    for(auto i = 0Lu; i < length; ++i) {
        newArray[i] = invocable(array[i]);
    }

    return newArray;
}

template<class... Invocables>
struct Map : Invocables... {
    using Invocables ::operator()...;
};

template<class... Invocables>
Map(Invocables...) -> Map<Invocables...>;

template<class... RightInvs>
constexpr auto
operator|(ID<>, Map<RightInvs...>&& rightInvocable) noexcept
{
    return ID{[rightInvocable = std::forward<Map<RightInvs...>>(
                       rightInvocable)](auto&& arg) {
        return map(std::forward<decltype(arg)>(arg), rightInvocable);
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

}    // namespace Barely

#endif    // BARELYFUNCTIONAL_MAP_HPP
