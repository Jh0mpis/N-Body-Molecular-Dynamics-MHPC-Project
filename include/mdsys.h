#ifndef MDSYS_H
#define MDSYS_H

// Principal struct, defines the system properties
typedef struct {
    double dt, mass, epsilon, sigma, box, rcut;
    double ekin, epot, temp;
    int natoms, nfi, nsteps, nthreads;
    double *rx, *ry, *rz;
    double *vx, *vy, *vz;
    double *fx, *fy, *fz;
    double *cx, *cy, *cz;
    
} mdsys_t;

#endif /* MDSYS_H */

