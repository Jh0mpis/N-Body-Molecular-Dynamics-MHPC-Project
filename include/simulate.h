#ifndef SIMULATE
#define SIMULATE

#include <stdio.h>
#include "mdsys.h"  // Run the simulation's principal cycle

// Initialize the system and runs the simulation
void run(mdsys_t *sys, FILE **erg, FILE **traj, const double initial_time, const int nprint);
void run_omp(mdsys_t *sys, FILE **erg, FILE **traj, const double initial_time, const int nprint);
// updating velocities and positions
static void velverlet(mdsys_t *sys);

#endif // !SIMULATE
