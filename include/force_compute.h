#ifndef FORCE_COMPUTE_H
#define FORCE_COMPUTE_H

#include "mdsys.h"  // struct definition header file

// Compute interaction force for all particles
void force(mdsys_t *sys);
// Compute kinetic energy for all particles
void ekin(mdsys_t *sys);

#endif /* FORCE_COMPUTE_H */
