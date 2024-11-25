#include <sys/time.h>
#include "../include/utilities.h"   // wallclock, pbc and azzero function header file

double wallclock() {
    struct timeval t;
    gettimeofday(&t, 0);
    return ((double)t.tv_sec) + 1.0e-6 * ((double)t.tv_usec);
}

void azzero(double *d, const int n) {
    for (int i = 0; i < n; ++i) {
        d[i] = 0.0;
    }
}

double my_pow(double base, int exp)
{
    double result = 1;
    for (;;)
    {
        if (exp & 1)
            result *= base;
        exp >>= 1;
        if (!exp)
            break;
        base *= base;
    }
    return result;
}
