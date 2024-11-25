#include <math.h>
#include "../include/constants.h"     // constants definitions header file
#include "../include/mdsys.h"         // System struct definition header file
#include "../include/utilities.h"     // timing, zero matrix and pbc functions header file
#include "../include/force_compute.h" // Computing force and kinetic energy functions file

static inline double pbc(double x, const double boxby2) {
    while (x >  boxby2) x -= 2.0 * boxby2;
    while (x < -boxby2) x += 2.0 * boxby2;
    return x;
}

// Computing the interaction force for each particle
void force(mdsys_t *sys) {
    double r, ffac;
    double rx, ry, rz;
    int i, j;

    /* Zero energy and forces */
    sys->epot = 0.0;
    azzero(sys->fx, sys->natoms);
    azzero(sys->fy, sys->natoms);
    azzero(sys->fz, sys->natoms);

    for (i = 0; i < sys->natoms; ++i) {
        for (j = 0; j < sys->natoms; ++j) {

            /* Skip self-interaction */
            if (i == j) continue;

            /* Minimum image convention */
            rx = pbc(sys->rx[i] - sys->rx[j], 0.5 * sys->box);
            ry = pbc(sys->ry[i] - sys->ry[j], 0.5 * sys->box);
            rz = pbc(sys->rz[i] - sys->rz[j], 0.5 * sys->box);
            r = sqrt(rx * rx + ry * ry + rz * rz);

            /* Compute force and energy if within cutoff */
            if (r < sys->rcut) {
                ffac = -4.0 * sys->epsilon * (-12.0 * pow(sys->sigma / r, 12.0) / r
                                              + 6.0 * pow(sys->sigma / r, 6.0) / r);

                sys->epot += 0.5 * 4.0 * sys->epsilon * (pow(sys->sigma / r, 12.0)
                                                         - pow(sys->sigma / r, 6.0));

                sys->fx[i] += rx / r * ffac;
                sys->fy[i] += ry / r * ffac;
                sys->fz[i] += rz / r * ffac;
            }
        }
    }
}

// Kinetic energy computation
void ekin(mdsys_t *sys) {

    sys->ekin = 0.0;
    for (int i = 0; i < sys->natoms; ++i) {
        sys->ekin += 0.5 * mvsq2e * sys->mass *
                     (sys->vx[i] * sys->vx[i] +
                      sys->vy[i] * sys->vy[i] +
                      sys->vz[i] * sys->vz[i]);
    }
    sys->temp = 2.0 * sys->ekin / (3.0 * sys->natoms - 3.0) / kboltz;
}
