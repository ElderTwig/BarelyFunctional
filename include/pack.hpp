#ifndef BARELYFUNCTIONAL_PACK_HPP
#define BARELYFUNCTIONAL_PACK_HPP

#include <type_traits>

namespace Barely {

template<class Invocable>
struct Inv : Invocable {
    using Invocable::operator();
};

struct Entry {
    template<class Invocable>
    constexpr auto
    operator|(Invocable invocable) const noexcept
    {
        return Inv<Invocable>{invocable};
    }
};

template<class OldInvocable, class Invocable>
constexpr auto
operator|(Inv<OldInvocable> inv, Invocable invocable) noexcept
{
    auto constexpr newInvocable = [=](auto&&... args) {
        return invocable(inv(std::forward<decltype(args)>(args)...));
    };

    return Inv<decltype(newInvocable)>{newInvocable};
}

}    // namespace Barely

#endif    // BARELYFUNCTIONAL_PACK_HPP