#ifdef ENABLE_OPENMP
  #include <omp.h>
#endif /* ifdef ENABLE_OPENMP */
#include "../include/constants.h"     // constants definitions header file
#include "../include/mdsys.h"         // System struct definition header file
#include "../include/utilities.h"     // timing, zero matrix and pbc functions header file
#include "../include/force_compute.h" // Computing force and kinetic energy functions file

static inline double pbc(double x, const double boxby2) {
    while (x >  boxby2) x -= 2.0 * boxby2;
    while (x < -boxby2) x += 2.0 * boxby2;
    return x;
}

void force(mdsys_t *sys) {
    const int natoms = sys->natoms;
    int nthreads;
    double register ffac;
    double rx, ry, rz;
    int i, j;

    #ifdef ENABLE_OPENMP
      nthreads = sys->nthreads;
    #else
      nthreads = 1;
    #endif /* ifdef ENABLE_OPENMP */

    azzero(sys->cx, natoms * nthreads);
    azzero(sys->cy, natoms * nthreads);
    azzero(sys->cz, natoms * nthreads);
    azzero(sys->fx, natoms);
    azzero(sys->fy, natoms);
    azzero(sys->fz, natoms);

    const double register rcsq = sys->rcut * sys->rcut;
    
    sys->epot = 0.0;

    #ifdef ENABLE_OPENMP
    #pragma omp parallel num_threads(nthreads)
    #endif
    {   
        int tid = 0;
        double epot_local = 0.0;
        #ifdef ENABLE_OPENMP
        tid = omp_get_thread_num();
        #endif

        int stride = nthreads;
        for (i = tid; i < natoms; i += stride) {
            double rx1 = sys->rx[i];
            double ry1 = sys->ry[i];
            double rz1 = sys->rz[i];
            for (int j = i + 1; j < natoms; ++j) {
                double rx, ry, rz, ffac;
                double register rsq; 

                /* Minimum image convention */
                rx = pbc(rx1 - sys->rx[j], 0.5 * sys->box);
                ry = pbc(ry1 - sys->ry[j], 0.5 * sys->box);
                rz = pbc(rz1 - sys->rz[j], 0.5 * sys->box);

                rsq = rx * rx + ry * ry + rz * rz;

                /* Compute forces and potential energy */
                if (rsq < rcsq) {

                    double register rsqinv = 1.0 / rsq;
                    double sr2 = sys->sigma * sys->sigma * rsqinv;
                    double sr6 = sr2 * sr2 * sr2;
                    double sr12 = sr6 * sr6;
                    double epsilon4 = 4.0 * sys->epsilon;

                    /* Accumulate potential energy */
                    epot_local += epsilon4 * (sr12 - sr6);

                    /* Compute force factor */
                    ffac = epsilon4 * (12.0 * sr12 - 6.0 * sr6) * (rsqinv);

                    int off = natoms * tid;
                    /* Apply forces */
                    sys->cx[i + off] += rx * ffac;
                    sys->cy[i + off] += ry * ffac;
                    sys->cz[i + off] += rz * ffac;

                    sys->cx[j + off] -= rx * ffac;
                    sys->cy[j + off] -= ry * ffac;
                    sys->cz[j + off] -= rz * ffac;
                }
            }
        }

        
        /* Sum forces from all threads */
        #ifdef ENABLE_OPENMP
        #pragma omp barrier
        #endif
        for (int i = 0; i < natoms; i += nthreads){
            int ii = i + tid;
            for (int t = 0; t < nthreads; t++){
                sys->fx[ii] += sys->cx[natoms * t + ii];
                sys->fy[ii] += sys->cy[natoms * t + ii];
                sys->fz[ii] += sys->cz[natoms * t + ii];
            }
        }
        /* Assign the accumulated potential energy */
        #pragma omp atomic
        sys->epot += epot_local;
    }
}


void ekin(mdsys_t *sys) {
    double ekin_local = 0.0; // Local variable for reduction

    #ifdef ENABLE_OPENMP
    #pragma omp parallel for reduction(+:ekin_local)
    #endif
    for (int i = 0; i < sys->natoms; ++i) {
        ekin_local += 0.5 * mvsq2e * sys->mass *
                      (sys->vx[i] * sys->vx[i] +
                       sys->vy[i] * sys->vy[i] +
                       sys->vz[i] * sys->vz[i]);
    }

    sys->ekin = ekin_local;

    sys->temp = 2.0 * sys->ekin / (3.0 * sys->natoms - 3.0) / kboltz;
}

