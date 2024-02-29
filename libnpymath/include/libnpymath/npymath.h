#ifndef _LIBNPYMATH_NPYMATH_H_
#define _LIBNPYMATH_NPYMATH_H_

#ifdef __cplusplus

struct _npymath_cdouble {
    double real, imag;
};

typedef struct _npymath_cdouble npymath_cdouble;

#else  /* !__cplusplus */

#include <complex.h>

#ifdef _MSC_VER

typedef _Dcomplex npymath_cdouble;

#else  /* !_MSC_VER */

typedef double _Complex npymath_cdouble;

#endif  /* _MSC_VER */

#endif  /* __cplusplus */

#ifdef __cplusplus

/*
 * This code is only seen when client code written in C++
 * uses libnpymath. Because the C complex type is not
 * ABI-compatible (calling conventions might differ)
 * with the struct type in C++, we have to "redefine" the
 * public exported symbols and then defined helpers that
 * accept pointers (which is okay, since the two types
 * are guaranteed to have the same memory layout).
 */

extern "C" {

double npymath_creal_ptr(npymath_cdouble *c);

static inline double
npymath_creal(npymath_cdouble c)
{
    return npymath_creal_ptr(&c);
}


double npymath_cimag_ptr(npymath_cdouble *c);

static inline double
npymath_cimag(npymath_cdouble c)
{
    return npymath_cimag_ptr(&c);
}


void npymath_cpack_ptr(npymath_cdouble *out, double real, double imag);

static inline npymath_cdouble
npymath_cpack(double real, double imag)
{
    npymath_cdouble out;
    npymath_cpack_ptr(&out, real, imag);
    return out;
}

}  /* extern "C" { */

#else  /* !__cplusplus */

/*
 * This is the part of the code that is seen at library compile-time
 * and when client code is compiled using a C compiler.
 * 
 * This way all of the library functions are defined as accepting
 * a "double _Complex" (see also typedef'ing code above).
 */

double npymath_creal(npymath_cdouble c);
double npymath_cimag(npymath_cdouble c);
npymath_cdouble npymath_cpack(double real, double imag);

#endif  /* __cplusplus */

#endif  /* _LIBNPYMATH_NPYMATH_H_ */
