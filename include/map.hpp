#ifndef BARELYFUNCTIONAL_MAP_HPP
#define BARELYFUNCTIONAL_MAP_HPP

#include "misc.hpp"
#include "plumbing.hpp"

#include <type_traits>
#include <optional>
#include <utility>
#include <array>

namespace Barely {

template<class Invocable, class T>
constexpr auto
map(Invocable&& invocable, std::optional<T> const& optional)
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

template<class Invocable, class T>
constexpr auto
map(Invocable&& invocable, std::optional<T>&& optional)
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

template<class Invocable, class T, size_t length>
constexpr auto
map(Invocable&& invocable, std::array<T, length> const& array)
{
    using ReturnT = decltype(invocable(array[0]));
    auto newArray = std::array<ReturnT, length>{};

    for(auto i = 0Lu; i < length; ++i) {
        newArray[i] = invocable(array[i]);
    }

    return newArray;
}

template<class Invocable, class T, size_t length>
constexpr auto
map(Invocable&& invocable, std::array<T, length>&& array)
{
    using ReturnT = decltype(invocable(array[0]));
    auto newArray = std::array<ReturnT, length>{};

    for(auto i = 0Lu; i < length; ++i) {
        newArray[i] = invocable(array[i]);
    }

    return newArray;
}

template<class... Invocables>
struct Map : ID<Invocables...> {
    constexpr Map(Invocables... invocables) noexcept :
                ID<Invocables...>{std::move(invocables)...}
    {}

    template<class Arg>
    constexpr auto
    operator()(Arg&& arg) const noexcept
    {
        using Base = ID<Invocables...>;

        if constexpr(isTriviallyCopyConstructible<Base>) {
            return map(Base{static_cast<Base>(*this)}, std::forward<Arg>(arg));
        }
        else {
            return map(static_cast<Base>(*this), std::forward<Arg>(arg));
        }
    }
};

template<class Invocable>
struct Map<Invocable> : Invocable {
    template<class Arg>
    constexpr auto
    operator()(Arg&& arg) const noexcept
    {
        if constexpr(isTriviallyCopyConstructible<Invocable>) {
            return map(
                    Invocable{static_cast<Invocable>(*this)},
                    std::forward<Arg>(arg));
        }
        else {
            return map(static_cast<Invocable>(*this), std::forward<Arg>(arg));
        }
    }
};

template<class Invocable>
Map(Invocable) -> Map<Invocable>;

}    // namespace Barely

#endif    // BARELYFUNCTIONAL_MAP_HPP
