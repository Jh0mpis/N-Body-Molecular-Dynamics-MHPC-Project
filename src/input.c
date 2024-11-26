#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "../include/input.h"       // input functions header file
#include "../include/constants.h"   // constants definition header file
#include "../include/mdsys.h"       // struct definition header file
#include "../include/utilities.h"   // wallclock, pbc and azzero functions header file

// read 1 line from the input file
static int get_a_line(FILE *fp, char *buf) {
    char tmp[BLEN], *ptr;

    if (fgets(tmp, BLEN, fp)) {
        unsigned int i;

        ptr = strchr(tmp, '#');
        if (ptr) *ptr = '\0';
        i = strlen(tmp) - 1;
        while (i >= 0 && isspace(tmp[i])) {
            tmp[i--] = '\0';
        }
        ptr = tmp;
        while (*ptr && isspace(*ptr)) { ++ptr; }
        strcpy(buf, ptr);
        return 0;
    } else {
        perror("Problem reading input");
        return -1;
    }
}

// Read the entire file and associated files
int read_input_files(mdsys_t *sys, FILE **erg, FILE **traj){
    char restfile[BLEN], line[BLEN];
    char trajfile[BLEN], ergfile[BLEN];
    FILE *fp;

    /* Read input file and setting some system properties */
    if (get_a_line(stdin, line)) return -1;
    sys->natoms = atoi(line);
    if (get_a_line(stdin, line)) return -1;
    sys->mass = atof(line);
    if (get_a_line(stdin, line)) return -1;
    sys->epsilon = atof(line);
    if (get_a_line(stdin, line)) return -1;
    sys->sigma = atof(line);
    if (get_a_line(stdin, line)) return -1;
    sys->rcut = atof(line);
    if (get_a_line(stdin, line)) return -1;
    sys->box = atof(line);

    if (get_a_line(stdin, restfile)) return -1;
    if (get_a_line(stdin, trajfile)) return -1;
    if (get_a_line(stdin, ergfile)) return -1;
    if (get_a_line(stdin, line)) return -1;
    sys->nsteps = atoi(line);

    if (get_a_line(stdin, line)) return -1;
    sys->dt = atof(line);
    if (get_a_line(stdin, line)) return -1;
    

    /* Allocate memory */
    sys->rx = (double *)malloc(sys->natoms * sizeof(double));
    sys->ry = (double *)malloc(sys->natoms * sizeof(double));
    sys->rz = (double *)malloc(sys->natoms * sizeof(double));
    sys->vx = (double *)malloc(sys->natoms * sizeof(double));
    sys->vy = (double *)malloc(sys->natoms * sizeof(double));
    sys->vz = (double *)malloc(sys->natoms * sizeof(double));
    sys->fx = (double *)malloc(sys->natoms * sizeof(double));
    sys->fy = (double *)malloc(sys->natoms * sizeof(double));
    sys->fz = (double *)malloc(sys->natoms * sizeof(double));

    /* Read restart */
    fp = fopen(restfile, "r");
    if (fp) {
        for (unsigned int i = 0; i < sys->natoms; ++i) {
            fscanf(fp, "%lf%lf%lf", sys->rx + i, sys->ry + i, sys->rz + i);
        }
        for (unsigned int i = 0; i < sys->natoms; ++i) {
            fscanf(fp, "%lf%lf%lf", sys->vx + i, sys->vy + i, sys->vz + i);
        }
        fclose(fp);
        azzero(sys->fx, sys->natoms);
        azzero(sys->fy, sys->natoms);
        azzero(sys->fz, sys->natoms);
    } else {
        perror("Cannot read restart file");
        return 3;
    }
  
    // Open the output files
    *erg = fopen(ergfile, "w");
    *traj = fopen(trajfile, "w");

  return atoi(line); // returning the printing steps
}
