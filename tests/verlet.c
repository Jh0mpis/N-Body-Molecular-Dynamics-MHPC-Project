/*
 * 1-d velocity verlet in reduced units
 */

#include "ljmd.h"

/* velocity verlet */

void verlet_1(mdsys_t *sys)
{
    int i;

    /* first part: propagate velocities by half and positions by full step */
    for (i=0; i<sys->natoms; ++i) {
        sys->vx[i] += 0.5 * sys->fx[i] / sys->mass;
        sys->rx[i] += sys->vx[i];
    }
}

void verlet_2(mdsys_t *sys)
{
    int i;

    /* second part: propagate velocities by another half step */
    for (i=0; i<sys->natoms; ++i) {
        sys->vx[i] += 0.5 * sys->fx[i] / sys->mass;
    }
}
