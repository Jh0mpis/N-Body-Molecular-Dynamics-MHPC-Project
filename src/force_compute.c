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
    int i, j;
    int natoms = sys->natoms;

    /* Zero energy and forces */
    sys->epot = 0.0;
    azzero(sys->fx, natoms);
    azzero(sys->fy, natoms);
    azzero(sys->fz, natoms);

    double *epot_thread;
    double **fx_thread, **fy_thread, **fz_thread;

    int nthreads;

    #pragma omp parallel
    {
        /* Obtain the actual number of threads */
        #pragma omp single
        {
            nthreads = omp_get_num_threads();

            epot_thread = (double *)calloc(nthreads, sizeof(double));
            fx_thread = (double **)malloc(nthreads * sizeof(double *));
            fy_thread = (double **)malloc(nthreads * sizeof(double *));
            fz_thread = (double **)malloc(nthreads * sizeof(double *));
            for (int t = 0; t < nthreads; t++) {
                fx_thread[t] = (double *)calloc(natoms, sizeof(double));
                fy_thread[t] = (double *)calloc(natoms, sizeof(double));
                fz_thread[t] = (double *)calloc(natoms, sizeof(double));
            }
        }

        int tid = omp_get_thread_num();
        double epot_local = 0.0;
        double r, ffac;
        double rx, ry, rz;

        #pragma omp for schedule(dynamic)
        for (i = 0; i < natoms - 1; ++i) {
            for (j = i + 1; j < natoms; ++j) {

                /* Minimum image convention */
                rx = pbc(sys->rx[i] - sys->rx[j], 0.5 * sys->box);
                ry = pbc(sys->ry[i] - sys->ry[j], 0.5 * sys->box);
                rz = pbc(sys->rz[i] - sys->rz[j], 0.5 * sys->box);
                r = sqrt(rx * rx + ry * ry + rz * rz);

                /* Compute force and energy if within cutoff */
                if (r < sys->rcut) {
                    double r6 = pow(sys->sigma / r, 6.0);
                    double r12 = r6 * r6;
                    double epsilon4 = 4.0 * sys->epsilon;

                    /* Potential energy */
                    epot_local += epsilon4 * (r12 - r6);

                    /* Force factor */
                    ffac = epsilon4 * (-12.0 * r12 + 6.0 * r6) / (r * r);

                    /* Forces: Newton's third law */
                    fx_thread[tid][i] += rx * ffac;
                    fy_thread[tid][i] += ry * ffac;
                    fz_thread[tid][i] += rz * ffac;

                    fx_thread[tid][j] -= rx * ffac;
                    fy_thread[tid][j] -= ry * ffac;
                    fz_thread[tid][j] -= rz * ffac;
                }
            }
        }

        epot_thread[tid] = epot_local;
    }

    /* Sum up contributions from all threads */
    for (int t = 0; t < nthreads; t++) {
        sys->epot += epot_thread[t];
        for (i = 0; i < natoms; ++i) {
            sys->fx[i] += fx_thread[t][i];
            sys->fy[i] += fy_thread[t][i];
            sys->fz[i] += fz_thread[t][i];
        }
        free(fx_thread[t]);
        free(fy_thread[t]);
        free(fz_thread[t]);
    }
    free(fx_thread);
    free(fy_thread);
    free(fz_thread);
    free(epot_thread);
}


void ekin(mdsys_t *sys) {
    double ekin_local = 0.0; // Local variable for reduction

    #pragma omp parallel for reduction(+:ekin_local)
    for (int i = 0; i < sys->natoms; ++i) {
        ekin_local += 0.5 * mvsq2e * sys->mass *
                      (sys->vx[i] * sys->vx[i] +
                       sys->vy[i] * sys->vy[i] +
                       sys->vz[i] * sys->vz[i]);
    }

    sys->ekin = ekin_local;

    sys->temp = 2.0 * sys->ekin / (3.0 * sys->natoms - 3.0) / kboltz;
}



// Computing the interaction force for each particle
void old_force(mdsys_t *sys) {
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
void old_ekin(mdsys_t *sys) {

    sys->ekin = 0.0;
    for (int i = 0; i < sys->natoms; ++i) {
        sys->ekin += 0.5 * mvsq2e * sys->mass *
                     (sys->vx[i] * sys->vx[i] +
                      sys->vy[i] * sys->vy[i] +
                      sys->vz[i] * sys->vz[i]);
    }
    sys->temp = 2.0 * sys->ekin / (3.0 * sys->natoms - 3.0) / kboltz;
}
