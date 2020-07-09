# A tacit DSL in c++.

https://en.wikipedia.org/wiki/Tacit_programming

---

To start a function chain, construct an ID object by capturing an overload
set like **ID{foo, bar...}** where foo, bar... are function objects.
Any objects with an **operator()** defined will work. You then get an object on
which you can either call the overloaded operator()s through or use the
overloaded **operator|** when an **ID** object is on the left.

**ID{} | foo** constructs **ID{foo}**, **ID{foo} | bar** constructs an **ID**
object with an **operator()** that forwards it's arguments to **ID{foo}**,
then gives the returned value to **bar::operator()**.

In other words:

    auto constexpr callable = Brly::ID{foo} | bar | fun | bun;
    callable(arg);
or

    auto constexpr callable = Brly::ID{} | foo | bar | fun | bun;
    callable(arg);
or

    (Brly::ID{foo} | bar | Brly::ID{fun} | bun)(arg);
is equivalent to:

    bun(fun(bar(foo(arg))));

This allows you to make linear callgraphs at compile time and avoid nested
function calls without using intermediate variables.

BarelyFunctional also supplies **Brly::visit**:

    Brly::visit(foo, bar) constructs an ID object which operator() calls
    ID{foo, bar}::operator() with the inner type of a variant.
    
    Brly::visit(foo, bar)(std::variant<a, b, c>{}) is equivalent to
    std::visit(Brly::ID{foo, bar}, std::variant<a, b, c>{})
**Brly::map**:

    Brly::map(foo, bar) constructs an ID object which operator() calls
    ID{foo, bar}::operator() for each element of the supplied argument and
    returns an equivalent type with the results. It is modeled after haskell map
    and it is defined for std::optional and std::array.
**Brly::uncurry**:

    Brly::uncurry(foo, bar) constructs an ID object which operator() calls
    std:apply(ID{foo, bar}, arg).
You can combine this like:

    auto constexpr callgraph = map(foo) | bar | visit(fun, bun);

Just include what components you want from ./include folder to use.
BarelyFunctional requires c++ 17
