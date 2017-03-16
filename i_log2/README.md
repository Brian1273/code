# Microbenchmarking ilog2()
*Copyright (c) 2016 Bart Massey*

This codebase is a micro-benchmarking framework together
with several implementations of the *ilog2()* function.  The
framework draws very heavily on an earlier
[framework](http://github.com/BartMassey/popcount) for
*popcount()*.

## Background

Some algorithms really want *ilog2()*, the integer logarithm
base 2 of an integer input. For example, *ilog2(n)* gives
the depth of a complete binary tree of *n* nodes. However,
speed isn't really needed here. Another example would be
audio dynamic range compression, where the function is
applied to each element of an audio sequence. Here,
throughput probably matters.

The *ilog2()* function is equivalent to finding the position
of the most significant bit in the binary representation of
the input. This interpretation is more widely advertised,
and is easier to think about when designing algorithms.

## Calculation

The obvious approaches are all present in this package:

* *Naïve right-to-left search* simply looks at bits
  from right to left until all the higher bits are zero.

* *Naïve left-to-right search* simply looks at bits
  from left to right until it finds a one.

* *Binary search* explores the bits starting from the middle
  and working out, making left jumps when it is too low and
  right jumps when it is too high. When it has found a one
  bit with all zero bits to the left, it has found the
  answer.

* *Inline assembly* can be used to implement the
  right-to-left search loop rather than letting the compiler
  code it. Here, it is done for for X86-64. The
  left-to-right case is omitted as kind of pointless.

* A *compiler built-in function* can be used to compute the
  answer directly. It generally compiles to reasonably
  efficient code on a given target machine. This approach
  has been tested with GCC and Clang.

* An *inline assembly instruction* can compute the answer
  directly. This has been done for X86-64, using an
  instruction that is available in some form throughout the
  X86 family.

## Building and Running the Benchmarks

* If you want to use the inline asms you'll need GCC, Clang,
  or some other GCC-compatible compiler. If not, alter the
  Makefile accordingly. You may also want to try
  machine-dependent compiler flags, although I'm skeptical
  they will matter much.

* Type `make`.

* Run with `./ilog2 10000` where the argument is the number
  of base iterations of each function. This number will be
  scaled by an internal factor based on expected speed
  of the benchmark, which allows it to finish faster
  but with the same answers and reasonable reliability.
