A tacit dsl in c++.

auto constexpr expression = Barely::ID{} | Barely::Visit{foo} | Barely::Visit{bar};

auto constexpr result = expression(variant);

is the same as

auto constexpr result = std::visit(bar, std::visit(foo, variant));

Look at the tests for all the syntax,
it is relatively simple as I have only implemented what I have immediate use for.
I will flesh this description out at a later date.
