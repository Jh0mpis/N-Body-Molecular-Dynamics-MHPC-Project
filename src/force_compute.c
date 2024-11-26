#include <math.h>
#include "../include/constants.h"     // constants definitions header file
#include "../include/mdsys.h"         // System struct definition header file
#include "../include/utilities.h"     // timing, zero matrix and pbc functions header file
#include "../include/force_compute.h" // Computing force and kinetic energy functions file

static inline double pbc(double x, const double boxby2)
{
    while (x > boxby2)
        x -= 2.0 * boxby2;
    while (x < -boxby2)
        x += 2.0 * boxby2;
    return x;
}

// Computing the interaction force for each particle
void force(mdsys_t *sys)
{
    const int natoms = sys->natoms;
    double register ffac;
    double rx, ry, rz;
    int i, j;

    /* Zero energy and forces */
    sys->epot = 0.0;

    azzero(sys->fx, natoms);
    azzero(sys->fy, natoms);
    azzero(sys->fz, natoms);
    const double sigma6 = sys->sigma * sys->sigma * sys->sigma * sys->sigma * sys->sigma * sys->sigma;
    const double register c6 = 4.0 * sys->epsilon * sigma6;
    const double register c12 = 4.0 * sys->epsilon * sigma6 * sigma6;

    double register rcsq = sys->rcut * sys->rcut;

    for (i = 0; i < (natoms)-1; ++i)
    {
        for (j = i + 1; j < (natoms); ++j)
        {

            /* Skip self-interaction */
            if (i == j)
                continue;

            /* Minimum image convention */
            rx = pbc(sys->rx[i] - sys->rx[j], 0.5 * sys->box);
            ry = pbc(sys->ry[i] - sys->ry[j], 0.5 * sys->box);
            rz = pbc(sys->rz[i] - sys->rz[j], 0.5 * sys->box);
            double register rsq;
            rsq = rx * rx + ry * ry + rz * rz;
            if (rsq < rcsq)
            {
                double register rsqinv = 1.0 / rsq;
                double sr2 = sys->sigma * sys->sigma * rsqinv;
                double sr6 = sr2 * sr2 * sr2;
                double sr12 = sr6 * sr6;
                double epsilon4 = 4.0 * sys->epsilon;

                ffac = epsilon4 * (12.0 * sr12 - 6.0 * sr6) * (rsqinv);
                sys->epot += epsilon4 * (sr12 - sr6);

                sys->fx[i] += rx * ffac;
                sys->fx[j] -= rx * ffac;
                sys->fy[i] += ry * ffac;
                sys->fy[j] -= ry * ffac;
                sys->fz[i] += rz * ffac;
                sys->fz[j] -= rz * ffac;
            }
        }
    }
}

// Kinetic energy computation
void ekin(mdsys_t *sys)
{

    sys->ekin = 0.0;
    for (int i = 0; i < sys->natoms; ++i)
    {
        sys->ekin += 0.5 * mvsq2e * sys->mass *
                     (sys->vx[i] * sys->vx[i] +
                      sys->vy[i] * sys->vy[i] +
                      sys->vz[i] * sys->vz[i]);
    }
    sys->temp = 2.0 * sys->ekin / (3.0 * sys->natoms - 3.0) / kboltz;
}
