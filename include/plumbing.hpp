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

struct Entry {
    template<class Invocable>
    constexpr auto
    operator>(Invocable invocable) const noexcept
    {
        return Inv<Invocable>{invocable};
    }
};

template<class OldInvocable, class Invocable>
constexpr auto
operator>(Inv<OldInvocable> oldInvocable, Invocable invocable) noexcept
{
    auto constexpr newInvocable = [=](auto&&... args) {
        return invocable(oldInvocable(std::forward<decltype(args)>(args)...));
    };

    return Inv<decltype(newInvocable)>{newInvocable};
}

template<class... Ts, class Visitor>
constexpr auto
unwrap(std::variant<Ts...>&& variant, Visitor&& visitor) noexcept
{
    return std::visit(
            std::forward<Visitor>(visitor),
            std::forward<std::variant<Ts...>>(variant));
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
    auto constexpr newInvocable = [=](auto&& arg) {
        return unwrap(
                oldInvocable(std::forward<decltype(arg)>(arg)),
                invocable);
    };

    return Inv<decltype(newInvocable)>{newInvocable};
}

}    // namespace Barely

#endif    // BARELYFUNCTIONAL_PACK_HPP