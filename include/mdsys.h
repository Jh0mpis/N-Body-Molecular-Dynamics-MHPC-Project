#ifndef MDSYS_H
#define MDSYS_H
  #ifdef ENABLE_OPENMPI
    #include <mpi.h>
  #endif

// Principal struct, defines the system properties
typedef struct {
    double dt, mass, epsilon, sigma, box, rcut;
    double ekin, epot, temp;
    int natoms, nfi, nsteps;
    #ifdef ENABLE_OPENMPI
      int rank, nps, local_size, offset;
    #endif
    double *rx, *ry, *rz;
    double *vx, *vy, *vz;
    double *fx, *fy, *fz;
} mdsys_t;

#endif /* MDSYS_H */

