#ifndef MDSYS_H
#define MDSYS_H

typedef struct {
    int natoms, nfi, nsteps;
    double dt, mass, epsilon, sigma, box, rcut;
    double ekin, epot, temp;
    double *rx, *ry, *rz;
    double *vx, *vy, *vz;
    double *fx, *fy, *fz;
} mdsys_t;

#endif /* MDSYS_H */

