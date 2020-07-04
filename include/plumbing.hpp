#ifndef BARELYFUNCTIONAL_PLUMBING_HPP
#define BARELYFUNCTIONAL_PLUMBING_HPP

#include "misc.hpp"
#include <type_traits>
#include <variant>
#include <optional>
#include <tuple>

namespace Barely {

template<class Invocable>
struct Inv : Invocable {
    using Invocable::operator();
};

template<class Return, class... Args>
struct Inv<Return (*)(Args...)> {
    Return (*invocable)(Args...);

    constexpr auto
    operator()(Args... args) const noexcept -> Return
    {
        return invocable(std::forward<Args>(args)...);
    }
};

template<class Invocable>
Inv(Invocable) -> Inv<Invocable>;

template<class OldInvocable, class Invocable>
constexpr auto
operator>(Inv<OldInvocable> oldInvocable, Invocable invocable) noexcept
{
    return Inv{[=](auto&&... args) {
        return invocable(oldInvocable(std::forward<decltype(args)>(args)...));
    }};
}

template<class... Ts, class Visitor>
constexpr auto
unwrap(std::variant<Ts...>&& variant, Visitor visitor) noexcept
{
    using ReturnType = std::tuple_element_t<0, std::tuple<Ts...>>;

    if constexpr((std::is_same_v<
                          ReturnType,
                          std::invoke_result_t<Visitor, Ts>> && ...)) {
        return std::visit(visitor, std::forward<std::variant<Ts...>>(variant));
    }
    else {
        return std::visit(
                [=](auto&& value) -> std::variant<Ts...> {
                    return visitor(std::forward<decltype(value)>(value));
                },
                std::forward<std::variant<Ts...>>(variant));
    }
}

template<class... Ts, class Function>
constexpr auto
unwrap(std::tuple<Ts...>&& args, Function&& function) noexcept
{
    return std::apply(
            std::forward<Function>(function),
            std::forward<std::tuple<Ts...>>(args));
}

template<class T, class Function>
constexpr auto
unwrap(std::optional<T>&& option, Function function) noexcept
{
    using ReturnT = std::invoke_result_t<Function, T>;

    if constexpr(isOptional<ReturnT>) {
        if(option.has_value()) {
            return function(*option);
        }

        return ReturnT{std::nullopt};
    }
    else {
        if(option.has_value()) {
            return std::optional<ReturnT>{function(*option)};
        }

        return std::optional<ReturnT>{std::nullopt};
    }
}

template<class OldInvocable, class Invocable>
constexpr auto
operator>=(Inv<OldInvocable> oldInvocable, Invocable invocable) noexcept
{
    return Inv{[=](auto&&... arg) {
        return unwrap(
                oldInvocable(std::forward<decltype(arg)>(arg)...),
                invocable);
    }};
}

struct Expr {
    template<class Invocable>
    constexpr auto
    operator>(Invocable invocable)
    {
        return Inv{invocable};
    }

    template<class Invocable>
    constexpr auto
    operator>=(Invocable invocable)
    {
        return Inv{[=](auto&& value) {
            return unwrap(std::forward<decltype(value)>(value), invocable);
        }};
    }
};

template<class... Types>
struct Pack {
    std::tuple<Types...> pack;

    constexpr Pack(Types&&... types) noexcept :
                pack{std::forward<Types>(types)...}
    {}

    template<class Invocable>
    constexpr auto
    operator|(Invocable&& invocable)
    {
        return std::apply(std::move(invocable), std::move(pack));
    }
};

}    // namespace Barely

#endif    // BARELYFUNCTIONAL_PACK_HPP