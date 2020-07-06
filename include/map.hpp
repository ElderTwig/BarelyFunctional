#ifndef BARELYFUNCTIONAL_MAP_HPP
#define BARELYFUNCTIONAL_MAP_HPP

#include "misc.hpp"

#include <type_traits>
#include <optional>
#include <utility>
#include <tuple>

namespace Barely {

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

template<size_t... Is, class... Ts, class Invocable>
constexpr auto
map(std::tuple<Ts...>&& tuple,
    Invocable&& invocable,
    std::integer_sequence<size_t, Is...>)
{
    return std::make_tuple(
            invocable(std::get<Is>(std::forward<std::tuple<Ts...>>(tuple)))...);
}

}    // namespace Barely

#endif    // BARELYFUNCTIONAL_MAP_HPP
