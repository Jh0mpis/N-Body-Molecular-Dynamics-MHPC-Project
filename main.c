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

int main(int argc, char **argv) {
    FILE *traj, *erg;
    mdsys_t sys;
    double t_start;

    printf("LJMD version %3.3f\n", LJMD_VERSION);

     printf("Using %d threads\n", omp_get_num_threads());

    t_start = wallclock();
    
    // Initialize the system from input files
    const int nprint = read_input_files(&sys, &erg, &traj);
    if (nprint <= 0 ) return 1;

    // Run the entire simulation
    run(&sys, &erg, &traj, t_start, nprint);

    // Deallocate
    clean(&sys, &erg, &traj);

    return 0;
}
