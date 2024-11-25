#ifndef VERLET_TIME_INTEGRATION_H
#define VERLET_TIME_INTEGRATION_H

#include "mdsys.h"  // Struct definition header file

// updating velocities and positions
void velverlet(mdsys_t *sys);
void old_velverlet(mdsys_t *sys);

#endif /* VERLET_TIME_INTEGRATION_H */
