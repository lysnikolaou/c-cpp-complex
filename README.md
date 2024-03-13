# c-cpp-complex

This repo is an attempt at using native complex numbers, while adhering to the
C and C++ standards and supporting different platforms and architectures.

Note: This repo only demonstrates using `double _Complex` numbers, but everything
is directly transferable to `float _Complex` and `long double _Complex`.

### Considerations

1. There are three sets of native complex numbers:
    - In C: `double _Complex` on non-Windows platforms and `_Dcomplex` on Windows.
    - In C++: `std::complex<double>`.
2. Because of the nature of `libnpymath` and the fact that it's a *C* compatibility
   layer, we want to avoid using C++ templates in it. Thus, we have to fall back to
   using a `struct _double_complex { double real, imag; };` in C++.
3. All four sets of complex numbers have the same memory layout. They're equivalent
   to an array of two double numbers stored consecutively in memory, with the first
   one being the real part and the second one being the imaginary part. However,
   they are not completely ABI-compatible, since calling conventions might differ
   between platforms.
4. The old version of `npymath` (the one that as of this writing still resides
   inside the NumPy source tree), defines all of the complex functions as accepting
   a `double _Complex` without a pointer, which means that the complex numbers will
   be passed-by-value, and we do not want to change that, since that would force all
   user code to change.

### Calling conventions for complex numbers

Calling convetions for complex numbers are not consistent across architectures. Some
architectures pass complex numbers in the stack, while others do so using registers.
More information on this can be found here:

1. [Github issue on complex types in Zig](https://github.com/ziglang/zig/issues/8465)
2. [GCC issue on `std::complex` and C99 complex interoperability on s390x and ppc64le](https://bugzilla.redhat.com/show_bug.cgi?id=1918519)

This might cause problems in user C++ code. In a C++ compilation unit `npymath_cimag`
will be declared like this:

```cpp
extern "C" double npymath_cimag(struct _npymath_cdouble c)
```

However, the function having been compiled with a C compiler will look like this:

```cpp
double npymath_cimag(double _Complex c)
```

Despite the fact the the two types have identical memory layout, different calling
conventions might lead to SEGFAULTs here, since some architectures will expect the
`c` argument to come from a register, but it's actually coming from the stack, since
we pass in a struct instead of a native complex numbers. The identical memory
layout will come into play, if we instead define `npymath_cimag` to accept a pointer
to a complex number. This way, the pointer will always point to a piece of memory that
stores a complex number, independent from whether it is a struct or a native complex
number.

This all means that there are two distinct cases:
- User code written in C: No problem here. Compilation of the static library and
compilation of user code happens with the same types, so no clashes can occur.
- User code written in C++: Might lead to SEGFAULTs, because of the calling
conventions problem described above. To circumvent the problem, we use
pass-by-reference by redefining the `npymath_cimag` function. See [the implementation
here](https://github.com/lysnikolaou/c-cpp-complex/blob/main/libnpymath/include/libnpymath/npymath.h#L28-L86)
for more.

### When not to do this

Doing this obviously comes with additional maintenance burdens, and shouldn't be the
first thing to try out for most projects. Before falling back to this approach, try
these alternative ways instead:
1. Support only C or C++ for complex number routines (and use the appropriate types)
2. Write all of your routines so that they accept pointers to the complex numbers
3. Fall back to this solution if you cannot, or you're not willing, to do one of the
above.


### Proposal

1. Implement this approach on [numpy/libnpymath](https://github.com/numpy/libnpymath)
for all of the routines that accept complex numbers.
2. NumPy and SciPy should be able to support this without any big changes. (a
compatibility layer may be needed for `libnpymath`, but that's another discussion)