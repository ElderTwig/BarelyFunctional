#ifndef BARELYFUNCTIONAL_WRAP_HPP
#define BARELYFUNCTIONAL_WRAP_HPP

#define BARELY_WRAP(functionName)                                   \
    [](auto&&... args) {                                            \
        return functionName(std::forward<decltype(args)>(args...)); \
    }

#endif    // BARELYFUNCTIONAL_WRAP_HPP
