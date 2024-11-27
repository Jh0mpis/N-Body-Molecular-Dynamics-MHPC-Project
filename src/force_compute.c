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
    int stride = 1;
    int init = 0, stop = sys->natoms;
    double rx, ry, rz;

    #ifdef ENABLE_OPENMP
      azzero(sys->cx, sys->natoms * sys->nthreads);
      azzero(sys->cy, sys->natoms * sys->nthreads);
      azzero(sys->cz, sys->natoms * sys->nthreads);
    #endif // ENABLE_OPENMP
  
    #ifdef ENABLE_OPENMPI
      double epot = 0;
      double temp_fx[sys->natoms], temp_fy[sys->natoms], temp_fz[sys->natoms];
      init = sys->local_size * sys->rank + sys->offset;
      stop = init + sys->local_size;
      epot = 0.0;

      azzero(temp_fx, sys->natoms);
      azzero(temp_fy, sys->natoms);
      azzero(temp_fz, sys->natoms);

      MPI_Bcast(sys->rx, sys->natoms, MPI_DOUBLE, 0, MPI_COMM_WORLD);
      MPI_Bcast(sys->ry, sys->natoms, MPI_DOUBLE, 0, MPI_COMM_WORLD);
      MPI_Bcast(sys->rz, sys->natoms, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    #endif // ENABLE_OPENMPI
  
    azzero(sys->fx, sys->natoms);
    azzero(sys->fy, sys->natoms);
    azzero(sys->fz, sys->natoms);

    const double register rcsq = sys->rcut * sys->rcut;
    
    sys->epot = 0.0;

    #ifdef ENABLE_OPENMP
    #pragma omp parallel
    #endif
    {   
        int tid = 0;
        #ifdef ENABLE_OPENMP
        double epot_local = 0.0;
        tid = omp_get_thread_num();
        stride = sys->nthreads;
        #endif

        for (int i = init + tid; i < stop; i += stride) {
            for (int j = i + 1; j < natoms; ++j) {
                double register ffac;
                double register rsq; 

                /* Minimum image convention */
                rx = pbc(sys->rx[i] - sys->rx[j], 0.5 * sys->box);
                ry = pbc(sys->ry[i] - sys->ry[j], 0.5 * sys->box);
                rz = pbc(sys->rz[i] - sys->rz[j], 0.5 * sys->box);

                rsq = rx * rx + ry * ry + rz * rz;

                /* Compute forces and potential energy */
                if (rsq < rcsq) {

                    double register rsqinv = 1.0 / rsq;
                    double sr2 = sys->sigma * sys->sigma * rsqinv;
                    double sr6 = sr2 * sr2 * sr2;
                    double sr12 = sr6 * sr6;
                    double epsilon4 = 4.0 * sys->epsilon;

                    /* Accumulate potential energy */
                  #ifdef ENABLE_OPENMP
                    epot_local += epsilon4 * (sr12 - sr6);
                  #endif //ENABLE_OPENMP

                    /* Compute force factor */
                    ffac = epsilon4 * (12.0 * sr12 - 6.0 * sr6) * (rsqinv);
                  #ifdef ENABLE_OPENMPI
                    #ifndef ENABLE_OPENMP
                    epot +=  epsilon4 * (sr12 - sr6);

                    temp_fx[i] += rx * ffac; 
                    temp_fx[j] -= rx  * ffac;

                    temp_fy[i] += ry  * ffac; 
                    temp_fy[j] -= ry  * ffac;

                    temp_fz[i] += rz  * ffac; 
                    temp_fz[j] -= rz  * ffac;
                    #endif //ENABLE_OPENMP
                  #endif  //ENABLE_OPENMPI

                  #ifdef ENABLE_OPENMP
                    int off = natoms * tid;
                    sys->cx[i + off] += rx * ffac;
                    sys->cy[i + off] += ry * ffac;
                    sys->cz[i + off] += rz * ffac;

                    sys->cx[j + off] -= rx * ffac;
                    sys->cy[j + off] -= ry * ffac;
                    sys->cz[j + off] -= rz * ffac;
                  #endif

                  #ifndef ENABLE_OPENMPI
                    #ifndef ENABLE_OPENMP
                    
                      sys->epot += epsilon4 * (sr12 - sr6);
                      
                      sys->fx[i] += rx * ffac;
                      sys->fy[i] += ry * ffac;
                      sys->fz[i] += rz * ffac;

                      sys->fx[j] -= rx * ffac;
                      sys->fy[j] -= ry * ffac;
                      sys->fz[j] -= rz * ffac;

                    #endif /* ifndef ENABLE_OPENMP */
                  #endif  // not ENABLE_OPENMPI
                }
            }
        }

        
        /* Sum forces from all threads */
        #ifdef ENABLE_OPENMP
        #pragma omp barrier
        for (int i = tid; i < natoms; i += sys->nthreads){
            for (int t = 0; t < sys->nthreads; t++){
              #ifdef ENABLE_OPENMPI
                temp_fx[i] += sys->cx[natoms * t + i];
                temp_fy[i] += sys->cy[natoms * t + i];
                temp_fz[i] += sys->cz[natoms * t + i];
              #else
                sys->fx[i] += sys->cx[natoms * t + i];
                sys->fy[i] += sys->cy[natoms * t + i];
                sys->fz[i] += sys->cz[natoms * t + i];
              #endif //ENABLE_OPENMPI
            }
        }
        /* Assign the accumulated potential energy */
        #pragma omp atomic
          #ifdef ENABLE_OPENMPI
            epot += epot_local;
          #else
            sys->epot += epot_local;
          #endif //ENABLE_OPENMPI
        #endif // ENABLE_OPENMP
    }
    #ifdef ENABLE_OPENMPI

      MPI_Reduce(&epot, &(sys->epot), 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

      MPI_Reduce(temp_fx, sys->fx, sys->natoms, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
      MPI_Reduce(temp_fy, sys->fy, sys->natoms, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
      MPI_Reduce(temp_fz, sys->fz, sys->natoms, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    #endif //ENABLE_OPENMPI
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

