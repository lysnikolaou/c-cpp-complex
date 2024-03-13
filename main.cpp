#include <cstdio>
#include <cstdlib>

#include "libnpymath/npymath.h"


int main(void)
{
    npymath_cdouble c = npymath_cpack(1, 1);

    double re = npymath_creal(c);
    double im = npymath_cimag(c);
    printf("Real: %f, Imag: %f\n", re, im);

    if (re != 1 || im != 1) {
        exit(EXIT_FAILURE);
    }

    return EXIT_SUCCESS;
}
