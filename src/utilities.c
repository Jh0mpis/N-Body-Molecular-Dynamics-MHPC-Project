#include <sys/time.h>
#include "../include/utilities.h"   // wallclock and azzero function header file

// function for timing
double wallclock() {
    struct timeval t;
    gettimeofday(&t, 0);
    return ((double)t.tv_sec) + 1.0e-6 * ((double)t.tv_usec);
}

// Function for fill an array with zeros
void azzero(double *d, const int n) {
    for (int i = 0; i < n; ++i) {
        d[i] = 0.0;
    }
}

