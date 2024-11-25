#include "../include/verlet_time_integration.h"   // velverlet function header file
#include "../include/constants.h"                 // constants definition header file
#include "../include/force_compute.h"             // Force and kinetic energy functions header file

void velverlet(mdsys_t *sys) {
    /* First part: propagate velocities by half and positions by full step */
    const double denom =  sys->dt / mvsq2e; 
    for (int i = 0; i < sys->natoms; ++i) {
        sys->vx[i] += 0.5 * denom * sys->fx[i] / sys->mass;
        sys->vy[i] += 0.5 * denom * sys->fy[i] / sys->mass;
        sys->vz[i] += 0.5 * denom * sys->fz[i] / sys->mass;

        sys->rx[i] += sys->dt * sys->vx[i];
        sys->ry[i] += sys->dt * sys->vy[i];
        sys->rz[i] += sys->dt * sys->vz[i];
    }

    /* Compute forces and potential energy */
    force(sys);

    /* Second part: propagate velocities by another half step */
    for (int i = 0; i < sys->natoms; ++i) {
        sys->vx[i] += 0.5 * denom * sys->fx[i] / sys->mass;
        sys->vy[i] += 0.5 * denom * sys->fy[i] / sys->mass;
        sys->vz[i] += 0.5 * denom * sys->fz[i] / sys->mass;
    }
}
