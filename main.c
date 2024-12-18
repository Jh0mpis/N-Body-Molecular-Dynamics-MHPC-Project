/*
 * Simple Lennard-Jones potential MD code with velocity Verlet.
 * Units: Length=Angstrom, Mass=amu; Energy=kcal
 *
 * Baseline C version.
 */

#include <stdio.h>
#include "include/mdsys.h"                    // Struct definition
#include "include/input.h"                    // get_a_line function
#include "include/utilities.h"                // wallclock, azzero, pbc
#include "include/cleanup.h"                  // Free the allocated stuff
#include "include/simulate.h"                 // Run the simulation 
#ifdef ENABLE_OPENMPI
  #include <mpi.h>
#endif
#ifdef ENABLE_OPENMP
#include <omp.h>
#endif

int main(int argc, char **argv) {
    
    mdsys_t sys;
    #ifdef ENABLE_OPENMPI
      // initializing MPI 
      MPI_Init(&argc, &argv);
      int rank, nps;

      MPI_Comm_size(MPI_COMM_WORLD, &nps);
      MPI_Comm_rank(MPI_COMM_WORLD, &rank);

      // Setting mpi variables
      sys.rank = rank;
      sys.nps = nps;
      sys.nthreads = 1;
      
      if(!sys.rank){
        printf("Running with OpenMPI using %d process\n", sys.nps);
        #ifndef ENABLE_OPENMP
        printf("LJMD version %3.3f\n", LJMD_VERSION);
        #endif
      }
    #endif

    FILE *traj, *erg;
    double t_start;


    #ifdef ENABLE_OPENMP
      // Setting up openMP variables
      #pragma omp parallel
      {
        int id_trhead = omp_get_thread_num();
        if(!id_trhead){
          sys.nthreads = omp_get_num_threads();
          #ifdef ENABLE_OPENMPI
          if(!rank){
            printf("Running with OpenMP using %d threads\n", sys.nthreads);
            printf("LJMD version %3.3f\n", LJMD_VERSION);
          }
          #else
            printf("Running with OpenMP using %d threads\n", sys.nthreads);
            printf("LJMD version %3.3f\n", LJMD_VERSION);
          #endif
        }
      }
    #endif

    #if !defined (ENABLE_OPENMPI) && !defined (ENABLE_OPENMP)
        printf("LJMD version %3.3f\n", LJMD_VERSION);
    #endif

    t_start = wallclock();
    
    // Initialize the system from input files
    const int nprint = read_input_files(&sys, &erg, &traj);
    if (nprint <= 0 ) return 1;

    // Run the entire simulation
    run(&sys, &erg, &traj, t_start, nprint);

    // Deallocate
    clean(&sys, &erg, &traj);

  #ifdef ENABLE_OPENMPI
    // Finalizing MPI region
    MPI_Finalize();
  #endif

    return 0;
}
