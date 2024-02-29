#include <cstdio>

#include "libnpymath/npymath.h"


int main(void)
{
    npymath_cdouble c = npymath_cpack(1, 1);
    printf("Real: %f, Imag: %f\n", npymath_creal(c), npymath_cimag(c));
}
