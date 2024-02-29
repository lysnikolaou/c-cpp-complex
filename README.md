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
   using a `struct _double_complex { double real, imag; };`.
3. All four sets of complex numbers have the same memory layout. They're equivalent
   to an array of two double numbers stored consecutively in memory, with the first
   one being the real part and the second one being the imaginary part. However,
   they are not completely ABI-compatible, since calling conventions might differ
   between platforms. For example, in some architectures, the native complex types
   will be passed-by-value using registers, while the struct type will be
   passed-by-reference using the stack. To circumvent this, we need to do the following:
    - User code in C: pass-by-value is fine, since the same types are used both
      during compilation of the static library and during compilation of user code.
    - User code in C++: pass-by-value might lead to SEGFAULTs,
      because the types used during the compilation of the static library and the
      compilation of user C++ code are not ABI-compatible. We're thus forced to use
      pass-by-reference, which is okay, since the types have the same memory layout.
4. The old version of `npymath` (the one that as of this writing still resides
   inside the NumPy source tree), defines all of the complex functions as accepting
   a `double _Complex` without a pointer, which means that the complex numbers will
   be passed-by-value. One solution to this, would be to change all of the exported
   functions to only accept pointers. However, that would force all user code to change
   to pass pointers to complex numbers, which we do not want to do.
