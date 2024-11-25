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

double pbc(double x, const double boxby2) {
    while (x >  boxby2) x -= 2.0 * boxby2;
    while (x < -boxby2) x += 2.0 * boxby2;
    return x;
}