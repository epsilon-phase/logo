Hi, this is a logo interpreter written in what we hope to be relatively modern
C++17 style.

We couldn't remember the actual syntax for logo, so we started work on our own
version. It's not perfect, in all likelihood it contains traps that we hadn't
even considered at the time.

It is not finished now, and with some luck, we will have something to show for
it, even if it's just realizing the techniques we used to write this are not
suited to their employment here.

The project's documentation is in doxygen, and will remain so because it tickles
us in this day and age(It *is* also quite a decent documentation generator, but
usage seems to have declined).

As of now, it doesn't contain a frontend, nor does it have the actual graphical
mechanisms. It just contains a parser and a lexer. It has many tests right now,
but we are aware that it will always need more, because a language is a massive
potential hole that might have thousands of issues.

It contains a parsing tester under tests/parsetester, which will be built if you
configure the project with `BUILD_TESTS=ON`. To build documentation enable the
`BUILD_DOCS` option, this requires doxygen to build.

Future Improvements
====

1. Improved Parsing Error reporting mechanism

    This does not yet support decent reporting mechanisms in parsing

2. Register Allocation

    Virtual registers are plentiful, but they are far from suitable for massive
    functions(perhaps they should not be so large, but sometimes you can't
    figure out a better way)