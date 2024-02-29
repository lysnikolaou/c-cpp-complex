#include "libnpymath/npymath.h"


double
npymath_creal_ptr(npymath_cdouble *c)
{
    return ((double *) c)[0];
}


double
npymath_creal(npymath_cdouble c)
{
    return ((double *) &c)[0];
}


double
npymath_cimag_ptr(npymath_cdouble *c)
{
    return ((double *) c)[1];
}


double npymath_cimag(npymath_cdouble c)
{
    return ((double *) &c)[1];
}


void
npymath_cpack_ptr(npymath_cdouble *out, double real, double imag)
{
    ((double *) out)[0] = real;
    ((double *) out)[1] = imag;
}


npymath_cdouble
npymath_cpack(double real, double imag)
{
    npymath_cdouble c;
    ((double *) &c)[0] = real;
    ((double *) &c)[1] = imag;
    return c;
}
