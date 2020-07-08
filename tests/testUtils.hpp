#ifndef BARELYFUNCTIONAL_TESTUTILS_HPP
#define BARELYFUNCTIONAL_TESTUTILS_HPP

struct Counter {
    int copyConstructed = 0;
    int moveConstructed = 0;

    constexpr Counter(Counter const& c) :
                copyConstructed(c.copyConstructed + 1),
                moveConstructed(c.moveConstructed)
    {}

    constexpr Counter(Counter&& c) :
                copyConstructed(c.copyConstructed),
                moveConstructed(c.moveConstructed + 1)
    {}

    Counter&
    operator=(Counter const&) = delete;

    Counter&
    operator=(Counter&&) = delete;

    constexpr Counter() = default;

    ~Counter() = default;
};

#endif    // BARELYFUNCTIONAL_TESTUTILS_HPP
