#ifndef BARELYFUNCTIONAL_MAP_HPP
#define BARELYFUNCTIONAL_MAP_HPP

#include "misc.hpp"
#include "plumbing.hpp"

#include <type_traits>
#include <optional>
#include <utility>
#include <array>

namespace Brly {

template<class Invocable, class T>
constexpr auto
mapImpl(Invocable&& invocable, std::optional<T> const& optional)
{
    using ReturnT = RemoveOptional_t<decltype(invocable(*optional))>;

    if(optional.has_value()) {
        return std::optional<ReturnT>{invocable(*optional)};
    }

    return std::optional<ReturnT>{std::nullopt};
}

template<class Invocable, class T>
constexpr auto
mapImpl(Invocable&& invocable, std::optional<T>&& optional)
{
    using ReturnT = RemoveOptional_t<decltype(invocable(*optional))>;

    if(optional.has_value()) {
        return std::optional<ReturnT>{invocable(*optional)};
    }

    return std::optional<ReturnT>{std::nullopt};
}

template<class Invocable, class T, size_t length>
constexpr auto
mapImpl(Invocable&& invocable, std::array<T, length> const& array)
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
mapImpl(Invocable&& invocable, std::array<T, length>&& array)
{
    using ReturnT = decltype(invocable(array[0]));
    auto newArray = std::array<ReturnT, length>{};

    for(auto i = 0Lu; i < length; ++i) {
        newArray[i] = invocable(array[i]);
    }

    return newArray;
}

template<class... Invocables>
constexpr auto
map(Invocables... invocables) noexcept
{
    return ID{[invocable = ID{std::move(invocables)...}](auto&& arg) {
        using Invocable = decltype(invocable);

        if constexpr(isTriviallyCopyConstructible<Invocable>) {
            return mapImpl(
                    Invocable{invocable},
                    std::forward<decltype(arg)>(arg));
        }
        else {
            return mapImpl(invocable, std::forward<decltype(arg)>(arg));
        }
    }};
}

}    // namespace Brly

#endif    // BARELYFUNCTIONAL_MAP_HPP
