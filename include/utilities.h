#ifndef UTIL_H
#define UTIL_H
#include "constants.h"
#include <math.h>
void azzero(double *d, const int n);
double pbc(double x, const double boxby2);
void ekin(mdsys_t *sys);
void velverlet(mdsys_t *sys);
#endif  /*UTIL_H */ 