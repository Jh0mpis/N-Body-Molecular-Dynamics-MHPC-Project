#ifndef UTILITIES_H
#define UTILITIES_H

// Timing function
double wallclock();
// Fill a matrix with zeros
void azzero(double *d, const int n);
// Get the periodic boundary conditions
double pbc(double x, const double boxby2);

#endif /* UTILITIES_H */
