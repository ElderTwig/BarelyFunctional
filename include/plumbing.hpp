#ifndef BARELYFUNCTIONAL_PLUMBING_HPP
#define BARELYFUNCTIONAL_PLUMBING_HPP

#include "misc.hpp"

#include <type_traits>
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
    using ReturnType = std::invoke_result_t<
            RightInvocable,
            std::tuple_element_t<0, std::tuple<Ts...>>>;

    if constexpr((std::is_same_v<
                          ReturnType,
                          std::invoke_result_t<RightInvocable, Ts>> && ...)) {
        return std::visit(
                std::forward<RightInvocable>(rightInvocable),
                std::forward<std::variant<Ts...>>(variant));
    }
    else {
        return std::visit(
                [rightInvocable = std::forward<RightInvocable>(rightInvocable)](
                        auto&& value)
                        -> VariantFromOverload_t<RightInvocable, Ts...> {
                    return rightInvocable(std::forward<decltype(value)>(value));
                },
                std::forward<std::variant<Ts...>>(variant));
    }
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
struct Inv : Invocable {
    using Invocable::operator();

    template<class Type>
    constexpr auto
    unwrap(Type&& value) const noexcept
    {
        return unwrapInto(std::forward<Type>(value), *this);
    }
};

template<class Invocable>
Inv(Invocable) -> Inv<Invocable>;

struct End {};

template<class LeftInvocable>
constexpr auto
operator|(LeftInvocable&& leftInvocable, End) noexcept
{
    return Inv{std::forward<LeftInvocable>(leftInvocable)};
}

template<class LeftInvocable, class RightInvocable>
constexpr auto
operator|=(
        LeftInvocable&& leftInvocable,
        Inv<RightInvocable>&& rightInvocable) noexcept
{
    return Inv{[rightInvocable = std::forward<RightInvocable>(rightInvocable),
                leftInvocable  = std::forward<LeftInvocable>(leftInvocable)](
                       auto&&... args) {
        return rightInvocable(
                leftInvocable(std::forward<decltype(args)>(args)...));
    }};
}

template<class LeftInvocable, class RightInvocable>
constexpr auto
operator>>=(
        LeftInvocable&& leftInvocable,
        Inv<RightInvocable>&& rightInvocable) noexcept
{
    return Inv{[leftInvocable  = std::forward<LeftInvocable>(leftInvocable),
                rightInvocable = std::forward<RightInvocable>(rightInvocable)](
                       auto&&... arg) {
        return unwrapInto(
                leftInvocable(std::forward<decltype(arg)>(arg)...),
                rightInvocable);
    }};    // namespace Barely
}

}    // namespace Barely

#endif    // BARELYFUNCTIONAL_PACK_HPP