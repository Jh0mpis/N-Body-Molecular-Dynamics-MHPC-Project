#include <math.h>
#include <omp.h>
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

    nthreads = sys->nthreads;

    azzero(sys->cx, natoms * nthreads);
    azzero(sys->cy, natoms * nthreads);
    azzero(sys->cz, natoms * nthreads);

    const double register rcsq = sys->rcut * sys->rcut;
    
    double epot_local = 0.0;

    #ifdef ENABLE_OMP
    #pragma omp parallel num_threads(nthreads)
    #endif
    {   
        int tid = 0;
        #ifdef ENABLE_OMP
        tid = omp_get_thread_num();
        #endif

        double *cx = sys->cx + tid * natoms;
        double *cy = sys->cy + tid * natoms;
        double *cz = sys->cz + tid * natoms;

        /* Each thread processes a subset of particles */
        double epot_thread = 0.0;

        int stride = nthreads;
        for (int i = 0; i < natoms - 1; i += stride) {
            int ii = i + tid;
            if (ii >= (sys->natoms -1)) break; //error: break statement used with OpenMP for loop
            double rx1 = sys->rx[ii];
            double ry1 = sys->ry[ii];
            double rz1 = sys->rz[ii];
            for (int j = ii + 1; j < natoms; ++j) {
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
                    #ifdef _OPENMP
                    #pragma omp atomic
                    #endif
                    epot_local += epsilon4 * (sr12 - sr6);

                    /* Compute force factor */
                    ffac = epsilon4 * (12.0 * sr12 - 6.0 * sr6) * (rsqinv);

                    /* Apply forces */
                    cx[i] += rx * ffac;
                    cy[i] += ry * ffac;
                    cz[i] += rz * ffac;

                    cx[j] -= rx * ffac;
                    cy[j] -= ry * ffac;
                    cz[j] -= rz * ffac;
                }
            }
        }

        
        /* Sum forces from all threads */
        #ifdef ENABLE_OMP
        #pragma omp barrier
        #endif
        int chunk_size = (natoms + nthreads - 1) / nthreads;
        int start = tid * chunk_size;
        int end = (start + chunk_size > natoms) ? natoms : start + chunk_size;

        for (int t = 1; t < nthreads; ++t) {
            int offset = t * natoms;
            for (int i = start; i < end; ++i) {
                sys->cx[i] += sys->cx[offset + i];
                sys->cy[i] += sys->cy[offset + i];
                sys->cz[i] += sys->cz[offset + i];
            }
        }   
    }

     // Update global forces and epot
    #ifdef ENABLE_OMP
    #pragma omp parallel for
    #endif
    for (int i = 0; i < natoms; ++i) {
        sys->fx[i] = sys->cx[i];
        sys->fy[i] = sys->cy[i];
        sys->fz[i] = sys->cz[i];
    }

    /* Assign the accumulated potential energy */
    sys->epot = epot_local;
}


void ekin(mdsys_t *sys) {
    double ekin_local = 0.0; // Local variable for reduction

    #ifdef ENABLE_OMP
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

