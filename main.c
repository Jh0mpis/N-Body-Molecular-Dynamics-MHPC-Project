/*
 * Simple Lennard-Jones potential MD code with velocity Verlet.
 * Units: Length=Angstrom, Mass=amu; Energy=kcal
 *
 * Baseline C version.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "constants.h"
#include "mdsys.h"
#include "input.h"
#include "utilities.h"
#include "force_compute.h"
#include "verlet_time_integration.h"
#include "output.h"

#define LJMD_VERSION 1.0

int main(int argc, char **argv) {
    int nprint;
    char restfile[BLEN], trajfile[BLEN], ergfile[BLEN], line[BLEN];
    FILE *fp, *traj, *erg;
    mdsys_t sys;
    double t_start;

    printf("LJMD version %3.1f\n", LJMD_VERSION);

    t_start = wallclock();

    /* Read input file */
    if (get_a_line(stdin, line)) return 1;
    sys.natoms = atoi(line);
    if (get_a_line(stdin, line)) return 1;
    sys.mass = atof(line);
    if (get_a_line(stdin, line)) return 1;
    sys.epsilon = atof(line);
    if (get_a_line(stdin, line)) return 1;
    sys.sigma = atof(line);
    if (get_a_line(stdin, line)) return 1;
    sys.rcut = atof(line);
    if (get_a_line(stdin, line)) return 1;
    sys.box = atof(line);
    if (get_a_line(stdin, restfile)) return 1;
    if (get_a_line(stdin, trajfile)) return 1;
    if (get_a_line(stdin, ergfile)) return 1;
    if (get_a_line(stdin, line)) return 1;
    sys.nsteps = atoi(line);
    if (get_a_line(stdin, line)) return 1;
    sys.dt = atof(line);
    if (get_a_line(stdin, line)) return 1;
    nprint = atoi(line);

    /* Allocate memory */
    sys.rx = (double *)malloc(sys.natoms * sizeof(double));
    sys.ry = (double *)malloc(sys.natoms * sizeof(double));
    sys.rz = (double *)malloc(sys.natoms * sizeof(double));
    sys.vx = (double *)malloc(sys.natoms * sizeof(double));
    sys.vy = (double *)malloc(sys.natoms * sizeof(double));
    sys.vz = (double *)malloc(sys.natoms * sizeof(double));
    sys.fx = (double *)malloc(sys.natoms * sizeof(double));
    sys.fy = (double *)malloc(sys.natoms * sizeof(double));
    sys.fz = (double *)malloc(sys.natoms * sizeof(double));

    /* Read restart */
    fp = fopen(restfile, "r");
    if (fp) {
        for (int i = 0; i < sys.natoms; ++i) {
            fscanf(fp, "%lf%lf%lf", sys.rx + i, sys.ry + i, sys.rz + i);
        }
        for (int i = 0; i < sys.natoms; ++i) {
            fscanf(fp, "%lf%lf%lf", sys.vx + i, sys.vy + i, sys.vz + i);
        }
        fclose(fp);
        azzero(sys.fx, sys.natoms);
        azzero(sys.fy, sys.natoms);
        azzero(sys.fz, sys.natoms);
    } else {
        perror("Cannot read restart file");
        return 3;
    }

    /* Initialize forces and energies */
    sys.nfi = 0;
    force(&sys);
    ekin(&sys);

    erg = fopen(ergfile, "w");
    traj = fopen(trajfile, "w");

    printf("Startup time: %10.3fs\n", wallclock() - t_start);
    printf("Starting simulation with %d atoms for %d steps.\n", sys.natoms, sys.nsteps);
    printf("     NFI            TEMP            EKIN                 EPOT              ETOT\n");
    output(&sys, erg, traj);

    /* Reset timer */
    t_start = wallclock();

    /**************************************************/
    /* Main MD loop */
    for (sys.nfi = 1; sys.nfi <= sys.nsteps; ++sys.nfi) {

        /* Write output, if requested */
        if ((sys.nfi % nprint) == 0)
            output(&sys, erg, traj);

        /* Propagate system and recompute energies */
        velverlet(&sys);
        ekin(&sys);
    }
    /**************************************************/

    /* Clean up: close files, free memory */
    printf("Simulation Done. Run time: %10.3fs\n", wallclock() - t_start);
    fclose(erg);
    fclose(traj);

    free(sys.rx);
    free(sys.ry);
    free(sys.rz);
    free(sys.vx);
    free(sys.vy);
    free(sys.vz);
    free(sys.fx);
    free(sys.fy);
    free(sys.fz);

    return 0;
}
