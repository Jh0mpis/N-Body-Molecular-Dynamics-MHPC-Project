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
      double rsq, ffac;
      double rx, ry, rz;
      int init, stop;

    #ifdef ENABLE_OPENMPI
      double epot;
      double temp_fx[sys->natoms], temp_fy[sys->natoms], temp_fz[sys->natoms];
    #endif

      const double c6 = 4.0 * sys->epsilon * exp(6 * log(sys->sigma));
      const double c12 = 4.0 * sys->epsilon * exp(12 * log(sys->sigma));
      const double rcsq = sys->rcut * sys->rcut; 

      /* Zero energy and forces */
      sys->epot = 0.0;

    #ifdef ENABLE_OPENMPI
      init = sys->local_size * sys->rank + sys->offset;
      stop = init + sys->local_size;
      epot = 0.0;

      azzero(temp_fx, sys->natoms);
      azzero(temp_fy, sys->natoms);
      azzero(temp_fz, sys->natoms);

      MPI_Bcast(sys->rx, sys->natoms, MPI_DOUBLE, 0, MPI_COMM_WORLD);
      MPI_Bcast(sys->ry, sys->natoms, MPI_DOUBLE, 0, MPI_COMM_WORLD);
      MPI_Bcast(sys->rz, sys->natoms, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    #else
      init = 0;
      stop = sys->natoms;
      
      azzero(sys->fx, sys->natoms);
      azzero(sys->fy, sys->natoms);
      azzero(sys->fz, sys->natoms);
    #endif

      for (int i = init; i < stop; ++i) {
        for (int j = i+1; j < (sys->natoms); ++j) {
            
            /* Minimum image convention */
            rx = pbc(sys->rx[i] - sys->rx[j], 0.5 * sys->box);
            ry = pbc(sys->ry[i] - sys->ry[j], 0.5 * sys->box);
            rz = pbc(sys->rz[i] - sys->rz[j], 0.5 * sys->box);

            rsq = rx * rx + ry * ry + rz * rz;

            if (rsq < rcsq) {
                double r6, rinv; 
                rinv = 1.0 / rsq;
                r6 = rinv * rinv * rinv; 

                ffac = (12.0 * c12 * r6 - 6.0 * c6) * r6 * rinv;               
                #ifdef ENABLE_OPENMPI
                  epot += r6 * (c12 * r6 - c6); 

                  temp_fx[i] += rx * ffac; 
                  temp_fx[j] -= rx  * ffac;

                  temp_fy[i] += ry  * ffac; 
                  temp_fy[j] -= ry  * ffac;

                  temp_fz[i] += rz  * ffac; 
                  temp_fz[j] -= rz  * ffac;
                #else
                  sys->epot += r6 * (c12 * r6 - c6); 

                  sys->fx[i] += rx * ffac; 
                  sys->fx[j] -= rx  * ffac;
  
                  sys->fy[i] += ry  * ffac; 
                  sys->fy[j] -= ry  * ffac;

                  sys->fz[i] += rz  * ffac; 
                  sys->fz[j] -= rz  * ffac;
                #endif
            }
        }
      }

    #ifdef ENABLE_OPENMPI

      MPI_Reduce(&epot, &(sys->epot), 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

      MPI_Reduce(temp_fx, sys->fx, sys->natoms, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
      MPI_Reduce(temp_fy, sys->fy, sys->natoms, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
      MPI_Reduce(temp_fz, sys->fz, sys->natoms, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    #endif
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
