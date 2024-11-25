#ifndef FORCE_COMPUTE_H
#define FORCE_COMPUTE_H

#include "mdsys.h"  // struct definition header file

// Compute interaction force for all particles
void force(mdsys_t *sys);
// Compute kinetic energy for all particles
void ekin(mdsys_t *sys);
// Compute interaction force for all particles
void old_force(mdsys_t *sys);
// Compute kinetic energy for all particles
void old_ekin(mdsys_t *sys);
// Apply the boundary conditions
static inline double pbc(double x, const double boxby2);

#endif /* FORCE_COMPUTE_H */
