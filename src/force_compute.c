#ifdef ENABLE_OPENMP
  #include <omp.h>
#endif // ENABLE_OPENMP 
#include "../include/constants.h"     // constants definitions header file
#include "../include/mdsys.h"         // System struct definition header file
#include "../include/utilities.h"     // timing, zero matrix functions header file
#include "../include/force_compute.h" // Computing force and kinetic energy functions file

// Function to get the boundary conditions
static inline double pbc(double x, const double boxby2) {
    while (x >  boxby2) x -= 2.0 * boxby2;
    while (x < -boxby2) x += 2.0 * boxby2;
    return x;
}

// Function to compute the interactive force
void force(mdsys_t *sys) {
    const int natoms = sys->natoms;
    int stride = 1;
    int init = 0, stop = sys->natoms;
    double rx, ry, rz;

    // if openMP or openMPI we use the cx, cy and cz arrays as containers
    #if defined(ENABLE_OPENMP) || defined(ENABLE_OPENMPI)
      azzero(sys->cx, sys->natoms * sys->nthreads);
      azzero(sys->cy, sys->natoms * sys->nthreads);
      azzero(sys->cz, sys->natoms * sys->nthreads);
    #endif // ENABLE_OPENMP
  
    #ifdef ENABLE_OPENMPI
      double epot;
      // overwrite the init, and stop variables
      init = sys->local_size * sys->rank + sys->offset;
      stop = init + sys->local_size;
      epot = 0.0;

      // All the process need to know the other particle positions so, we broadcast from 0.
      MPI_Bcast(sys->rx, sys->natoms, MPI_DOUBLE, 0, MPI_COMM_WORLD);
      MPI_Bcast(sys->ry, sys->natoms, MPI_DOUBLE, 0, MPI_COMM_WORLD);
      MPI_Bcast(sys->rz, sys->natoms, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    #endif // ENABLE_OPENMPI
  
    #ifndef ENABLE_OPENMPI
      // We need to initialize the forces to zero to add the new forces
      azzero(sys->fx, sys->natoms);
      azzero(sys->fy, sys->natoms);
      azzero(sys->fz, sys->natoms);
    #endif // not ENABLE_OPENMPI

    const double register rcsq = sys->rcut * sys->rcut;
    
    // Initializing the potential energy to zero
    sys->epot = 0.0;

    #ifdef ENABLE_OPENMP
    // Initializing the openMP pragma parallel region
    #pragma omp parallel
    #endif
    {   
        int tid = 0;
        #ifdef ENABLE_OPENMP
        // Defining and overwriting variables if omp is enabled
        double epot_local = 0.0;
        tid = omp_get_thread_num();
        stride = sys->nthreads;
        #endif

        // Loop over the particles
        for (int i = init + tid; i < stop; i += stride) {
            // Loop over the interactive particles
            // Start from i+1 because of third Newton law
            for (int j = i + 1; j < natoms; ++j) {
                double register ffac;
                double register rsq; 

                // Applying boundary conditions for the distance between particles
                rx = pbc(sys->rx[i] - sys->rx[j], 0.5 * sys->box);
                ry = pbc(sys->ry[i] - sys->ry[j], 0.5 * sys->box);
                rz = pbc(sys->rz[i] - sys->rz[j], 0.5 * sys->box);

                // Computing the squared distance
                rsq = rx * rx + ry * ry + rz * rz;

                // If particle is inside the cutoff then add the interaction
                if (rsq < rcsq) {

                    // Defining values for the force and potential energy
                    double register rsqinv = 1.0 / rsq;
                    double sr2 = sys->sigma * sys->sigma * rsqinv;
                    double sr6 = sr2 * sr2 * sr2;
                    double sr12 = sr6 * sr6;
                    double epsilon4 = 4.0 * sys->epsilon;


                    // Compute force factor
                    ffac = epsilon4 * (12.0 * sr12 - 6.0 * sr6) * (rsqinv);

                  #if defined(ENABLE_OPENMPI) && !defined(ENABLE_OPENMP)
                    // Computing the potential energy and force if mpi but not omp
                    epot +=  epsilon4 * (sr12 - sr6);

                    sys->cx[i] += rx * ffac; 
                    sys->cx[j] -= rx  * ffac;

                    sys->cy[i] += ry  * ffac; 
                    sys->cy[j] -= ry  * ffac;

                    sys->cz[i] += rz  * ffac; 
                    sys->cz[j] -= rz  * ffac;
                  #endif  //ENABLE_OPENMPI && not ENABLE_OPENMP

                  #ifdef ENABLE_OPENMP
                    // Computing the potential energy and force if omp
                    epot_local += epsilon4 * (sr12 - sr6);

                    // Calculating the offset
                    int off = natoms * tid;
                    sys->cx[i + off] += rx * ffac;
                    sys->cy[i + off] += ry * ffac;
                    sys->cz[i + off] += rz * ffac;

                    sys->cx[j + off] -= rx * ffac;
                    sys->cy[j + off] -= ry * ffac;
                    sys->cz[j + off] -= rz * ffac;
                  #endif

                  #if !defined(ENABLE_OPENMPI) && !defined (ENABLE_OPENMP)
                    // Defining the behavior for the serial version
                    
                      sys->epot += epsilon4 * (sr12 - sr6);
                      
                      sys->fx[i] += rx * ffac;
                      sys->fy[i] += ry * ffac;
                      sys->fz[i] += rz * ffac;

                      sys->fx[j] -= rx * ffac;
                      sys->fy[j] -= ry * ffac;
                      sys->fz[j] -= rz * ffac;

                  #endif  // not ENABLE_OPENMPI && not ENABLE_OPENMPI
                }
            }
        }

        
        // Sum forces from all threads
        #ifdef ENABLE_OPENMP
        #pragma omp barrier
        for (int i = tid; i < natoms; i += sys->nthreads){
              #ifdef ENABLE_OPENMPI
            for (int t = 1; t < sys->nthreads; t++){
                sys->cx[i] += sys->cx[natoms * t + i];
                sys->cy[i] += sys->cy[natoms * t + i];
                sys->cz[i] += sys->cz[natoms * t + i];
              #else
            for (int t = 0; t < sys->nthreads; t++){
                sys->fx[i] += sys->cx[natoms * t + i];
                sys->fy[i] += sys->cy[natoms * t + i];
                sys->fz[i] += sys->cz[natoms * t + i];
              #endif //ENABLE_OPENMPI
            }
        }
        // Assign the accumulated potential energy
        #pragma omp atomic
          #ifdef ENABLE_OPENMPI
            epot += epot_local;
          #else
            sys->epot += epot_local;
          #endif //ENABLE_OPENMPI
        #endif // ENABLE_OPENMP
    }
    #ifdef ENABLE_OPENMPI

      // Reducing the potential energy and forces in the rank 0 process
      MPI_Reduce(&epot, &(sys->epot), 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

      MPI_Reduce(sys->cx, sys->fx, sys->natoms, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
      MPI_Reduce(sys->cy, sys->fy, sys->natoms, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
      MPI_Reduce(sys->cz, sys->fz, sys->natoms, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    #endif //ENABLE_OPENMPI
}

// Function for compute the kinetic energy
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

    // computing the Temperature
    sys->temp = 2.0 * sys->ekin / (3.0 * sys->natoms - 3.0) / kboltz;
}

