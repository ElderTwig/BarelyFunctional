#ifndef BARELYFUNCTIONAL_PLUMBING_HPP
#define BARELYFUNCTIONAL_PLUMBING_HPP

#include "misc.hpp"
#include "map.hpp"

#include <type_traits>
#include <utility>
#include <variant>
#include <optional>
#include <tuple>

namespace Barely {

template<class... Ts, class RightInvocable>
constexpr auto
unwrapInto(
        std::variant<Ts...>&& variant,
        RightInvocable&& rightInvocable) noexcept
{
    return std::visit(
            std::forward<RightInvocable>(rightInvocable),
            std::forward<std::variant<Ts...>>(variant));
}

template<class... Ts, class RightInvocable>
constexpr auto
unwrapInto(std::tuple<Ts...>&& args, RightInvocable&& rightInvocable) noexcept
{
    return std::apply(
            std::forward<RightInvocable>(rightInvocable),
            std::forward<std::tuple<Ts...>>(args));
}

template<class T, class RightInvocable>
constexpr auto
unwrapInto(std::optional<T>&& option, RightInvocable&& rightInvocable) noexcept
{
    using ReturnT = std::invoke_result_t<RightInvocable, T>;

    if constexpr(isOptional<ReturnT>) {
        if(option.has_value()) {
            return rightInvocable(*option);
        }

        return ReturnT{std::nullopt};
    }
    else {
        if(option.has_value()) {
            return std::optional<ReturnT>{rightInvocable(*option)};
        }

        return std::optional<ReturnT>{std::nullopt};
    }
}

template<class Invocable>
struct ID : Invocable {
    using Invocable::operator();
};

template<class Invocable>
ID(Invocable) -> ID<Invocable>;

template<class Invocable>
struct Map : Invocable {
    using Invocable::operator();
};

template<class Invocable>
Map(Invocable) -> Map<Invocable>;

template<class LeftInvocable, class RightInvocable>
constexpr auto
operator|(
        ID<LeftInvocable>&& leftInvocable,
        RightInvocable&& rightInvocable) noexcept
{
    return ID{[rightInvocable = std::forward<RightInvocable>(rightInvocable),
               leftInvocable  = std::forward<LeftInvocable>(leftInvocable)](
                      auto&&... args) {
        return rightInvocable(
                leftInvocable(std::forward<decltype(args)>(args)...));
    }};
}

template<class LeftInvocable, class RightInvocable>
constexpr auto
operator|(
        ID<LeftInvocable>&& leftInvocable,
        Map<RightInvocable>&& rightInvocable) noexcept
{
    return ID{[rightInvocable = std::forward<RightInvocable>(rightInvocable),
               leftInvocable  = std::forward<LeftInvocable>(leftInvocable)](
                      auto&&... args) {
        using LeftReturnT =
                std::invoke_result_t<LeftInvocable, decltype(args)...>;

        if constexpr(isTuple<LeftReturnT>) {
            return map(
                    leftInvocable(std::forward<decltype(args)>(args)...),
                    rightInvocable,
                    std::make_index_sequence<std::tuple_size_v<
                            std::remove_reference_t<LeftReturnT>>>{});
        }
        else {
            return map(
                    leftInvocable(std::forward<decltype(args)>(args)...),
                    rightInvocable);
        }
    }};
}

auto constexpr a = ID{[] {
                       return std::make_tuple(1, 2.0);
                   }}
                   | Map{[](auto a) {
                         return a * 2;
                     }};
auto constexpr b = a();

}    // namespace Barely

#endif    // BARELYFUNCTIONAL_PACK_HPP
