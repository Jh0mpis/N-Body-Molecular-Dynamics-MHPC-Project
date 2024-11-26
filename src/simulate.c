#include "../include/simulate.h"                  // run function header file
#include "../include/mdsys.h"                     // struct function header file
#include "../include/force_compute.h"             // Force function header file
#include "../include/utilities.h"                 // azzero, pbc and wallclock header file
#include "../include/output.h"                    // output function header file
#include "../include/verlet_time_integration.h"   // velverlet function header file

// Principal loop function
void run(mdsys_t *sys, FILE **erg, FILE **traj, const double initial_time, const int nprint){
    double t_start;
  
    /* Initialize forces and energies */
    sys->nfi = 0;
    force(&(*sys));
    ekin(&(*sys));

    #ifdef ENABLE_OPENMPI
      if(!sys->rank){
        // Printing the latency time
        printf("Startup time: %10.10fs\n", wallclock() - initial_time);
        printf("Starting simulation with %d atoms for %d steps.\n", sys->natoms, sys->nsteps);
        printf("     NFI            TEMP            EKIN                 EPOT              ETOT\n");
        output(sys, *erg, *traj);
      }
    #else
      // Printing the latency time
      printf("Startup time: %10.3fs\n", wallclock() - initial_time);
      printf("Starting simulation with %d atoms for %d steps.\n", sys->natoms, sys->nsteps);
      printf("     NFI            TEMP            EKIN                 EPOT              ETOT\n");
      output(sys, *erg, *traj);
    #endif

    /* Reset timer */
    t_start = wallclock();

    /* Main MD loop */
    for (sys->nfi = 1; sys->nfi <= sys->nsteps; ++sys->nfi) {

        /* Write output, if requested */
        if ((sys->nfi % nprint) == 0){
            #ifdef ENABLE_OPENMPI
              if(!sys->rank){
                output(sys, *erg, *traj);
              }
            #else
              output(sys, *erg, *traj);
            #endif
        }

        /* Propagate system and recompute energies */
        velverlet(sys);
        ekin(sys);
    }

    #ifdef ENABLE_OPENMPI
      if(!sys->rank){
        /* Clean up: close files, free memory */
        printf("Simulation Done. Run time: %10.10fs\n", wallclock() - t_start);
      }
    #else
      /* Clean up: close files, free memory */
      printf("Simulation Done. Run time: %10.10fs\n", wallclock() - t_start);
    #endif
}
