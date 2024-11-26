#ifndef MDSYS_H
#define MDSYS_H

// Principal struct, defines the system properties
typedef struct {
    double dt, mass, epsilon, sigma, box, rcut;
    double ekin, epot, temp;
    double *rx, *ry, *rz;
    double *vx, *vy, *vz;
    double *fx, *fy, *fz;
    unsigned int natoms, nfi, nsteps;
} mdsys_t;

#endif /* MDSYS_H */

